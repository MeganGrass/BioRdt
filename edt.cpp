
#include "stdafx.h"
#include FRAMEWORK_H

#include BIO2_H

// Bio 1.5
BOOL Bio_Hazard_2_Proto_Room::EdtWrite(std::vector <EDT_TABLE_WORK*> Data, _iobuf * FILE, ULONG _Offset) {

	// Error
	if (!Data.size()) { return FAIL; }

	// Write
	ULONG Pointer = _Offset;
	for (ULONG i = 0; i < Data.size(); i++) {
		Write(FILE, Pointer, Data[i], sizeof(tagEDT_TABLE_WORK));
		Pointer += sizeof(tagEDT_TABLE_WORK);
	}

	// Terminate
	return SUCCESS;
}
BOOL Bio_Hazard_2_Proto_Room::EdtWrite(std::vector <EDT_TABLE_WORK*> Data, CONST CHAR * _FileName, ...) {

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
	BOOL bResult = EdtWrite(Data, _File, 0x00);

	// Cleanup
	fclose(_File);

	// Terminate
	return bResult;
}
BOOL Bio_Hazard_2_Proto_Room::RdtEdtXml(std::vector <EDT_TABLE_WORK*> Data, tinyxml2::XMLDocument * Xml, tinyxml2::XMLElement * Element) {

	// Data
	tinyxml2::XMLElement * XmlBuffer = Xml->NewElement("edt");
	XmlBuffer->SetAttribute("nData", (UINT)Data.size());
	ULONG iData = NULL;
	do {

		// Print
		tinyxml2::XMLElement * XmlMeta = Xml->NewElement("data");
		XmlMeta->SetAttribute("id_check", Data[iData]->id_check);
		XmlMeta->SetAttribute("pan_prog", Data[iData]->pan_prog);
		XmlMeta->SetAttribute("tone_pri", Data[iData]->tone_pri);
		XmlMeta->SetAttribute("monopoly", Data[iData]->monopoly);
		XmlBuffer->InsertEndChild(XmlMeta);

		// Complete
		iData++;

	} while (iData != Data.size());
	Element->InsertEndChild(XmlBuffer);


	// Terminate
	return SUCCESS;
}
// Bio 2
BOOL Bio_Hazard_2_Room::EdtWrite(std::vector <EDT_TABLE_WORK*> Data, _iobuf * FILE, ULONG _Offset) {

	// Error
	if (!Data.size()) { return FAIL; }

	// Write
	ULONG Pointer = _Offset;
	for (ULONG i = 0; i < Data.size(); i++) {
		Write(FILE, Pointer, Data[i], sizeof(tagEDT_TABLE_WORK));
		Pointer += sizeof(tagEDT_TABLE_WORK);
	}

	// Terminate
	return SUCCESS;
}
BOOL Bio_Hazard_2_Room::EdtWrite(std::vector <EDT_TABLE_WORK*> Data, CONST CHAR * _FileName, ...) {

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
	BOOL bResult = EdtWrite(Data, _File, 0x00);

	// Cleanup
	fclose(_File);

	// Terminate
	return bResult;
}
BOOL Bio_Hazard_2_Room::RdtEdtXml(std::vector <EDT_TABLE_WORK*> Data, tinyxml2::XMLDocument * Xml, tinyxml2::XMLElement * Element) {

	// Data
	tinyxml2::XMLElement * XmlData = Element->FirstChildElement("edt");
	if (!XmlData) { XmlData = Xml->NewElement("edt"); }
	XmlData->SetAttribute("nData", (UINT)Data.size());
	ULONG iData = NULL;
	do {

		// Print
		tinyxml2::XMLElement * XmlMeta = XmlData->FirstChildElement(Get((CHAR*)"data%02d", iData));
		if (!XmlMeta) { XmlMeta = Xml->NewElement(Get((CHAR*)"data%02d", iData)); }
		XmlMeta->SetAttribute("id_check", Data[iData]->id_check);
		XmlMeta->SetAttribute("pan_prog", Data[iData]->pan_prog);
		XmlMeta->SetAttribute("tone_pri", Data[iData]->tone_pri);
		XmlMeta->SetAttribute("monopoly", Data[iData]->monopoly);
		XmlData->InsertEndChild(XmlMeta);

		// Complete
		iData++;

	} while (iData != Data.size());
	Element->InsertEndChild(XmlData);


	// Terminate
	return SUCCESS;
}