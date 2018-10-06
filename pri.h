#pragma once

// Structure
#pragma pack(push, 1)
typedef struct tagBio2RoomPriSquare {
	UCHAR u, v;
	UCHAR x, y;
	SHORT Depth;
	USHORT Size;
} Bio2RoomPriSquare;
typedef struct tagBio2RoomPriRect {
	UCHAR u, v;
	UCHAR x, y;
	SHORT Depth;
	USHORT tPage;
	USHORT w, h;
} Bio2RoomPriRect;
typedef struct tagBio2RoomPriData {
	BOOL bSquare;
	Bio2RoomPriSquare Square;
	Bio2RoomPriRect Rect;
} Bio2RoomPriData;
typedef struct tagBio2RoomPriIndex {
	USHORT nSprite;
	USHORT unk;
	SHORT X;
	SHORT Y;
	std::vector <Bio2RoomPriData*> Data;
} Bio2RoomPriIndex;
typedef struct tagBio2RoomPri {
	USHORT nIndex;
	USHORT nData;
	std::vector <Bio2RoomPriIndex*> Index;
	ULONG Size;	// Total File Size (*.PRI)
} Bio2RoomPri;
#pragma pack(pop)
