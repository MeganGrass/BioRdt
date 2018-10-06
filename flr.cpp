
#include "stdafx.h"
#include FRAMEWORK_H

#include BIO2_H

// Bio 1.5
Bio2RoomFlr * Bio_Hazard_2_Proto_Room::FlrRead(_iobuf * FILE, ULONG _Offset) {

	// Structure
	Bio2RoomFlr * Floor = new tagBio2RoomFlr;
	Floor->nData = NULL;
	Floor->Data.clear();

	// Error
	if (!IsOpen(FILE)) { return Floor; }

	// Header
	Read(FILE, _Offset, &Floor->nData, sizeof(unsigned short int));

	// Data
	ULONG Pointer = _Offset + sizeof(unsigned short int);
	for (USHORT n = 0; n < Floor->nData; n++) {
		Bio2RoomFlrData * Data = new tagBio2RoomFlrData;
		Read(FILE, Pointer, Data, sizeof(tagBio2RoomFlrData));
		Floor->Data.push_back(Data);
		Pointer += sizeof(tagBio2RoomFlrData);
	}

	// Terminate
	return Floor;
}
Bio2RoomFlr * Bio_Hazard_2_Proto_Room::FlrRead(CONST CHAR * _FileName, ...) {

	// _FileName
	va_list _ArgList = { NULL };
	__crt_va_start(_ArgList, _FileName);
	_StrLen = (_vscprintf(_FileName, _ArgList) + 2);
	CHAR * FileName = new CHAR[(_StrLen * 2)];
	RtlSecureZeroMemory(FileName, (_StrLen * 2));
	vsprintf_s(FileName, _StrLen, _FileName, _ArgList);
	__crt_va_end(_ArgList);

	// Error
	if (!Exists(FileName)) { delete[] FileName; return FAIL; }

	// Open
	_iobuf * _File = Open(READ_FILE, FileName);
	delete[] FileName;
	if (!_File) { return FAIL; }

	// Structure
	Bio2RoomFlr * Floor = FlrRead(_File, 0x00);

	// Cleanup
	fclose(_File);

	// Terminate
	return Floor;
}
BOOL Bio_Hazard_2_Proto_Room::FlrWrite(Bio2RoomFlr * Floor, _iobuf * FILE, ULONG _Offset) {

	// Error
	if (!IsOpen(FILE)) { return FAIL; }

	// Header
	Write(FILE, _Offset, &Floor->nData, sizeof(unsigned short int));

	// Data
	ULONG Pointer = _Offset + sizeof(unsigned short int);
	for (UCHAR n = 0; n < Floor->nData; n++) {
		Write(FILE, Pointer, Floor->Data[n], sizeof(tagBio2RoomFlrData));
		Pointer += sizeof(tagBio2RoomFlrData);
	}

	// Complete
	USHORT Terminator = NULL;
	Write(FILE, Pointer, &Terminator, sizeof(unsigned short int));

	// Terminate
	return SUCCESS;
}
BOOL Bio_Hazard_2_Proto_Room::FlrWrite(Bio2RoomFlr * Floor, CONST CHAR * _FileName, ...) {

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
	BOOL bResult = FlrWrite(Floor, _File, 0x00);

	// Cleanup
	fclose(_File);

	// Terminate
	return bResult;
}
BOOL Bio_Hazard_2_Proto_Room::RdtFlrXml(Bio2RoomFlr * Floor, tinyxml2::XMLDocument * Xml, tinyxml2::XMLElement * Element) {

	// Data
	ULONG iData = NULL;
	do {

		// Print
		tinyxml2::XMLElement * XmlMeta = Element->FirstChildElement(Get((CHAR*)"dat%02d", iData));
		if (!XmlMeta) { XmlMeta = Xml->NewElement(Get((CHAR*)"dat%02d", iData)); }
		XmlMeta->SetAttribute("x", Floor->Data[iData]->X);
		XmlMeta->SetAttribute("z", Floor->Data[iData]->Z);
		XmlMeta->SetAttribute("w", Floor->Data[iData]->W);
		XmlMeta->SetAttribute("d", Floor->Data[iData]->D);
		XmlMeta->SetAttribute("Se_no", Floor->Data[iData]->Se_no);
		XmlMeta->SetAttribute("nFloor", Floor->Data[iData]->Floor_height);
		Element->InsertEndChild(XmlMeta);

		// Complete
		iData++;

	} while (iData != Floor->nData);


	// Terminate
	return SUCCESS;
}
// Bio 2
Bio2RoomFlr * Bio_Hazard_2_Room::FlrRead(_iobuf * FILE, ULONG _Offset) {

	// Structure
	Bio2RoomFlr * Floor = new tagBio2RoomFlr;
	Floor->nData = NULL;
	Floor->Data.clear();

	// Error
	if (!IsOpen(FILE)) { return Floor; }

	// Header
	Read(FILE, _Offset, &Floor->nData, sizeof(unsigned short int));

	// Data
	ULONG Pointer = _Offset + sizeof(unsigned short int);
	for (USHORT n = 0; n < Floor->nData; n++) {
		Bio2RoomFlrData * Data = new tagBio2RoomFlrData;
		Read(FILE, Pointer, Data, sizeof(tagBio2RoomFlrData));
		Floor->Data.push_back(Data);
		Pointer += sizeof(tagBio2RoomFlrData);
	}

	// Terminate
	return Floor;
}
Bio2RoomFlr * Bio_Hazard_2_Room::FlrRead(CONST CHAR * _FileName, ...) {

	// _FileName
	va_list _ArgList = { NULL };
	__crt_va_start(_ArgList, _FileName);
	_StrLen = (_vscprintf(_FileName, _ArgList) + 2);
	CHAR * FileName = new CHAR[(_StrLen * 2)];
	RtlSecureZeroMemory(FileName, (_StrLen * 2));
	vsprintf_s(FileName, _StrLen, _FileName, _ArgList);
	__crt_va_end(_ArgList);

	// Error
	if (!Exists(FileName)) { delete[] FileName; return FAIL; }

	// Open
	_iobuf * _File = Open(READ_FILE, FileName);
	delete[] FileName;
	if (!_File) { return FAIL; }

	// Structure
	Bio2RoomFlr * Floor = FlrRead(_File, 0x00);

	// Cleanup
	fclose(_File);

	// Terminate
	return Floor;
}
BOOL Bio_Hazard_2_Room::FlrWrite(Bio2RoomFlr * Floor, _iobuf * FILE, ULONG _Offset) {

	// Error
	if (!IsOpen(FILE)) { return FAIL; }

	// Header
	Write(FILE, _Offset, &Floor->nData, sizeof(unsigned short int));

	// Data
	ULONG Pointer = _Offset + sizeof(unsigned short int);
	for (UCHAR n = 0; n < Floor->nData; n++) {
		Write(FILE, Pointer, Floor->Data[n], sizeof(tagBio2RoomFlrData));
		Pointer += sizeof(tagBio2RoomFlrData);
	}

	// Complete
	USHORT Terminator = NULL;
	Write(FILE, Pointer, &Terminator, sizeof(unsigned short int));

	// Terminate
	return SUCCESS;
}
BOOL Bio_Hazard_2_Room::FlrWrite(Bio2RoomFlr * Floor, CONST CHAR * _FileName, ...) {

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
	BOOL bResult = FlrWrite(Floor, _File, 0x00);

	// Cleanup
	fclose(_File);

	// Terminate
	return bResult;
}
BOOL Bio_Hazard_2_Room::RdtFlrXml(Bio2RoomFlr * Floor, tinyxml2::XMLDocument * Xml, tinyxml2::XMLElement * Element) {

	// Data
	ULONG iData = NULL;
	do {

		// Print
		tinyxml2::XMLElement * XmlMeta = Element->FirstChildElement(Get((CHAR*)"dat%02d", iData));
		if (!XmlMeta) { XmlMeta = Xml->NewElement(Get((CHAR*)"dat%02d", iData)); }
		XmlMeta->SetAttribute("x", Floor->Data[iData]->X);
		XmlMeta->SetAttribute("z", Floor->Data[iData]->Z);
		XmlMeta->SetAttribute("w", Floor->Data[iData]->W);
		XmlMeta->SetAttribute("d", Floor->Data[iData]->D);
		XmlMeta->SetAttribute("Se_no", Floor->Data[iData]->Se_no);
		XmlMeta->SetAttribute("nFloor", Floor->Data[iData]->Floor_height);
		Element->InsertEndChild(XmlMeta);

		// Complete
		iData++;

	} while (iData != Floor->nData);


	// Terminate
	return SUCCESS;
}