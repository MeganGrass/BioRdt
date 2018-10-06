
#include "stdafx.h"
#include FRAMEWORK_H

#include BIO2_H

// Bio 1.5
RCUT Bio_Hazard_2_Proto_Room::RidRead(_iobuf * FILE, ULONG _Offset) {

	// Buffer
	RCUT Position = { NULL };
	Position.end_flg = OFF;
	Position.ViewR = 0x683C;
	for (ULONG n = 0; n < 3; n++) { Position.View_p[n] = NULL; }
	for (ULONG n = 0; n < 3; n++) { Position.View_r[n] = NULL; }
	Position.pSp = NULL;

	// Read
	Read(FILE, _Offset, &Position, sizeof(tagRCUT));

	// Calculated on Write
	Position.end_flg = OFF;

	// Terminate
	return Position;
}
RCUT Bio_Hazard_2_Proto_Room::RidRead(CONST CHAR * _FileName, ...) {

	// Buffer
	RCUT Position = { NULL };
	Position.end_flg = OFF;
	Position.ViewR = 0x683C;
	for (ULONG n = 0; n < 3; n++) { Position.View_p[n] = NULL; }
	for (ULONG n = 0; n < 3; n++) { Position.View_r[n] = NULL; }
	Position.pSp = NULL;

	// _FileName
	va_list _ArgList = { NULL };
	__crt_va_start(_ArgList, _FileName);
	_StrLen = (_vscprintf(_FileName, _ArgList) + 2);
	CHAR * FileName = new CHAR[(_StrLen * 2)];
	RtlSecureZeroMemory(FileName, (_StrLen * 2));
	vsprintf_s(FileName, _StrLen, _FileName, _ArgList);
	__crt_va_end(_ArgList);

	// Error
	if (!Exists(FileName)) { delete[] FileName; return Position; }

	// Open
	_iobuf * _File = Open(READ_FILE, FileName);
	delete[] FileName;
	if (!_File) { return Position; }

	// Read
	Position = RidRead(_File, 0x00);

	// Cleanup
	fclose(_File);

	// Terminate
	return Position;
}
BOOL Bio_Hazard_2_Proto_Room::RidWrite(std::vector <RCUT> Position, _iobuf * FILE, ULONG _Offset) {

	// Error
	if (!IsOpen(FILE)) { return FAIL; }

	// Flag
	Position[Position.size() - 1].end_flg = ON;

	// Write
	if (Position.size()) {
		Write(FILE, 0x00, &Position[0], sizeof(tagRCUT)*Rcut.size());
	}
	else { return FAIL; }

	// Terminate
	return SUCCESS;
}
BOOL Bio_Hazard_2_Proto_Room::RidWrite(std::vector <RCUT> Position, CONST CHAR * _FileName, ...) {

	// Error
	if (!Position.size()) { return FAIL; }

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
	BOOL bResult = RidWrite(Position, _File, 0x00);

	// Cleanup
	fclose(_File);

	// Terminate
	return bResult;
}
BOOL Bio_Hazard_2_Proto_Room::RdtRidRead(_iobuf * FILE) {

	// Read
	ULONG iData = Header->nCut;
	ULONG pData = Header->pRcut;
	Rcut.clear();
	do {
		RCUT Data = RidRead(FILE, pData);
		Rcut.push_back(Data);
		pData += sizeof(tagRCUT);
		iData--;
	} while (iData);

	// Terminate
	return SUCCESS;
}
BOOL Bio_Hazard_2_Proto_Room::RdtRidXml(tinyxml2::XMLDocument * Xml, tinyxml2::XMLElement * Element) {

	// Print
	ULONG nCut = NULL;
	ULONG iData = Header->nCut;
	do {
		tinyxml2::XMLElement * XmlMeta = NULL;
		XmlMeta = Element->FirstChildElement(Get((CHAR*)"cut%02d", nCut));
		if (!XmlMeta) {
			XmlMeta = Xml->NewElement(Get((CHAR*)"cut%02d", nCut));
		}
		XmlMeta->SetAttribute("ViewR", Rcut[nCut].ViewR);
		XmlMeta->SetAttribute("FromX", Rcut[nCut].View_p[0]);
		XmlMeta->SetAttribute("FromY", Rcut[nCut].View_p[1]);
		XmlMeta->SetAttribute("FromZ", Rcut[nCut].View_p[2]);
		XmlMeta->SetAttribute("ToX", Rcut[nCut].View_r[0]);
		XmlMeta->SetAttribute("ToY", Rcut[nCut].View_r[1]);
		XmlMeta->SetAttribute("ToZ", Rcut[nCut].View_r[2]);
		Element->InsertEndChild(XmlMeta);
		nCut++;
		iData--;
	} while (iData);

	// Terminate
	return SUCCESS;
}
// Bio 2
RCUT Bio_Hazard_2_Room::RidRead(_iobuf * FILE, ULONG _Offset) {

	// Buffer
	RCUT Position = { NULL };
	Position.end_flg = OFF;
	Position.ViewR = 0x683C;
	for (ULONG n = 0; n < 3; n++) { Position.View_p[n] = NULL; }
	for (ULONG n = 0; n < 3; n++) { Position.View_r[n] = NULL; }
	Position.pSp = NULL;

	// Read
	Read(FILE, _Offset, &Position, sizeof(tagRCUT));

	// Calculated on Write
	Position.end_flg = OFF;

	// Terminate
	return Position;
}
RCUT Bio_Hazard_2_Room::RidRead(CONST CHAR * _FileName, ...) {

	// Buffer
	RCUT Position = { NULL };
	Position.end_flg = OFF;
	Position.ViewR = 0x683C;
	for (ULONG n = 0; n < 3; n++) { Position.View_p[n] = NULL; }
	for (ULONG n = 0; n < 3; n++) { Position.View_r[n] = NULL; }
	Position.pSp = NULL;

	// _FileName
	va_list _ArgList = { NULL };
	__crt_va_start(_ArgList, _FileName);
	_StrLen = (_vscprintf(_FileName, _ArgList) + 2);
	CHAR * FileName = new CHAR[(_StrLen * 2)];
	RtlSecureZeroMemory(FileName, (_StrLen * 2));
	vsprintf_s(FileName, _StrLen, _FileName, _ArgList);
	__crt_va_end(_ArgList);

	// Error
	if (!Exists(FileName)) { delete[] FileName; return Position; }

	// Open
	_iobuf * _File = Open(READ_FILE, FileName);
	delete[] FileName;
	if (!_File) { return Position; }

	// Read
	Position = RidRead(_File, 0x00);

	// Cleanup
	fclose(_File);

	// Terminate
	return Position;
}
BOOL Bio_Hazard_2_Room::RidWrite(std::vector <RCUT> Position, _iobuf * FILE, ULONG _Offset) {

	// Error
	if (!IsOpen(FILE)) { return FAIL; }

	// Flag
	Position[Position.size() - 1].end_flg = ON;

	// Write
	if (Position.size()) {
		Write(FILE, 0x00, &Position[0], sizeof(tagRCUT)*Rcut.size());
	}
	else { return FAIL; }

	// Terminate
	return SUCCESS;
}
BOOL Bio_Hazard_2_Room::RidWrite(std::vector <RCUT> Position, CONST CHAR * _FileName, ...) {

	// Error
	if (!Position.size()) { return FAIL; }

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
	BOOL bResult = RidWrite(Position, _File, 0x00);

	// Cleanup
	fclose(_File);

	// Terminate
	return bResult;
}
BOOL Bio_Hazard_2_Room::RdtRidRead(_iobuf * FILE) {

	// Read
	ULONG iData = Header->nCut;
	ULONG pData = Header->pRcut;
	Rcut.clear();
	do {
		RCUT Data = RidRead(FILE, pData);
		Rcut.push_back(Data);
		pData += sizeof(tagRCUT);
		iData--;
	} while (iData);

	// Terminate
	return SUCCESS;
}
BOOL Bio_Hazard_2_Room::RdtRidXml(tinyxml2::XMLDocument * Xml, tinyxml2::XMLElement * Element) {

	// Print
	ULONG nCut = NULL;
	ULONG iData = Header->nCut;
	do {
		tinyxml2::XMLElement * XmlMeta = NULL;
		XmlMeta = Element->FirstChildElement(Get((CHAR*)"cut%02d", nCut));
		if (!XmlMeta) {
			XmlMeta = Xml->NewElement(Get((CHAR*)"cut%02d", nCut));
		}
		XmlMeta->SetAttribute("ViewR", Rcut[nCut].ViewR);
		XmlMeta->SetAttribute("FromX", Rcut[nCut].View_p[0]);
		XmlMeta->SetAttribute("FromY", Rcut[nCut].View_p[1]);
		XmlMeta->SetAttribute("FromZ", Rcut[nCut].View_p[2]);
		XmlMeta->SetAttribute("ToX", Rcut[nCut].View_r[0]);
		XmlMeta->SetAttribute("ToY", Rcut[nCut].View_r[1]);
		XmlMeta->SetAttribute("ToZ", Rcut[nCut].View_r[2]);
		Element->InsertEndChild(XmlMeta);
		nCut++;
		iData--;
	} while (iData);

	// Terminate
	return SUCCESS;
}