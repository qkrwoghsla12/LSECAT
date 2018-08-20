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
 *  2016 Raimarius Delgado
*/
/****************************************************************************/
#ifndef SANYODENKI_ESI_H
#define SANYODENKI_ESI_H

#include "embdECATM.h"

/****************************************************************************/

#define SANYO_SERVO  0x00007595, 0x00000000 // Vendor ID , Product Code

#define SANYO_SERVO_REGS(x) \
	{ALIAS_POSITION(x), SANYO_SERVO, SANYODENKI_CONTROL_WORD , &sanyoCtrlWordOff[x]}, \
	{ALIAS_POSITION(x), SANYO_SERVO, SANYODENKI_TARGET_VELOCITY, &sanyoTargVelOff[x]}, \
	{ALIAS_POSITION(x), SANYO_SERVO, SANYODENKI_STATUS_WORD, &sanyoStatWordOff[x]}, \
	{ALIAS_POSITION(x), SANYO_SERVO, SANYODENKI_ACTUAL_POSITION, &sanyoActlPosOff[x]}, \
	{ALIAS_POSITION(x), SANYO_SERVO, SANYODENKI_ACTUAL_VELOCITY, &sanyoActlVelOff[x]}

/****************************************************************************/

unsigned int sanyoCtrlWordOff[SANYODENKI_SLAVENUM]	=	{0,};
unsigned int sanyoTargVelOff[SANYODENKI_SLAVENUM]	=	{0,};
unsigned int sanyoStatWordOff[SANYODENKI_SLAVENUM]	=	{0,};
unsigned int sanyoActlPosOff[SANYODENKI_SLAVENUM]	=	{0,};
unsigned int sanyoActlVelOff[SANYODENKI_SLAVENUM]	=	{0,};

/*****************************************************************************/

ec_pdo_entry_info_t sanyo_pdo_entries[] = {
    {SANYODENKI_CONTROL_WORD,16}, // Controlword 
    {SANYODENKI_TARGET_VELOCITY,32}, // Target Velocity 
    {SANYODENKI_STATUS_WORD,16}, // Statusword 
    {SANYODENKI_ACTUAL_POSITION,32}, // Position Actual Value 
    {SANYODENKI_ACTUAL_VELOCITY,32}, // Velocity Actual Value 
};
ec_pdo_info_t sanyo_pdos[] = {
    {0x1601, 2, sanyo_pdo_entries + 0}, // Tx PDO Mapping 
    {0x1a01, 3, sanyo_pdo_entries + 2}, // Rx PDO Mapping 
};
ec_sync_info_t sanyo_syncs[] = {
    {0, EC_DIR_OUTPUT, 	0, NULL, 		EC_WD_DISABLE},
    {1, EC_DIR_INPUT, 	0, NULL, 		EC_WD_DISABLE},
    {2, EC_DIR_OUTPUT, 	1, sanyo_pdos + 0, EC_WD_DISABLE},
    {3, EC_DIR_INPUT, 	1, sanyo_pdos + 1, EC_WD_DISABLE},
    {0xff}
};

/*****************************************************************************/
unsigned short sanyoGetStatusVal(unsigned short statusWord)
{
	if((statusWord & 0x4F) == 0x00) {
		statusWord = SANYO_NOT_READY_TO_SWITCH_ON;
	}
	else if((statusWord & 0x4F) == 0x08) {
		statusWord =  SANYO_FAULT;
	}
	else if((statusWord & 0x4F) == 0x40) {
		statusWord =  SANYO_SWITCH_ON_DISABLED;
	}
	else if((statusWord & 0x6F) == 0x27) {
		statusWord =  SANYO_OPERATION_ENABLED;
	}
	else if((statusWord & 0x6F) == 0x23) {
		statusWord =  SANYO_SWITCH_ON_ENABLED;
	}
	else if((statusWord & 0x6F) == 0x21) {
		statusWord =  SANYO_READY_TO_SWITCH_ON;
	}
	else if((statusWord & 0x6F) == 0x07) {
		statusWord =  SANYO_QUICK_STOP_ACTIVE;
	}
	else if((statusWord & 0x4F) == 0x0F) {
		statusWord =  SANYO_FAULT_REACTION_ACTIVE;
	}
	else {
		return 0xFFFF;
	}
	return statusWord;
}
/*****************************************************************************/
const char* sanyoGetStatusStr(unsigned short statusWord)
{
	if((statusWord & 0x4F) == 0x00) {
		return "Not ready to Switch On";
	}
	else if((statusWord & 0x4F) == 0x08) {
		return "Fault";
	}
	else if((statusWord & 0x4F) == 0x40) {
		return "Switch Off";
	}
	else if((statusWord & 0x6F) == 0x27) {
		return "Operation Enabled";
	}
	else if((statusWord & 0x6F) == 0x23) {
		return "Switch On Enabled";
	}
	else if((statusWord & 0x6F) == 0x21) {
		return "Ready to Switch On";
	}
	else if((statusWord & 0x6F) == 0x07) {
		return "Quick Stop Activate";
	}
	else if((statusWord & 0x4F) == 0x0F) {
		return "Fault Reaction Activate";
	}else{
	
	return "Unknown Status";
	}
}


/*****************************************************************************/
#endif //SANYODENKI_ESI_H

