# LSECAT
Ethercat Servo LSmecapion

--------------------------------------------------------------------------------------------
ZYBO-Z7-20, Beagle Bone Black 에서 LS mecapion 사의 L7N 이더캣 서보를 제어하는 예제입니다
--------------------------------------------------------------------------------------------
makefile 수정
XENOMAI_PATH = xenomai 경로
INC_ECAT     = ethercat 헤더 경로
LIB_DIR      = ethercat 라이브러리 경로
--------------------------------------------------------------------------------------------
실험 파라미터 변경
main.h

ECATCTRL_TASK_PRIORITY	= L7N 이더캣 서보를 제어하는 TASK 의 우선순위

CPUSPIN_TASK_PRIORITY	  = xenomai stress 를 주는 TASK 의 우선순위

ECATCTRL_TASK_PERIOD    = ECATCTRL_TASK 와 CPUSPIN_TASK 의 주기

SEC_DURATION            = 테스트를 수행하는 시간(s) 

CPUSPIN                 = define 되어 있을 경우 ECATCTRL_TASK 외에 xenomai stress TASK 추가로 생성 
                          define 되어있지 않을 경우 ECATCTRL_TASK 만 생성

PROCESS                 = define 되어 있을 경우 ECATCTRL_TASK 에 연산과정추가 
--------------------------------------------------------------------------------------------
ECATCTRL_TASK = 이더캣 서보를 제어하는 TASK
CPUSPIN_TASK  = 시스템에 부하를 주기 위해 만든 더미 TASK
