#pragma once

typedef struct tagBio2RoomFlrData {
	SHORT X;
	SHORT Z;
	USHORT W;
	USHORT D;
	USHORT Se_no;
	USHORT Floor_height;
} Bio2RoomFlrData;
typedef struct tagBio2RoomFlr {
	USHORT nData;
	std::vector <tagBio2RoomFlrData*> Data;
} Bio2RoomFlr;