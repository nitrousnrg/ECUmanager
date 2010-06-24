#ifndef COMMCORE_FREEEMS_H
#define COMMCORE_FREEEMS_H

#include "qt4application.h"
//#include <QByteArray>
#include "commthread.h"

/* Header flag masks */
/* Always has flags (obviously)			*/
/* Always has payload ID so no flag		*/
/* Always has checksum, so no flag		*/

#define HEADER_HAS_ACK		1
#define HEADER_IS_NACK		(1<<1)
#define HEADER_HAS_ADDRS	(1<<2)
#define HEADER_HAS_LENGTH	(1<<3)
#define HEADER_RESERVED_A	(1<<4)
#define HEADER_RESERVED_B	(1<<5)
#define HEADER_RESERVED_C	(1<<6)
#define HEADER_RESERVED_D	(1<<7)

/*&&&&&&&&&&&&&&&&&&&&  Payload Type ID Name Value Pairs  &&&&&&&&&&&&&&&&&&&&*/

/* Please note, requests use even ID numbers and their matching responses use */
/* the odd number immediately above the original request ID. There are a few  */
/* examples where either a request is not required or a response not sent. In */
/* such cases the odd ID may be used for an packet asyncronously sent from    */
/* the EMS to the PC. These will be well commented when present.			  */


/*&&&&&&&&&&&&&&&&&&&&&&&&  Protocol Payload Type IDs  &&&&&&&&&&&&&&&&&&&&&&&*/

/* Firmware Independent functions */
#define requestInterfaceVersion		  0
#define requestFirmwareVersion		  2
#define requestMaxPacketSize		  4
#define requestEchoPacketReturn		  6
#define requestSoftSystemReset		  8	/* System comes up fresh, so no response */
//efine replytoSoftSystemReset		  9	/* This is reserved */
#define requestHardSystemReset		 10	/* System comes up fresh, so no response */
//efine replytoHardSystemReset		 11	/* This is reserved */


//efine requestAsyncErrorCode		 12	/* This is reserved */
#define asyncErrorCodePacket		 13	/* NOTE : Unrequested error event code packet */
//efine requestAsyncDebugInfo		 14	/* This is reserved */
#define asyncDebugInfoPacket		 15	/* NOTE : Unrequested debug information packet */


/*&&&&&&&&&&&&&&&&&&&&&&&&  Firmware Payload Type IDs  &&&&&&&&&&&&&&&&&&&&&&&*/

/* Whole block manipulation */
#define replaceBlockInRAM			256
#define replaceBlockInFlash			258
#define retrieveBlockFromRAM		260
#define retrieveBlockFromFlash		262
#define burnBlockFromRamToFlash 	264
#define eraseAllBlocksFromFlash 	266
#define burnAllBlocksOfFlash 		268

/* Main table manipulation */
#define adjustMainTableCell			300
#define adjustMainTableRPMAxis		302
#define	adjustMainTableLoadAxis		304

/* 2d table manipulation */
#define adjust2dTableAxis			306
#define adjust2dTableCell			308

/* Datalog request packets */
#define requestBasicDatalog			400
#define responseBasicDatalog		401 /* Defined because it can be used both synchronously and asynchronously */
#define requestConfigurableDatalog	402
#define responseConfigurableDatalog	403 /* Defined because it can be used both synchronously and asynchronously */
#define setAsyncDatalogType			404

/* Special function */
#define forwardPacketOverCAN		500
#define forwardPacketOverOtherUART	502

/* Generic memory grabber for debugging */
#define retrieveArbitraryMemory		600

/* 8 payload IDs for testing purposes */
#define testPayloadIDPacketType0	65520
#define testPayloadIDPacketType1	65522
#define testPayloadIDPacketType2	65524
#define testPayloadIDPacketType3	65526
#define testPayloadIDPacketType4	65528
#define testPayloadIDPacketType5	65530
#define testPayloadIDPacketType6	65532
#define testPayloadIDPacketType7	65534

#endif							 // COMMCORE_FREEEMS_H
