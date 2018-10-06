#pragma once

typedef struct tagBio2RoomBlkData {
	SHORT Pos_x;
	SHORT Pos_z;
	SHORT Size_x;
	SHORT Size_z;
	USHORT Dir_bit;
	USHORT Abut;
} Bio2RoomBlkData;
typedef struct tagBio2RoomBlk {
	ULONG nData;
	std::vector <Bio2RoomBlkData*> Data;
} Bio2RoomBlk;