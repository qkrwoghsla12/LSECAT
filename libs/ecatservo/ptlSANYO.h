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
#ifndef SANYODENKI_PTL_H
#define SANYODENKI_PTL_H

/* LS Mecapion L7NA004B PROTOCOL */
#define SANYODENKI_CONTROL_WORD			0x6040,0x00
#define SANYO_SHUTDOWN				0x0006
#define SANYO_READY				0x0006
#define	SANYO_SWITCH_ON				0x0007
#define	SANYO_SWITCH_OFF			0x0007
#define	SANYO_DISABLE_VOLTAGE			0x0000
#define	SANYO_QUICK_STOP			0x0002
#define	SANYO_DISABLE_OPERATION		0x0007
#define	SANYO_ENABLE_OPERATION			0x000F
#define	SANYO_FAULT_RESET			0x0080

#define	SANYODENKI_STATUS_WORD			0x6041,0x00
#define	SANYO_NOT_READY_TO_SWITCH_ON		0x0000
#define	SANYO_SWITCH_ON_DISABLED		0x0040
#define	SANYO_READY_TO_SWITCH_ON		0x0021
#define	SANYO_SWITCH_ON_ENABLED		0x0023
#define	SANYO_OPERATION_ENABLED		0x0027
#define	SANYO_QUICK_STOP_ACTIVE		0x0007
#define	SANYO_FAULT_REACTION_ACTIVE		0x000F
#define	SANYO_FAULT				0x0008

#define	SANYODENKI_OPERATION_MODE		0x6060,0x00	
#define SANYO_PROFILE_POSITION			0x01
#define SANYO_PROFILE_VELOCITY			0x03
#define SANYO_PROFILE_TORQUE			0x04
#define SANYO_HOMING				0x06
#define SANYO_INTERPOLATED_POSITION		0x07
#define SANYO_CYCLIC_POSITION			0x08
#define SANYO_CYCLIC_VELOCITY			0x09
#define SANYO_CYCLIC_TORQUE			0x10

#define	SANYODENKI_ACTUAL_POSITION		0x6064, 0x00	//	RO,	int32
#define	SANYODENKI_ACTUAL_VELOCITY		0x606C, 0x00	//	RO,	int32
#define	SANYODENKI_TARGET_POSITION		0x607A, 0x00	//	RW,	int32
#define	SANYODENKI_TARGET_VELOCITY		0x60FF, 0x00	//	RW,	int32

#define SANYO_17BIT_ENCRES			131072
#define SANYO_ACTIVATE_WORD			0x0300
#define SANYO_SYNC0_SHIFT			125000

#define SANYOR2_MAXRPM				(3000)
/****************************************************************************/
#endif //SANYODENKI_PTL_H

