#ifndef FREEEMS_H
#define FREEEMS_H


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


#define DERIVED_VARS_SIZE sizeof(DerivedVar)
#define DERIVED_VARS_WIDTH IN_OUT_BANKS	/* How many elements per array currently 2 at 25/6/08 */
#define DERIVED_VARS_UNIT 2				/* How large each element is in bytes (short = 2 bytes) */
/* Use this block to make it easy to manage the derived variables */
typedef struct {
	/* Calculated from core vars */
	unsigned short LoadMain;			/* Configurable unit of load */
//	unsigned short LoadSecondary;

	unsigned short VEMain;			/* Divide by 512 to get % */
//	unsigned short VESecondary;

	unsigned short Lambda;			/* Divide by 32768 to get Lamda */
	unsigned short AirFlow;			/* top half */
	unsigned short densityAndFuel;	/* bottom half */

	unsigned short BasePW;			/* In timer ticks of 0.8us */
	unsigned short IDT;				/* 0.8us ticks */
	unsigned short ETE;				/* 0.8us ticks */
	signed short TFCTotal;			/* Transient fuel correction */

	unsigned short FinalPW;			/* In timer ticks of 0.8us */
	unsigned short RefPW;			/* In timer ticks of 0.8us */

	unsigned short sp1;		/*  */
	unsigned short sp2;		/*  */
	unsigned short sp3;		/*  */
	unsigned short sp4;		/*  */
	unsigned short sp5;		/*  */

//	unsigned short ;		/*  */
} DerivedVar;


#define RUNTIME_VARS_SIZE sizeof(RuntimeVar)
#define RUNTIME_VARS_LENGTH 13		/* How many runtime vars */
#define RUNTIME_VARS_UNIT 2				/* How large each element is in bytes (short = 2 bytes) */
/* Use this block to manage the execution time of various functions loops and ISRs etc */
typedef struct {
	/* Engine position and RPM code runtimes */
	unsigned short primaryInputLeadingRuntime;
	unsigned short primaryInputTrailingRuntime;
	unsigned short secondaryInputLeadingRuntime;
	unsigned short secondaryInputTrailingRuntime;

	/* Mathematics runtimes */
	unsigned short calcsRuntime;
	unsigned short genCoreVarsRuntime;
	unsigned short genDerivedVarsRuntime;
	unsigned short mathTotalRuntime;
	unsigned short mathSumRuntime;

	unsigned short RTCRuntime;

	/*  */
	unsigned short mainLoopRuntime;
	unsigned short logSendingRuntime;
	unsigned short serialISRRuntime;
} RuntimeVar;


#define ISR_LATENCY_VARS_SIZE sizeof(ISRLatencyVar)
#define ISR_LATENCY_VARS_LENGTH 2		/* How many latency vars */
#define ISR_LATENCY_VARS_UNIT 2				/* How large each element is in bytes (short = 2 bytes) */
/* Use this block to manage the execution time of various functions loops and ISRs etc */
typedef struct {
	/* Engine position and RPM code latencies */
	unsigned short primaryInputLatency;
	unsigned short secondaryInputLatency;

	/* Injector latencies */
	unsigned short Injector1Latency;
	unsigned short Injector2Latency;
	unsigned short Injector3Latency;
	unsigned short Injector4Latency;
	unsigned short Injector5Latency;
	unsigned short Injector6Latency;

	unsigned short DwellLatency;
	unsigned short IgniteLatency;

	/* Not an ISR, but important none the less */
	unsigned short mathLatency;
	unsigned short mathSampleTimeStamp0;
	unsigned short mathSampleTimeStamp1;
} ISRLatencyVar;


#define CORE_VARS_SIZE sizeof(CoreVar)
#define CORE_VARS_LENGTH 16				/* How many arrays */
#define CORE_VARS_UNIT 2				/* How large each element is in bytes (short = 2 bytes) */
/* Use this block to make it easy to manage the core variables */
typedef struct {
	/* Calculated and averaged from ADC0 readings */
	unsigned short IAT;			/* Inlet Air Temperature (MAT JS)			: 0.0	-    655.35		(0.01 Kelvin (/100))		*/
	unsigned short CHT;			/* Coolant / Head Temperature (CLT JS)		: 0.0	-    655.35		(0.01 Kelvin (/100))		*/
	unsigned short TPS;			/* Throttle Position Sensor (TPS JS)		: 0.0	-    102.39		(0.001? % (/640))			*/
	unsigned short EGO;			/* Exhaust Gas Oxygen (O2 JS)				: 0.000	-      1.999999	(0.0001? lambda (/32768))	*/
	unsigned short MAP;			/* Manifold Absolute Pressure (5euroh1)		: 0.0	-    655.35		(0.01 kPa (/100))			*/
	unsigned short AAP;			/* Atmospheric Absolute Pressure (6euroh1)	: 0.0	-    655.35		(0.01 kPa (/100))			*/
	unsigned short BRV;			/* Battery Reference Voltage (4euroh1)		: 0.000	-     65.535	(0.001 Volts (/1000))		*/
	unsigned short MAT;			/* Manifold Air Temperature (Spare JS)		: 0.0	-    655.35		(0.01 Kelvin (/100))		*/

	/* Calculated and averaged from ADC1 readings */
	unsigned short EGO2;		/* Exhaust Gas Oxygen (NC)					: 0.000	-      1.999999	(0.0001? lambda (/32768))	*/
	unsigned short IAP;			/* Intercooler Absolute Pressure (NC)		: 0.0	-    655.35		(0.01 kPa (/100))			*/
	unsigned short MAF;			/* Mass Air Flow							: 0.0	-  65535.0		(raw units from lookup)		*/

	/* Calculated from MAP and TPS history */
	unsigned short DMAP;		/* Delta MAP kPa/second or similar */
	unsigned short DTPS;		/* Delta TPS %/second or similar */

	/* Calculated from engine position data */
	unsigned short RPM;			/* Revolutions Per Minute (Calced)			: 0		-  32767.5		(0.5 RPM (/2))				*/
	unsigned short DRPM;		/* Delta RPM (Calced)						: 0		-  32767.5		(0.5 RPM/Second (/2))		*/
	unsigned short DDRPM;		/* Delta Delta RPM (Calced)					: 0		-  32767.5		(0.5 RPM/Second^2 (/2))		*/
} CoreVar;


#define ADC_ARRAY_SIZE sizeof(ADCArray)
#define ADC_ARRAY_LENGTH 16				/* How many arrays */
#define ADC_ARRAY_UNIT 2				/* How large each element is in bytes (short = 2 bytes) */
/* Use this block to ensure that the components are contiguous and we can then reference them via offsets and pointers */
typedef struct {
	/* ADC0 raw readings */
	unsigned short IAT;			/* Inlet Air Temperature (MAT JS)			*/ /* COMPULSORY! */
	unsigned short CHT;			/* Coolant / Head Temperature (CLT JS)		*/ /* COMPULSORY! */
	unsigned short TPS;			/* Throttle Position Sensor (TPS JS)		*/ /* Reduced performance without */
	unsigned short EGO;			/* Exhaust Gas Oxygen (O2 JS)				*/ /* Recommended */
	unsigned short MAP;			/* Manifold Absolute Pressure (5euroh1)		*/ /* COMPULSORY OR TPS OR MAF */
	unsigned short AAP;			/* Atmospheric Absolute Pressure (6euroh1)	*/ /* Recommended */
	unsigned short BRV;			/* Battery Reference Voltage (4euroh1)		*/ /* COMPULSORY! */
	unsigned short MAT;			/* Manifold Air Temperature (Spare JS)		*/ /* Could help heat soak issues */

	/* ADC1 raw readings */
	unsigned short EGO2;			/* Exhaust Gas Oxygen (NC)					*/ /* V engine option */
	unsigned short IAP;			/* Intercooler Absolute Pressure (NC)		*/ /* Turbo engine option */
	unsigned short MAF;			/* Mass Air Flow (NC)						*/ /* OEM engine option */
	unsigned short SpareADC3;		/* Spare ADC1 port 3 (NC)					*/
	unsigned short SpareADC4;		/* Spare ADC1 port 4 (NC)					*/
	unsigned short SpareADC5;		/* Spare ADC1 port 5 (NC)					*/
	unsigned short SpareADC6;		/* Spare ADC1 port 6 (NC)					*/
	unsigned short SpareADC7;		/* Spare ADC1 port 7 (NC)					*/
} ADCArray;


#define MAINTABLE_SIZE sizeof(mainTable)
#define MAINTABLE_RPM_LENGTH 24			/* How many cells on the X axis */
#define MAINTABLE_LOAD_LENGTH 19		/* How many cells on the Y axis */
#define MAINTABLE_MAX_RPM_LENGTH 27		/* How many cells on the X axis max */
#define MAINTABLE_MAX_LOAD_LENGTH 21	/* How many cells on the Y axis max */
#define MAINTABLE_MAX_MAIN_LENGTH 462	/* 924B 462 shorts maximum main table length */


/**	@struct maintable
 *
 * @brief Main Table Structure definition
 *
 * Use this block to manage the main VE, Lambda and Timing tables. Tables can
 * be made any size from 1x1 to 27x17 or 22x21 provided that the multiple of
 * the two doesn't exceed the max main length and neither axis exceeds it's
 * specified maximum.
 *
 * Currently the following tables use this structure:
 * - VETableMainFlash;
 * - VETableSecondaryFlash;
 * - VETableTertiaryFlash;
 * - LambdaTableFlash;
 * - IgnitionAdvanceTableMainFlash;
 * - IgnitionAdvanceTableSecondaryFlash;
 * - InjectionAdvanceTableMainFlash;
 * - InjectionAdvanceTableSecondaryFlash;
 * - VETableMainFlash2;
 * - VETableSecondaryFlash2;
 * - VETableTertiaryFlash2;
 * - LambdaTableFlash2;
 * - IgnitionAdvanceTableMainFlash2;
 * - IgnitionAdvanceTableSecondaryFlash2;
 * - InjectionAdvanceTableMainFlash2;
 * - InjectionAdvanceTableSecondaryFlash2;
 *
 * For a more detailed discussion of how this structure is actually used...
 *
 * @see lookupPagedMainTableCellValue
 *
 * @author Fred Cooke
 */
typedef struct {
	unsigned short RPMLength;						/* The length of the RPM axis array */
	unsigned short LoadLength;						/* The length of the Load axis array */
	unsigned short RPM[MAINTABLE_MAX_RPM_LENGTH];		/* The array of RPM (X) axis values */
	unsigned short Load[MAINTABLE_MAX_LOAD_LENGTH];		/* The array of Load (Y) axis values */
	unsigned short Table[MAINTABLE_MAX_MAIN_LENGTH];	/* The table as an array of values */
} mainTable;


#define TWODTABLEUS_SIZE sizeof(twoDTableUS)
#define TWODTABLEUS_LENGTH 16
/* This block used for various curves */
typedef struct {
	unsigned short Axis[TWODTABLEUS_LENGTH];
	unsigned short Values[TWODTABLEUS_LENGTH];
} twoDTableUS;


#define TWODTABLESS_SIZE sizeof(twoDTableSS)
#define TWODTABLESS_LENGTH 16
/* This block used for various curves */
typedef struct {
	signed short Axis[TWODTABLESS_LENGTH];
	signed short Values[TWODTABLESS_LENGTH];
} twoDTableSS;


#define TWODTABLEMS_SIZE sizeof(twoDTableMS)
#define TWODTABLEMS_LENGTH 16
/* This block used for various curves */
typedef struct {
	unsigned short Axis[TWODTABLEMS_LENGTH];
	signed short Values[TWODTABLEMS_LENGTH];
} twoDTableMS;


#define TWODTABLEUC_SIZE sizeof(twoDTableUC)
#define TWODTABLEUC_LENGTH 8
/* This block used for various curves */
typedef struct {
	unsigned char Axis[TWODTABLEUC_LENGTH];
	unsigned char Values[TWODTABLEUC_LENGTH];
} twoDTableUC;

/* ECUmanager error codes for protocol */
#define start_byte_inconsistency 1
#define stop_byte_inconsistency 2
#define escape_byte_inconsistency 3
#define checksum_error 4
#define payload_size_inconsistency 5


#endif // FREEEMSSTRUCTS_H
