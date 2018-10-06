#pragma once

// atari: collision; overlapping (e.g. of two objects in a video game)

// Object Collision (*.SCA) (Shape List) [Final/Retail]
#define Sca_hit_box			0	// Rectangle		//						// 
#define Sca_hit_naname_a	1	// Right Triangle	// x--	z--		\|		// 
#define Sca_hit_naname_b	2	// Right Triangle	// x++	z--		|/		// 
#define Sca_hit_naname_c	3	// Right Triangle	// x--	z++		/|		// 
#define Sca_hit_naname_d	4	// Right Triangle	// x++	z++		|\		// 
#define Sca_hit_hishi		5	// Rhombus			// |x/w| + |z/d| = 1	// Diamond
#define Sca_hit_circle		6	// Circle			//						// 
#define Sca_hit_koban_x		7	// Oval				// x=(-x,0), z=(z,0)	// Ellipse, Rectangle w/Rounded corners on X-Axis
#define Sca_hit_koban_z		8	// Oval				// x=(x,0), z=(-z,0)	// Ellipse, Rectangle w/Rounded corners on Z-Axis
#define Sca_hit_box_1		9	// Rectangle		// Climb Up				// Found in 304
#define Sca_hit_box_2		10	// Rectangle		// Jump Down			// Found in 304
#define Sca_hit_slope		11	// Reflex Angle		//						// Found in 200
#define Sca_hit_box_3		12	// Rectangle		// Stairs				// Found in 200
#define Sca_hit_curve		13	// Cylinder			//						// found in 40B and 40F

// Object Collision (*.SCA) (Shape List) [November 6, 1996]
#define Sca_hit_null		0	// (null)			// x=0, z=0, y=0		// 
#define Sca_hit_box_p		1	// Rectangle/Square	//						// 
#define Sca_hit_hishi_p		2	// Rhombus			// |x/w| + |z/d| = 1	// Diamond
#define Sca_hit_circle_p	3	// Circle			//						// 
#define Sca_hit_naname_a_p	4	// Right Triangle	// x--	z--		\|		// 
#define Sca_hit_naname_b_p	5	// Right Triangle	// x++	z--		|/		// 
#define Sca_hit_naname_c_p	6	// Right Triangle	// x--	z++		/|		// 
#define Sca_hit_naname_d_p	7	// Right Triangle	// x++	z++		|\		// 
#define Sca_hit_koban_x_p	8	// Oval				// x=(x,0), z=(-z,0)	// Ellipse (Rectangle w/Rounded corners on X-Axis)
#define Sca_hit_koban_z_p	9	// Oval				// x=(-x,0), z=(z,0)	// Ellipse (Rectangle w/Rounded corners on Z-Axis)

#pragma pack(push, 1)
typedef struct tagBio2RoomSca {
	SCA_HEAD Header;	// One (1) is subtracted from Header->Num in RdtOpen() function
	std::vector <SCA_DATA> Data;
} Bio2RoomSca;
typedef struct tagBio2RoomScaProtoHeader {
	SHORT Cx;
	SHORT Cz;
	ULONG nData0;
	ULONG nData1;
	ULONG nData2;
	ULONG nData3;
	ULONG nData4;
} Bio2RoomScaProtoHeader;
typedef struct tagBio2RoomScaProtoData {
	USHORT W;
	USHORT D;
	SHORT X;
	SHORT Z;
	union uId {
		USHORT Id;
		struct {
			USHORT shape : 4;
			USHORT unk0 : 4;	// 0x00 = Weapon Collision OFF (Desk, etc)
								// 0x08 = Weapon Collision ON (Wall, etc)
			USHORT unk1 : 1;	// 0x00 = nFloor Height OFF (Cannot Walk Under)
								// 0x01 = nFloor Height ON (Can Walk Under)
			USHORT unk2 : 1;	// 0x00 = 
								// 0x01 = 
			USHORT unk3 : 1;	// 0x00 = Enemy Collision OFF
								// 0x01 = Enemy Collision ON
			USHORT unk4 : 1;	// 0x00 = 
								// 0x01 = 
			USHORT unk5 : 1;	// 0x00 = 
								// 0x01 = 
			USHORT unk6 : 1;	// 0x00 = Bullet Collision OFF
								// 0x01 = Bullet Collision ON
			USHORT unk7 : 1;	// 0x00 = Object Collision OFF
								// 0x01 = Object Collision ON
			USHORT unk8 : 1;	// 0x00 = Player Collision OFF
								// 0x01 = Player Collision ON
		} s;
	} Id;
	union uType {
		USHORT Type;
		struct {
			USHORT h : 2;		// Width Multiplier X/W (0-3)
			USHORT y : 2;		// Depth Multiplier Z/D (0-3)
								// These values seem to determine if the
								// W/D values match or exceed the necessary
								// requirements for actual collision, where
								// the player comes into contact with said
								// collision data.
			USHORT Type : 2;	// Stair/Slope/Platform Access
								// 00	Climb/Ascend/Jump down from Z-Axis
								// 01	Climb/Ascend/Jump up from Z-Axis
								// 02	Climb/Ascend/Jump down from X-Axis
								// 03	Climb/Ascend/Jump up from X-Axis
			USHORT tFloor : 6;// Height Multiplier (0-9)
							  // This value determines both shadow and
							  // bullet deflection height (nFloor).
			USHORT Nine : 4;	// This value is always 9
								// There's only one exception in the entire
								// game - Retail ROOM104 (Wrecked Motorcycle)
		} s;
	} Type;
} Bio2RoomScaProtoData;
typedef struct tagBio2RoomScaProto {
	Bio2RoomScaProtoHeader Header;
	std::vector <Bio2RoomScaProtoData*> Data0;
	std::vector <Bio2RoomScaProtoData*> Data1;
	std::vector <Bio2RoomScaProtoData*> Data2;
	std::vector <Bio2RoomScaProtoData*> Data3;
	std::vector <Bio2RoomScaProtoData*> Data4;
	ULONG Size;	// Total File Size (*.SCA)
} Bio2RoomScaProto;
#pragma pack(pop)
