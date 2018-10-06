
#include "stdafx.h"
#include FRAMEWORK_H

#include BIO2_H

// Bio 1.5
std::vector <Bio2RoomRvdData*> Bio_Hazard_2_Proto_Room::RvdSortData(std::vector <Bio2RoomRvdData*> Trigger) {

	// Sort
	std::vector <Bio2RoomRvdData*> Temp;
	for (UCHAR n = 0; n < Header->nCut; n++) {
		Bio2RoomRvdData * Temp0 = new tagBio2RoomRvdData;
		Temp.push_back(Temp0);
		for (UCHAR y = 0; y < Header->nCut; y++) {
			if (Trigger[n]->ToCut != y) {}
			else { Temp.push_back(Trigger[n]); }
		}
	}

	// Terminate
	return Temp;
}
Bio2RoomRvd * Bio_Hazard_2_Proto_Room::RvdSort(Bio2RoomRvd * Trigger) {

	// Sort
	Bio2RoomRvd * Temp = new tagBio2RoomRvd;
	for (UCHAR n = 0; n < Header->nCut; n++) {
		for (UCHAR x = 0; x < Trigger->Data.size(); x++) {
			if (Trigger->Data[x]->ToCut != n) {}
			else { Temp->Data.push_back(Trigger->Data[x]); }
		}
	}

	// Terminate
	return Temp;
}
std::vector <Bio2RoomRvd*> Bio_Hazard_2_Proto_Room::RvdSort(std::vector <Bio2RoomRvd*> Trigger) {

	// Error
	if (!Trigger.size()) { return Trigger; }

	// Sort
	std::vector <Bio2RoomRvd*> Temp;
	for (UCHAR n = 0; n < Header->nCut; n++) {
		Bio2RoomRvd * Temp0 = new tagBio2RoomRvd;
		Temp.push_back(Temp0);
		for (UCHAR y = 0; y < Header->nCut; y++) {
			for (UCHAR x = 0; x < Trigger[n]->Data.size(); x++) {
				if (Trigger[n]->Data[x]->ToCut != y) {}
				else { Temp[n]->Data.push_back(Trigger[n]->Data[x]); }
			}
		}
	}

	// Terminate
	return Temp;
}
Bio2RoomRvdData * Bio_Hazard_2_Proto_Room::RvdRead(_iobuf * FILE, ULONG _Offset) {

	// Structure
	Bio2RoomRvdData * Trigger = new tagBio2RoomRvdData;

	// Read
	Read(FILE, _Offset, Trigger, sizeof(tagBio2RoomRvdData));

	// Terminate
	return Trigger;
}
Bio2RoomRvdData * Bio_Hazard_2_Proto_Room::RvdRead(CONST CHAR * _FileName, ...) {

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
	Bio2RoomRvdData * Trigger = RvdRead(_File, 0x00);

	// Cleanup
	fclose(_File);

	// Terminate
	return Trigger;
}
BOOL Bio_Hazard_2_Proto_Room::RvdWrite(Bio2RoomRvdData * Trigger, _iobuf * FILE, ULONG _Offset) {

	// Error
	if (!IsOpen(FILE)) { return FAIL; }

	// Write
	Write(FILE, _Offset, Trigger, sizeof(tagBio2RoomRvdData));

	// Terminate
	return SUCCESS;
}
BOOL Bio_Hazard_2_Proto_Room::RvdWrite(Bio2RoomRvdData * Trigger, CONST CHAR * _FileName, ...) {

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
	BOOL bResult = RvdWrite(Trigger, _File, 0x00);

	// Cleanup
	fclose(_File);

	// Terminate
	return bResult;
}
BOOL Bio_Hazard_2_Proto_Room::RvdArchive(std::vector <Bio2RoomRvd*> Trigger, _iobuf * FILE, ULONG _Offset) {

	// Error
	if (!Trigger.size()) { return FAIL; }

	// Write
	ULONG Pointer = _Offset;
	for (UCHAR n = 0; n < Header->nCut; n++) {
		if (Trigger[n]->Data.size()) {
			for (UCHAR x = 0; x < Trigger[n]->Data.size(); x++) {
				RvdWrite(Trigger[n]->Data[x], FILE, Pointer);
				Pointer += sizeof(tagBio2RoomRvdData);
			}
		}
	}

	// Complete
	ULONG Terminator = 0xFFFFFFFF;
	Write(FILE, Pointer, &Terminator, sizeof(unsigned long int));

	// Terminate
	return SUCCESS;
}
BOOL Bio_Hazard_2_Proto_Room::RvdArchive(std::vector <Bio2RoomRvd*> Trigger, CONST CHAR * _FileName, ...) {

	// Error
	if (!Trigger.size()) { return FAIL; }

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
	BOOL bResult = RvdArchive(Trigger, _File, 0x00);

	// Cleanup
	fclose(_File);

	// Terminate
	return bResult;

	// Terminate
	return SUCCESS;
}
BOOL Bio_Hazard_2_Proto_Room::RdtRvdRead(_iobuf * FILE) {

	// Read
	ULONG nRvd = NULL;
	ULONG Terminator = NULL;
	ULONG Pointer = Header->pVcut;
	Rvd.clear();
	for (UCHAR n = 0; n < Header->nCut; n++) {

		// Buffer
		Bio2RoomRvd * Buffer = new tagBio2RoomRvd;
		Rvd.push_back(Buffer);

		// Read
		nRvd = NULL;
		ULONG pPointer = Pointer;
		do {
			Read(FILE, Pointer, &Terminator, sizeof(unsigned long int));
			if (Terminator != 0xFFFFFFFF) {
				Bio2RoomRvdData * Trigger = RvdRead(FILE, Pointer);
				if (Trigger->FromCut != n) {}
				else { Rvd[n]->Data.push_back(Trigger); }
			}
			else { break; }
			Pointer += sizeof(tagBio2RoomRvdData);
			nRvd++;
		} while (nRvd != (256 + 16) && Terminator != 0xFFFFFFFF);

		// Complete
		Rvd[n] = RvdSort(Rvd[n]);
		Pointer = pPointer;
	}

	// Terminate
	return SUCCESS;
}
BOOL Bio_Hazard_2_Proto_Room::RdtRvdXml(std::vector <Bio2RoomRvd*> Trigger, tinyxml2::XMLDocument * Xml, tinyxml2::XMLElement * Element) {

	// Error
	if (!Trigger.size()) { return FAIL; }

	// Print
	ULONG nCut = NULL;
	ULONG iData = Header->nCut;
	do {

		// Camera
		tinyxml2::XMLElement * XmlMeta = NULL;
		XmlMeta = Element->FirstChildElement(Get((CHAR*)"cut%02d", nCut));
		if (!XmlMeta) {
			XmlMeta = Xml->NewElement(Get((CHAR*)"cut%02d", nCut));
		}

		// Switch
		if (Trigger[nCut]->Data.size()) {
			for (UCHAR x = 0; x < Trigger[nCut]->Data.size(); x++) {
				tinyxml2::XMLElement * XmlData = NULL;
				if (!x) {
					XmlData = XmlMeta->FirstChildElement("view");
					if (!XmlData) {
						XmlData = Xml->NewElement("view");
					}
				} else {
					XmlData = XmlMeta->FirstChildElement(Get((CHAR*)"switch%02d", x));
					if (!XmlData) {
						XmlData = Xml->NewElement(Get((CHAR*)"switch%02d", x));
					}
				}
				XmlData->SetAttribute("ToCut", Trigger[nCut]->Data[x]->ToCut);
				XmlData->SetAttribute("OnOff", Trigger[nCut]->Data[x]->OnOff);
				XmlData->SetAttribute("nFloor", Trigger[nCut]->Data[x]->nFloor);
				XmlData->SetAttribute("x0", Trigger[nCut]->Data[x]->X0);
				XmlData->SetAttribute("x1", Trigger[nCut]->Data[x]->X1);
				XmlData->SetAttribute("x2", Trigger[nCut]->Data[x]->X2);
				XmlData->SetAttribute("x3", Trigger[nCut]->Data[x]->X3);
				XmlData->SetAttribute("z0", Trigger[nCut]->Data[x]->Z0);
				XmlData->SetAttribute("z1", Trigger[nCut]->Data[x]->Z1);
				XmlData->SetAttribute("z2", Trigger[nCut]->Data[x]->Z2);
				XmlData->SetAttribute("z3", Trigger[nCut]->Data[x]->Z3);
				XmlMeta->InsertEndChild(XmlData);
			}
		}

		// Complete
		Element->InsertEndChild(XmlMeta);
		nCut++;
		iData--;
	} while (iData);

	// Terminate
	return SUCCESS;
}
// Bio 2
std::vector <Bio2RoomRvdData*> Bio_Hazard_2_Room::RvdSortData(std::vector <Bio2RoomRvdData*> Trigger) {

	// Sort
	std::vector <Bio2RoomRvdData*> Temp;
	for (UCHAR n = 0; n < Header->nCut; n++) {
		Bio2RoomRvdData * Temp0 = new tagBio2RoomRvdData;
		Temp.push_back(Temp0);
		for (UCHAR y = 0; y < Header->nCut; y++) {
			if (Trigger[n]->ToCut != y) {}
			else { Temp.push_back(Trigger[n]); }
		}
	}

	// Terminate
	return Temp;
}
Bio2RoomRvd * Bio_Hazard_2_Room::RvdSort(Bio2RoomRvd * Trigger) {

	// Sort
	Bio2RoomRvd * Temp = new tagBio2RoomRvd;
	for (UCHAR n = 0; n < Header->nCut; n++) {
		for (UCHAR x = 0; x < Trigger->Data.size(); x++) {
			if (Trigger->Data[x]->ToCut != n) {}
			else { Temp->Data.push_back(Trigger->Data[x]); }
		}
	}

	// Terminate
	return Temp;
}
std::vector <Bio2RoomRvd*> Bio_Hazard_2_Room::RvdSort(std::vector <Bio2RoomRvd*> Trigger) {

	// Error
	if (!Trigger.size()) { return Trigger; }

	// Sort
	std::vector <Bio2RoomRvd*> Temp;
	for (UCHAR n = 0; n < Header->nCut; n++) {
		Bio2RoomRvd * Temp0 = new tagBio2RoomRvd;
		Temp.push_back(Temp0);
		for (UCHAR y = 0; y < Header->nCut; y++) {
			for (UCHAR x = 0; x < Trigger[n]->Data.size(); x++) {
				if (Trigger[n]->Data[x]->ToCut != y) {}
				else { Temp[n]->Data.push_back(Trigger[n]->Data[x]); }
			}
		}
	}

	// Terminate
	return Temp;
}
Bio2RoomRvdData * Bio_Hazard_2_Room::RvdRead(_iobuf * FILE, ULONG _Offset) {

	// Structure
	Bio2RoomRvdData * Trigger = new tagBio2RoomRvdData;

	// Read
	Read(FILE, _Offset, Trigger, sizeof(tagBio2RoomRvdData));

	// Terminate
	return Trigger;
}
Bio2RoomRvdData * Bio_Hazard_2_Room::RvdRead(CONST CHAR * _FileName, ...) {

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
	Bio2RoomRvdData * Trigger = RvdRead(_File, 0x00);

	// Cleanup
	fclose(_File);

	// Terminate
	return Trigger;
}
BOOL Bio_Hazard_2_Room::RvdWrite(Bio2RoomRvdData * Trigger, _iobuf * FILE, ULONG _Offset) {

	// Error
	if (!IsOpen(FILE)) { return FAIL; }

	// Write
	Write(FILE, _Offset, Trigger, sizeof(tagBio2RoomRvdData));

	// Terminate
	return SUCCESS;
}
BOOL Bio_Hazard_2_Room::RvdWrite(Bio2RoomRvdData * Trigger, CONST CHAR * _FileName, ...) {

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
	BOOL bResult = RvdWrite(Trigger, _File, 0x00);

	// Cleanup
	fclose(_File);

	// Terminate
	return bResult;
}
BOOL Bio_Hazard_2_Room::RvdArchive(std::vector <Bio2RoomRvd*> Trigger, _iobuf * FILE, ULONG _Offset) {

	// Error
	if (!Trigger.size()) { return FAIL; }

	// Write
	ULONG Pointer = _Offset;
	for (UCHAR n = 0; n < Header->nCut; n++) {
		Trigger[n] = RvdSort(Trigger[n]);
		if (Trigger[n]->Data.size()) {
			for (UCHAR x = 0; x < Trigger[n]->Data.size(); x++) {
				RvdWrite(Trigger[n]->Data[x], FILE, Pointer);
				Pointer += sizeof(tagBio2RoomRvdData);
			}
		}
	}

	// Complete
	ULONG Terminator = 0xFFFFFFFF;
	Write(FILE, Pointer, &Terminator, sizeof(unsigned long int));

	// Terminate
	return SUCCESS;
}
BOOL Bio_Hazard_2_Room::RvdArchive(std::vector <Bio2RoomRvd*> Trigger, CONST CHAR * _FileName, ...) {

	// Error
	if (!Trigger.size()) { return FAIL; }

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
	BOOL bResult = RvdArchive(Trigger, _File, 0x00);

	// Cleanup
	fclose(_File);

	// Terminate
	return bResult;

	// Terminate
	return SUCCESS;
}
BOOL Bio_Hazard_2_Room::RdtRvdRead(_iobuf * FILE) {

	// Read
	ULONG nRvd = NULL;
	ULONG Terminator = NULL;
	ULONG Pointer = Header->pVcut;
	Rvd.clear();
	for (UCHAR n = 0; n < Header->nCut; n++) {

		// Buffer
		Bio2RoomRvd * Buffer = new tagBio2RoomRvd;
		Rvd.push_back(Buffer);

		// Read
		nRvd = NULL;
		ULONG pPointer = Pointer;
		do {
			Read(FILE, Pointer, &Terminator, sizeof(unsigned long int));
			if (Terminator != 0xFFFFFFFF) {
				Bio2RoomRvdData * Trigger = RvdRead(FILE, Pointer);
				if (Trigger->FromCut != n) {}
				else { Rvd[n]->Data.push_back(Trigger); }
			}
			else { break; }
			Pointer += sizeof(tagBio2RoomRvdData);
			nRvd++;
		} while (nRvd != (256 + 16) && Terminator != 0xFFFFFFFF);

		// Complete
		Rvd[n] = RvdSort(Rvd[n]);
		Pointer = pPointer;
	}

	// Terminate
	return SUCCESS;
}
BOOL Bio_Hazard_2_Room::RdtRvdXml(std::vector <Bio2RoomRvd*> Trigger, tinyxml2::XMLDocument * Xml, tinyxml2::XMLElement * Element) {

	// Error
	if (!Trigger.size()) { return FAIL; }

	// Print
	ULONG nCut = NULL;
	ULONG iData = Header->nCut;
	do {

		// Camera
		tinyxml2::XMLElement * XmlMeta = NULL;
		XmlMeta = Element->FirstChildElement(Get((CHAR*)"cut%02d", nCut));
		if (!XmlMeta) {
			XmlMeta = Xml->NewElement(Get((CHAR*)"cut%02d", nCut));
		}

		// Switch
		if (Trigger[nCut]->Data.size()) {
			for (UCHAR x = 0; x < Trigger[nCut]->Data.size(); x++) {
				tinyxml2::XMLElement * XmlData = NULL;
				if (!x) {
					XmlData = XmlMeta->FirstChildElement("view");
					if (!XmlData) {
						XmlData = Xml->NewElement("view");
					}
				} else {
					XmlData = XmlMeta->FirstChildElement(Get((CHAR*)"switch%02d", x));
					if (!XmlData) {
						XmlData = Xml->NewElement(Get((CHAR*)"switch%02d", x));
					}
				}
				XmlData->SetAttribute("ToCut", Trigger[nCut]->Data[x]->ToCut);
				XmlData->SetAttribute("OnOff", Trigger[nCut]->Data[x]->OnOff);
				XmlData->SetAttribute("nFloor", Trigger[nCut]->Data[x]->nFloor);
				XmlData->SetAttribute("x0", Trigger[nCut]->Data[x]->X0);
				XmlData->SetAttribute("x1", Trigger[nCut]->Data[x]->X1);
				XmlData->SetAttribute("x2", Trigger[nCut]->Data[x]->X2);
				XmlData->SetAttribute("x3", Trigger[nCut]->Data[x]->X3);
				XmlData->SetAttribute("z0", Trigger[nCut]->Data[x]->Z0);
				XmlData->SetAttribute("z1", Trigger[nCut]->Data[x]->Z1);
				XmlData->SetAttribute("z2", Trigger[nCut]->Data[x]->Z2);
				XmlData->SetAttribute("z3", Trigger[nCut]->Data[x]->Z3);
				XmlMeta->InsertEndChild(XmlData);
			}
		}

		// Complete
		Element->InsertEndChild(XmlMeta);
		nCut++;
		iData--;
	} while (iData);

	// Terminate
	return SUCCESS;
}
