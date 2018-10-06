
#include "stdafx.h"
#include FRAMEWORK_H

#include BIO2_H

// Bio 1.5
Bio2RoomScaProto * Bio_Hazard_2_Proto_Room::ScaRead(_iobuf * FILE, ULONG _Offset) {

	// Structure
	Bio2RoomScaProto * Collision = new tagBio2RoomScaProto;
	RtlSecureZeroMemory(&Collision->Header, sizeof(tagBio2RoomScaProtoHeader));
	Collision->Data0.clear();
	Collision->Data1.clear();
	Collision->Data2.clear();
	Collision->Data3.clear();
	Collision->Data4.clear();
	Collision->Size = NULL;

	// Error
	if (!IsOpen(FILE)) { return Collision; }

	// Header
	ULONG Pointer = _Offset;
	Read(FILE, Pointer, &Collision->Header, sizeof(tagBio2RoomScaProtoHeader));

	// Data
	Pointer += sizeof(tagBio2RoomScaProtoHeader);
	for (ULONG n = 0; n < Collision->Header.nData0; n++, Pointer += sizeof(tagBio2RoomScaProtoData)) {
		Bio2RoomScaProtoData * Data = new tagBio2RoomScaProtoData;
		Read(FILE, Pointer, Data, sizeof(tagBio2RoomScaProtoData));
		Collision->Data0.push_back(Data);
	}
	for (ULONG n = 0; n < Collision->Header.nData1; n++, Pointer += sizeof(tagBio2RoomScaProtoData)) {
		Bio2RoomScaProtoData * Data = new tagBio2RoomScaProtoData;
		Read(FILE, Pointer, Data, sizeof(tagBio2RoomScaProtoData));
		Collision->Data1.push_back(Data);
	}
	for (ULONG n = 0; n < Collision->Header.nData2; n++, Pointer += sizeof(tagBio2RoomScaProtoData)) {
		Bio2RoomScaProtoData * Data = new tagBio2RoomScaProtoData;
		Read(FILE, Pointer, Data, sizeof(tagBio2RoomScaProtoData));
		Collision->Data2.push_back(Data);
	}
	for (ULONG n = 0; n < Collision->Header.nData3; n++, Pointer += sizeof(tagBio2RoomScaProtoData)) {
		Bio2RoomScaProtoData * Data = new tagBio2RoomScaProtoData;
		Read(FILE, Pointer, Data, sizeof(tagBio2RoomScaProtoData));
		Collision->Data3.push_back(Data);
	}
	for (ULONG n = 0; n < Collision->Header.nData4; n++, Pointer += sizeof(tagBio2RoomScaProtoData)) {
		Bio2RoomScaProtoData * Data = new tagBio2RoomScaProtoData;
		Read(FILE, Pointer, Data, sizeof(tagBio2RoomScaProtoData));
		Collision->Data4.push_back(Data);
	}

	// Size
	Read(FILE, Pointer, &Collision->Size, sizeof(unsigned long int));

	// Terminate
	return Collision;
}
Bio2RoomScaProto * Bio_Hazard_2_Proto_Room::ScaRead(CONST CHAR * _FileName, ...) {

	// _FileName
	va_list _ArgList = { NULL };
	__crt_va_start(_ArgList, _FileName);
	_StrLen = (_vscprintf(_FileName, _ArgList) + 2);
	CHAR * FileName = new CHAR[(_StrLen * 2)];
	RtlSecureZeroMemory(FileName, (_StrLen * 2));
	vsprintf_s(FileName, _StrLen, _FileName, _ArgList);
	__crt_va_end(_ArgList);

	// Error
	if (!Exists(FileName)) { delete[] FileName; return FALSE; }

	// Open
	_iobuf * _File = Open(READ_FILE, FileName);
	delete[] FileName;
	if (!_File) { return FALSE; }

	// Structure
	Bio2RoomScaProto * Buffer = ScaRead(_File, 0x00);

	// Cleanup
	fclose(_File);

	// Terminate
	return Buffer;

}
BOOL Bio_Hazard_2_Proto_Room::ScaWrite(Bio2RoomScaProto * Collision, BOOL Convert, _iobuf * FILE, ULONG _Offset) {

	// Error
	if (!IsOpen(FILE)) { return FAIL; }

	// Standard
	if (!Convert) {
		ULONG Pointer = _Offset;
		Write(FILE, Pointer, &Collision->Header, sizeof(tagBio2RoomScaProtoHeader));
		Pointer += sizeof(tagBio2RoomScaProtoHeader);
		for (ULONG n = 0; n < Collision->Header.nData0; n++, Pointer += sizeof(tagBio2RoomScaProtoData)) {
			Write(FILE, Pointer, Collision->Data0[n], sizeof(tagBio2RoomScaProtoData));
		}
		for (ULONG n = 0; n < Collision->Header.nData1; n++, Pointer += sizeof(tagBio2RoomScaProtoData)) {
			Write(FILE, Pointer, Collision->Data1[n], sizeof(tagBio2RoomScaProtoData));
		}
		for (ULONG n = 0; n < Collision->Header.nData2; n++, Pointer += sizeof(tagBio2RoomScaProtoData)) {
			Write(FILE, Pointer, Collision->Data2[n], sizeof(tagBio2RoomScaProtoData));
		}
		for (ULONG n = 0; n < Collision->Header.nData3; n++, Pointer += sizeof(tagBio2RoomScaProtoData)) {
			Write(FILE, Pointer, Collision->Data3[n], sizeof(tagBio2RoomScaProtoData));
		}
		for (ULONG n = 0; n < Collision->Header.nData4; n++, Pointer += sizeof(tagBio2RoomScaProtoData)) {
			Write(FILE, Pointer, Collision->Data4[n], sizeof(tagBio2RoomScaProtoData));
		}
		Write(FILE, Pointer, &Collision->Size, sizeof(unsigned long int));
	} else {

		// Count
		ULONG nSca = NULL;
		nSca += Collision->Header.nData0;
		nSca += Collision->Header.nData1;
		nSca += Collision->Header.nData2;
		nSca += Collision->Header.nData3;
		nSca += Collision->Header.nData4;

		// Header
		SCA_HEAD ScaHead2 = { NULL };
		ScaHead2.Cx = Collision->Header.Cx;
		ScaHead2.Cz = Collision->Header.Cz;
		ScaHead2.Num = (nSca + 1);
		ScaHead2.Ceiling = 0xFFFFEAE8;
		ScaHead2.Dummy = 0xC5C5C5C5;
		Write(FILE, _Offset, &ScaHead2, sizeof(tagSCA_HEAD));

		// Data
		ULONG Pointer = _Offset + sizeof(tagSCA_HEAD);
		SCA_DATA ScaData2 = { NULL };
		for (ULONG n = 0; n < Collision->Header.nData0; n++, Pointer += sizeof(tagSCA_DATA)) {

			// Coordinate
			ScaData2.X = Collision->Data0[n]->X;
			ScaData2.Z = Collision->Data0[n]->Z;
			ScaData2.W = Collision->Data0[n]->W;
			ScaData2.D = Collision->Data0[n]->D;
			ScaData2.Floor = 3;

			// Id
			ScaData2.Id.s.unk0 = 0x08;		// Weapon Collision ON
			ScaData2.Id.s.unk1 = 0x00;		// nFloor OFF
			ScaData2.Id.s.unk2 = 0x00;		// 
			ScaData2.Id.s.unk3 = 0x01;		// Enemy Collision ON
			ScaData2.Id.s.unk4 = 0x00;		// 
			ScaData2.Id.s.unk5 = 0x00;		// 
			ScaData2.Id.s.unk6 = 0x01;		// Bullet Collision ON
			ScaData2.Id.s.unk7 = 0x01;		// Object Collision ON
			ScaData2.Id.s.unk8 = 0x01;		// Player Collision ON

											// Type
			ScaData2.Type.s.h = 3;			// 
			ScaData2.Type.s.y = 3;			// 
			ScaData2.Type.s.Type = 0;		// Non-Interactive
			ScaData2.Type.s.tFloor = 2;		// 
			ScaData2.Type.s.Nine = 9;		// Always 9

											// Shape
			switch (Collision->Data0[n]->Id.s.shape)
			{
			case 1:	// Sca_hit_box
				ScaData2.Id.s.shape = Sca_hit_box;
				break;
			case 2:	// Sca_hit_hishi
				ScaData2.Id.s.shape = Sca_hit_hishi;
				break;
			case 3:	// Sca_hit_circle
				ScaData2.Id.s.shape = Sca_hit_circle;
				break;
			case 4:	// Sca_hit_naname_a
				ScaData2.Id.s.shape = Sca_hit_naname_a;
				break;
			case 5:	// Sca_hit_naname_b
				ScaData2.Id.s.shape = Sca_hit_naname_b;
				break;
			case 6:	// Sca_hit_naname_c
				ScaData2.Id.s.shape = Sca_hit_naname_c;
				break;
			case 7:	// Sca_hit_naname_d
				ScaData2.Id.s.shape = Sca_hit_naname_d;
				break;
			case 8:	// Sca_hit_koban_x
				ScaData2.Id.s.shape = Sca_hit_koban_x;
				break;
			case 9:	// Sca_hit_koban_z
				ScaData2.Id.s.shape = Sca_hit_koban_z;
				break;
			}

			// Write
			Write(FILE, Pointer, &ScaData2, sizeof(tagSCA_DATA));
		}
		for (ULONG n = 0; n < Collision->Header.nData1; n++, Pointer += sizeof(tagSCA_DATA)) {

			// Coordinate
			ScaData2.X = Collision->Data1[n]->X;
			ScaData2.Z = Collision->Data1[n]->Z;
			ScaData2.W = Collision->Data1[n]->W;
			ScaData2.D = Collision->Data1[n]->D;
			ScaData2.Floor = 3;

			// Id
			ScaData2.Id.s.unk0 = 0x08;		// Weapon Collision ON
			ScaData2.Id.s.unk1 = 0x00;		// nFloor OFF
			ScaData2.Id.s.unk2 = 0x00;		// 
			ScaData2.Id.s.unk3 = 0x01;		// Enemy Collision ON
			ScaData2.Id.s.unk4 = 0x00;		// 
			ScaData2.Id.s.unk5 = 0x00;		// 
			ScaData2.Id.s.unk6 = 0x01;		// Bullet Collision ON
			ScaData2.Id.s.unk7 = 0x01;		// Object Collision ON
			ScaData2.Id.s.unk8 = 0x01;		// Player Collision ON

											// Type
			ScaData2.Type.s.h = 3;			// 
			ScaData2.Type.s.y = 3;			// 
			ScaData2.Type.s.Type = 0;		// Non-Interactive
			ScaData2.Type.s.tFloor = 2;		// 
			ScaData2.Type.s.Nine = 9;		// Always 9

											// Shape
			switch (Collision->Data1[n]->Id.s.shape)
			{
			case 1:	// Sca_hit_box
				ScaData2.Id.s.shape = Sca_hit_box;
				break;
			case 2:	// Sca_hit_hishi
				ScaData2.Id.s.shape = Sca_hit_hishi;
				break;
			case 3:	// Sca_hit_circle
				ScaData2.Id.s.shape = Sca_hit_circle;
				break;
			case 4:	// Sca_hit_naname_a
				ScaData2.Id.s.shape = Sca_hit_naname_a;
				break;
			case 5:	// Sca_hit_naname_b
				ScaData2.Id.s.shape = Sca_hit_naname_b;
				break;
			case 6:	// Sca_hit_naname_c
				ScaData2.Id.s.shape = Sca_hit_naname_c;
				break;
			case 7:	// Sca_hit_naname_d
				ScaData2.Id.s.shape = Sca_hit_naname_d;
				break;
			case 8:	// Sca_hit_koban_x
				ScaData2.Id.s.shape = Sca_hit_koban_x;
				break;
			case 9:	// Sca_hit_koban_z
				ScaData2.Id.s.shape = Sca_hit_koban_z;
				break;
			}

			// Write
			Write(FILE, Pointer, &ScaData2, sizeof(tagSCA_DATA));
		}
		for (ULONG n = 0; n < Collision->Header.nData2; n++, Pointer += sizeof(tagSCA_DATA)) {

			// Coordinate
			ScaData2.X = Collision->Data2[n]->X;
			ScaData2.Z = Collision->Data2[n]->Z;
			ScaData2.W = Collision->Data2[n]->W;
			ScaData2.D = Collision->Data2[n]->D;
			ScaData2.Floor = 3;

			// Id
			ScaData2.Id.s.unk0 = 0x08;		// Weapon Collision ON
			ScaData2.Id.s.unk1 = 0x00;		// nFloor OFF
			ScaData2.Id.s.unk2 = 0x00;		// 
			ScaData2.Id.s.unk3 = 0x01;		// Enemy Collision ON
			ScaData2.Id.s.unk4 = 0x00;		// 
			ScaData2.Id.s.unk5 = 0x00;		// 
			ScaData2.Id.s.unk6 = 0x01;		// Bullet Collision ON
			ScaData2.Id.s.unk7 = 0x01;		// Object Collision ON
			ScaData2.Id.s.unk8 = 0x01;		// Player Collision ON

											// Type
			ScaData2.Type.s.h = 3;			// 
			ScaData2.Type.s.y = 3;			// 
			ScaData2.Type.s.Type = 0;		// Non-Interactive
			ScaData2.Type.s.tFloor = 2;		// 
			ScaData2.Type.s.Nine = 9;		// Always 9

											// Shape
			switch (Collision->Data2[n]->Id.s.shape)
			{
			case 1:	// Sca_hit_box
				ScaData2.Id.s.shape = Sca_hit_box;
				break;
			case 2:	// Sca_hit_hishi
				ScaData2.Id.s.shape = Sca_hit_hishi;
				break;
			case 3:	// Sca_hit_circle
				ScaData2.Id.s.shape = Sca_hit_circle;
				break;
			case 4:	// Sca_hit_naname_a
				ScaData2.Id.s.shape = Sca_hit_naname_a;
				break;
			case 5:	// Sca_hit_naname_b
				ScaData2.Id.s.shape = Sca_hit_naname_b;
				break;
			case 6:	// Sca_hit_naname_c
				ScaData2.Id.s.shape = Sca_hit_naname_c;
				break;
			case 7:	// Sca_hit_naname_d
				ScaData2.Id.s.shape = Sca_hit_naname_d;
				break;
			case 8:	// Sca_hit_koban_x
				ScaData2.Id.s.shape = Sca_hit_koban_x;
				break;
			case 9:	// Sca_hit_koban_z
				ScaData2.Id.s.shape = Sca_hit_koban_z;
				break;
			}

			// Write
			Write(FILE, Pointer, &ScaData2, sizeof(tagSCA_DATA));
		}
		for (ULONG n = 0; n < Collision->Header.nData3; n++, Pointer += sizeof(tagSCA_DATA)) {

			// Coordinate
			ScaData2.X = Collision->Data3[n]->X;
			ScaData2.Z = Collision->Data3[n]->Z;
			ScaData2.W = Collision->Data3[n]->W;
			ScaData2.D = Collision->Data3[n]->D;
			ScaData2.Floor = 3;

			// Id
			ScaData2.Id.s.unk0 = 0x08;		// Weapon Collision ON
			ScaData2.Id.s.unk1 = 0x00;		// nFloor OFF
			ScaData2.Id.s.unk2 = 0x00;		// 
			ScaData2.Id.s.unk3 = 0x01;		// Enemy Collision ON
			ScaData2.Id.s.unk4 = 0x00;		// 
			ScaData2.Id.s.unk5 = 0x00;		// 
			ScaData2.Id.s.unk6 = 0x01;		// Bullet Collision ON
			ScaData2.Id.s.unk7 = 0x01;		// Object Collision ON
			ScaData2.Id.s.unk8 = 0x01;		// Player Collision ON

											// Type
			ScaData2.Type.s.h = 3;			// 
			ScaData2.Type.s.y = 3;			// 
			ScaData2.Type.s.Type = 0;		// Non-Interactive
			ScaData2.Type.s.tFloor = 2;		// 
			ScaData2.Type.s.Nine = 9;		// Always 9

											// Shape
			switch (Collision->Data3[n]->Id.s.shape)
			{
			case 1:	// Sca_hit_box
				ScaData2.Id.s.shape = Sca_hit_box;
				break;
			case 2:	// Sca_hit_hishi
				ScaData2.Id.s.shape = Sca_hit_hishi;
				break;
			case 3:	// Sca_hit_circle
				ScaData2.Id.s.shape = Sca_hit_circle;
				break;
			case 4:	// Sca_hit_naname_a
				ScaData2.Id.s.shape = Sca_hit_naname_a;
				break;
			case 5:	// Sca_hit_naname_b
				ScaData2.Id.s.shape = Sca_hit_naname_b;
				break;
			case 6:	// Sca_hit_naname_c
				ScaData2.Id.s.shape = Sca_hit_naname_c;
				break;
			case 7:	// Sca_hit_naname_d
				ScaData2.Id.s.shape = Sca_hit_naname_d;
				break;
			case 8:	// Sca_hit_koban_x
				ScaData2.Id.s.shape = Sca_hit_koban_x;
				break;
			case 9:	// Sca_hit_koban_z
				ScaData2.Id.s.shape = Sca_hit_koban_z;
				break;
			}

			// Write
			Write(FILE, Pointer, &ScaData2, sizeof(tagSCA_DATA));
		}
		for (ULONG n = 0; n < Collision->Header.nData4; n++, Pointer += sizeof(tagSCA_DATA)) {

			// Coordinate
			ScaData2.X = Collision->Data4[n]->X;
			ScaData2.Z = Collision->Data4[n]->Z;
			ScaData2.W = Collision->Data4[n]->W;
			ScaData2.D = Collision->Data4[n]->D;
			ScaData2.Floor = 3;

			// Id
			ScaData2.Id.s.unk0 = 0x08;		// Weapon Collision ON
			ScaData2.Id.s.unk1 = 0x00;		// nFloor OFF
			ScaData2.Id.s.unk2 = 0x00;		// 
			ScaData2.Id.s.unk3 = 0x01;		// Enemy Collision ON
			ScaData2.Id.s.unk4 = 0x00;		// 
			ScaData2.Id.s.unk5 = 0x00;		// 
			ScaData2.Id.s.unk6 = 0x01;		// Bullet Collision ON
			ScaData2.Id.s.unk7 = 0x01;		// Object Collision ON
			ScaData2.Id.s.unk8 = 0x01;		// Player Collision ON

											// Type
			ScaData2.Type.s.h = 3;			// 
			ScaData2.Type.s.y = 3;			// 
			ScaData2.Type.s.Type = 0;		// Non-Interactive
			ScaData2.Type.s.tFloor = 2;		// 
			ScaData2.Type.s.Nine = 9;		// Always 9

											// Shape
			switch (Collision->Data4[n]->Id.s.shape)
			{
			case 1:	// Sca_hit_box
				ScaData2.Id.s.shape = Sca_hit_box;
				break;
			case 2:	// Sca_hit_hishi
				ScaData2.Id.s.shape = Sca_hit_hishi;
				break;
			case 3:	// Sca_hit_circle
				ScaData2.Id.s.shape = Sca_hit_circle;
				break;
			case 4:	// Sca_hit_naname_a
				ScaData2.Id.s.shape = Sca_hit_naname_a;
				break;
			case 5:	// Sca_hit_naname_b
				ScaData2.Id.s.shape = Sca_hit_naname_b;
				break;
			case 6:	// Sca_hit_naname_c
				ScaData2.Id.s.shape = Sca_hit_naname_c;
				break;
			case 7:	// Sca_hit_naname_d
				ScaData2.Id.s.shape = Sca_hit_naname_d;
				break;
			case 8:	// Sca_hit_koban_x
				ScaData2.Id.s.shape = Sca_hit_koban_x;
				break;
			case 9:	// Sca_hit_koban_z
				ScaData2.Id.s.shape = Sca_hit_koban_z;
				break;
			}

			// Write
			Write(FILE, Pointer, &ScaData2, sizeof(tagSCA_DATA));
		}

	}

	// Terminate
	return SUCCESS;
}
BOOL Bio_Hazard_2_Proto_Room::ScaWrite(Bio2RoomScaProto * Collision, BOOL Convert, CONST CHAR * _FileName, ...) {

	// _FileName
	va_list _ArgList = { NULL };
	__crt_va_start(_ArgList, _FileName);
	_StrLen = (_vscprintf(_FileName, _ArgList) + 2);
	CHAR * FileName = new CHAR[(_StrLen * 2)];
	RtlSecureZeroMemory(FileName, (_StrLen * 2));
	vsprintf_s(FileName, _StrLen, _FileName, _ArgList);
	__crt_va_end(_ArgList);

	// Open
	_iobuf * _File = Open(CREATE_FILE, FileName);
	delete[] FileName;
	if (!_File) { return FAIL; }

	// Write
	BOOL bResult = ScaWrite(Collision, Convert, _File, 0x00);

	// Cleanup
	fclose(_File);

	// Terminate
	return bResult;
}
BOOL Bio_Hazard_2_Proto_Room::RdtScaXml(Bio2RoomScaProto * Collision, BOOL Convert, tinyxml2::XMLDocument * Xml, tinyxml2::XMLElement * Element) {

	// Write
	ULONG iData = NULL;
	ULONG iPointer = NULL;
	if (!Convert) {

		// Header
		tinyxml2::XMLElement * XmlHeader = Element->FirstChildElement("header");
		if (!XmlHeader) { XmlHeader = Xml->NewElement("header"); }
		XmlHeader->SetAttribute("Cx", Collision->Header.Cx);
		XmlHeader->SetAttribute("Cz", Collision->Header.Cz);
		XmlHeader->SetAttribute("nData0", (UINT)Collision->Header.nData0);
		XmlHeader->SetAttribute("nData1", (UINT)Collision->Header.nData1);
		XmlHeader->SetAttribute("nData2", (UINT)Collision->Header.nData2);
		XmlHeader->SetAttribute("nData3", (UINT)Collision->Header.nData3);
		XmlHeader->SetAttribute("nData4", (UINT)Collision->Header.nData4);
		Element->InsertEndChild(XmlHeader);
		iPointer += sizeof(tagBio2RoomScaProtoHeader);

		// Data 0
		if (Collision->Header.nData0) {
			iData = NULL;
			tinyxml2::XMLElement * XmlData = Element->FirstChildElement("data0");
			if (!XmlData) { XmlData = Xml->NewElement("data0"); }
			do {
				tinyxml2::XMLElement * XmlBuffer = XmlData->FirstChildElement(Get((CHAR*)"obj%02d", iData));
				if (!XmlBuffer) { XmlBuffer = Xml->NewElement(Get((CHAR*)"obj%02d", iData)); }
				XmlBuffer->SetAttribute("x", Collision->Data0[iData]->X);
				XmlBuffer->SetAttribute("z", Collision->Data0[iData]->Z);
				XmlBuffer->SetAttribute("w", Collision->Data0[iData]->W);
				XmlBuffer->SetAttribute("d", Collision->Data0[iData]->D);
				XmlBuffer->SetAttribute("Shape", Collision->Data0[iData]->Id.s.shape);
				XmlBuffer->SetAttribute("Weapon", Collision->Data0[iData]->Id.s.unk0);
				XmlBuffer->SetAttribute("nFloor", Collision->Data0[iData]->Id.s.unk1);
				XmlBuffer->SetAttribute("unk2", Collision->Data0[iData]->Id.s.unk2);
				XmlBuffer->SetAttribute("Enemy", Collision->Data0[iData]->Id.s.unk3);
				XmlBuffer->SetAttribute("unk4", Collision->Data0[iData]->Id.s.unk4);
				XmlBuffer->SetAttribute("unk5", Collision->Data0[iData]->Id.s.unk5);
				XmlBuffer->SetAttribute("Bullet", Collision->Data0[iData]->Id.s.unk6);
				XmlBuffer->SetAttribute("Object", Collision->Data0[iData]->Id.s.unk7);
				XmlBuffer->SetAttribute("Player", Collision->Data0[iData]->Id.s.unk8);
				XmlBuffer->SetAttribute("h", Collision->Data0[iData]->Type.s.h);
				XmlBuffer->SetAttribute("y", Collision->Data0[iData]->Type.s.y);
				XmlBuffer->SetAttribute("Type", Collision->Data0[iData]->Type.s.Type);
				XmlBuffer->SetAttribute("tFloor", Collision->Data0[iData]->Type.s.tFloor);
				XmlBuffer->SetAttribute("Nine", Collision->Data0[iData]->Type.s.Nine);
				XmlData->InsertEndChild(XmlBuffer);
				iPointer += sizeof(tagBio2RoomScaProtoData);
				iData++;

			} while (iData != Collision->Header.nData0);
			Element->InsertEndChild(XmlData);
		}

		// Data 1
		if (Collision->Header.nData1) {
			iData = NULL;
			tinyxml2::XMLElement * XmlData = Element->FirstChildElement("data0");
			if (!XmlData) { XmlData = Xml->NewElement("data0"); }
			do {
				tinyxml2::XMLElement * XmlBuffer = XmlData->FirstChildElement(Get((CHAR*)"obj%02d", iData));
				if (!XmlBuffer) { XmlBuffer = Xml->NewElement(Get((CHAR*)"obj%02d", iData)); }
				XmlBuffer->SetAttribute("x", Collision->Data1[iData]->X);
				XmlBuffer->SetAttribute("z", Collision->Data1[iData]->Z);
				XmlBuffer->SetAttribute("w", Collision->Data1[iData]->W);
				XmlBuffer->SetAttribute("d", Collision->Data1[iData]->D);
				XmlBuffer->SetAttribute("Shape", Collision->Data1[iData]->Id.s.shape);
				XmlBuffer->SetAttribute("Weapon", Collision->Data1[iData]->Id.s.unk0);
				XmlBuffer->SetAttribute("nFloor", Collision->Data1[iData]->Id.s.unk1);
				XmlBuffer->SetAttribute("unk2", Collision->Data1[iData]->Id.s.unk2);
				XmlBuffer->SetAttribute("Enemy", Collision->Data1[iData]->Id.s.unk3);
				XmlBuffer->SetAttribute("unk4", Collision->Data1[iData]->Id.s.unk4);
				XmlBuffer->SetAttribute("unk5", Collision->Data1[iData]->Id.s.unk5);
				XmlBuffer->SetAttribute("Bullet", Collision->Data1[iData]->Id.s.unk6);
				XmlBuffer->SetAttribute("Object", Collision->Data1[iData]->Id.s.unk7);
				XmlBuffer->SetAttribute("Player", Collision->Data1[iData]->Id.s.unk8);
				XmlBuffer->SetAttribute("h", Collision->Data1[iData]->Type.s.h);
				XmlBuffer->SetAttribute("y", Collision->Data1[iData]->Type.s.y);
				XmlBuffer->SetAttribute("Type", Collision->Data1[iData]->Type.s.Type);
				XmlBuffer->SetAttribute("tFloor", Collision->Data1[iData]->Type.s.tFloor);
				XmlBuffer->SetAttribute("Nine", Collision->Data1[iData]->Type.s.Nine);
				XmlData->InsertEndChild(XmlBuffer);
				iPointer += sizeof(tagBio2RoomScaProtoData);
				iData++;

			} while (iData != Collision->Header.nData1);
			Element->InsertEndChild(XmlData);
		}

		// Data 2
		if (Collision->Header.nData2) {
			iData = NULL;
			tinyxml2::XMLElement * XmlData = Element->FirstChildElement("data0");
			if (!XmlData) { XmlData = Xml->NewElement("data0"); }
			do {
				tinyxml2::XMLElement * XmlBuffer = XmlData->FirstChildElement(Get((CHAR*)"obj%02d", iData));
				if (!XmlBuffer) { XmlBuffer = Xml->NewElement(Get((CHAR*)"obj%02d", iData)); }
				XmlBuffer->SetAttribute("x", Collision->Data2[iData]->X);
				XmlBuffer->SetAttribute("z", Collision->Data2[iData]->Z);
				XmlBuffer->SetAttribute("w", Collision->Data2[iData]->W);
				XmlBuffer->SetAttribute("d", Collision->Data2[iData]->D);
				XmlBuffer->SetAttribute("Shape", Collision->Data2[iData]->Id.s.shape);
				XmlBuffer->SetAttribute("Weapon", Collision->Data2[iData]->Id.s.unk0);
				XmlBuffer->SetAttribute("nFloor", Collision->Data2[iData]->Id.s.unk1);
				XmlBuffer->SetAttribute("unk2", Collision->Data2[iData]->Id.s.unk2);
				XmlBuffer->SetAttribute("Enemy", Collision->Data2[iData]->Id.s.unk3);
				XmlBuffer->SetAttribute("unk4", Collision->Data2[iData]->Id.s.unk4);
				XmlBuffer->SetAttribute("unk5", Collision->Data2[iData]->Id.s.unk5);
				XmlBuffer->SetAttribute("Bullet", Collision->Data2[iData]->Id.s.unk6);
				XmlBuffer->SetAttribute("Object", Collision->Data2[iData]->Id.s.unk7);
				XmlBuffer->SetAttribute("Player", Collision->Data2[iData]->Id.s.unk8);
				XmlBuffer->SetAttribute("h", Collision->Data2[iData]->Type.s.h);
				XmlBuffer->SetAttribute("y", Collision->Data2[iData]->Type.s.y);
				XmlBuffer->SetAttribute("Type", Collision->Data2[iData]->Type.s.Type);
				XmlBuffer->SetAttribute("tFloor", Collision->Data2[iData]->Type.s.tFloor);
				XmlBuffer->SetAttribute("Nine", Collision->Data2[iData]->Type.s.Nine);
				XmlData->InsertEndChild(XmlBuffer);
				iPointer += sizeof(tagBio2RoomScaProtoData);
				iData++;

			} while (iData != Collision->Header.nData2);
			Element->InsertEndChild(XmlData);
		}

		// Data 3
		if (Collision->Header.nData3) {
			iData = NULL;
			tinyxml2::XMLElement * XmlData = Element->FirstChildElement("data0");
			if (!XmlData) { XmlData = Xml->NewElement("data0"); }
			do {
				tinyxml2::XMLElement * XmlBuffer = XmlData->FirstChildElement(Get((CHAR*)"obj%02d", iData));
				if (!XmlBuffer) { XmlBuffer = Xml->NewElement(Get((CHAR*)"obj%02d", iData)); }
				XmlBuffer->SetAttribute("x", Collision->Data3[iData]->X);
				XmlBuffer->SetAttribute("z", Collision->Data3[iData]->Z);
				XmlBuffer->SetAttribute("w", Collision->Data3[iData]->W);
				XmlBuffer->SetAttribute("d", Collision->Data3[iData]->D);
				XmlBuffer->SetAttribute("Shape", Collision->Data3[iData]->Id.s.shape);
				XmlBuffer->SetAttribute("Weapon", Collision->Data3[iData]->Id.s.unk0);
				XmlBuffer->SetAttribute("nFloor", Collision->Data3[iData]->Id.s.unk1);
				XmlBuffer->SetAttribute("unk2", Collision->Data3[iData]->Id.s.unk2);
				XmlBuffer->SetAttribute("Enemy", Collision->Data3[iData]->Id.s.unk3);
				XmlBuffer->SetAttribute("unk4", Collision->Data3[iData]->Id.s.unk4);
				XmlBuffer->SetAttribute("unk5", Collision->Data3[iData]->Id.s.unk5);
				XmlBuffer->SetAttribute("Bullet", Collision->Data3[iData]->Id.s.unk6);
				XmlBuffer->SetAttribute("Object", Collision->Data3[iData]->Id.s.unk7);
				XmlBuffer->SetAttribute("Player", Collision->Data3[iData]->Id.s.unk8);
				XmlBuffer->SetAttribute("h", Collision->Data3[iData]->Type.s.h);
				XmlBuffer->SetAttribute("y", Collision->Data3[iData]->Type.s.y);
				XmlBuffer->SetAttribute("Type", Collision->Data3[iData]->Type.s.Type);
				XmlBuffer->SetAttribute("tFloor", Collision->Data3[iData]->Type.s.tFloor);
				XmlBuffer->SetAttribute("Nine", Collision->Data3[iData]->Type.s.Nine);
				XmlData->InsertEndChild(XmlBuffer);
				iPointer += sizeof(tagBio2RoomScaProtoData);
				iData++;

			} while (iData != Collision->Header.nData3);
			Element->InsertEndChild(XmlData);
		}

		// Data 4
		if (Collision->Header.nData4) {
			iData = NULL;
			tinyxml2::XMLElement * XmlData = Element->FirstChildElement("data0");
			if (!XmlData) { XmlData = Xml->NewElement("data0"); }
			do {
				tinyxml2::XMLElement * XmlBuffer = XmlData->FirstChildElement(Get((CHAR*)"obj%02d", iData));
				if (!XmlBuffer) { XmlBuffer = Xml->NewElement(Get((CHAR*)"obj%02d", iData)); }
				XmlBuffer->SetAttribute("x", Collision->Data4[iData]->X);
				XmlBuffer->SetAttribute("z", Collision->Data4[iData]->Z);
				XmlBuffer->SetAttribute("w", Collision->Data4[iData]->W);
				XmlBuffer->SetAttribute("d", Collision->Data4[iData]->D);
				XmlBuffer->SetAttribute("Shape", Collision->Data4[iData]->Id.s.shape);
				XmlBuffer->SetAttribute("Weapon", Collision->Data4[iData]->Id.s.unk0);
				XmlBuffer->SetAttribute("nFloor", Collision->Data4[iData]->Id.s.unk1);
				XmlBuffer->SetAttribute("unk2", Collision->Data4[iData]->Id.s.unk2);
				XmlBuffer->SetAttribute("Enemy", Collision->Data4[iData]->Id.s.unk3);
				XmlBuffer->SetAttribute("unk4", Collision->Data4[iData]->Id.s.unk4);
				XmlBuffer->SetAttribute("unk5", Collision->Data4[iData]->Id.s.unk5);
				XmlBuffer->SetAttribute("Bullet", Collision->Data4[iData]->Id.s.unk6);
				XmlBuffer->SetAttribute("Object", Collision->Data4[iData]->Id.s.unk7);
				XmlBuffer->SetAttribute("Player", Collision->Data4[iData]->Id.s.unk8);
				XmlBuffer->SetAttribute("h", Collision->Data4[iData]->Type.s.h);
				XmlBuffer->SetAttribute("y", Collision->Data4[iData]->Type.s.y);
				XmlBuffer->SetAttribute("Type", Collision->Data4[iData]->Type.s.Type);
				XmlBuffer->SetAttribute("tFloor", Collision->Data4[iData]->Type.s.tFloor);
				XmlBuffer->SetAttribute("Nine", Collision->Data4[iData]->Type.s.Nine);
				XmlData->InsertEndChild(XmlBuffer);
				iPointer += sizeof(tagBio2RoomScaProtoData);
				iData++;

			} while (iData != Collision->Header.nData4);
			Element->InsertEndChild(XmlData);
		}

	}
	else {

		// Count
		ULONG nSca = NULL;
		nSca += Collision->Header.nData0;
		nSca += Collision->Header.nData1;
		nSca += Collision->Header.nData2;
		nSca += Collision->Header.nData3;
		nSca += Collision->Header.nData4;

		// Header
		SCA_HEAD ScaHead2 = { NULL };
		ScaHead2.Cx = Collision->Header.Cx;
		ScaHead2.Cz = Collision->Header.Cz;
		ScaHead2.Num = (nSca + 1);
		ScaHead2.Ceiling = 0xFFFFEAE8;
		ScaHead2.Dummy = 0xC5C5C5C5;
		tinyxml2::XMLElement * XmlHeader = Element->FirstChildElement("header");
		if (!XmlHeader) { XmlHeader = Xml->NewElement("header"); }
		XmlHeader->SetAttribute("Cx", ScaHead2.Cx);
		XmlHeader->SetAttribute("Cz", ScaHead2.Cz);
		XmlHeader->SetAttribute("nObj", (UINT)ScaHead2.Num);
		XmlHeader->SetAttribute("Ceiling", ScaHead2.Ceiling);
		XmlHeader->SetAttribute("Dummy", Get((CHAR*)"%08X", ScaHead2.Dummy));
		Element->InsertEndChild(XmlHeader);

		// Data
		SCA_DATA ScaData2 = { NULL };
		for (ULONG n = 0; n < Collision->Header.nData0; n++) {

			// Coordinate
			ScaData2.X = Collision->Data0[n]->X;
			ScaData2.Z = Collision->Data0[n]->Z;
			ScaData2.W = Collision->Data0[n]->W;
			ScaData2.D = Collision->Data0[n]->D;
			ScaData2.Floor = 3;

			// Id
			ScaData2.Id.s.unk0 = 0x08;		// Weapon Collision ON
			ScaData2.Id.s.unk1 = 0x00;		// nFloor OFF
			ScaData2.Id.s.unk2 = 0x00;		// 
			ScaData2.Id.s.unk3 = 0x01;		// Enemy Collision ON
			ScaData2.Id.s.unk4 = 0x00;		// 
			ScaData2.Id.s.unk5 = 0x00;		// 
			ScaData2.Id.s.unk6 = 0x01;		// Bullet Collision ON
			ScaData2.Id.s.unk7 = 0x01;		// Object Collision ON
			ScaData2.Id.s.unk8 = 0x01;		// Player Collision ON

											// Type
			ScaData2.Type.s.h = 3;			// 
			ScaData2.Type.s.y = 3;			// 
			ScaData2.Type.s.Type = 0;		// Non-Interactive
			ScaData2.Type.s.tFloor = 2;		// 
			ScaData2.Type.s.Nine = 9;		// Always 9

											// Shape
			switch (Collision->Data0[n]->Id.s.shape)
			{
			case 1:	// Sca_hit_box
				ScaData2.Id.s.shape = Sca_hit_box;
				break;
			case 2:	// Sca_hit_hishi
				ScaData2.Id.s.shape = Sca_hit_hishi;
				break;
			case 3:	// Sca_hit_circle
				ScaData2.Id.s.shape = Sca_hit_circle;
				break;
			case 4:	// Sca_hit_naname_a
				ScaData2.Id.s.shape = Sca_hit_naname_a;
				break;
			case 5:	// Sca_hit_naname_b
				ScaData2.Id.s.shape = Sca_hit_naname_b;
				break;
			case 6:	// Sca_hit_naname_c
				ScaData2.Id.s.shape = Sca_hit_naname_c;
				break;
			case 7:	// Sca_hit_naname_d
				ScaData2.Id.s.shape = Sca_hit_naname_d;
				break;
			case 8:	// Sca_hit_koban_x
				ScaData2.Id.s.shape = Sca_hit_koban_x;
				break;
			case 9:	// Sca_hit_koban_z
				ScaData2.Id.s.shape = Sca_hit_koban_z;
				break;
			}

			// Print
			tinyxml2::XMLElement * XmlMeta = Element->FirstChildElement(Get((CHAR*)"obj%02d", n));
			if (!XmlMeta) { XmlMeta = Xml->NewElement(Get((CHAR*)"obj%02d", n)); }
			XmlMeta->SetAttribute("x", ScaData2.X);
			XmlMeta->SetAttribute("z", ScaData2.Z);
			XmlMeta->SetAttribute("w", ScaData2.W);
			XmlMeta->SetAttribute("d", ScaData2.D);
			XmlMeta->SetAttribute("Shape", ScaData2.Id.s.shape);
			XmlMeta->SetAttribute("Weapon", ScaData2.Id.s.unk0);
			XmlMeta->SetAttribute("nFloor", ScaData2.Id.s.unk1);
			XmlMeta->SetAttribute("unk2", ScaData2.Id.s.unk2);
			XmlMeta->SetAttribute("Enemy", ScaData2.Id.s.unk3);
			XmlMeta->SetAttribute("unk4", ScaData2.Id.s.unk4);
			XmlMeta->SetAttribute("unk5", ScaData2.Id.s.unk5);
			XmlMeta->SetAttribute("Bullet", ScaData2.Id.s.unk6);
			XmlMeta->SetAttribute("Object", ScaData2.Id.s.unk7);
			XmlMeta->SetAttribute("Player", ScaData2.Id.s.unk8);
			XmlMeta->SetAttribute("h", ScaData2.Type.s.h);
			XmlMeta->SetAttribute("y", ScaData2.Type.s.y);
			XmlMeta->SetAttribute("Type", ScaData2.Type.s.Type);
			XmlMeta->SetAttribute("tFloor", ScaData2.Type.s.tFloor);
			XmlMeta->SetAttribute("Nine", ScaData2.Type.s.Nine);
			Element->InsertEndChild(XmlMeta);
		}
		for (ULONG n = 0; n < Collision->Header.nData1; n++) {

			// Coordinate
			ScaData2.X = Collision->Data1[n]->X;
			ScaData2.Z = Collision->Data1[n]->Z;
			ScaData2.W = Collision->Data1[n]->W;
			ScaData2.D = Collision->Data1[n]->D;
			ScaData2.Floor = 3;

			// Id
			ScaData2.Id.s.unk0 = 0x08;		// Weapon Collision ON
			ScaData2.Id.s.unk1 = 0x00;		// nFloor OFF
			ScaData2.Id.s.unk2 = 0x00;		// 
			ScaData2.Id.s.unk3 = 0x01;		// Enemy Collision ON
			ScaData2.Id.s.unk4 = 0x00;		// 
			ScaData2.Id.s.unk5 = 0x00;		// 
			ScaData2.Id.s.unk6 = 0x01;		// Bullet Collision ON
			ScaData2.Id.s.unk7 = 0x01;		// Object Collision ON
			ScaData2.Id.s.unk8 = 0x01;		// Player Collision ON

											// Type
			ScaData2.Type.s.h = 3;			// 
			ScaData2.Type.s.y = 3;			// 
			ScaData2.Type.s.Type = 0;		// Non-Interactive
			ScaData2.Type.s.tFloor = 2;		// 
			ScaData2.Type.s.Nine = 9;		// Always 9

											// Shape
			switch (Collision->Data1[n]->Id.s.shape)
			{
			case 1:	// Sca_hit_box
				ScaData2.Id.s.shape = Sca_hit_box;
				break;
			case 2:	// Sca_hit_hishi
				ScaData2.Id.s.shape = Sca_hit_hishi;
				break;
			case 3:	// Sca_hit_circle
				ScaData2.Id.s.shape = Sca_hit_circle;
				break;
			case 4:	// Sca_hit_naname_a
				ScaData2.Id.s.shape = Sca_hit_naname_a;
				break;
			case 5:	// Sca_hit_naname_b
				ScaData2.Id.s.shape = Sca_hit_naname_b;
				break;
			case 6:	// Sca_hit_naname_c
				ScaData2.Id.s.shape = Sca_hit_naname_c;
				break;
			case 7:	// Sca_hit_naname_d
				ScaData2.Id.s.shape = Sca_hit_naname_d;
				break;
			case 8:	// Sca_hit_koban_x
				ScaData2.Id.s.shape = Sca_hit_koban_x;
				break;
			case 9:	// Sca_hit_koban_z
				ScaData2.Id.s.shape = Sca_hit_koban_z;
				break;
			}

			// Print
			tinyxml2::XMLElement * XmlMeta = Element->FirstChildElement(Get((CHAR*)"obj%02d", n));
			if (!XmlMeta) { XmlMeta = Xml->NewElement(Get((CHAR*)"obj%02d", n)); }
			XmlMeta->SetAttribute("x", ScaData2.X);
			XmlMeta->SetAttribute("z", ScaData2.Z);
			XmlMeta->SetAttribute("w", ScaData2.W);
			XmlMeta->SetAttribute("d", ScaData2.D);
			XmlMeta->SetAttribute("Shape", ScaData2.Id.s.shape);
			XmlMeta->SetAttribute("Weapon", ScaData2.Id.s.unk0);
			XmlMeta->SetAttribute("nFloor", ScaData2.Id.s.unk1);
			XmlMeta->SetAttribute("unk2", ScaData2.Id.s.unk2);
			XmlMeta->SetAttribute("Enemy", ScaData2.Id.s.unk3);
			XmlMeta->SetAttribute("unk4", ScaData2.Id.s.unk4);
			XmlMeta->SetAttribute("unk5", ScaData2.Id.s.unk5);
			XmlMeta->SetAttribute("Bullet", ScaData2.Id.s.unk6);
			XmlMeta->SetAttribute("Object", ScaData2.Id.s.unk7);
			XmlMeta->SetAttribute("Player", ScaData2.Id.s.unk8);
			XmlMeta->SetAttribute("h", ScaData2.Type.s.h);
			XmlMeta->SetAttribute("y", ScaData2.Type.s.y);
			XmlMeta->SetAttribute("Type", ScaData2.Type.s.Type);
			XmlMeta->SetAttribute("tFloor", ScaData2.Type.s.tFloor);
			XmlMeta->SetAttribute("Nine", ScaData2.Type.s.Nine);
			Element->InsertEndChild(XmlMeta);
		}
		for (ULONG n = 0; n < Collision->Header.nData2; n++) {

			// Coordinate
			ScaData2.X = Collision->Data2[n]->X;
			ScaData2.Z = Collision->Data2[n]->Z;
			ScaData2.W = Collision->Data2[n]->W;
			ScaData2.D = Collision->Data2[n]->D;
			ScaData2.Floor = 3;

			// Id
			ScaData2.Id.s.unk0 = 0x08;		// Weapon Collision ON
			ScaData2.Id.s.unk1 = 0x00;		// nFloor OFF
			ScaData2.Id.s.unk2 = 0x00;		// 
			ScaData2.Id.s.unk3 = 0x01;		// Enemy Collision ON
			ScaData2.Id.s.unk4 = 0x00;		// 
			ScaData2.Id.s.unk5 = 0x00;		// 
			ScaData2.Id.s.unk6 = 0x01;		// Bullet Collision ON
			ScaData2.Id.s.unk7 = 0x01;		// Object Collision ON
			ScaData2.Id.s.unk8 = 0x01;		// Player Collision ON

											// Type
			ScaData2.Type.s.h = 3;			// 
			ScaData2.Type.s.y = 3;			// 
			ScaData2.Type.s.Type = 0;		// Non-Interactive
			ScaData2.Type.s.tFloor = 2;		// 
			ScaData2.Type.s.Nine = 9;		// Always 9

											// Shape
			switch (Collision->Data2[n]->Id.s.shape)
			{
			case 1:	// Sca_hit_box
				ScaData2.Id.s.shape = Sca_hit_box;
				break;
			case 2:	// Sca_hit_hishi
				ScaData2.Id.s.shape = Sca_hit_hishi;
				break;
			case 3:	// Sca_hit_circle
				ScaData2.Id.s.shape = Sca_hit_circle;
				break;
			case 4:	// Sca_hit_naname_a
				ScaData2.Id.s.shape = Sca_hit_naname_a;
				break;
			case 5:	// Sca_hit_naname_b
				ScaData2.Id.s.shape = Sca_hit_naname_b;
				break;
			case 6:	// Sca_hit_naname_c
				ScaData2.Id.s.shape = Sca_hit_naname_c;
				break;
			case 7:	// Sca_hit_naname_d
				ScaData2.Id.s.shape = Sca_hit_naname_d;
				break;
			case 8:	// Sca_hit_koban_x
				ScaData2.Id.s.shape = Sca_hit_koban_x;
				break;
			case 9:	// Sca_hit_koban_z
				ScaData2.Id.s.shape = Sca_hit_koban_z;
				break;
			}

			// Print
			tinyxml2::XMLElement * XmlMeta = Element->FirstChildElement(Get((CHAR*)"obj%02d", n));
			if (!XmlMeta) { XmlMeta = Xml->NewElement(Get((CHAR*)"obj%02d", n)); }
			XmlMeta->SetAttribute("x", ScaData2.X);
			XmlMeta->SetAttribute("z", ScaData2.Z);
			XmlMeta->SetAttribute("w", ScaData2.W);
			XmlMeta->SetAttribute("d", ScaData2.D);
			XmlMeta->SetAttribute("Shape", ScaData2.Id.s.shape);
			XmlMeta->SetAttribute("Weapon", ScaData2.Id.s.unk0);
			XmlMeta->SetAttribute("nFloor", ScaData2.Id.s.unk1);
			XmlMeta->SetAttribute("unk2", ScaData2.Id.s.unk2);
			XmlMeta->SetAttribute("Enemy", ScaData2.Id.s.unk3);
			XmlMeta->SetAttribute("unk4", ScaData2.Id.s.unk4);
			XmlMeta->SetAttribute("unk5", ScaData2.Id.s.unk5);
			XmlMeta->SetAttribute("Bullet", ScaData2.Id.s.unk6);
			XmlMeta->SetAttribute("Object", ScaData2.Id.s.unk7);
			XmlMeta->SetAttribute("Player", ScaData2.Id.s.unk8);
			XmlMeta->SetAttribute("h", ScaData2.Type.s.h);
			XmlMeta->SetAttribute("y", ScaData2.Type.s.y);
			XmlMeta->SetAttribute("Type", ScaData2.Type.s.Type);
			XmlMeta->SetAttribute("tFloor", ScaData2.Type.s.tFloor);
			XmlMeta->SetAttribute("Nine", ScaData2.Type.s.Nine);
			Element->InsertEndChild(XmlMeta);
		}
		for (ULONG n = 0; n < Collision->Header.nData3; n++) {

			// Coordinate
			ScaData2.X = Collision->Data3[n]->X;
			ScaData2.Z = Collision->Data3[n]->Z;
			ScaData2.W = Collision->Data3[n]->W;
			ScaData2.D = Collision->Data3[n]->D;
			ScaData2.Floor = 3;

			// Id
			ScaData2.Id.s.unk0 = 0x08;		// Weapon Collision ON
			ScaData2.Id.s.unk1 = 0x00;		// nFloor OFF
			ScaData2.Id.s.unk2 = 0x00;		// 
			ScaData2.Id.s.unk3 = 0x01;		// Enemy Collision ON
			ScaData2.Id.s.unk4 = 0x00;		// 
			ScaData2.Id.s.unk5 = 0x00;		// 
			ScaData2.Id.s.unk6 = 0x01;		// Bullet Collision ON
			ScaData2.Id.s.unk7 = 0x01;		// Object Collision ON
			ScaData2.Id.s.unk8 = 0x01;		// Player Collision ON

											// Type
			ScaData2.Type.s.h = 3;			// 
			ScaData2.Type.s.y = 3;			// 
			ScaData2.Type.s.Type = 0;		// Non-Interactive
			ScaData2.Type.s.tFloor = 2;		// 
			ScaData2.Type.s.Nine = 9;		// Always 9

											// Shape
			switch (Collision->Data3[n]->Id.s.shape)
			{
			case 1:	// Sca_hit_box
				ScaData2.Id.s.shape = Sca_hit_box;
				break;
			case 2:	// Sca_hit_hishi
				ScaData2.Id.s.shape = Sca_hit_hishi;
				break;
			case 3:	// Sca_hit_circle
				ScaData2.Id.s.shape = Sca_hit_circle;
				break;
			case 4:	// Sca_hit_naname_a
				ScaData2.Id.s.shape = Sca_hit_naname_a;
				break;
			case 5:	// Sca_hit_naname_b
				ScaData2.Id.s.shape = Sca_hit_naname_b;
				break;
			case 6:	// Sca_hit_naname_c
				ScaData2.Id.s.shape = Sca_hit_naname_c;
				break;
			case 7:	// Sca_hit_naname_d
				ScaData2.Id.s.shape = Sca_hit_naname_d;
				break;
			case 8:	// Sca_hit_koban_x
				ScaData2.Id.s.shape = Sca_hit_koban_x;
				break;
			case 9:	// Sca_hit_koban_z
				ScaData2.Id.s.shape = Sca_hit_koban_z;
				break;
			}

			// Print
			tinyxml2::XMLElement * XmlMeta = Element->FirstChildElement(Get((CHAR*)"obj%02d", n));
			if (!XmlMeta) { XmlMeta = Xml->NewElement(Get((CHAR*)"obj%02d", n)); }
			XmlMeta->SetAttribute("x", ScaData2.X);
			XmlMeta->SetAttribute("z", ScaData2.Z);
			XmlMeta->SetAttribute("w", ScaData2.W);
			XmlMeta->SetAttribute("d", ScaData2.D);
			XmlMeta->SetAttribute("Shape", ScaData2.Id.s.shape);
			XmlMeta->SetAttribute("Weapon", ScaData2.Id.s.unk0);
			XmlMeta->SetAttribute("nFloor", ScaData2.Id.s.unk1);
			XmlMeta->SetAttribute("unk2", ScaData2.Id.s.unk2);
			XmlMeta->SetAttribute("Enemy", ScaData2.Id.s.unk3);
			XmlMeta->SetAttribute("unk4", ScaData2.Id.s.unk4);
			XmlMeta->SetAttribute("unk5", ScaData2.Id.s.unk5);
			XmlMeta->SetAttribute("Bullet", ScaData2.Id.s.unk6);
			XmlMeta->SetAttribute("Object", ScaData2.Id.s.unk7);
			XmlMeta->SetAttribute("Player", ScaData2.Id.s.unk8);
			XmlMeta->SetAttribute("h", ScaData2.Type.s.h);
			XmlMeta->SetAttribute("y", ScaData2.Type.s.y);
			XmlMeta->SetAttribute("Type", ScaData2.Type.s.Type);
			XmlMeta->SetAttribute("tFloor", ScaData2.Type.s.tFloor);
			XmlMeta->SetAttribute("Nine", ScaData2.Type.s.Nine);
			Element->InsertEndChild(XmlMeta);
		}
		for (ULONG n = 0; n < Collision->Header.nData4; n++) {

			// Coordinate
			ScaData2.X = Collision->Data4[n]->X;
			ScaData2.Z = Collision->Data4[n]->Z;
			ScaData2.W = Collision->Data4[n]->W;
			ScaData2.D = Collision->Data4[n]->D;
			ScaData2.Floor = 3;

			// Id
			ScaData2.Id.s.unk0 = 0x08;		// Weapon Collision ON
			ScaData2.Id.s.unk1 = 0x00;		// nFloor OFF
			ScaData2.Id.s.unk2 = 0x00;		// 
			ScaData2.Id.s.unk3 = 0x01;		// Enemy Collision ON
			ScaData2.Id.s.unk4 = 0x00;		// 
			ScaData2.Id.s.unk5 = 0x00;		// 
			ScaData2.Id.s.unk6 = 0x01;		// Bullet Collision ON
			ScaData2.Id.s.unk7 = 0x01;		// Object Collision ON
			ScaData2.Id.s.unk8 = 0x01;		// Player Collision ON

											// Type
			ScaData2.Type.s.h = 3;			// 
			ScaData2.Type.s.y = 3;			// 
			ScaData2.Type.s.Type = 0;		// Non-Interactive
			ScaData2.Type.s.tFloor = 2;		// 
			ScaData2.Type.s.Nine = 9;		// Always 9

											// Shape
			switch (Collision->Data4[n]->Id.s.shape)
			{
			case 1:	// Sca_hit_box
				ScaData2.Id.s.shape = Sca_hit_box;
				break;
			case 2:	// Sca_hit_hishi
				ScaData2.Id.s.shape = Sca_hit_hishi;
				break;
			case 3:	// Sca_hit_circle
				ScaData2.Id.s.shape = Sca_hit_circle;
				break;
			case 4:	// Sca_hit_naname_a
				ScaData2.Id.s.shape = Sca_hit_naname_a;
				break;
			case 5:	// Sca_hit_naname_b
				ScaData2.Id.s.shape = Sca_hit_naname_b;
				break;
			case 6:	// Sca_hit_naname_c
				ScaData2.Id.s.shape = Sca_hit_naname_c;
				break;
			case 7:	// Sca_hit_naname_d
				ScaData2.Id.s.shape = Sca_hit_naname_d;
				break;
			case 8:	// Sca_hit_koban_x
				ScaData2.Id.s.shape = Sca_hit_koban_x;
				break;
			case 9:	// Sca_hit_koban_z
				ScaData2.Id.s.shape = Sca_hit_koban_z;
				break;
			}

			// Print
			tinyxml2::XMLElement * XmlMeta = Element->FirstChildElement(Get((CHAR*)"obj%02d", n));
			if (!XmlMeta) { XmlMeta = Xml->NewElement(Get((CHAR*)"obj%02d", n)); }
			XmlMeta->SetAttribute("x", ScaData2.X);
			XmlMeta->SetAttribute("z", ScaData2.Z);
			XmlMeta->SetAttribute("w", ScaData2.W);
			XmlMeta->SetAttribute("d", ScaData2.D);
			XmlMeta->SetAttribute("Shape", ScaData2.Id.s.shape);
			XmlMeta->SetAttribute("Weapon", ScaData2.Id.s.unk0);
			XmlMeta->SetAttribute("nFloor", ScaData2.Id.s.unk1);
			XmlMeta->SetAttribute("unk2", ScaData2.Id.s.unk2);
			XmlMeta->SetAttribute("Enemy", ScaData2.Id.s.unk3);
			XmlMeta->SetAttribute("unk4", ScaData2.Id.s.unk4);
			XmlMeta->SetAttribute("unk5", ScaData2.Id.s.unk5);
			XmlMeta->SetAttribute("Bullet", ScaData2.Id.s.unk6);
			XmlMeta->SetAttribute("Object", ScaData2.Id.s.unk7);
			XmlMeta->SetAttribute("Player", ScaData2.Id.s.unk8);
			XmlMeta->SetAttribute("h", ScaData2.Type.s.h);
			XmlMeta->SetAttribute("y", ScaData2.Type.s.y);
			XmlMeta->SetAttribute("Type", ScaData2.Type.s.Type);
			XmlMeta->SetAttribute("tFloor", ScaData2.Type.s.tFloor);
			XmlMeta->SetAttribute("Nine", ScaData2.Type.s.Nine);
			Element->InsertEndChild(XmlMeta);
		}
	}

	// Terminate
	return SUCCESS;
}
Bio2RoomScaProto * Bio_Hazard_2_Proto_Room::ScaCleanup(Bio2RoomScaProto * Collision) {

	// Terminate
	return Collision;
}
// Bio 2
Bio2RoomSca * Bio_Hazard_2_Room::ScaRead(_iobuf * FILE, ULONG _Offset) {

	// Structure
	Bio2RoomSca * Collision = new tagBio2RoomSca;

	// Error
	if (!IsOpen(FILE)) { return Collision; }

	// Header
	Read(FILE, _Offset, &Collision->Header, sizeof(tagSCA_HEAD));
	Collision->Header.Num--;

	// Data
	ULONG iData = Collision->Header.Num;
	ULONG pData = Header->pSca + sizeof(tagSCA_HEAD);
	do {
		SCA_DATA Data = { NULL };
		Read(FILE, pData, &Data, sizeof(tagSCA_DATA));
		Collision->Data.push_back(Data);
		pData += sizeof(tagSCA_DATA);
		iData--;
	} while (iData);

	// Terminate
	return Collision;
}
Bio2RoomSca * Bio_Hazard_2_Room::ScaRead(CONST CHAR * _FileName, ...) {

	// _FileName
	va_list _ArgList = { NULL };
	__crt_va_start(_ArgList, _FileName);
	_StrLen = (_vscprintf(_FileName, _ArgList) + 2);
	CHAR * FileName = new CHAR[(_StrLen * 2)];
	RtlSecureZeroMemory(FileName, (_StrLen * 2));
	vsprintf_s(FileName, _StrLen, _FileName, _ArgList);
	__crt_va_end(_ArgList);

	// Error
	if (!Exists(FileName)) { delete[] FileName; return FALSE; }

	// Open
	_iobuf * _File = Open(READ_FILE, FileName);
	delete[] FileName;
	if (!_File) { return FALSE; }

	// Structure
	Bio2RoomSca * Collision = ScaRead(_File, 0x00);

	// Cleanup
	fclose(_File);

	// Terminate
	return Collision;

}
BOOL Bio_Hazard_2_Room::ScaWrite(Bio2RoomSca * Collision, _iobuf * FILE, ULONG _Offset) {

	// Error
	if (!IsOpen(FILE)) { return FAIL; }

	// Write
	Collision->Header.Num++;
	Write(FILE, _Offset, &Collision->Header, sizeof(tagSCA_HEAD));
	Write(FILE, sizeof(tagSCA_HEAD), &Collision->Data[0], sizeof(tagSCA_DATA)*Collision->Data.size());
	Collision->Header.Num--;

	// Terminate
	return SUCCESS;
}
BOOL Bio_Hazard_2_Room::ScaWrite(Bio2RoomSca * Collision, CONST CHAR * _FileName, ...) {

	// _FileName
	va_list _ArgList = { NULL };
	__crt_va_start(_ArgList, _FileName);
	_StrLen = (_vscprintf(_FileName, _ArgList) + 2);
	CHAR * FileName = new CHAR[(_StrLen * 2)];
	RtlSecureZeroMemory(FileName, (_StrLen * 2));
	vsprintf_s(FileName, _StrLen, _FileName, _ArgList);
	__crt_va_end(_ArgList);

	// Open
	_iobuf * _File = Open(CREATE_FILE, FileName);
	delete[] FileName;
	if (!_File) { return FAIL; }

	// Write
	BOOL bResult = ScaWrite(Collision, _File, 0x00);

	// Cleanup
	fclose(_File);

	// Terminate
	return bResult;
}
BOOL Bio_Hazard_2_Room::RdtScaXml(Bio2RoomSca * Collision, tinyxml2::XMLDocument * Xml, tinyxml2::XMLElement * Element) {

	// Header
	tinyxml2::XMLElement * XmlHeader = Element->FirstChildElement("header");
	if (!XmlHeader) { XmlHeader = Xml->NewElement("header"); }
	XmlHeader->SetAttribute("Cx", Collision->Header.Cx);
	XmlHeader->SetAttribute("Cz", Collision->Header.Cz);
	XmlHeader->SetAttribute("nObj", (UINT)Collision->Header.Num);
	XmlHeader->SetAttribute("Ceiling", Collision->Header.Ceiling);
	XmlHeader->SetAttribute("Dummy", Get((CHAR*)"%08X", Collision->Header.Dummy));
	Element->InsertEndChild(XmlHeader);

	// Data
	ULONG iData = NULL;
	do {

		// Print
		tinyxml2::XMLElement * XmlMeta = Element->FirstChildElement(Get((CHAR*)"obj%02d", iData));
		if (!XmlMeta) { XmlMeta = Xml->NewElement(Get((CHAR*)"obj%02d", iData)); }
		XmlMeta->SetAttribute("x", Collision->Data[iData].X);
		XmlMeta->SetAttribute("z", Collision->Data[iData].Z);
		XmlMeta->SetAttribute("w", Collision->Data[iData].W);
		XmlMeta->SetAttribute("d", Collision->Data[iData].D);
		XmlMeta->SetAttribute("Shape", Collision->Data[iData].Id.s.shape);
		XmlMeta->SetAttribute("Weapon", Collision->Data[iData].Id.s.unk0);
		XmlMeta->SetAttribute("nFloor", Collision->Data[iData].Id.s.unk1);
		XmlMeta->SetAttribute("unk2", Collision->Data[iData].Id.s.unk2);
		XmlMeta->SetAttribute("Enemy", Collision->Data[iData].Id.s.unk3);
		XmlMeta->SetAttribute("unk4", Collision->Data[iData].Id.s.unk4);
		XmlMeta->SetAttribute("unk5", Collision->Data[iData].Id.s.unk5);
		XmlMeta->SetAttribute("Bullet", Collision->Data[iData].Id.s.unk6);
		XmlMeta->SetAttribute("Object", Collision->Data[iData].Id.s.unk7);
		XmlMeta->SetAttribute("Player", Collision->Data[iData].Id.s.unk8);
		XmlMeta->SetAttribute("h", Collision->Data[iData].Type.s.h);
		XmlMeta->SetAttribute("y", Collision->Data[iData].Type.s.y);
		XmlMeta->SetAttribute("Type", Collision->Data[iData].Type.s.Type);
		XmlMeta->SetAttribute("tFloor", Collision->Data[iData].Type.s.tFloor);
		XmlMeta->SetAttribute("Nine", Collision->Data[iData].Type.s.Nine);
		Element->InsertEndChild(XmlMeta);

		// Complete
		iData++;

	} while (iData != Collision->Header.Num);


	// Terminate
	return SUCCESS;
}