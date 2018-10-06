
#include "stdafx.h"
#include FRAMEWORK_H

#include BIO2_H

// Bio 1.5
Bio2EffectSprite * Bio_Hazard_2_Proto_Room::EspDummy(VOID) {

	// Structure
	Bio2EffectSprite * Sprite = new tagBio2EffectSprite;
	Sprite->Id.clear();
	Sprite->Pointer.clear();
	Sprite->Data.clear();
	Sprite->Tim.clear();

	// Terminate
	return Sprite;
}
BOOL Bio_Hazard_2_Proto_Room::EspWriteEff(Bio2EffectSprite * Sprite, UCHAR &Id, _iobuf * FILE, ULONG _Offset) {

	// Error
	if (!IsOpen(FILE)) { return FAIL; }

	// Write
	Write(FILE, _Offset, Sprite->Data[Id], Sprite->Size[Id]);

	// Terminate
	return SUCCESS;
}
BOOL Bio_Hazard_2_Proto_Room::EspWriteEff(Bio2EffectSprite * Sprite, UCHAR &Id, CONST CHAR * _FileName, ...) {

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
	BOOL bResult = EspWriteEff(Sprite, Id, _File, 0x00);

	// Cleanup
	fclose(_File);

	// Terminate
	return bResult;
}
BOOL Bio_Hazard_2_Proto_Room::EspWrite(Bio2EffectSprite * Sprite, CONST CHAR * _Directory, ...) {

	// _Directory
	va_list _ArgList = { NULL };
	__crt_va_start(_ArgList, _Directory);
	_StrLen = (_vscprintf(_Directory, _ArgList) + 2);
	CHAR * Directory = new CHAR[(_StrLen * 2)];
	RtlSecureZeroMemory(Directory, (_StrLen * 2));
	vsprintf_s(Directory, _StrLen, _Directory, _ArgList);
	__crt_va_end(_ArgList);

	// Write
	UCHAR nEsp = NULL;
	do {
		if (Sprite->Id[nEsp] != 0xFF && Sprite->Pointer[nEsp] != 0xFFFFFFFF) {
			EspWriteEff(Sprite, nEsp, (CHAR*)"%s\\esp%02X.eff", Directory, Sprite->Id[nEsp]);
		}
		nEsp++;
	} while (nEsp != 8);

	// Terminate
	return SUCCESS;
}
Bio2EffectSprite * Bio_Hazard_2_Proto_Room::EspReadArchive(_iobuf * FILE, ULONG pEsp_hed, ULONG pEsp_end) {

	// Structure
	Bio2EffectSprite * Sprite = EspDummy();

	// Error
	if (!pEsp_hed) { return Sprite; }
	if (!pEsp_end) { return Sprite; }
	if (!IsOpen(FILE)) { return FAIL; }

	// 
	Sprite->Id.clear();
	Sprite->Pointer.clear();
	Sprite->Size.clear();
	Sprite->Data.clear();
	UCHAR lCnt = NULL;
	UCHAR iCnt = NULL;
	UCHAR iId = NULL;
	ULONG iPointer = NULL;
	ULONG iSize = NULL;
	std::vector <ULONG> vSize;
	ULONG pppId = pEsp_hed;
	ULONG pppPointer = pEsp_end;
	ULONG pppSize = pEsp_end - sizeof(unsigned long int) * 8;
	ULONG pppData = pEsp_hed + sizeof(unsigned char) * 8;
	do {

		// Id
		Read(FILE, pppId, &iId, sizeof(unsigned char));
		Sprite->Id.push_back(iId);

		// Pointer
		Read(FILE, pppPointer, &iPointer, sizeof(unsigned long int));
		Sprite->Pointer.push_back(iPointer);

		// Size (Temp)
		if (Sprite->Id[lCnt] != 0xFF && Sprite->Pointer[lCnt] != 0xFFFFFFFF) {
			Read(FILE, pppSize, &iSize, sizeof(unsigned long int));
			vSize.push_back(iSize);
			iCnt++;
		}

		// Complete
		pppId++;
		pppPointer -= sizeof(unsigned long int);
		pppSize -= iSize + sizeof(unsigned long int);
		lCnt++;

	} while (lCnt != 8);
	lCnt = NULL;
	do {

		// Read
		if (Sprite->Id[lCnt] != 0xFF && Sprite->Pointer[lCnt] != 0xFFFFFFFF) {

			// Size
			vSize[iCnt - 1] += sizeof(unsigned long int);
			Sprite->Size.push_back(vSize[iCnt - 1]);

			// Data
			UCHAR * Buffer = new UCHAR[Sprite->Size[lCnt]];
			Read(FILE, pppData, Buffer, Sprite->Size[lCnt]);
			Sprite->Data.push_back(Buffer);

			// Complete
			iCnt--;
			pppData += Sprite->Size[lCnt];

		}
		else { break; }

		// Complete
		lCnt++;

	} while (lCnt != 8);

	// Complete
	vSize.clear();

	// Terminate
	return Sprite;
}
BOOL Bio_Hazard_2_Proto_Room::EspReadTexture(Bio2EffectSprite * Sprite, _iobuf * FILE, ULONG pEsp_tim, ULONG pEsp_tim_end) {

	// Error
	if (!pEsp_tim) { return FAIL; }
	if (!pEsp_tim_end) { return FAIL; }
	if (!IsOpen(FILE)) { return FAIL; }

	// Read
	ULONG lCnt = NULL;
	ULONG pppTexture = pEsp_tim;
	ULONG pppPointer = pEsp_tim_end - sizeof(unsigned long int);
	ULONG Pointer = NULL;
	do {
		Read(FILE, pppPointer, &Pointer, sizeof(unsigned long int));
		if (Sprite->Id[lCnt] != 0xFF && Sprite->Pointer[lCnt] != 0xFFFFFFFF && Pointer != 0xFFFFFFFF) {
			pppTexture += Pointer;
			SonyTexture * Tim = Texture->TimRead(FILE, pppTexture);
			Sprite->Tim.push_back(Tim);
		} else { break; }
		pppPointer -= sizeof(unsigned long int);
		lCnt++;
	} while (lCnt != 8);

	// Terminate
	return SUCCESS;
}
BOOL Bio_Hazard_2_Proto_Room::EspWriteTexture(Bio2EffectSprite * Sprite, CONST CHAR * _Directory, ...) {

	// _Directory
	va_list _ArgList = { NULL };
	__crt_va_start(_ArgList, _Directory);
	_StrLen = (_vscprintf(_Directory, _ArgList) + 2);
	CHAR * Directory = new CHAR[(_StrLen * 2)];
	RtlSecureZeroMemory(Directory, (_StrLen * 2));
	vsprintf_s(Directory, _StrLen, _Directory, _ArgList);
	__crt_va_end(_ArgList);

	// Write
	UCHAR nEsp = NULL;
	do {
		if (Sprite->Id[nEsp] != 0xFF && Sprite->Pointer[nEsp] != 0xFFFFFFFF) {
			Texture->TimWrite(Sprite->Tim[nEsp], (CHAR*)"%s\\esp%02X.tim", Directory, Sprite->Id[nEsp]);
		}
		nEsp++;
	} while (nEsp != 8);

	// Terminate
	return SUCCESS;
}
// Bio 2
Bio2EffectSprite * Bio_Hazard_2_Room::EspDummy(VOID) {

	// Structure
	Bio2EffectSprite * Sprite = new tagBio2EffectSprite;
	Sprite->Id.clear();
	Sprite->Pointer.clear();
	Sprite->Data.clear();
	Sprite->Tim.clear();

	// Terminate
	return Sprite;
}
BOOL Bio_Hazard_2_Room::EspWriteEff(Bio2EffectSprite * Sprite, UCHAR &Id, _iobuf * FILE, ULONG _Offset) {

	// Error
	if (!IsOpen(FILE)) { return FAIL; }

	// Write
	Write(FILE, _Offset, Sprite->Data[Id], Sprite->Size[Id]);

	// Terminate
	return SUCCESS;
}
BOOL Bio_Hazard_2_Room::EspWriteEff(Bio2EffectSprite * Sprite, UCHAR &Id, CONST CHAR * _FileName, ...) {

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
	BOOL bResult = EspWriteEff(Sprite, Id, _File, 0x00);

	// Cleanup
	fclose(_File);

	// Terminate
	return bResult;
}
BOOL Bio_Hazard_2_Room::EspWrite(Bio2EffectSprite * Sprite, CONST CHAR * _Directory, ...) {

	// _Directory
	va_list _ArgList = { NULL };
	__crt_va_start(_ArgList, _Directory);
	_StrLen = (_vscprintf(_Directory, _ArgList) + 2);
	CHAR * Directory = new CHAR[(_StrLen * 2)];
	RtlSecureZeroMemory(Directory, (_StrLen * 2));
	vsprintf_s(Directory, _StrLen, _Directory, _ArgList);
	__crt_va_end(_ArgList);

	// Write
	UCHAR nEsp = NULL;
	do {
		if (Sprite->Id[nEsp] != 0xFF && Sprite->Pointer[nEsp] != 0xFFFFFFFF) {
			EspWriteEff(Sprite, nEsp, (CHAR*)"%s\\esp%02X.eff", Directory, Sprite->Id[nEsp]);
		}
		nEsp++;
	} while (nEsp != 8);

	// Terminate
	return SUCCESS;
}
Bio2EffectSprite * Bio_Hazard_2_Room::EspReadArchive(_iobuf * FILE, ULONG pEsp_hed, ULONG pEsp_end) {

	// Structure
	Bio2EffectSprite * Sprite = EspDummy();

	// Error
	if (!pEsp_hed) { return Sprite; }
	if (!pEsp_end) { return Sprite; }
	if (!IsOpen(FILE)) { return FAIL; }

	// 
	Sprite->Id.clear();
	Sprite->Pointer.clear();
	Sprite->Size.clear();
	Sprite->Data.clear();
	UCHAR lCnt = NULL;
	UCHAR iCnt = NULL;
	UCHAR iId = NULL;
	ULONG iPointer = NULL;
	ULONG iSize = NULL;
	std::vector <ULONG> vSize;
	ULONG pppId = pEsp_hed;
	ULONG pppPointer = pEsp_end;
	ULONG pppSize = pEsp_end - sizeof(unsigned long int) * 8;
	ULONG pppData = pEsp_hed + sizeof(unsigned char) * 8;
	do {

		// Id
		Read(FILE, pppId, &iId, sizeof(unsigned char));
		Sprite->Id.push_back(iId);

		// Pointer
		Read(FILE, pppPointer, &iPointer, sizeof(unsigned long int));
		Sprite->Pointer.push_back(iPointer);

		// Size (Temp)
		if (Sprite->Id[lCnt] != 0xFF && Sprite->Pointer[lCnt] != 0xFFFFFFFF) {
			Read(FILE, pppSize, &iSize, sizeof(unsigned long int));
			vSize.push_back(iSize);
			iCnt++;
		}

		// Complete
		pppId++;
		pppPointer -= sizeof(unsigned long int);
		pppSize -= iSize + sizeof(unsigned long int);
		lCnt++;

	} while (lCnt != 8);
	lCnt = NULL;
	do {

		// Read
		if (Sprite->Id[lCnt] != 0xFF && Sprite->Pointer[lCnt] != 0xFFFFFFFF) {

			// Size
			vSize[iCnt - 1] += sizeof(unsigned long int);
			Sprite->Size.push_back(vSize[iCnt - 1]);

			// Data
			UCHAR * Buffer = new UCHAR[Sprite->Size[lCnt]];
			Read(FILE, pppData, Buffer, Sprite->Size[lCnt]);
			Sprite->Data.push_back(Buffer);

			// Complete
			iCnt--;
			pppData += Sprite->Size[lCnt];

		}
		else { break; }

		// Complete
		lCnt++;

	} while (lCnt != 8);

	// Complete
	vSize.clear();

	// Terminate
	return Sprite;
}
BOOL Bio_Hazard_2_Room::EspReadTexture(Bio2EffectSprite * Sprite, _iobuf * FILE, ULONG pEsp_tim, ULONG pEsp_tim_end) {

	// Error
	if (!pEsp_tim) { return FAIL; }
	if (!pEsp_tim_end) { return FAIL; }
	if (!IsOpen(FILE)) { return FAIL; }

	// Read
	ULONG lCnt = NULL;
	ULONG pppTexture = pEsp_tim;
	ULONG pppPointer = pEsp_tim_end - sizeof(unsigned long int);
	ULONG Pointer = NULL;
	do {
		Read(FILE, pppPointer, &Pointer, sizeof(unsigned long int));
		if (Sprite->Id[lCnt] != 0xFF && Sprite->Pointer[lCnt] != 0xFFFFFFFF && Pointer != 0xFFFFFFFF) {
			pppTexture += Pointer;
			SonyTexture * Tim = Texture->TimRead(FILE, pppTexture);
			Sprite->Tim.push_back(Tim);
		}
		else { break; }
		pppPointer -= sizeof(unsigned long int);
		lCnt++;
	} while (lCnt != 8);

	// Terminate
	return SUCCESS;
}
BOOL Bio_Hazard_2_Room::EspWriteTexture(Bio2EffectSprite * Sprite, CONST CHAR * _Directory, ...) {

	// _Directory
	va_list _ArgList = { NULL };
	__crt_va_start(_ArgList, _Directory);
	_StrLen = (_vscprintf(_Directory, _ArgList) + 2);
	CHAR * Directory = new CHAR[(_StrLen * 2)];
	RtlSecureZeroMemory(Directory, (_StrLen * 2));
	vsprintf_s(Directory, _StrLen, _Directory, _ArgList);
	__crt_va_end(_ArgList);

	// Write
	UCHAR nEsp = NULL;
	do {
		if (Sprite->Id[nEsp] != 0xFF && Sprite->Pointer[nEsp] != 0xFFFFFFFF) {
			Texture->TimWrite(Sprite->Tim[nEsp], (CHAR*)"%s\\esp%02X.tim", Directory, Sprite->Id[nEsp]);
		}
		nEsp++;
	} while (nEsp != 8);

	// Terminate
	return SUCCESS;
}
BOOL Bio_Hazard_2_Room::RdtEspXml(Bio2EffectSprite * Sprite, tinyxml2::XMLDocument * Xml, tinyxml2::XMLElement * Element) {

	ULONG lCnt = NULL;
	do {

		// Print
		if (Sprite->Id[lCnt] != 0xFF && Sprite->Pointer[lCnt] != 0xFFFFFFFF) {
			tinyxml2::XMLElement * XmlData = Element->FirstChildElement(Get((CHAR*)"data%02d", lCnt));
			if (!XmlData) { XmlData = Xml->NewElement(Get((CHAR*)"data%02d", lCnt)); }
			XmlData->SetAttribute("id", Get((CHAR*)"%02X", Sprite->Id[lCnt]));
			XmlData->SetAttribute("runtime", Get((CHAR*)"effect\\esp%02X.eff", Sprite->Id[lCnt]));
			XmlData->SetAttribute("texture", Get((CHAR*)"effect\\esp%02X.tim", Sprite->Id[lCnt]));
			Element->InsertEndChild(XmlData);

		}
		else { break; }

		// Complete
		lCnt++;

	} while (lCnt != 8);

	// Terminate
	return SUCCESS;
}