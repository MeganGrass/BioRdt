
#include "stdafx.h"
#include FRAMEWORK_H

#include BIO2_H

// Bio 1.5
Bio2RoomPri * Bio_Hazard_2_Proto_Room::PriRead(_iobuf * FILE, ULONG _Offset) {

	// Structure
	Bio2RoomPri * Buffer = new tagBio2RoomPri;
	Buffer->nIndex = 0xFFFF;
	Buffer->nData = 0xFFFF;
	Buffer->Size = 0x04;
	Buffer->Index.clear();

	// Error
	if (!IsOpen(FILE)) { return Buffer; }
	if (_Offset != NULL && _Offset != 0xFFFFFFFF) {
	}
	else { return Buffer; }

	// Read
	ULONG Pointer = _Offset;
	Bio2RoomPriIndex * Index = { NULL };
	Bio2RoomPriData * Data = { NULL };
	Read(FILE, _Offset, &Buffer->nIndex, sizeof(unsigned short int));
	Read(FILE, _Offset + 2, &Buffer->nData, sizeof(unsigned short int));
	_Offset += 0x04;
	if (Buffer->nIndex != NULL && Buffer->nIndex != 0xFFFF) {
		for (USHORT x = 0; x < Buffer->nIndex; x++, _Offset += 0x08) {
			Index = new Bio2RoomPriIndex;
			Index->Data.clear();
			Read(FILE, _Offset, Index, 0x08);
			Buffer->Index.push_back(Index);
		}
		for (USHORT x = 0; x < Buffer->nIndex; x++) {
			for (USHORT y = 0; y < Buffer->Index[x]->nSprite; y++) {
				Data = new tagBio2RoomPriData;
				Buffer->Index[x]->Data.push_back(Data);
				Read(FILE, _Offset, &Buffer->Index[x]->Data[y]->Rect, sizeof(tagBio2RoomPriRect));
				Read(FILE, _Offset, &Buffer->Index[x]->Data[y]->Square, sizeof(tagBio2RoomPriSquare));
				if (!GetByte(1, Buffer->Index[x]->Data[y]->Rect.tPage)) {
					_Offset += sizeof(tagBio2RoomPriRect);
					Buffer->Index[x]->Data[y]->bSquare = FALSE;
				}
				else {
					_Offset += sizeof(tagBio2RoomPriSquare);
					Buffer->Index[x]->Data[y]->bSquare = TRUE;
				}
			}
		}
	}
	else {
		Buffer->nIndex = 0xFFFF;
		Buffer->nData = 0xFFFF;
	}

	// Complete
	Buffer->Size = (_Offset - Pointer);

	// Terminate
	return Buffer;
}
Bio2RoomPri * Bio_Hazard_2_Proto_Room::PriRead(CONST CHAR * _FileName, ...) {

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
	Bio2RoomPri * Buffer = PriRead(_File, 0x00);

	// Cleanup
	fclose(_File);

	// Terminate
	return Buffer;

}
BOOL Bio_Hazard_2_Proto_Room::PriWrite(Bio2RoomPri * Sprite, BOOL Convert, _iobuf * FILE, ULONG _Offset) {

	// Error
	if (!IsOpen(FILE)) { return FAIL; }

	// Write
	ULONG Depth = NULL;
	ULONG tPage = NULL;
	ULONG Size = NULL;
	Write(FILE, _Offset, &Sprite->nIndex, sizeof(unsigned short int));
	Write(FILE, _Offset + 2, &Sprite->nData, sizeof(unsigned short int));
	if (Sprite->nIndex != NULL && Sprite->nIndex != 0xFFFF)
	{
		_Offset += 0x04;
		for (USHORT x = 0; x < Sprite->nIndex; x++, _Offset += 0x08) {
			Write(FILE, _Offset, Sprite->Index[x], 0x08);
		}
		for (USHORT x = 0; x < Sprite->nIndex; x++) {
			for (USHORT y = 0; y < Sprite->Index[x]->nSprite; y++) {
				if (!Sprite->Index[x]->Data[y]->bSquare) {
					Depth = Sprite->Index[x]->Data[y]->Rect.Depth;
					tPage = Sprite->Index[x]->Data[y]->Rect.tPage;
					if (Convert) {
						Sprite->Index[x]->Data[y]->Rect.tPage = NULL;
						Sprite->Index[x]->Data[y]->Rect.Depth *= 2;
					}
					Write(FILE, _Offset, &Sprite->Index[x]->Data[y]->Rect, sizeof(tagBio2RoomPriRect));
					_Offset += sizeof(tagBio2RoomPriRect);
					Sprite->Index[x]->Data[y]->Rect.tPage = tPage;
					Sprite->Index[x]->Data[y]->Rect.Depth = Depth;
				}
				else {
					Depth = Sprite->Index[x]->Data[y]->Square.Depth;
					Size = Sprite->Index[x]->Data[y]->Square.Size;
					if (Convert) {
						Sprite->Index[x]->Data[y]->Square.Depth *= 2;
						Sprite->Index[x]->Data[y]->Square.Size = GetByte(1, Sprite->Index[x]->Data[y]->Square.Size);
					}
					Write(FILE, _Offset, &Sprite->Index[x]->Data[y]->Square, sizeof(tagBio2RoomPriSquare));
					_Offset += sizeof(tagBio2RoomPriSquare);
					Sprite->Index[x]->Data[y]->Square.Size = Size;
					Sprite->Index[x]->Data[y]->Square.Depth = Depth;
				}
			}
		}
	}

	// Terminate
	return SUCCESS;
}
BOOL Bio_Hazard_2_Proto_Room::PriWrite(Bio2RoomPri * Sprite, BOOL Convert, CONST CHAR * _FileName, ...) {

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
	BOOL bResult = PriWrite(Sprite, Convert, _File, 0x00);

	// Cleanup
	fclose(_File);

	// Terminate
	return bResult;
}
BOOL Bio_Hazard_2_Proto_Room::PriArchive(std::vector <Bio2RoomPri*> Sprite, BOOL Convert, _iobuf * FILE, ULONG _Offset) {

	// Error
	if (!Sprite.size()) { return FAIL; }

	// Work
	BOOL bResult = NULL;
	ULONG nSprite = NULL;
	ULONG nSpriteMax = NULL;
	std::vector <ULONG> Pointer;
	Pointer.clear();
	for (UCHAR n = 0; n < Header->nCut; n++) {

		// Write
		Pointer.push_back(_Offset);
		bResult = PriWrite(Sprite[n], Convert, FILE, _Offset);
		if (!bResult) { return bResult; }
		_Offset += Sprite[n]->Size;

		// nSprite
		if (Sprite[n]->nData != NULL && Sprite[n]->nData != 0xFFFF) {
			nSprite = GetU32(Sprite[n]->nData);
			if (nSprite >= (nSpriteMax + 1)) { nSpriteMax = nSprite; }
		}

	}

	// Index
	ULONG nCut = Header->nCut - 1;
	do {
		Write(FILE, _Offset, &Pointer[nCut], sizeof(unsigned long int));
		_Offset += sizeof(unsigned long int);
		nCut--;
	} while (nCut);
	nCut = NULL;
	Write(FILE, _Offset, &nCut, sizeof(unsigned long int));
	Write(FILE, _Offset + 4, &nSpriteMax, sizeof(unsigned long int));
	Pointer.clear();

	// Terminate
	return SUCCESS;
}
BOOL Bio_Hazard_2_Proto_Room::PriArchive(std::vector <Bio2RoomPri*> Sprite, BOOL Convert, CONST CHAR * _FileName, ...) {

	// Error
	if (!Sprite.size()) { return FAIL; }

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
	BOOL bResult = PriArchive(Sprite, Convert, _File, 0x00);

	// Cleanup
	fclose(_File);

	// Terminate
	return bResult;
}
BOOL Bio_Hazard_2_Proto_Room::RdtPriXml(std::vector <RCUT> Rcut, std::vector <Bio2RoomPri*> Sprite, BOOL Convert, tinyxml2::XMLDocument * Xml, tinyxml2::XMLElement * Element) {

	// Error
	if (!Rcut.size()) { return FAIL; }
	if (!Sprite.size()) { return FAIL; }

	// Print
	ULONG nCut = NULL;
	ULONG Depth = NULL;
	ULONG tPage = NULL;
	ULONG Size = NULL;
	ULONG iData = Header->nCut;
	do {

		// Camera
		tinyxml2::XMLElement * XmlMeta = NULL;
		XmlMeta = Element->FirstChildElement(Get((CHAR*)"cut%02d", nCut));
		if (!XmlMeta) {
			XmlMeta = Xml->NewElement(Get((CHAR*)"cut%02d", nCut));
		}

		// Switch
		if (Sprite[nCut]->nData != NULL && Sprite[nCut]->nData != 0xFFFF) {
			for (USHORT x = 0; x < Sprite[nCut]->nIndex; x++) {
				tinyxml2::XMLElement * XmlIndex = Xml->NewElement("obj");
				XmlIndex->SetAttribute("nSprite", Sprite[nCut]->Index[x]->nSprite);
				XmlIndex->SetAttribute("unk", Sprite[nCut]->Index[x]->unk);
				XmlIndex->SetAttribute("x", Sprite[nCut]->Index[x]->X);
				XmlIndex->SetAttribute("y", Sprite[nCut]->Index[x]->Y);
				for (USHORT y = 0; y < Sprite[nCut]->Index[x]->nSprite; y++) {
					tinyxml2::XMLElement * XmlData = Xml->NewElement("data");
					if (!Sprite[nCut]->Index[x]->Data[y]->bSquare) {
						Depth = Sprite[nCut]->Index[x]->Data[y]->Rect.Depth;
						tPage = Sprite[nCut]->Index[x]->Data[y]->Rect.tPage;
						if (Convert) {
							Sprite[nCut]->Index[x]->Data[y]->Rect.tPage = NULL;
							Sprite[nCut]->Index[x]->Data[y]->Rect.Depth *= 2;
						}
						XmlData->SetAttribute("type", "rect");
						XmlData->SetAttribute("u", Sprite[nCut]->Index[x]->Data[y]->Rect.u);
						XmlData->SetAttribute("v", Sprite[nCut]->Index[x]->Data[y]->Rect.v);
						XmlData->SetAttribute("x", Sprite[nCut]->Index[x]->Data[y]->Rect.x);
						XmlData->SetAttribute("y", Sprite[nCut]->Index[x]->Data[y]->Rect.y);
						XmlData->SetAttribute("w", Sprite[nCut]->Index[x]->Data[y]->Rect.w);
						XmlData->SetAttribute("h", Sprite[nCut]->Index[x]->Data[y]->Rect.h);
						XmlData->SetAttribute("Depth", Sprite[nCut]->Index[x]->Data[y]->Rect.Depth);
						Sprite[nCut]->Index[x]->Data[y]->Rect.tPage = tPage;
						Sprite[nCut]->Index[x]->Data[y]->Rect.Depth = Depth;
					} else {
						Depth = Sprite[nCut]->Index[x]->Data[y]->Square.Depth;
						Size = Sprite[nCut]->Index[x]->Data[y]->Square.Size;
						if (Convert) {
							Sprite[nCut]->Index[x]->Data[y]->Square.Depth *= 2;
							Sprite[nCut]->Index[x]->Data[y]->Square.Size = GetByte(1, Sprite[nCut]->Index[x]->Data[y]->Square.Size);
						}
						XmlData->SetAttribute("type", "square");
						XmlData->SetAttribute("u", Sprite[nCut]->Index[x]->Data[y]->Square.u);
						XmlData->SetAttribute("v", Sprite[nCut]->Index[x]->Data[y]->Square.v);
						XmlData->SetAttribute("x", Sprite[nCut]->Index[x]->Data[y]->Square.x);
						XmlData->SetAttribute("y", Sprite[nCut]->Index[x]->Data[y]->Square.y);
						XmlData->SetAttribute("w", Sprite[nCut]->Index[x]->Data[y]->Square.Size);
						XmlData->SetAttribute("Depth", Sprite[nCut]->Index[x]->Data[y]->Square.Depth);
						Sprite[nCut]->Index[x]->Data[y]->Square.Size = Size;
						Sprite[nCut]->Index[x]->Data[y]->Square.Depth = Depth;
					}
					XmlIndex->InsertEndChild(XmlData);
				}
				XmlMeta->InsertEndChild(XmlIndex);
			}
		} else {
			tinyxml2::XMLElement * XmlData = Xml->NewElement("data");
			XmlData->SetAttribute("type", -1);
			XmlMeta->InsertEndChild(XmlData);
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
Bio2RoomPri * Bio_Hazard_2_Room::PriRead(_iobuf * FILE, ULONG _Offset) {

	// Structure
	Bio2RoomPri * Buffer = new tagBio2RoomPri;
	Buffer->nIndex = 0xFFFF;
	Buffer->nData = 0xFFFF;
	Buffer->Size = 0x04;
	Buffer->Index.clear();

	// Error
	if (!IsOpen(FILE)) { return Buffer; }
	if (_Offset != NULL && _Offset != 0xFFFFFFFF) {
	} else { return Buffer; }

	// Read
	ULONG Pointer = _Offset;
	Bio2RoomPriIndex * Index = { NULL };
	Bio2RoomPriData * Data = { NULL };
	Read(FILE, _Offset, &Buffer->nIndex, sizeof(unsigned short int));
	Read(FILE, _Offset + 2, &Buffer->nData, sizeof(unsigned short int));
	_Offset += 0x04;
	if (Buffer->nIndex != NULL && Buffer->nIndex != 0xFFFF) {
		for (USHORT x = 0; x < Buffer->nIndex; x++, _Offset += 0x08) {
			Index = new Bio2RoomPriIndex;
			Index->Data.clear();
			Read(FILE, _Offset, Index, 0x08);
			Buffer->Index.push_back(Index);
		}
		for (USHORT x = 0; x < Buffer->nIndex; x++) {
			for (USHORT y = 0; y < Buffer->Index[x]->nSprite; y++) {
				Data = new tagBio2RoomPriData;
				Buffer->Index[x]->Data.push_back(Data);
				Read(FILE, _Offset, &Buffer->Index[x]->Data[y]->Rect, sizeof(tagBio2RoomPriRect));
				Read(FILE, _Offset, &Buffer->Index[x]->Data[y]->Square, sizeof(tagBio2RoomPriSquare));
				if (!Buffer->Index[x]->Data[y]->Rect.tPage) {
					_Offset += sizeof(tagBio2RoomPriRect);
					Buffer->Index[x]->Data[y]->bSquare = FALSE;
				} else {
					_Offset += sizeof(tagBio2RoomPriSquare);
					Buffer->Index[x]->Data[y]->bSquare = TRUE;
				}
			}
		}
	} else {
		Buffer->nIndex = 0xFFFF;
		Buffer->nData = 0xFFFF;
	}

	// Complete
	Buffer->Size = (_Offset - Pointer);

	// Terminate
	return Buffer;
}
Bio2RoomPri * Bio_Hazard_2_Room::PriRead(CONST CHAR * _FileName, ...) {

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
	Bio2RoomPri * Buffer = PriRead(_File, 0x00);

	// Cleanup
	fclose(_File);

	// Terminate
	return Buffer;

}
BOOL Bio_Hazard_2_Room::PriWrite(Bio2RoomPri * Sprite, _iobuf * FILE, ULONG _Offset) {

	// Error
	if (!IsOpen(FILE)) { return FAIL; }

	// Write
	Write(FILE, _Offset, &Sprite->nIndex, sizeof(unsigned short int));
	Write(FILE, _Offset + 2, &Sprite->nData, sizeof(unsigned short int));
	if (Sprite->nIndex != NULL && Sprite->nIndex != 0xFFFF)
	{
		_Offset += 0x04;
		for (USHORT x = 0; x < Sprite->nIndex; x++, _Offset += 0x08) {
			Write(FILE, _Offset, Sprite->Index[x], 0x08);
		}
		for (USHORT x = 0; x < Sprite->nIndex; x++) {
			for (USHORT y = 0; y < Sprite->Index[x]->nSprite; y++) {
				if (!Sprite->Index[x]->Data[y]->bSquare) {
					Write(FILE, _Offset, &Sprite->Index[x]->Data[y]->Rect, sizeof(tagBio2RoomPriRect));
					_Offset += sizeof(tagBio2RoomPriRect);
				}
				else {
					Write(FILE, _Offset, &Sprite->Index[x]->Data[y]->Square, sizeof(tagBio2RoomPriSquare));
					_Offset += sizeof(tagBio2RoomPriSquare);
				}
			}
		}
		/*for (USHORT x = 0; x < Sprite->nData; x++) {
			if (!Sprite->Data[x]->bSquare) {
				Write(FILE, _Offset, &Sprite->Data[x]->Rect, sizeof(tagBio2RoomPriRect));
				_Offset += sizeof(tagBio2RoomPriRect);
			} else {
				Write(FILE, _Offset, &Sprite->Data[x]->Square, sizeof(tagBio2RoomPriSquare));
				_Offset += sizeof(tagBio2RoomPriSquare);
			}
		}*/
	}

	// Terminate
	return SUCCESS;
}
BOOL Bio_Hazard_2_Room::PriWrite(Bio2RoomPri * Sprite, CONST CHAR * _FileName, ...) {

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
	BOOL bResult = PriWrite(Sprite, _File, 0x00);

	// Cleanup
	fclose(_File);

	// Terminate
	return bResult;
}
BOOL Bio_Hazard_2_Room::PriArchive(std::vector <Bio2RoomPri*> Sprite, _iobuf * FILE, ULONG _Offset) {

	// Error
	if (!Sprite.size()) { return FAIL; }

	// Work
	BOOL bResult = NULL;
	ULONG nSprite = NULL;
	ULONG nSpriteMax = NULL;
	std::vector <ULONG> Pointer;
	Pointer.clear();
	for (UCHAR n = 0; n < Header->nCut; n++) {

		// Write
		Pointer.push_back(_Offset);
		bResult = PriWrite(Sprite[n], FILE, _Offset);
		if (!bResult) { return bResult; }
		_Offset += Sprite[n]->Size;

		// nSprite
		if (Sprite[n]->nData != NULL && Sprite[n]->nData != 0xFFFF) {
			nSprite = GetU32(Sprite[n]->nData);
			if (nSprite >= (nSpriteMax + 1)) { nSpriteMax = nSprite; }
		}

	}

	// Index
	ULONG nCut = Header->nCut - 1;
	do {
		Write(FILE, _Offset, &Pointer[nCut], sizeof(unsigned long int));
		_Offset += sizeof(unsigned long int);
		nCut--;
	} while (nCut);
	nCut = NULL;
	Write(FILE, _Offset, &nCut, sizeof(unsigned long int));
	Write(FILE, _Offset + 4, &nSpriteMax, sizeof(unsigned long int));
	Pointer.clear();

	// Terminate
	return SUCCESS;
}
BOOL Bio_Hazard_2_Room::PriArchive(std::vector <Bio2RoomPri*> Sprite, CONST CHAR * _FileName, ...) {

	// Error
	if (!Sprite.size()) { return FAIL; }

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
	BOOL bResult = PriArchive(Sprite, _File, 0x00);

	// Cleanup
	fclose(_File);

	// Terminate
	return bResult;
}
BOOL Bio_Hazard_2_Room::RdtPriXml(std::vector <RCUT> Rcut, std::vector <Bio2RoomPri*> Sprite, tinyxml2::XMLDocument * Xml, tinyxml2::XMLElement * Element) {

	// Error
	if (!Rcut.size()) { return FAIL; }
	if (!Sprite.size()) { return FAIL; }

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
		if (Sprite[nCut]->nData != NULL && Sprite[nCut]->nData != 0xFFFF) {
			for (USHORT x = 0; x < Sprite[nCut]->nIndex; x++) {
				tinyxml2::XMLElement * XmlIndex = XmlMeta->FirstChildElement(Get((CHAR*)"obj%02d", x));
				if (!XmlIndex) { XmlIndex = Xml->NewElement(Get((CHAR*)"obj%02d", x)); }
				XmlIndex->SetAttribute("nSprite", Sprite[nCut]->Index[x]->nSprite);
				XmlIndex->SetAttribute("unk", Sprite[nCut]->Index[x]->unk);
				XmlIndex->SetAttribute("x", Sprite[nCut]->Index[x]->X);
				XmlIndex->SetAttribute("y", Sprite[nCut]->Index[x]->Y);
				for (USHORT y = 0; y < Sprite[nCut]->Index[x]->nSprite; y++) {
					tinyxml2::XMLElement * XmlData = XmlIndex->FirstChildElement(Get((CHAR*)"data%02d", y));
					if (!XmlData) { XmlData = Xml->NewElement(Get((CHAR*)"data%02d", y)); }
					if (!Sprite[nCut]->Index[x]->Data[y]->bSquare) {
						XmlData->SetAttribute("type", "rect");
						XmlData->SetAttribute("u", Sprite[nCut]->Index[x]->Data[y]->Rect.u);
						XmlData->SetAttribute("v", Sprite[nCut]->Index[x]->Data[y]->Rect.v);
						XmlData->SetAttribute("x", Sprite[nCut]->Index[x]->Data[y]->Rect.x);
						XmlData->SetAttribute("y", Sprite[nCut]->Index[x]->Data[y]->Rect.y);
						XmlData->SetAttribute("w", Sprite[nCut]->Index[x]->Data[y]->Rect.w);
						XmlData->SetAttribute("h", Sprite[nCut]->Index[x]->Data[y]->Rect.h);
						XmlData->SetAttribute("Depth", Sprite[nCut]->Index[x]->Data[y]->Rect.Depth);
					} else {
						XmlData->SetAttribute("type", "square");
						XmlData->SetAttribute("u", Sprite[nCut]->Index[x]->Data[y]->Square.u);
						XmlData->SetAttribute("v", Sprite[nCut]->Index[x]->Data[y]->Square.v);
						XmlData->SetAttribute("x", Sprite[nCut]->Index[x]->Data[y]->Square.x);
						XmlData->SetAttribute("y", Sprite[nCut]->Index[x]->Data[y]->Square.y);
						XmlData->SetAttribute("w", Sprite[nCut]->Index[x]->Data[y]->Square.Size);
						XmlData->SetAttribute("Depth", Sprite[nCut]->Index[x]->Data[y]->Square.Depth);
					}
					XmlIndex->InsertEndChild(XmlData);
				}
				XmlMeta->InsertEndChild(XmlIndex);
			}
		} else {
			tinyxml2::XMLElement * XmlData = Xml->NewElement("data");
			XmlData->SetAttribute("type", -1);
			XmlMeta->InsertEndChild(XmlData);
		}

		// Complete
		Element->InsertEndChild(XmlMeta);
		nCut++;
		iData--;
	} while (iData);

	// Terminate
	return SUCCESS;
}