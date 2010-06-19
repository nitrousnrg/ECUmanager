#ifndef COMMCORE_FREEEMS_H
#define COMMCORE_FREEEMS_H

#include "qt4application.h"
//#include <QByteArray>
#include "commthread.h"

/* payloadID: complete as required for extended functionality

  even: PC->FreeEMS
  odd:  FreeEMS->PC

 */
enum PayloadID{ requestInterfaceVersion,
                responseInterfaceVersion,
                requestFirmwareVersion,
                responseFirmwareVersion,
                requestMaxPacketSize,
                responseMaxPacketSize,
                echoPacket,
                echoedPacket,
                requestSoftReset,               //8
                requestHardReset=10,
                FreeEMS_error = 13,
                FreeEMS_debug = 15,


            };

/* header flags definitions */
#define PROTOCOL    0x80        //Protocol/firmware
#define ACK         0x40
#define ACK_TYPE    0x20
#define HAS_ADDRESS 0x10
#define HAS_LENGTH  0x08







#endif // COMMCORE_FREEEMS_H
