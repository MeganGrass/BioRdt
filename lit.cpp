
#include "stdafx.h"
#include FRAMEWORK_H

#include BIO2_H

// Bio 1.5
LIGHT_CUT_DATA Bio_Hazard_2_Proto_Room::LitRead(_iobuf * FILE, ULONG _Offset) {

	// Buffer
	LIGHT_CUT_DATA Light = { NULL };
	RtlSecureZeroMemory(&Light, sizeof(tagLIGHT_CUT_DATA));

	// Read
	Read(FILE, _Offset, &Light, sizeof(tagLIGHT_CUT_DATA));

	// Terminate
	return Light;
}
LIGHT_CUT_DATA Bio_Hazard_2_Proto_Room::LitRead(CONST CHAR * _FileName, ...) {

	// Buffer
	LIGHT_CUT_DATA Light = { NULL };
	RtlSecureZeroMemory(&Light, sizeof(tagLIGHT_CUT_DATA));

	// _FileName
	va_list _ArgList = { NULL };
	__crt_va_start(_ArgList, _FileName);
	_StrLen = (_vscprintf(_FileName, _ArgList) + 2);
	CHAR * FileName = new CHAR[(_StrLen * 2)];
	RtlSecureZeroMemory(FileName, (_StrLen * 2));
	vsprintf_s(FileName, _StrLen, _FileName, _ArgList);
	__crt_va_end(_ArgList);

	// Error
	if (!Exists(FileName)) { delete[] FileName; return Light; }

	// Open
	_iobuf * _File = Open(READ_FILE, FileName);
	delete[] FileName;
	if (!_File) { return Light; }

	// Read
	Light = LitRead(_File, 0x00);

	// Cleanup
	fclose(_File);

	// Terminate
	return Light;
}
BOOL Bio_Hazard_2_Proto_Room::LitWrite(std::vector <LIGHT_CUT_DATA> Light, _iobuf * FILE, ULONG _Offset) {

	// Error
	if (!IsOpen(FILE)) { return FAIL; }

	// Write
	if (Light.size()) {
		Write(FILE, 0x00, &Light[0], sizeof(tagLIGHT_CUT_DATA)*Light.size());
	}
	else { return FAIL; }

	// Terminate
	return SUCCESS;
}
BOOL Bio_Hazard_2_Proto_Room::LitWrite(std::vector <LIGHT_CUT_DATA> Light, CONST CHAR * _FileName, ...) {

	// Error
	if (!Light.size()) { return FAIL; }

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
	BOOL bResult = LitWrite(Light, _File, 0x00);

	// Cleanup
	fclose(_File);

	// Terminate
	return bResult;
}
BOOL Bio_Hazard_2_Proto_Room::RdtLitRead(_iobuf * FILE) {

	// Read
	ULONG iData = Header->nCut;
	ULONG pData = Header->pLight;
	Lit.clear();
	do {
		LIGHT_CUT_DATA Data = { NULL };
		Read(FILE, pData, &Data, sizeof(tagLIGHT_CUT_DATA));
		Lit.push_back(Data);
		pData += sizeof(tagLIGHT_CUT_DATA);
		iData--;
	} while (iData);

	// Terminate
	return SUCCESS;
}
BOOL Bio_Hazard_2_Proto_Room::RdtLitXml(std::vector <LIGHT_CUT_DATA> Light, tinyxml2::XMLDocument * Xml, tinyxml2::XMLElement * Element) {

	// Print
	ULONG nCut = NULL;
	ULONG iData = NULL;
	do {
		tinyxml2::XMLElement * XmlMeta = NULL;
		XmlMeta = Element->FirstChildElement(Get((CHAR*)"cut%02d", nCut));
		if (!XmlMeta) {
			XmlMeta = Xml->NewElement(Get((CHAR*)"cut%02d", nCut));
		}
		XmlMeta->SetAttribute("Mag", Light[nCut].Mag);
		for (UCHAR i = 0; i < 3; i++) {
			tinyxml2::XMLElement * XmlBuffer = XmlMeta->FirstChildElement(Get((CHAR*)"light%02d", i));
			if (!XmlBuffer) {
				XmlBuffer = Xml->NewElement(Get((CHAR*)"light%02d", i));
			}
			XmlBuffer->SetAttribute("Mode", Light[nCut].Mode[i]);
			XmlBuffer->SetAttribute("r", Light[nCut].Col[i][0]);
			XmlBuffer->SetAttribute("g", Light[nCut].Col[i][1]);
			XmlBuffer->SetAttribute("b", Light[nCut].Col[i][2]);
			XmlBuffer->SetAttribute("Ambient", Light[nCut].Ambient[i]);
			XmlBuffer->SetAttribute("x", Light[nCut].Pos[i][0]);
			XmlBuffer->SetAttribute("y", Light[nCut].Pos[i][1]);
			XmlBuffer->SetAttribute("z", Light[nCut].Pos[i][2]);
			XmlBuffer->SetAttribute("l", Light[nCut].L[i]);
			XmlMeta->InsertEndChild(XmlBuffer);
		}
		Element->InsertEndChild(XmlMeta);
		nCut++;
		iData++;
	} while (iData != Header->nCut);

	// Terminate
	return SUCCESS;
}
// Bio 2
LIGHT_CUT_DATA Bio_Hazard_2_Room::LitRead(_iobuf * FILE, ULONG _Offset) {

	// Buffer
	LIGHT_CUT_DATA Light = { NULL };
	RtlSecureZeroMemory(&Light, sizeof(tagLIGHT_CUT_DATA));

	// Read
	Read(FILE, _Offset, &Light, sizeof(tagLIGHT_CUT_DATA));

	// Terminate
	return Light;
}
LIGHT_CUT_DATA Bio_Hazard_2_Room::LitRead(CONST CHAR * _FileName, ...) {

	// Buffer
	LIGHT_CUT_DATA Light = { NULL };
	RtlSecureZeroMemory(&Light, sizeof(tagLIGHT_CUT_DATA));

	// _FileName
	va_list _ArgList = { NULL };
	__crt_va_start(_ArgList, _FileName);
	_StrLen = (_vscprintf(_FileName, _ArgList) + 2);
	CHAR * FileName = new CHAR[(_StrLen * 2)];
	RtlSecureZeroMemory(FileName, (_StrLen * 2));
	vsprintf_s(FileName, _StrLen, _FileName, _ArgList);
	__crt_va_end(_ArgList);

	// Error
	if (!Exists(FileName)) { delete[] FileName; return Light; }

	// Open
	_iobuf * _File = Open(READ_FILE, FileName);
	delete[] FileName;
	if (!_File) { return Light; }

	// Read
	Light = LitRead(_File, 0x00);

	// Cleanup
	fclose(_File);

	// Terminate
	return Light;
}
BOOL Bio_Hazard_2_Room::LitWrite(std::vector <LIGHT_CUT_DATA> Light, _iobuf * FILE, ULONG _Offset) {

	// Error
	if (!IsOpen(FILE)) { return FAIL; }

	// Write
	if (Light.size()) {
		Write(FILE, 0x00, &Light[0], sizeof(tagLIGHT_CUT_DATA)*Light.size());
	}
	else { return FAIL; }

	// Terminate
	return SUCCESS;
}
BOOL Bio_Hazard_2_Room::LitWrite(std::vector <LIGHT_CUT_DATA> Light, CONST CHAR * _FileName, ...) {

	// Error
	if (!Light.size()) { return FAIL; }

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
	BOOL bResult = LitWrite(Light, _File, 0x00);

	// Cleanup
	fclose(_File);

	// Terminate
	return bResult;
}
BOOL Bio_Hazard_2_Room::RdtLitRead(_iobuf * FILE) {

	// Read
	ULONG iData = Header->nCut;
	ULONG pData = Header->pLight;
	Lit.clear();
	do {
		LIGHT_CUT_DATA Data = { NULL };
		Read(FILE, pData, &Data, sizeof(tagLIGHT_CUT_DATA));
		Lit.push_back(Data);
		pData += sizeof(tagLIGHT_CUT_DATA);
		iData--;
	} while (iData);

	// Terminate
	return SUCCESS;
}
BOOL Bio_Hazard_2_Room::RdtLitXml(std::vector <LIGHT_CUT_DATA> Light, tinyxml2::XMLDocument * Xml, tinyxml2::XMLElement * Element) {

	// Print
	ULONG nCut = NULL;
	ULONG iData = NULL;
	do {
		tinyxml2::XMLElement * XmlMeta = NULL;
		XmlMeta = Element->FirstChildElement(Get((CHAR*)"cut%02d", nCut));
		if (!XmlMeta) {
			XmlMeta = Xml->NewElement(Get((CHAR*)"cut%02d", nCut));
		}
		XmlMeta->SetAttribute("Mag", Light[nCut].Mag);
		for (UCHAR i = 0; i < 3; i++) {
			tinyxml2::XMLElement * XmlBuffer = XmlMeta->FirstChildElement(Get((CHAR*)"light%02d", i));
			if (!XmlBuffer) {
				XmlBuffer = Xml->NewElement(Get((CHAR*)"light%02d", i));
			}
			XmlBuffer->SetAttribute("Mode", Light[nCut].Mode[i]);
			XmlBuffer->SetAttribute("r", Light[nCut].Col[i][0]);
			XmlBuffer->SetAttribute("g", Light[nCut].Col[i][1]);
			XmlBuffer->SetAttribute("b", Light[nCut].Col[i][2]);
			XmlBuffer->SetAttribute("Ambient", Light[nCut].Ambient[i]);
			XmlBuffer->SetAttribute("x", Light[nCut].Pos[i][0]);
			XmlBuffer->SetAttribute("y", Light[nCut].Pos[i][1]);
			XmlBuffer->SetAttribute("z", Light[nCut].Pos[i][2]);
			XmlBuffer->SetAttribute("l", Light[nCut].L[i]);
			XmlMeta->InsertEndChild(XmlBuffer);
		}
		Element->InsertEndChild(XmlMeta);
		nCut++;
		iData++;
	} while (iData != Header->nCut);

	// Terminate
	return SUCCESS;
}