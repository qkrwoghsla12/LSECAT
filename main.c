/*
 *  This file is owned by the Embedded Systems Laboratory of Seoul National University of Science and Technology
 *  to test EtherCAT master protocol using the IgH EtherCAT master userspace library.	
 *  
 *  
 *  IgH EtherCAT master library for Linux is found at the following URL: 
 *  <http://www.etherlab.org/en/ethercat>
 *
 *
 *
 *
 *  2015 Raimarius Delgado
*/
/****************************************************************************/
#include "main.h"
/****************************************************************************/
int  XenoInit(void);
void XenoQuit(void);
void DoInput();
void SignalHandler(int signum);
void PrintEval(int BufPrd[], int BufExe[], int BufJit[], int ArraySize);
void FilePrintEval(char *FileName,int BufPrd[], int BufExe[], int BufJit[], int BufCollect[], int BufProcess[], int BufTranslate[],int Bufspintime[], int ArraySize);
/****************************************************************************/
int spincnt=0;
int cpuspintime=0;

#ifdef CPUSPIN
void CpuSpinTask(void *arg){
        RTIME start,end, execend;
        end = rt_timer_read();

	while (1){
		rt_task_wait_period(NULL);
		rt_timer_spin(cpuspintime);

                rt_timer_spin(cpuspintime);
                BufSpinPeriodTime[spincnt] = SpinTaskPeriod = ((int)start - (int)end);
                execend = rt_timer_read();
                BufSpinJitter[spincnt] = MathAbsValI(SpinTaskPeriod - (int)ECATCTRL_TASK_PERIOD);
                BufSpinExec[spincnt] = (int)execend - (int)start;
                end = start;
                spincnt++;
	}
}
#endif

void EcatCtrlTask(void *arg){
	int iSlaveCnt;
	int iTaskTick = 0;
	RtmEcatPeriodEnd = rt_timer_read();

	while (1){
	rt_task_wait_period(NULL);
	RtmEcatPeriodStart = rt_timer_read();
	EcatReceiveProcessDomain();
	EcatState = EcatStatusCheck();
	RtmCollectTime = rt_timer_read();
	/*
	 * Do reading of the current process data from here
	 * before processing and sending to Tx
	 */
	/*Read EtherCAT Datagram*/
	if(EcatState.master_state == OP && EcatState.slave_state == OP){
		for(iSlaveCnt=0; iSlaveCnt < SANYODENKI_SLAVENUM; ++iSlaveCnt){
			sanyoStatusWord[iSlaveCnt] = sanyoGetStatusWordN(iSlaveCnt);
			sanyoActualVelR[iSlaveCnt] = sanyoGetActualVelocityN(iSlaveCnt);

			if(sanyoPrevStatusWord[iSlaveCnt] == sanyoStatusWord[iSlaveCnt]){
				continue;
			}
			switch(sanyoStatusWord[iSlaveCnt]){	
				case SANYO_SWITCH_ON_DISABLED:
					sanyoReady(iSlaveCnt);
					break;
				case SANYO_READY_TO_SWITCH_ON:
					sanyoSwitchOn(iSlaveCnt);
					break;
				case SANYO_SWITCH_ON_ENABLED:
					sanyoServoOn(iSlaveCnt);
					break;
				case SANYO_OPERATION_ENABLED:
					sanyoServoOp = RUN;
					break;
				case SANYO_FAULT:
					sanyoFaultReset(iSlaveCnt);
					break;
				default:
					break;
			}
			sanyoPrevStatusWord[iSlaveCnt] = sanyoStatusWord[iSlaveCnt]; 
		}
	}
	switch(sanyoServoOp){
		case RUN:
			if (bTimingFlag == FALSE) bTimingFlag = TRUE;
			sanyoSetVelocity(_ALLNODE,500);
			break;
		case STOP:
			sanyoShutDown(_ALLNODE);
			bQuitFlag = TRUE;
			break;
		default:
			break;
	}
	/* write application time to master */
	RtmEcatMasterAppTime = rt_timer_read();
	EcatWriteAppTimeToMaster((uint64_t)RtmEcatMasterAppTime);

	#ifdef PROCESS
	volatile int i;
	volatile int ii;
	for(i=0;i<15000;i++){
		ii=ii+i*2;
	}
	#endif
	/* send process data */
	RtmProcessTime =  rt_timer_read();
	EcatSendProcessDomain();
	RtmEcatExecTime = rt_timer_read();
	EcatPeriod 	 = ((int)RtmEcatPeriodStart - (int)RtmEcatPeriodEnd);
	EcatExecution = ((int)RtmEcatExecTime - (int)RtmEcatPeriodStart);
	EcatJitter	 = MathAbsValI(EcatPeriod - (int)ECATCTRL_TASK_PERIOD);
	EcatCollect	= (int)RtmCollectTime - (int)RtmEcatPeriodStart;
	EcatProcess	= (int)RtmProcessTime - (int)RtmCollectTime;
	EcatTranslate	= (int)RtmEcatExecTime - (int)RtmProcessTime;
	//rt_printf("%d.%03d\n",EcatProcess/1000000, EcatProcess%1000000);
	//rt_printf("%d.%03d\n",EcatPeriod/1000000, EcatPeriod%1000000);

	if(bTimingFlag == TRUE) {
		BufEcatPeriodTime[iBufEcatDataCnt] 	=   EcatPeriod;
		BufEcatExecTime[iBufEcatDataCnt]  	=   EcatExecution;
		BufEcatJitter[iBufEcatDataCnt]  	=   EcatJitter;
		BufEcatCollect[iBufEcatDataCnt]  	=   EcatCollect;
		BufEcatProcess[iBufEcatDataCnt]  	=   EcatProcess;
		BufEcatTranslate[iBufEcatDataCnt]  	=   EcatTranslate;
		++iBufEcatDataCnt;

		if(iBufEcatDataCnt == BUF_SIZE){
			bTimingFlag = FALSE;
			sanyoServoOp = STOP;
			}
	}

   	   RtmEcatPeriodEnd = RtmEcatPeriodStart;
	   iTaskTick++;
    }
}
/****************************************************************************/
int main(int argc, char **argv){
	int ret = 0;
	char *filename;
	if(argv[1]){
		cpuspintime = atoi(argv[1])*1000;
		filename = argv[2];
		//printf("spin : %d\n",cpuspintime);
	}
	else{
		printf("usage : ./start.sh cpuspintime(us) filename\n");
	}
	printf("\nspin : %d\n",cpuspintime);

	/* Interrupt Handler "ctrl+c"  */
	signal(SIGTERM, SignalHandler);
        signal(SIGINT, SignalHandler);

	/* EtherCAT Init */
   	if ((ret = EcatInit(ECATCTRL_TASK_PERIOD,SANYO_CYCLIC_VELOCITY))!=0){
		fprintf(stderr, 
			"Failed to Initiate IgH EtherCAT Master!\n");
		return ret;
	}

	mlockall(MCL_CURRENT|MCL_FUTURE); //Lock Memory to Avoid Memory Swapping

	/* RT-task */
	if ((ret = XenoInit())!=0){
		fprintf(stderr, "Failed to Initiate Xenomai Services!\n");
		return ret;
	}

	while (1) {
		usleep(1);
		if (bQuitFlag) break;
	}

	FilePrintEval((char *)filename,BufEcatPeriodTime,BufEcatExecTime,BufEcatJitter,BufEcatCollect, BufEcatProcess, BufEcatTranslate,BufSpinTime,iBufEcatDataCnt);
#ifdef CPUSPIN
	FilePrintEval((char *)"spintask_period.csv",BufSpinPeriodTime,BufSpinExec,BufSpinJitter,BufEcatCollect, BufEcatProcess, BufEcatTranslate,BufSpinTime,spincnt);
#endif
	PrintEval(BufEcatPeriodTime,BufEcatExecTime,BufEcatJitter,iBufEcatDataCnt);

	XenoQuit();
	EcatQuit();

return ret;
}

/****************************************************************************/
void SignalHandler(int signum){
		bQuitFlag = TRUE;
}

/****************************************************************************/

int XenoInit(void){

	rt_print_auto_init(1); //RTDK

	printf("Creating Xenomai Realtime Task(s)...");
	if(rt_task_create(&TskEcatCtrl,"EtherCAT Control", 0, ECATCTRL_TASK_PRIORITY,ECATCTRL_TASK_MODE)){
      		fprintf(stderr, "Failed to create Ecat Control Task\n");
		return _EMBD_RET_ERR_;
	}

	#ifdef CPUSPIN
	if(rt_task_create(&CpuSpin,"Cpu Spin", 0, CPUSPIN_TASK_PRIORITY, ECATCTRL_TASK_MODE)){
      		fprintf(stderr, "Failed to create Cpu Spin Task\n");
		return _EMBD_RET_ERR_;
	}
	#endif
	#ifdef PROCESS
		printf("add process\n");
	#endif
	printf("OK!\n");


	printf("Making Realtime Task(s) Periodic...");
	if(rt_task_set_periodic(&TskEcatCtrl, TM_NOW,rt_timer_ns2ticks(ECATCTRL_TASK_PERIOD))){
		fprintf(stderr, "Failed to Make Ecat Control Task Periodic\n");
		return _EMBD_RET_ERR_;
	}

	printf("Starting Xenomai Realtime Task EcatCtrl...");
	if(rt_task_start(&TskEcatCtrl, &EcatCtrlTask, NULL)){
		fprintf(stderr, "Failed to start Ecat Control Task\n");
		return _EMBD_RET_ERR_;
	}

	#ifdef CPUSPIN
	if(rt_task_set_periodic(&CpuSpin, TM_NOW, rt_timer_ns2ticks(ECATCTRL_TASK_PERIOD))){
		fprintf(stderr, "Failed to Make Cpu Spin Task Periodic\n");
		return _EMBD_RET_ERR_;
	}

	printf("Starting Xenomai Realtime Task CpuSpin...");
	if(rt_task_start(&CpuSpin, &CpuSpinTask, NULL)){
		fprintf(stderr, "Failed to start Cpu Spin Task\n");
		return _EMBD_RET_ERR_;
	}
	#endif
	printf("OK!\n");

	return _EMBD_RET_SCC_;
}

/****************************************************************************/

void XenoQuit(void){
	rt_task_suspend(&TskEcatCtrl);
	rt_task_delete(&TskEcatCtrl);
	#ifdef CPUSPIN
	rt_task_suspend(&CpuSpin);
	rt_task_delete(&CpuSpin);
	#endif
	printf("\033[%dm%s\033[0m",95,"Xenomai Task(s) Deleted!\n");
}

/****************************************************************************/

void PrintEval(int BufPrd[], int BufExe[], int BufJit[], int ArraySize){

/* MATH_STATS: Simple Statistical Analysis (libs/embedded/embdMATH.h)
 * 	float ave;
 * 	float max;
 * 	float min;
 * 	float std; */
	MATH_STATS EcatPeriodStat, EcatExecStat, EcatJitterStat;

	EcatPeriodStat = GetStatistics(BufPrd, ArraySize,SCALE_1M);  
	printf("\n[Period] Max: %.6f Min: %.6f Ave: %.6f St. D: %.6f\n", EcatPeriodStat.max,
			EcatPeriodStat.min,
			EcatPeriodStat.ave,
			EcatPeriodStat.std);
	EcatExecStat = GetStatistics(BufExe, ArraySize,SCALE_1K);  
	printf("[Exec]	 Max: %.3f Min: %.3f Ave: %.3f St. D: %.3f\n", EcatExecStat.max,
			EcatExecStat.min,
			EcatExecStat.ave,
			EcatExecStat.std);
	EcatJitterStat = GetStatistics(BufJit, ArraySize,SCALE_1K);  
	printf("[Jitter] Max: %.3f Min: %.3f Ave: %.3f St. D: %.3f\n", EcatJitterStat.max,
			EcatJitterStat.min,
			EcatJitterStat.ave,
			EcatJitterStat.std);
}

/****************************************************************************/

void FilePrintEval(char *FileName,int BufPrd[], int BufExe[], int BufJit[], int BufCollect[], int BufProcess[], int BufTranslate[],int Bufspintime[], int ArraySize){

	FILE *FileEcatTiming;
	int iCnt;

	FileEcatTiming = fopen(FileName, "w");

	for(iCnt=0; iCnt < ArraySize; ++iCnt){
		fprintf(FileEcatTiming,"%d.%06d,%d.%03d,%d.%03d,%d.%03d,%d.%03d,%d.%03d,%d.%06d\n",
				BufPrd[iCnt]/SCALE_1M,
				BufPrd[iCnt]%SCALE_1M,
				BufExe[iCnt]/SCALE_1K,
				BufExe[iCnt]%SCALE_1K,
				BufJit[iCnt]/SCALE_1K,
				BufJit[iCnt]%SCALE_1K,
				BufCollect[iCnt]/SCALE_1K,
				BufCollect[iCnt]%SCALE_1K,
				BufProcess[iCnt]/SCALE_1K,
				BufProcess[iCnt]%SCALE_1K,
				BufTranslate[iCnt]/SCALE_1K,
				BufTranslate[iCnt]%SCALE_1K,
				Bufspintime[iCnt]/SCALE_1M,
				Bufspintime[iCnt]%SCALE_1M);
	}
	fclose(FileEcatTiming);
}

/****************************************************************************/
