#pragma once

// Sony
#include "..\\..\\Sony\\sony_tim.h"
#include "..\\..\\Sony\\sony_vab.h"
#include "..\\..\\Sony\\sony_tmd.h"
#ifdef DIRECT_X
#include "..\\..\\Sony\\sony_dx9.h"
#endif

// Resident Evil 2
#include "..\\Model\\bio2_model.h"
#include "..\\Message\\bio2_message.h"
#include "..\\Script v2\\bio2_script.h"

// Editor
#include "..\\..\\Bio1\\Bio_Windows_Editor.h"

// Collision (Retail)
#define Sca_hit_box			0	// Square or Rectangle
#define Sca_hit_naname_a	1	// Oblique Triangle	// SouthEast
#define Sca_hit_naname_b	2	// Oblique Triangle	// SouthWest
#define Sca_hit_naname_c	3	// Oblique Triangle	// NorthWest
#define Sca_hit_naname_d	4	// Oblique Triangle	// NorthEast
#define Sca_hit_hishi		5	// Rhombus (diamond)
#define Sca_hit_circle		6	// Circle
#define Sca_hit_koban_x		7	// Oblong Circle (Square/Rect with Round Edges)
#define Sca_hit_koban_z		8	// Oblong Circle (Square/Rect with Round Edges)
#define Sca_hit_box_1		9	// Square or Rectangle (Climb Up, found in 304)
#define Sca_hit_box_2		10	// Square or Rectangle (Jump Down, found in 304)
#define Sca_hit_slope		11	// Oblique Angle (found in 200)
#define Sca_hit_box_3		12	// Square or Rectangle (Stairs, found in 200)
#define Sca_hit_curve		13	// Torus (found in 40B and 40F)

#define BIO2_MAX_CAMERAS		16
#define BIO2_MAX_SPRITE_GROUPS	32
#define BIO2_MAX_SPRITES		256
#define BIO2_MAX_ZONES			256
#define BIO2_MAX_COLLISIONS		128
#define BIO2_MAX_EFFECT			8

#pragma pack(push, 1)
typedef struct tagResident_Evil_2_Pri_Square {
	UCHAR u, v;
	UCHAR x, y;
	SHORT Depth;
	USHORT Size;
} Resident_Evil_2_Pri_Square;
typedef struct tagResident_Evil_2_Pri_Rect {
	UCHAR u, v;				// Texture U/V Coordinates
	UCHAR x, y;				// Screen X/Z Position
	SHORT Depth;			// Screen Z-Buffer Depth
	USHORT tPage;			// Texture Page (Bio2 Nov 96 only, zero otherwise)
	USHORT w, h;			// Texture Width and Height
} Resident_Evil_2_Pri_Rect;
typedef struct tagResident_Evil_2_Pri_Group {
	USHORT nSprite{};			// Amount of Sprites in this Group
	USHORT unk{};				// CLUT (Always 30720)
	SHORT AddX{};				// Add to Sprite.X for actual Screen Pos
	SHORT AddY{};				// Add to Sprite.Y for actual Screen Pos
	std::vector <Resident_Evil_2_Pri_Rect*> Sprite;
} Resident_Evil_2_Pri_Group;
typedef struct tagResident_Evil_2_Camera_Sprite {
	USHORT nGroup{};			// Amount of Groups (0xFFFF if no data)
	USHORT nSpriteTotal{};	// Amount of Sprites in all Groups (0xFFFF if no data)
	std::vector <Resident_Evil_2_Pri_Group*> Group;
	ULONG Size{};				// Total Size (Not included in data)
} Resident_Evil_2_Camera_Sprite;

typedef struct tagResident_Evil_2_Camera_Data {
	USHORT end_flg;
	USHORT ViewR;
	LONG View_p[3];
	LONG View_r[3];
	ULONG pSp;
} Resident_Evil_2_Camera_Data;
typedef struct tagResident_Evil_2_Angle_Data {
	UCHAR Be_flg;
	CHAR nFloor;
	UCHAR Fcut;
	UCHAR Tcut;
	SHORT Xz[4][2];
} Resident_Evil_2_Angle_Data;
typedef struct tagResident_Evil_2_Light_Data {
	UCHAR Mag;
	UCHAR Mode[3];
	UCHAR Col[3][3];
	UCHAR Ambient[3];
	SHORT Pos[3][3];
	USHORT L[3];
} Resident_Evil_2_Light_Data;
typedef struct tagResident_Evil_2_Collision_Data {
	SHORT X;
	SHORT Z;
	USHORT W;
	USHORT D;
	union uId {
		USHORT Id;
		struct {
			USHORT Shape : 4;		// 0	// Square or Rectangle
									// 1	// Oblique Triangle	(SouthEast) \|
									// 2	// Oblique Triangle	(SouthWest) |/
									// 3	// Oblique Triangle	(NorthWest) /|
									// 4	// Oblique Triangle	(NorthEast) |\
									// 5	// Rhombus (Diamond)
									// 6	// Circle
									// 7	// Oval (Square/Rect with Round Edges on X-Axis)
									// 8	// Oval (Square/Rect with Round Edges on Z-Axis)
									// 9	// Square or Rectangle (Climb Up)
									// 10	// Square or Rectangle (Jump Down)
									// 11	// Slope (Oblique Angle)
									// 12	// Square or Rectangle (Stairs)
									// 13	// Curve (Sloped Pipe, found in 40B and 40F)
			USHORT unk0 : 4;		// 0x00 = Weapon Collision OFF (Desk, etc)
									// 0x08 = Weapon Collision ON (Wall, etc)
			USHORT unk1 : 1;		// 0x00 = nFloor Height OFF (Cannot Walk Under)
									// 0x01 = nFloor Height ON (Can Walk Under)
			USHORT unk2 : 1;		// 0x00 = 
									// 0x01 = 
			USHORT unk3 : 1;		// 0x00 = Enemy Collision OFF
									// 0x01 = Enemy Collision ON
			USHORT unk4 : 1;		// 0x00 = 
									// 0x01 = 
			USHORT unk5 : 1;		// 0x00 = 
									// 0x01 = 
			USHORT unk6 : 1;		// 0x00 = Bullet Collision OFF
									// 0x01 = Bullet Collision ON
			USHORT unk7 : 1;		// 0x00 = Object Collision OFF
									// 0x01 = Object Collision ON
			USHORT unk8 : 1;		// 0x00 = Player Collision OFF
									// 0x01 = Player Collision ON
		} s;
	} Id;
	union uType {
		USHORT Type;
		struct {
			USHORT Area : 4;		// Axis Area(s) of Collision
									// This value is generated using axis center point areas and should never be set manually
									// Axis center point areas:
									//
									//		2 | 1
									//		-----
									//		8 | 4
									//
									// Each value is OR'd together where the collision's bottom-left, top-left, bottom-right,
									// and top-right points are located in the axis center point areas, with some caveats.
									//
									// Formula:
									//		ULONG Sca_get_area(LONG X, LONG Z, LONG Sx, LONG Sz) {
									//			return (1 << ((UINT)(X - Sx) >> 0x1f)) << ((UINT)(Z - Sz) >> 0x1e & 2);
									//		}
									//		ULONG Area0 = Sca_get_area((X - 1800), (Z - 1800), Header->Cx, Header->Cz);
									//		ULONG Area1 = Sca_get_area((X + (W + 1800)), (Z - 1800), Header->Cx, Header->Cz);
									//		ULONG Area2 = Sca_get_area((X - 1800), (Z + (D + 1800)), Header->Cx, Header->Cz);
									//		ULONG Area4 = Sca_get_area((X + (W + 1800)), (Z + (D + 1800)), Header->Cx, Header->Cz);
									//		Area = (Area0 | Area1 | Area2 | Area3);

			USHORT StairWD : 1;		// Stair/Slope High
									// 00	// High = X (if StairXZ=0) or Z (if StairXZ=1)
											// Low = X+W (if StairXZ=0) or Z+D (if StairXZ=1)
									// 01	// High = X+W (if StairXZ=0) or Z+D (if StairXZ=1)
											// Low = X (if StairXZ=0) or Z (if StairXZ=1)

			USHORT StairXZ : 1;		// Stair/Slope Access
									// 00	// Horizontal (X-Axis)
									// 01	// Vertical (Z-Axis)

			USHORT nFloor : 6;		// Height in units of -1800
									// Accepted values are 0-31 (Range: 0, -57600)
									// Used by the Sca_get_high function to obtain height of collision
									// Actual Height = ((Height * -200) + (nFloor * -1800));
									// This value is irregular in ROOM104:
									//		 Wrecked Vehicles (Value: 33)

			USHORT Height : 4;		// Height in units of -200
									// Accepted values are 0-15 (Range: 0, -3000)
									// Used by the Sca_get_high function to obtain height of collision
									// This value is normally set to 9 by default to ensure the height is at least -1800
									// This value is irregular in ROOM104:
									//		 Wrecked Vehicles (Value: 2)
									//		 Wrecked Motorcycle (Value: 1)
		} s;
	} Type;
	ULONG Floor;					// Ground in units of -1800
									// Accepted bits are 0-31 (Range: 0, -55800)
									// Used by the Sca_get_low function to obtain ground of collision
									//
									// Formula:
									//		Floor = 0;	// Reset to null before setting
									// 
									//		if (!Bit) {		// Ground height is zero (0)
									//			ULONG High = ((Height * -200) + (nFloor * -1800));
									//			High /= -1800;
									//			if (!High) { Floor = 1; }	// Floor variable cannot be zero (0)
									//			else for (ULONG nBit = NULL; nBit < High; nBit++) { Floor |= 1 << nBit; }
									//		}
									//
									//		// Collision is not at ground level (Ceiling, lifted platform, etc)
									//		else Floor |= 1 << Bit;
} Resident_Evil_2_Collision_Data;
typedef struct tagResident_Evil_2_Collision_Header {
	SHORT Cx;		// Axis Center Point - Used to generate "Area" bitfield of "Type" variable
	SHORT Cz;		// Axis Center Point - Used to generate "Area" bitfield of "Type" variable
	ULONG nData;	// Collision Amount (-1 for actual amount)
	LONG Ceiling;	// 
	ULONG Color;	// RGB color of collision; unused, probably dev leftovers
} Resident_Evil_2_Collision_Header;
typedef struct tagResident_Evil_2_Collision {
	Resident_Evil_2_Collision_Header Header{};
	std::vector <Resident_Evil_2_Collision_Data*> Data;
} Resident_Evil_2_Collision;
typedef struct tagResident_Evil_2_Floor_Data {
	SHORT X;
	SHORT Z;
	USHORT W;
	USHORT D;
	USHORT Se_no;
	USHORT Floor_height;
} Resident_Evil_2_Floor_Data;
typedef struct tagResident_Evil_2_Block_Data {
	SHORT Pos_x;
	SHORT Pos_z;
	SHORT Size_x;
	SHORT Size_z;
	SHORT X;
	USHORT dBit;
} Resident_Evil_2_Block_Data;
typedef struct tagResident_Evil_2_Object_Model {
	std::vector <SonyTexture*> Tim;
	std::vector <RESIDENT_EVIL_2_MODEL*> Md1;
	std::vector <ULONG> pMd1;
	std::vector <ULONG> pTim;
	std::vector <size_t> Md1Len;
	std::vector <ULONG> TimLen;
	std::vector <BOOL> ZeroMd1;
	std::vector <BOOL> ZeroTim;
	std::vector <PS1DX9_MODEL*> Dx9;
} Resident_Evil_2_Object_Model;
typedef struct tagResident_Evil_2_Effect_Sprite {
	std::vector <UCHAR*> Id;
	std::vector <UCHAR*> Data;
	std::vector <ULONG> pData;
	std::vector <ULONG> DataLen;
	std::vector <ULONG> pTim;
	std::vector <SonyTexture*> Tim;
} Resident_Evil_2_Effect_Sprite;
typedef union tagResident_Evil_2_Edt_Data {
	UINT Value;
	struct {
		UINT Unknown : 16;	// Always Zero
		UINT tone_pri : 4;
		UINT id_check : 4;	// VAG ID (sound sample located in VAB)
		UINT monopoly : 4;
		UINT pan_prog : 4;
	} s;
} Resident_Evil_2_Edt_Data;

typedef struct tagResident_Evil_2_Nov96_Collision_Header {
	SHORT Cx;
	SHORT Cz;
	UINT nData0;
	UINT nData1;
	UINT nData2;
	UINT nData3;
	UINT nData4;
} Resident_Evil_2_Nov96_Collision_Header;
typedef struct tagResident_Evil_2_Nov96_Collision_Data {
	USHORT W;
	USHORT D;
	SHORT X;
	SHORT Z;
	union uId {
		USHORT Id;
		struct {
			USHORT Shape : 8;		// 0 = *unused*
									// 1 = Box
									// 2 = Hishi
									// 3 = Circle
									// 4 = Naname A
									// 5 = Naname B
									// 6 = Naname C
									// 7 = Naname D
									// 8 = Koban X
									// 9 = Koban Z

			USHORT unk : 8;			// 0 = ???
									// 1 = ???
		} s;
	} Id;
	union uType {
		USHORT Type;
		struct {
			USHORT Climb : 8;			// 1 = Jump Down (Sca_hit_box_2)
										// 2 = Climb Up (Sca_hit_box_1)
										// E = 
			USHORT High : 4;			// Height (* -1800 + Low)
			USHORT Low : 4;				// Ground (* -1800)
		} s;
	} Type;
} Resident_Evil_2_Nov96_Collision_Data;
typedef struct tagResident_Evil_2_Nov96_Collision {
	Resident_Evil_2_Nov96_Collision_Header Header{};
	std::vector <Resident_Evil_2_Nov96_Collision_Data*> Data0;
	std::vector <Resident_Evil_2_Nov96_Collision_Data*> Data1;
	std::vector <Resident_Evil_2_Nov96_Collision_Data*> Data2;
	std::vector <Resident_Evil_2_Nov96_Collision_Data*> Data3;
	std::vector <Resident_Evil_2_Nov96_Collision_Data*> Data4;
} Resident_Evil_2_Nov96_Collision;

typedef struct tagResident_Evil_2_Rdt_Nov96_Header {
	UCHAR nSprite;		// XX // 0x00
	UCHAR nCut;			// XX // 0x01
	UCHAR nOmodel;		// XX // 0x02
	UCHAR nItem;		// XX // 0x03
	UCHAR nDoor;		// XX // 0x04
	UCHAR nRoom_at;		// XX // 0x05
	UCHAR Reverb_lv;	// XX // 0x06
	UCHAR nSprite_max;	// XX // 0x07
	ULONG pEdt0;		//  0 // 0x08
	ULONG pVh0;			//  1 // 0x0C
	ULONG pVb0;			//  2 // 0x10
	ULONG pEdt1;		//  3 // 0x14
	ULONG pVh1;			//  4 // 0x18
	ULONG pVb1;			//  5 // 0x1C
	ULONG pSca;			//  6 // 0x20
	ULONG pRcut;		//  7 // 0x24
	ULONG pVcut;		//  8 // 0x28
	ULONG pLight;		//  9 // 0x2C
	ULONG pOmodel;		// 10 // 0x30
	ULONG pFloor;		// 11 // 0x34
	ULONG pBlock;		// 12 // 0x38
	ULONG pMessage;		// 13 // 0x3C
	ULONG pScdx;		// 14 // 0x40
	ULONG pScd0;		// 15 // 0x44
	ULONG pScd1;		// 16 // 0x48
	ULONG pEsp_hed;		// 17 // 0x4C
	ULONG pEsp_end;		// 18 // 0x50
	ULONG pEsp_tim;		// 19 // 0x54
	ULONG pEsp_tim_end;	// 20 // 0x58
	ULONG pRbj;			// 21 // 0x5C
} Resident_Evil_2_Rdt_Nov96_Header;
typedef struct tagResident_Evil_2_Rdt_Trial_Header {
	UCHAR nSprite;		// XX // 0x00
	UCHAR nCut;			// XX // 0x01
	UCHAR nOmodel;		// XX // 0x02
	UCHAR nItem;		// XX // 0x03
	UCHAR nDoor;		// XX // 0x04
	UCHAR nRoom_at;		// XX // 0x05
	UCHAR Reverb_lv;	// XX // 0x06
	UCHAR nSprite_max;	// XX // 0x07
	ULONG pEdt0;		//  0 // 0x08
	ULONG pVh0;			//  1 // 0x0C
	ULONG pEdt1;		//  2 // 0x10
	ULONG pVh1;			//  3 // 0x14
	ULONG pVb;			//  4 // 0x18
	ULONG pRbj_end;		//  5 // 0x1C
	ULONG pSca;			//  6 // 0x20
	ULONG pRcut;		//  7 // 0x24
	ULONG pVcut;		//  8 // 0x28
	ULONG pLight;		//  9 // 0x2C
	ULONG pOmodel;		// 10 // 0x30	// tagMODEL_LINK_INFO
	ULONG pFloor;		// 11 // 0x34
	ULONG pBlock;		// 12 // 0x38
	ULONG pMessage;		// 13 // 0x3C
	ULONG pMessage_sub;	// 14 // 0x40
	ULONG pScrl;		// 15 // 0x44
	ULONG pScdx;		// 16 // 0x48
	ULONG pScd;			// 17 // 0x4C
	ULONG pEsp_hed;		// 18 // 0x50
	ULONG pEsp_end;		// 19 // 0x54
	ULONG pEsp_tim;		// 20 // 0x58
	ULONG pEsp_tim_end;	// 21 // 0x5C
	ULONG pRbj;			// 22 // 0x60
} Resident_Evil_2_Rdt_Trial_Header;
typedef struct tagResident_Evil_2_Rdt_Header {
	UCHAR nSprite;		// XX // 0x00
	UCHAR nCut;			// XX // 0x01
	UCHAR nOmodel;		// XX // 0x02
	UCHAR nItem;		// XX // 0x03
	UCHAR nDoor;		// XX // 0x04
	UCHAR nRoom_at;		// XX // 0x05
	UCHAR Reverb_lv;	// XX // 0x06
	UCHAR nSprite_max;	// XX // 0x07
	ULONG pEdt;			//  0 // 0x08
	ULONG pVh;			//  1 // 0x0C
	ULONG pVb;			//  2 // 0x10
	ULONG pZero0;		//  3 // 0x14 // always zero (0)
	ULONG pZero1;		//  4 // 0x18 // always zero (0)
	ULONG pRbj_end;		//  5 // 0x1C
	ULONG pSca;			//  6 // 0x20
	ULONG pRcut;		//  7 // 0x24
	ULONG pVcut;		//  8 // 0x28
	ULONG pLight;		//  9 // 0x2C
	ULONG pOmodel;		// 10 // 0x30 // tagMODEL_LINK_INFO
	ULONG pFloor;		// 11 // 0x34
	ULONG pBlock;		// 12 // 0x38
	ULONG pMessage;		// 13 // 0x3C
	ULONG pMessage_sub;	// 14 // 0x40
	ULONG pScrl;		// 15 // 0x44
	ULONG pScdx;		// 16 // 0x48	// System function (called only once when setting a room)
	ULONG pScd;			// 17 // 0x4C	// System function (called every loop)
	ULONG pEsp_hed;		// 18 // 0x50
	ULONG pEsp_end;		// 19 // 0x54
	ULONG pEsp_tim;		// 20 // 0x58
	ULONG pEsp_tim_end;	// 21 // 0x5C
	ULONG pRbj;			// 22 // 0x60
} Resident_Evil_2_Rdt_Header;
#pragma pack(pop)

// Generic Data
typedef struct tagResident_Evil_2_Data {
	std::vector <ULONG> pData;		// Absolute pointer to data (for debugging)
	std::vector <UCHAR*> Data;		// Data buffer
	std::vector <ULONG> Len;		// Data size
} Resident_Evil_2_Data;
typedef struct tagResident_Evil_2_Script_Data {
	std::vector <ULONG> pFile;			// Absolute pointer to data (for debugging)
	std::vector <UCHAR*> File;			// Data buffer
	std::vector <ULONG> FileLen;		// Data size
	std::vector <UCHAR*> Bytecode;		// Buffer for one file only	// Bytecode elements of SCD file
	std::vector <ULLONG> pBytecode;		// Buffer for one file only	// Absolute pointer to all Bytecode elements in file (from start of data)
	std::vector <ULLONG> pGotoIndex;	// Buffer for one file only	// pBytecode index where "Goto" label should precede
} Resident_Evil_2_Script_Data;

enum class Resident_Evil_2_Rdt_Pointer {
	pRE2_OBJECT_POINTER,	// Object Model
	pRE2_VCUT,				// Camera Switch
	pRE2_LIT,				// Light
	pRE2_PRI,				// Camera Sprite
	pRE2_SCA,				// Collision
	pRE2_BLOCK,				// AI Guide
	pRE2_FLOOR,				// Floor Sound
	pRE2_SCD0,				// Script Init	// System function (called only once when setting a room)
	pRE2_SCD1,				// Script Init	// System function (called every loop)
	pRE2_MSG0,				// Message Text
	pRE2_MSG1,				// Message Text
	pRE2_SCROLL,			// 320x240 Unheadered TIM
	pRE2_OBJECT_MODEL,		// Object Model
	pRE2_ESP_HED,			// Effect Sprite Header
	pRE2_ESP_END,			// Effect Sprite Data
	pRE2_RBJ,				// Animation
	pRE2_RBJ_END,			// Animation
	pRE2_EDT,				// Sound Attributes
	pRE2_VH,				// VAB Header
	pRE2_VB,				// VAB Sample
	pRE2_ESP_TIM,			// Effect Sprite Texture
	pRE2_ESP_TIM_END,		// Effect Sprite Texture
	pRE2_OBJECT_TEXTURE,	// Object Model (TIM)
	pRE2_RDT_FILESIZE,		// End Of File
};

class Resident_Evil_2_Room {
private:

	// System
	System_File ISystem_File, * File;
	System_Text ISystem_Text, * Text;

	// Sony
	Sony_PlayStation_Texture ISony_PlayStation_Texture;
	Sony_PlayStation_Soundbank ISony_PlayStation_Soundbank, * Soundbank;

	// Debug
	BOOL DebugLog(VOID);
	BOOL DebugLogNov96(VOID);
	BOOL DebugSprite(VOID);

	// Camera
	BOOL OpenCamera(_iobuf* FILE, ULONG _Offset);
	BOOL SaveCamera(_iobuf* FILE, ULONG _Offset);

	// Camera Sprite
	UINT GetSpriteMax(VOID);
	ULONG GetFirstSpritePtr(VOID);
	BOOL OpenSprite(BOOL UseSpritePointers, _iobuf* FILE, ULONG _Offset);
	BOOL SaveSprite(BOOL UpdateSpritePointers, _iobuf* FILE, ULONG _Offset);
	VOID DummySprite(VOID);

	// Zone
	BOOL OpenZone(_iobuf* FILE, ULONG _Offset);
	BOOL SaveZone(_iobuf* FILE, ULONG _Offset);

	// Light
	BOOL OpenLight(_iobuf* FILE, ULONG _Offset);
	BOOL SaveLight(_iobuf* FILE, ULONG _Offset);

	// Collision
	BOOL OpenCollision(_iobuf* FILE, ULONG _Offset);
	BOOL SaveCollision(_iobuf* FILE, ULONG _Offset);

	// Block
	BOOL OpenBlock(_iobuf* FILE, ULONG _Offset);
	BOOL SaveBlock(_iobuf* FILE, ULONG _Offset);

	// Floor
	BOOL OpenFloor(_iobuf* FILE, ULONG _Offset);
	BOOL SaveFloor(_iobuf* FILE, ULONG _Offset);

	// Script
	BOOL OpenScript(Resident_Evil_2_Script_Data& Container, _iobuf* FILE, ULONG _Offset);
	BOOL SaveScript(Resident_Evil_2_Script_Data& Container, _iobuf* FILE, ULONG _Offset);

	// Message
	BOOL OpenMessage(Resident_Evil_2_Data& Container, _iobuf* FILE, ULONG _Offset);
	BOOL SaveMessage(Resident_Evil_2_Data& Container, _iobuf* FILE, ULONG _Offset);

	// Model
	VOID GetModelLinks(_iobuf* FILE);

	// File
	VOID ParseNov96Header(VOID);
	VOID ParseTrialHeader(VOID);
	BOOL Open(_iobuf* FILE);

	// Create
	BOOL Assemble(USHORT nMessage, USHORT nMessage_sub, USHORT nScdX, USHORT nScd, USHORT nEsp);

#ifdef DIRECT_X

	// Collision
	VOID DrawBox(SHORT X, SHORT Z, USHORT W, USHORT D, LONG Low, LONG High, DWORD Color, BOOL bShape);
	VOID DrawHishi(SHORT X, SHORT Z, USHORT W, USHORT D, LONG Low, LONG High, DWORD Color, BOOL bShape);
	VOID DrawCircle(SHORT X, SHORT Z, USHORT W, USHORT D, LONG Low, LONG High, DWORD Color, BOOL bShape);
	VOID DrawNanameA(SHORT X, SHORT Z, USHORT W, USHORT D, LONG Low, LONG High, DWORD Color, BOOL bShape);
	VOID DrawNanameB(SHORT X, SHORT Z, USHORT W, USHORT D, LONG Low, LONG High, DWORD Color, BOOL bShape);
	VOID DrawNanameC(SHORT X, SHORT Z, USHORT W, USHORT D, LONG Low, LONG High, DWORD Color, BOOL bShape);
	VOID DrawNanameD(SHORT X, SHORT Z, USHORT W, USHORT D, LONG Low, LONG High, DWORD Color, BOOL bShape);
	VOID DrawKobanXZ(SHORT X, SHORT Z, USHORT W, USHORT D, USHORT Floor, DWORD Color);

	VOID DrawShapeHishi(SHORT X, SHORT Z, USHORT W, USHORT D, LONG Low, LONG High, DWORD Color);
	VOID DrawShapeNanameA(SHORT X, SHORT Z, USHORT W, USHORT D, LONG Low, LONG High, DWORD Color);
	VOID DrawShapeNanameB(SHORT X, SHORT Z, USHORT W, USHORT D, LONG Low, LONG High, DWORD Color);
	VOID DrawShapeNanameC(SHORT X, SHORT Z, USHORT W, USHORT D, LONG Low, LONG High, DWORD Color);
	VOID DrawShapeNanameD(SHORT X, SHORT Z, USHORT W, USHORT D, LONG Low, LONG High, DWORD Color);
	VOID DrawShapeStairs(Resident_Evil_2_Collision_Data* Data, DWORD Color, BOOL bShape);

#endif

public:

	// Sony
	Sony_PlayStation_Texture * Texture;
#ifdef DIRECT_X
	Sony_PlayStation_DX9 * Sony;
#endif

	// Resident Evil 2
	Resident_Evil_2_Animation IResident_Evil_2_Animation, * Anim;
	Resident_Evil_2_Model IResident_Evil_2_Model, * Model;
	Resident_Evil_2_Message IResident_Evil_2_Message, * Message;
	Resident_Evil_2_Script IResident_Evil_2_Script, * Script;

	// Flag
	BOOL IsOpen;
	BOOL IsNov96;
	BOOL TrialEdition;

	// Meta
	WCHAR* Directory;
	WCHAR* NameBase;
	UINT Stage_no;
	UINT Room_no;
	UINT Pl_id;
	ULONG Filesize;

	// Data
	Resident_Evil_2_Rdt_Nov96_Header Nov96{};
	Resident_Evil_2_Rdt_Trial_Header Trial{};
	Resident_Evil_2_Rdt_Header Header{};
	std::vector <Resident_Evil_2_Camera_Data*> Rid;
	Resident_Evil_2_Camera_Sprite* Pri;
	std::vector <Resident_Evil_2_Angle_Data*> Rvd;
	std::vector <Resident_Evil_2_Light_Data*> Lit;
	Resident_Evil_2_Nov96_Collision ScaNov96;
	Resident_Evil_2_Collision Sca;
	std::vector <Resident_Evil_2_Block_Data*> Blk;
	std::vector <Resident_Evil_2_Floor_Data*> Flr;
	Resident_Evil_2_Data Message0;
	Resident_Evil_2_Data Message1;
	Resident_Evil_2_Script_Data Script0;
	Resident_Evil_2_Script_Data Script1;
	Resident_Evil_2_Script_Data Script2;
	SonyTexture* Scroll;
	Resident_Evil_2_Object_Model Object;
	Resident_Evil_2_Effect_Sprite Esp;
	Resident_Evil_2_Animation_Container* Rbj;
	std::vector <Resident_Evil_2_Edt_Data*> Edt0;
	std::vector <Resident_Evil_2_Edt_Data*> Edt1;
	SonySoundbank* Vab0;
	SonySoundbank* Vab1;

	// Editor
	Resident_Evil_Windows_Editor* Editor;

	// Boot
	Resident_Evil_2_Room(VOID) {
		File = &ISystem_File;
		Text = &ISystem_Text;
		Texture = &ISony_PlayStation_Texture;
		Soundbank = &ISony_PlayStation_Soundbank;
#ifdef DIRECT_X
		Sony = nullptr;
#endif
		Pri = nullptr;
		Anim = &IResident_Evil_2_Animation;
		Message = &IResident_Evil_2_Message;
		Model = &IResident_Evil_2_Model;
		Script = &IResident_Evil_2_Script;
		IsOpen = FALSE;
		IsNov96 = FALSE;
		TrialEdition = FALSE;
		Directory = nullptr;
		NameBase = nullptr;
		Stage_no = NULL;
		Room_no = NULL;
		Pl_id = NULL;
		Filesize = NULL;
		Editor = nullptr;
		Scroll = nullptr;
		Rbj = nullptr;
		Vab0 = nullptr;
		Vab1 = nullptr;
	}
	virtual ~Resident_Evil_2_Room(VOID) {}

	// Camera
	BOOL OpenCamera(CONST WCHAR* _Filename, ...);
	BOOL SaveCamera(CONST WCHAR* _Filename, ...);
	BOOL CreateCamera(VOID);
	BOOL DeleteCamera(UINT iCut);
	BOOL CopyCamera(UINT iCut, Resident_Evil_2_Camera_Data& Buffer);
	BOOL PasteCamera(UINT iCut, Resident_Evil_2_Camera_Data& Buffer);

	// Camera Sprite
	BOOL DeleteSprite(UINT iCut);
	BOOL CreateSpriteGroup(UINT iCut, UINT iGroup);
	BOOL CreateSpriteEntry(UINT iCut, UINT iGroup, UINT iEntry);
	BOOL DeleteSpriteGroup(UINT iCut, UINT iGroup);
	BOOL DeleteSpriteEntry(UINT iCut, UINT iGroup, UINT iEntry);
	BOOL OpenSprite(CONST WCHAR* _Filename, ...);
	BOOL SaveSprite(CONST WCHAR* _Filename, ...);
	VOID CloseSprite(VOID);
	BOOL CombinePri(VOID);

	// Zone
	VOID SortZones(VOID);
	BOOL OpenZone(CONST WCHAR* _Filename, ...);
	BOOL SaveZone(CONST WCHAR* _Filename, ...);
	BOOL CreateZone(UINT iCut);
	BOOL DeleteZone(UINT iCut);
	BOOL DeleteZone(UINT iCut, UINT Entry);
	BOOL CopyZone(UINT iCut, UINT Entry, Resident_Evil_2_Angle_Data& Buffer);
	BOOL PasteZone(UINT iCut, UINT Entry, Resident_Evil_2_Angle_Data& Buffer);
	UINT GetZoneAmount(UINT iCut, UINT* iElement = NULL);

	// Light
	BOOL OpenLight(CONST WCHAR* _Filename, ...);
	BOOL SaveLight(CONST WCHAR* _Filename, ...);
	BOOL CreateLight(VOID);
	BOOL DeleteLight(UINT iCut);
	BOOL CopyLight(UINT iCut, Resident_Evil_2_Light_Data& Buffer);
	BOOL PasteLight(UINT iCut, Resident_Evil_2_Light_Data& Buffer);

	// Collision
	VOID ScaSort(VOID);
	size_t ScaCount(VOID);
	size_t ScaGroupCount(UINT Group);
	BOOL OpenCollision(CONST WCHAR* _Filename, ...);
	BOOL SaveCollision(CONST WCHAR* _Filename, ...);
	BOOL CreateCollision(VOID);
	BOOL DeleteCollision(UINT Entry);
	BOOL CopyCollision(UINT Entry, Resident_Evil_2_Collision_Data& Buffer);
	BOOL PasteCollision(UINT Entry, Resident_Evil_2_Collision_Data& Buffer);
	BOOL CreateCollisionNov96(UINT Group);
	BOOL DeleteCollisionNov96(UINT Group, UINT Entry);
	BOOL CopyCollisionNov96(UINT Group, UINT Entry, Resident_Evil_2_Nov96_Collision_Data& Buffer);
	BOOL PasteCollisionNov96(UINT Group, UINT Entry, Resident_Evil_2_Nov96_Collision_Data& Buffer);
	VOID CloseCollision(VOID);
	VOID ScaCalcCenter(VOID);
	ULONG ScaGetArea(LONG X, LONG Z, LONG Sx, LONG Sz);
	VOID ScaSetArea(Resident_Evil_2_Collision_Header* Header, Resident_Evil_2_Collision_Data* Data);
	LONG ScaGetHigh(Resident_Evil_2_Collision_Data* Data);
	LONG ScaGetLow(Resident_Evil_2_Collision_Data* Data);
	LONG ScaGetHighLow96(Resident_Evil_2_Nov96_Collision_Data* Data, BOOL HiLo);
	ULONG ScaGetFloorBit(Resident_Evil_2_Collision_Data* Data);
	ULONG ScaSetFloorBit(Resident_Evil_2_Collision_Data* Data, ULONG Bit);

	// Block
	BOOL* GetBlockBits(UINT Entry);
	BOOL SetBlockBits(UINT Entry);
	BOOL OpenBlock(CONST WCHAR* _Filename, ...);
	BOOL SaveBlock(CONST WCHAR* _Filename, ...);
	BOOL CreateBlock(VOID);
	BOOL DeleteBlock(UINT Entry);
	BOOL CopyBlock(UINT Entry, Resident_Evil_2_Block_Data& Buffer);
	BOOL PasteBlock(UINT Entry, Resident_Evil_2_Block_Data& Buffer);

	// Floor
	BOOL OpenFloor(CONST WCHAR* _Filename, ...);
	BOOL SaveFloor(CONST WCHAR* _Filename, ...);
	BOOL CreateFloor(VOID);
	BOOL DeleteFloor(UINT Entry);
	BOOL CopyFloor(UINT Entry, Resident_Evil_2_Floor_Data& Buffer);
	BOOL PasteFloor(UINT Entry, Resident_Evil_2_Floor_Data& Buffer);

	// Script
	BOOL OpenScript(Resident_Evil_2_Script_Data& Container, CONST WCHAR* _Namebase, CONST WCHAR* _Directory, ...);
	BOOL SaveScript(Resident_Evil_2_Script_Data& Container, BOOL Disassemble, CONST WCHAR* _Namebase, CONST WCHAR* _Directory, ...);
	BOOL DisassembleScripts(CONST WCHAR* _Filename, ...);

	// Message
	BOOL OpenMessage(Resident_Evil_2_Data& Container, CONST WCHAR* _Namebase, CONST WCHAR* _Directory, ...);
	BOOL SaveMessage(Resident_Evil_2_Data& Container, BOOL DisassembleToBMP, BOOL DisassembleToTXT, CONST WCHAR* _Namebase, CONST WCHAR* _Directory, ...);
	BOOL DisassembleMessages(CONST WCHAR* _Filename, ...);

	// Model
	BOOL AddModel(CONST WCHAR* Modelname, CONST WCHAR* Texturename);
	BOOL DeleteModel(UINT iEntry);
	BOOL ReplaceModel(UINT iEntry, CONST WCHAR* Modelname);
	BOOL ReplaceTexture(UINT iEntry, CONST WCHAR* Texturename);

	// File
	BOOL Open(CONST WCHAR* _Filename, ...);
	VOID Close(BOOL bIsOpen);

	// Create
	ULONG GetHeaderPointer(Resident_Evil_2_Rdt_Pointer Id);
	BOOL Assemble(CONST WCHAR* _Filename, ...);
	BOOL SaveConfig(VOID);
	BOOL Save(VOID);

	// Collision
	LONG Sca_Test(Resident_Evil_2_Collision_Data* pSca_dat);
	USHORT Sca_get_dir(Sony_PlayStation_Vector* pPos, Resident_Evil_2_Collision_Data* pT_xz);
	LONG Sca_get_pos(LONG Dir, Resident_Evil_2_Collision_Data* pSca_dat);
	LONG Sca_get_stairs_high(Sony_PlayStation_Vector* pPos, LONG Low_y, LONG High_y, Resident_Evil_2_Collision_Data* pT_xz);
	LONG Sca_get_slope_high(LONG Pos_x, LONG Pos_z, LONG R, Resident_Evil_2_Collision_Data* pT_xz);

#ifdef DIRECT_X
	// Camera
	VOID DrawCamera(UINT iCut);
	// Mask
	VOID DrawMasks(UINT iCut);
	VOID DrawMasksNew(UINT iCut);
	// Light
	VOID DrawLight(UINT iCut);
	// Zone
	VOID DrawZone(UCHAR iCut);
	// Collision
	VOID DrawShapeBox(SHORT X, SHORT Z, USHORT W, USHORT D, LONG Low, LONG High, SHORT RotX, SHORT RotY, SHORT RotZ, DWORD Color);
	VOID DrawCollisionNov96(VOID);
	VOID DrawCollision(VOID);
	// Block
	VOID DrawBlock(VOID);
	// Floor
	VOID DrawFloor(VOID);
#endif

#ifdef USE_IMGUI
	VOID CameraEditor(VOID);
	VOID SpriteEditor(VOID);
	VOID ZoneEditor(VOID);
	VOID LightEditor(VOID);
	VOID MessageEditor(VOID);
	VOID DisplayOpcodeNode(OpcodeTreeNode* Opcode, OpcodeTreeNode* all_nodes);
	VOID DisplayBytecodeNode(BytecodeTreeNode* Bytecode);
	VOID ScriptEditor(VOID);
	VOID ModelEditor(VOID);
	VOID AnimationEditor(VOID);
	VOID EffectEditor(VOID);
	VOID CollisionEditor(VOID);
	VOID BlockEditor(VOID);
	VOID FloorEditor(VOID);
#endif

};