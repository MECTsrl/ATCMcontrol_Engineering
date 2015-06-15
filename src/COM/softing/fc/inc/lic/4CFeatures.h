

//SPROD_4C_20_CL Feature bits: WBRRRRRR-RRRRRRRR-RRRRRRRR-RRRRIIII
// I:  number of IOs
// W:  Bundle bit, if set the CL license contains
//     also a PC-Engineering (bundle limited) license.
// B:  Bundle bit, ignored by control
//     If set the CL license contains also a workbenach
//     and a PC-Engineering bundle limited license.
// R:  Reserved Bits
//
// The W and B bit are used on registration to create a
// Workbench and/or PC-Engineering (bundle limited) license.
// The W/B bits are removed before actually registering the control

#define CL_FEAT_IOS_40    0x1  //IIII = 0001      40  IOs (starting edition)
#define CL_FEAT_IOS_100   0x2  //IIII = 0010     100  IOs
#define CL_FEAT_IOS_200   0x3  //IIII = 0011     200  IOs
#define CL_FEAT_IOS_500   0x4  //IIII = 0100     500  IOs
#define CL_FEAT_IOS_1K0   0x5  //IIII = 0101     1000 IOs
#define CL_FEAT_IOS_2K0   0x6  //IIII = 0110     2000 IOs
#define CL_FEAT_IOS_3K0   0x7  //IIII = 0111     3000 IOs
#define CL_FEAT_IOS_5K0   0x8  //IIII = 1000     5000 IOs
#define CL_FEAT_IOS_10K0  0x9  //IIII = 1001    10000 IOs

#define CL_FEAT_WORKBENCH 0x80000000
#define CL_FEAT_BUNDLE    0x40000000


// SPROD_4C_20_PC_EL feature bits: WRRRRRRR-RRRRRRRR-RRRRRRRR-RRRRRRRF
#define PC_EL_FEAT_WORKBENCH    0x80000000
#define PC_EL_FEAT_FULLVERSION  0x1
// The W bit is used on registration to create a Workbench license.
// If F is set: it is a full version, otherwise only local downloads
// and only one 4cpc target is allowed (bundle restriction)
// The W bit is removed before actually registering the PC-engineering

//SPROD_4C_20_EC_EL feature bits: WRRRRRRR-RRRRRRRR-RRRRRRRR-RRRRRRRF
#define EC_EL_FEAT_WORKBENCH   0x80000000
#define EC_EL_FEAT_FULLVERSION 0x1
// The W bit is used on registration to create a Workbench license.
// If F is set: it is a full version, otherwise only 6 Bool IOs and only 2
// 4cec Targets are allowed (starting edition restriction)
// The W bit is removed before actually registering the Ethernet-engineering

//SPROD_4C_20_FC_EL feature bits: WRRRRRRR-RRRRRRRR-RRRRRRRR-RRRRRRRF
#define FC_EL_FEAT_WORKBENCH	0x80000000
#define FC_EL_FEAT_FULLVERSION	0x1
// The W bit is used on registration to create a Workbench license.
// If F is set: it is a full version, otherwise only 6 Bool IOs and only 2
// 4cec Targets are allowed (starting edition restriction)
// The W bit is removed before actually registering the FieldController-engineering

//SPROD_4C_21_BC_EL feature bits: WRRRRRRR-RRRRRRRR-RRRRRRRR-RRRRRRRF
#define BC_EL_FEAT_WORKBENCH	0x80000000
#define BC_EL_FEAT_FULLVERSION	0x1
// The W bit is used on registration to create a Workbench license.

//SPROD_4C_21_CE_EL feature bits: WRRRRRRR-RRRRRRRR-RRRRRRRR-RRRRRRRF
#define CE_EL_FEAT_WORKBENCH	0x80000000
#define CE_EL_FEAT_FULLVERSION	0x1
// The W bit is used on registration to create a Workbench license.

//SPROD_4C_21_GA_EL feature bits: WRRRRRRR-RRRRRRRR-RRRRRRRR-RRRRRRRF
#define GA_EL_FEAT_WORKBENCH	0x80000000
#define GA_EL_FEAT_FULLVERSION	0x1
// The W bit is used on registration to create a Workbench license.


///////////////////////////////////////////////////////////////////////
// DTM features
///////////////////////////////////////////////////////////////////////
//SPROD_PROFIBUS_DTM feature bits:
#define PROFIBUS_DTM_FEAT_FULLVERSION	0x0	// 0000 Full Version
#define PROFIBUS_DTM_FEAT_SINGLECHANNEL	0x1 // 0001 Single Channel

// SPROD_FDT_LDDTM feature bits:	// DTM-HSE-LD
#define LDDTM_FEAT_FULLVERSION		0x0	// 0000 Full Version
#define LDDTM_FEAT_DEVICE_1		0x1 // IIII = 0001; 1 Linking Device
#define LDDTM_FEAT_DEVICE_2		0x2  //IIII = 0010; 2 Linking Devices
#define LDDTM_FEAT_DEVICE_3		0x3  //IIII = 0011; 3 Linking Devices
#define LDDTM_FEAT_DEVICE_4		0x4  //IIII = 0100; 4 Linking Devices
#define LDDTM_FEAT_DEVICE_5		0x5  //IIII = 0101; 5 Linking Devices
#define LDDTM_FEAT_DEVICE_6		0x6  //IIII = 0110; 6 Linking Devices
#define LDDTM_FEAT_DEVICE_7		0x7  //IIII = 0111; 7 Linking Devices
#define LDDTM_FEAT_DEVICE_8		0x8  //IIII = 1000; 8 Linking Devices
#define LDDTM_FEAT_DEVICE_9		0x9  //IIII = 1001; 9 Linking Devices
#define LDDTM_FEAT_DEVICE_10		0xA  //IIII = 1010; 10 Linking Devices
#define LDDTM_FEAT_DEVICE_20		0xB  //IIII = 1011; 20 Linking Devices
#define LDDTM_FEAT_DEVICE_30		0xC  //IIII = 1100; 30 Linking Devices
#define LDDTM_FEAT_DEVICE_40		0xD  //IIII = 1101; 40 Linking Devices
#define LDDTM_FEAT_DEVICE_50		0xE  //IIII = 1110; 50 Linking Devices
#define LDDTM_FEAT_DEVICE_100		0xF  //IIII = 1111; 100 Linking Devices

// SPROD_FDT_PALINKDTM feature bits:
#define PALINKDTM_FEAT_FULLVERSION	0x0  // 0000 Full Version
#define PALINKDTM_FEAT_PALINK_1		0x1  // IIII = 0001; 1   PA Link
#define PALINKDTM_FEAT_PALINK_5		0x2  // IIII = 0010; 5   PA Links
#define PALINKDTM_FEAT_PALINK_10	0x3  // IIII = 0011; 10  PA Links
#define PALINKDTM_FEAT_PALINK_50	0x4  // IIII = 0100; 50  PA Links
#define PALINKDTM_FEAT_PALINK_100	0x5  // IIII = 0101; 100 PA Links

// SPROD_FDT_ET200MDTM feature bits:
#define ET200MDTM_FEAT_FULLVERSION	0x0  // 0000 Full Version
#define ET200MDTM_FEAT_1	0x1  // IIII = 0001; 1   ET 200M
#define ET200MDTM_FEAT_5	0x2  // IIII = 0010; 5   ET 200M
#define ET200MDTM_FEAT_10	0x3  // IIII = 0011; 10  ET 200M
#define ET200MDTM_FEAT_50	0x4  // IIII = 0100; 50  ET 200M
#define ET200MDTM_FEAT_100	0x5  // IIII = 0101; 100 ET 200M

///////////////////////////////////////////////////////////////////////
// OCT (OPC Connector Tools) features
///////////////////////////////////////////////////////////////////////
//SPROD_OCT_CONCENTRATOR feature bits:
#define OCT_CONCENTRATOR_FEAT_10   0x01 //  10 Server
#define OCT_CONCENTRATOR_FEAT_50   0x02 //  50 Server
#define OCT_CONCENTRATOR_FEAT_100  0x03 // 100 Server

//SPROD_OCT_SERVER_CONNECTIONS feature bits:
#define SPROD_OCT_SERVER_CONNECTIONS_FEAT_10	0x01 //  10 Server
#define SPROD_OCT_SERVER_CONNECTIONS_FEAT_20	0x02 //  20 Server
#define SPROD_OCT_SERVER_CONNECTIONS_FEAT_50	0x03 //  50 Server
#define SPROD_OCT_SERVER_CONNECTIONS_FEAT_100	0x04 //  100 Server

//SPROD_OCT_COLLECTOR_ITEMS
#define SPROD_OCT_COLLECTOR_ITEMS_FEAT_250		0x01 //  250 Items
#define SPROD_OCT_COLLECTOR_ITEMS_FEAT_1000		0x02 //  1000 Items
#define SPROD_OCT_COLLECTOR_ITEMS_FEAT_2500		0x03 //  2500 Items
#define SPROD_OCT_COLLECTOR_ITEMS_FEAT_5000		0x04 //  5000 Items
#define SPROD_OCT_COLLECTOR_ITEMS_FEAT_10000	0x05 //  10000 Items
  
///////////////////////////////////////////////////////////////////////
// S7 OPC Server features
///////////////////////////////////////////////////////////////////////
//SPROD_S7_OPC_SERVER (22)
#define S7_FEAT_256	0x0  // IIII = 0000; full version (256 SPS)
#define S7_FEAT_8	0x1  // IIII = 0001; 8 SPS; default 
#define S7_FEAT_32	0x2  // IIII = 0010; 32 SPS
