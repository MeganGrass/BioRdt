#pragma once

typedef struct tagBio2RoomRvdData {
	UCHAR OnOff;		// Be_flg
	CHAR nFloor;		// Typically -1 (0xFF)
	UCHAR FromCut;		// Fcut
	UCHAR ToCut;		// Tcut
	SHORT X0;	SHORT Z0;
	SHORT X1;	SHORT Z1;
	SHORT X2;	SHORT Z2;
	SHORT X3;	SHORT Z3;
} Bio2RoomRvdData;
typedef struct tagBio2RoomRvd {
	ULONG nData;
	std::vector <Bio2RoomRvdData*> Data;
} Bio2RoomRvd;
