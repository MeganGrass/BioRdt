
#include "stdafx.h"
#include FRAMEWORK_H

#include BIO2_H

// Bio 1.5
Bio2RoomBlk * Bio_Hazard_2_Proto_Room::BlkRead(_iobuf * FILE, ULONG _Offset) {

	// Structure
	Bio2RoomBlk * Block = new tagBio2RoomBlk;
	Block->nData = NULL;
	Block->Data.clear();

	// Error
	if (!IsOpen(FILE)) { return Block; }

	// Header
	Read(FILE, _Offset, &Block->nData, sizeof(unsigned long int));

	// Data
	ULONG Pointer = _Offset + sizeof(unsigned long int);
	for (UCHAR n = 0; n < Block->nData; n++) {
		Bio2RoomBlkData * Data = new tagBio2RoomBlkData;
		Read(FILE, Pointer, Data, sizeof(tagBio2RoomBlkData));
		Block->Data.push_back(Data);
		Pointer += sizeof(tagBio2RoomBlkData);
	}

	// Terminate
	return Block;
}
Bio2RoomBlk * Bio_Hazard_2_Proto_Room::BlkRead(CONST CHAR * _FileName, ...) {

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
	Bio2RoomBlk * Block = BlkRead(_File, 0x00);

	// Cleanup
	fclose(_File);

	// Terminate
	return Block;
}
BOOL Bio_Hazard_2_Proto_Room::BlkWrite(Bio2RoomBlk * Block, _iobuf * FILE, ULONG _Offset) {

	// Error
	if (!IsOpen(FILE)) { return FAIL; }

	// Header
	Write(FILE, _Offset, &Block->nData, sizeof(unsigned long int));

	// Data
	ULONG Pointer = _Offset + sizeof(unsigned long int);
	for (UCHAR n = 0; n < Block->nData; n++) {
		Write(FILE, Pointer, Block->Data[n], sizeof(tagBio2RoomBlkData));
		Pointer += sizeof(tagBio2RoomBlkData);
	}

	// Terminate
	return SUCCESS;
}
BOOL Bio_Hazard_2_Proto_Room::BlkWrite(Bio2RoomBlk * Block, CONST CHAR * _FileName, ...) {

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
	BOOL bResult = BlkWrite(Block, _File, 0x00);

	// Cleanup
	fclose(_File);

	// Terminate
	return bResult;
}
BOOL Bio_Hazard_2_Proto_Room::RdtBlkXml(Bio2RoomBlk * Block, tinyxml2::XMLDocument * Xml, tinyxml2::XMLElement * Element) {

	// Data
	ULONG iData = NULL;
	do {

		// Print
		tinyxml2::XMLElement * XmlMeta = Element->FirstChildElement(Get((CHAR*)"dir%02d", iData));
		if (!XmlMeta) { XmlMeta = Xml->NewElement(Get((CHAR*)"dir%02d", iData)); }
		XmlMeta->SetAttribute("x0", Block->Data[iData]->Pos_x);
		XmlMeta->SetAttribute("x1", Block->Data[iData]->Size_x);
		XmlMeta->SetAttribute("z0", Block->Data[iData]->Pos_z);
		XmlMeta->SetAttribute("z1", Block->Data[iData]->Size_z);
		XmlMeta->SetAttribute("Dir_bit", Block->Data[iData]->Dir_bit);
		XmlMeta->SetAttribute("Abut", Block->Data[iData]->Abut);
		Element->InsertEndChild(XmlMeta);

		// Complete
		iData++;

	} while (iData != Block->nData);


	// Terminate
	return SUCCESS;
}
// Bio 2
Bio2RoomBlk * Bio_Hazard_2_Room::BlkRead(_iobuf * FILE, ULONG _Offset) {

	// Structure
	Bio2RoomBlk * Block = new tagBio2RoomBlk;
	Block->nData = NULL;
	Block->Data.clear();

	// Error
	if (!IsOpen(FILE)) { return Block; }

	// Header
	Read(FILE, _Offset, &Block->nData, sizeof(unsigned long int));

	// Data
	ULONG Pointer = _Offset + sizeof(unsigned long int);
	for (UCHAR n = 0; n < Block->nData; n++) {
		Bio2RoomBlkData * Data = new tagBio2RoomBlkData;
		Read(FILE, Pointer, Data, sizeof(tagBio2RoomBlkData));
		Block->Data.push_back(Data);
		Pointer += sizeof(tagBio2RoomBlkData);
	}

	// Terminate
	return Block;
}
Bio2RoomBlk * Bio_Hazard_2_Room::BlkRead(CONST CHAR * _FileName, ...) {

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
	Bio2RoomBlk * Block = BlkRead(_File, 0x00);

	// Cleanup
	fclose(_File);

	// Terminate
	return Block;
}
BOOL Bio_Hazard_2_Room::BlkWrite(Bio2RoomBlk * Block, _iobuf * FILE, ULONG _Offset) {

	// Error
	if (!IsOpen(FILE)) { return FAIL; }

	// Header
	Write(FILE, _Offset, &Block->nData, sizeof(unsigned long int));

	// Data
	ULONG Pointer = _Offset + sizeof(unsigned long int);
	for (UCHAR n = 0; n < Block->nData; n++) {
		Write(FILE, Pointer, Block->Data[n], sizeof(tagBio2RoomBlkData));
		Pointer += sizeof(tagBio2RoomBlkData);
	}

	// Terminate
	return SUCCESS;
}
BOOL Bio_Hazard_2_Room::BlkWrite(Bio2RoomBlk * Block, CONST CHAR * _FileName, ...) {

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
	BOOL bResult = BlkWrite(Block, _File, 0x00);

	// Cleanup
	fclose(_File);

	// Terminate
	return bResult;
}
BOOL Bio_Hazard_2_Room::RdtBlkXml(Bio2RoomBlk * Block, tinyxml2::XMLDocument * Xml, tinyxml2::XMLElement * Element) {

	// Data
	ULONG iData = NULL;
	do {

		// Print
		tinyxml2::XMLElement * XmlMeta = Element->FirstChildElement(Get((CHAR*)"dir%02d", iData));
		if (!XmlMeta) { XmlMeta = Xml->NewElement(Get((CHAR*)"dir%02d", iData)); }
		XmlMeta->SetAttribute("x0", Block->Data[iData]->Pos_x);
		XmlMeta->SetAttribute("x1", Block->Data[iData]->Size_x);
		XmlMeta->SetAttribute("z0", Block->Data[iData]->Pos_z);
		XmlMeta->SetAttribute("z1", Block->Data[iData]->Size_z);
		XmlMeta->SetAttribute("Dir_bit", Block->Data[iData]->Dir_bit);
		XmlMeta->SetAttribute("Abut", Block->Data[iData]->Abut);
		Element->InsertEndChild(XmlMeta);

		// Complete
		iData++;

	} while (iData != Block->nData);


	// Terminate
	return SUCCESS;
}