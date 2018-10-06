
#include "stdafx.h"
#include FRAMEWORK_H

#include BIO2_H

// Boot
Bio_Hazard_2_Proto_Room::Bio_Hazard_2_Proto_Room(VOID) {
}
Bio_Hazard_2_Proto_Room::~Bio_Hazard_2_Proto_Room(VOID) {
}
Bio_Hazard_2_Room::Bio_Hazard_2_Room(VOID) {
}
Bio_Hazard_2_Room::~Bio_Hazard_2_Room(VOID) {
}
// Bio 1.5
BOOL Bio_Hazard_2_Proto_Room::RdtRead(CONST CHAR * _FileName, ...) {

	// Flag
	RdtIsLoaded = FALSE;

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
	if (!_File) {
		delete[] FileName;
		return FALSE;
	}

	// Xml
	tinyxml2::XMLDocument Xml;
	tinyxml2::XMLElement * pRoot = Xml.NewElement("rdt");
	tinyxml2::XMLElement * XmlRid = Xml.NewElement("rid");
	tinyxml2::XMLElement * XmlHeader = Xml.NewElement("header");
	tinyxml2::XMLElement * XmlRvd = Xml.NewElement("rvd");
	tinyxml2::XMLElement * XmlLit = Xml.NewElement("lit");
	tinyxml2::XMLElement * XmlPri = Xml.NewElement("pri");
	tinyxml2::XMLElement * XmlSca = Xml.NewElement("sca");
	tinyxml2::XMLElement * XmlBlk = Xml.NewElement("blk");
	tinyxml2::XMLElement * XmlFlr = Xml.NewElement("flr");
	tinyxml2::XMLElement * XmlSound = Xml.NewElement("sound");

	// Meta
	Directory = GetDirectory(FileName);
	CHAR * NameBase = GetNamebase(FileName);
	Stage_no = ToInt((CHAR *)"0x%c\0", NameBase[4]);
	Room_no = ToInt((CHAR *)"0x%c%c\0", NameBase[5], NameBase[6]);
	Pl_id = ToInt((CHAR *)"0x%c\0", NameBase[7]);
	delete[] NameBase;
	delete[] FileName;
	pRoot->SetAttribute("game", "Bio2");
	if (ConvertToFinal) { pRoot->SetAttribute("version", "Final"); pRoot->SetAttribute("original", "1996Nov05"); }
	else { pRoot->SetAttribute("version", "1996Nov05"); }
	pRoot->SetAttribute("stage", Stage_no);
	pRoot->SetAttribute("room", Room_no);
	pRoot->SetAttribute("player", Pl_id);

	// Header
	Header = new tagBio2RoomProto;
	Read(_File, 0x00, Header, sizeof(tagBio2RoomProto));
	XmlHeader->SetAttribute("nSprite", Header->nSprite);
	XmlHeader->SetAttribute("nCut", Header->nCut);
	XmlHeader->SetAttribute("nOmodel", Header->nOmodel);
	XmlHeader->SetAttribute("nItem", Header->nItem);
	XmlHeader->SetAttribute("nDoor", Header->nDoor);
	XmlHeader->SetAttribute("nRoom_at", Header->nRoom_at);
	XmlHeader->SetAttribute("Reverb_lv", Header->Reverb_lv);

	// Camera Position
	if (Header->pRcut) {
		RdtRidRead(_File);
		RdtRidXml(&Xml, XmlRid);
		RidWrite(Rcut, (CHAR*)"%sROOM%X%02X%X\\camera.rid", Directory, Stage_no, Room_no, Pl_id);
		XmlRid->SetAttribute("file", "camera\\view.rid");
	}

	// Camera Trigger
	if (Header->pVcut) {
		RdtRvdRead(_File);
		RdtRvdXml(Rvd, &Xml, XmlRvd);
		RvdArchive(Rvd, (CHAR*)"%sROOM%X%02X%X\\camera\\trigger.rvd", Directory, Stage_no, Room_no, Pl_id);
		XmlRvd->SetAttribute("file", "camera\\trigger.rvd");
	}

	// Camera Light
	if (Header->pLight) {
		RdtLitRead(_File);
		RdtLitXml(Lit, &Xml, XmlLit);
		LitWrite(Lit, "%sROOM%X%02X%X\\camera\\light.lit", Directory, Stage_no, Room_no, Pl_id);
		XmlLit->SetAttribute("file", "camera\\light.lit");
	}

	// Camera Sprite
	if (Header->pRcut && Rcut.size()) {
		Bio2RoomPri * Sprite = NULL;
		Pri.clear();
		for (UCHAR n = 0; n < Header->nCut; n++) {
			Sprite = PriRead(_File, Rcut[n].pSp);
			Pri.push_back(Sprite);
		}
		PriArchive(Pri, ConvertToFinal, (CHAR*)"%sROOM%X%02X%X\\sprite.pri", Directory, Stage_no, Room_no, Pl_id);
		RdtPriXml(Rcut, Pri, ConvertToFinal, &Xml, XmlPri);
		XmlPri->SetAttribute("file", "sprite.pri");
	}

	// Object Collision
	if (Header->pSca) {
		Sca = ScaRead(_File, Header->pSca);
		RdtScaXml(Sca, ConvertToFinal, &Xml, XmlSca);
		ScaWrite(Sca, ConvertToFinal, (CHAR*)"%sROOM%X%02X%X\\collision.sca", Directory, Stage_no, Room_no, Pl_id);
		XmlSca->SetAttribute("file", "collision.sca");
	}

	// Block Direction
	if (Header->pBlock) {
		Blk = BlkRead(_File, Header->pBlock);
		XmlBlk->SetAttribute("file", "block.blk");
		XmlBlk->SetAttribute("nData", (UINT)Blk->nData);
		RdtBlkXml(Blk, &Xml, XmlBlk);
		BlkWrite(Blk, (CHAR*)"%sROOM%X%02X%X\\block.blk", Directory, Stage_no, Room_no, Pl_id);
	}

	// Floor Data
	if (Header->pFloor) {
		Flr = FlrRead(_File, Header->pFloor);
		XmlFlr->SetAttribute("file", "floor.flr");
		XmlFlr->SetAttribute("nData", Flr->nData);
		RdtFlrXml(Flr, &Xml, XmlFlr);
		FlrWrite(Flr, (CHAR*)"%sROOM%X%02X%X\\floor.flr", Directory, Stage_no, Room_no, Pl_id);
	}

	// Script (Main)
	if (Header->pScdx) {
		USHORT nScd = NULL;
		Read(_File, Header->pScdx, &nScd, sizeof(unsigned short int));
		if (nScd) { nScd /= sizeof(unsigned short int); }
		if (nScd) {
			ULONG _Offset = NULL;
			ULONG _ElementSize = NULL;
			ULONG Pointer = Header->pScdx;
			for (USHORT n = 0; n < nScd; n++, Pointer += sizeof(unsigned short int))
			{
				_Offset = NULL;
				Read(_File, Pointer, &_Offset, sizeof(unsigned short int));
				if (_Offset) {
					_Offset += Header->pScdx;
					_ElementSize = Script->InitScanProto(_File, _Offset);
					CreateFromSource(_File, _Offset, _ElementSize, "%sROOM%X%02X%X\\event\\main%02d.scd", Directory, Stage_no, Room_no, Pl_id, n);
					Script->UpdatePrototype("%sROOM%X%02X%X\\event\\main%02d.scd", Directory, Stage_no, Room_no, Pl_id, n);
				}
			}
		}
	}

	// Script (Sub)
	if (Header->pScd) {
		USHORT nScd = NULL;
		Read(_File, Header->pScd, &nScd, sizeof(unsigned short int));
		if (nScd) { nScd /= sizeof(unsigned short int); }
		if (nScd) {
			ULONG _Offset = NULL;
			ULONG _ElementSize = NULL;
			ULONG Pointer = Header->pScd;
			for (USHORT n = 0; n < nScd; n++, Pointer += sizeof(unsigned short int))
			{
				_Offset = NULL;
				Read(_File, Pointer, &_Offset, sizeof(unsigned short int));
				if (_Offset) {
					_Offset += Header->pScd;
					_ElementSize = Script->InitScanProto(_File, _Offset);
					CreateFromSource(_File, _Offset, _ElementSize, "%sROOM%X%02X%X\\event\\sub%02d.scd", Directory, Stage_no, Room_no, Pl_id, n);
					Script->UpdatePrototype("%sROOM%X%02X%X\\event\\sub%02d.scd", Directory, Stage_no, Room_no, Pl_id, n);
				}
			}
		}
	}

	// Script (null)
	if (Header->pScdOld) {
		USHORT nScd = NULL;
		Read(_File, Header->pScdOld, &nScd, sizeof(unsigned short int));
		if (nScd) { nScd /= sizeof(unsigned short int); }
		if (nScd) {
			ULONG _Offset = NULL;
			ULONG _ElementSize = NULL;
			ULONG Pointer = Header->pScdOld;
			for (USHORT n = 0; n < nScd; n++, Pointer += sizeof(unsigned short int))
			{
				_Offset = NULL;
				Read(_File, Pointer, &_Offset, sizeof(unsigned short int));
				if (_Offset) {
					_Offset += Header->pScdOld;
					_ElementSize = Script->InitScanProto(_File, _Offset);
					CreateFromSource(_File, _Offset, _ElementSize, "%sROOM%X%02X%X\\event\\null%02d.scd", Directory, Stage_no, Room_no, Pl_id, n);
					Script->UpdatePrototype("%sROOM%X%02X%X\\event\\null%02d.scd", Directory, Stage_no, Room_no, Pl_id, n);
				}
			}
		}
	}

	// Object Model
	if (Header->pOmodel) {
		ULONG Pointer = Header->pOmodel;
		ULONG OfsTim = NULL;
		ULONG OfsMd1 = NULL;
		pOmodelTim.clear();
		pOmodelMd1.clear();
		ObjTim.clear();
		ObjMd1.clear();
		for (UCHAR n = 0; n < Header->nOmodel; n++, Pointer += 8) {

			// Texture
			Read(_File, Pointer, &OfsTim, sizeof(unsigned long int));
			pOmodelTim.push_back(OfsTim);
			if (pOmodelTim[n] != NULL && pOmodelTim[n] != 0xFFFFFFFF) {
				SonyTexture * Buffer = Texture->TimRead(_File, pOmodelTim[n]);
				ObjTim.push_back(Buffer);
			}
			else {	// Dummy
				SonyTexture * Buffer = Texture->TimDummy();
				ObjTim.push_back(Buffer);
			}

			// Model
			Read(_File, Pointer + 4, &OfsMd1, sizeof(unsigned long int));
			pOmodelMd1.push_back(OfsMd1);
			if (pOmodelMd1[n] != NULL && pOmodelMd1[n] != 0xFFFFFFFF) {
				Bio2Model * Buffer = Model->Md1Read(_File, pOmodelMd1[n]);
				ObjMd1.push_back(Buffer);
			}
			else {	// Dummy
				Bio2Model * Buffer = Model->Md1Dummy();
				ObjMd1.push_back(Buffer);
			}

			// Write
			Model->Md1Write(ObjMd1[n], (CHAR*)"%sROOM%X%02X%X\\object\\model%02d.md1", Directory, Stage_no, Room_no, Pl_id, n);
			Model->Md1DebugLog(ObjMd1[n], (CHAR*)"%sROOM%X%02X%X\\object\\model%02d.log", Directory, Stage_no, Room_no, Pl_id, n);
			Texture->TimWrite(ObjTim[n], (CHAR*)"%sROOM%X%02X%X\\object\\model%02d.tim", Directory, Stage_no, Room_no, Pl_id, n);
		}
	}

	// Effect Sprite
	if (Header->pEsp_hed) {

		// Runtime
		ULONG Size = (Header->pEsp_end - Header->pEsp_hed);
		if (Header->pEsp_hed != 0xFFFFFFFF &&
			Header->pEsp_end != 0xFFFFFFFF &&
			Size != NULL) {
			Esp = EspReadArchive(_File, Header->pEsp_hed, Header->pEsp_end);
			EspWrite(Esp, (CHAR*)"%sROOM%X%02X%X\\effect\\", Directory, Stage_no, Room_no, Pl_id);
		}
		else { Esp = EspDummy(); }

		// Texture
		Size = (Header->pEsp_tim_end - Header->pEsp_tim);
		if (Header->pEsp_tim != 0xFFFFFFFF &&
			Header->pEsp_tim_end != 0xFFFFFFFF &&
			Size != NULL) {
			EspReadTexture(Esp, _File, Header->pEsp_tim, Header->pEsp_tim_end);
			EspWriteTexture(Esp, (CHAR*)"%sROOM%X%02X%X\\effect\\", Directory, Stage_no, Room_no, Pl_id);
		}

	}

	// Soundbank
	if (Header->pEdt0) {
		ULONG nEdt = NULL;
		ULONG Pointer = Header->pEdt0;
		do {
			EDT_TABLE_WORK * Buffer = new tagEDT_TABLE_WORK;
			Read(_File, Pointer, Buffer, sizeof(tagEDT_TABLE_WORK));
			Edt0.push_back(Buffer);
			Pointer += sizeof(unsigned long int);
			nEdt++;
		} while (nEdt != 32);
		EdtWrite(Edt0, (CHAR*)"%sROOM%X%02X%X\\sound\\sound0.edt", Directory, Stage_no, Room_no, Pl_id);
		RdtEdtXml(Edt0, &Xml, XmlSound);
	}

	// Soundbank
	if (Header->pVh0) {
		SonySoundbank * Buffer = Soundbank->VabReadVh(_File, Header->pVh0);
		if (Header->pVb0) { Soundbank->VabReadVb(Buffer, _File, Header->pVb0); }
		Vab.push_back(Buffer);
		Soundbank->VabWrite(Vab[0], (CHAR*)"%sROOM%X%02X%X\\sound\\sound0.vab", Directory, Stage_no, Room_no, Pl_id);
		tinyxml2::XMLElement * XmlMeta = Xml.NewElement("vab");
		XmlMeta->SetAttribute("file", "sound\\sound0.vab");
		XmlSound->InsertEndChild(XmlMeta);
	}

	// Soundbank
	if (Header->pEdt1) {
		ULONG nEdt = NULL;
		ULONG Pointer = Header->pEdt1;
		do {
			EDT_TABLE_WORK * Buffer = new tagEDT_TABLE_WORK;
			Read(_File, Pointer, Buffer, sizeof(tagEDT_TABLE_WORK));
			Edt1.push_back(Buffer);
			Pointer += sizeof(unsigned long int);
			nEdt++;
		} while (nEdt != 24);
		EdtWrite(Edt1, (CHAR*)"%sROOM%X%02X%X\\sound\\sound1.edt", Directory, Stage_no, Room_no, Pl_id);
		RdtEdtXml(Edt1, &Xml, XmlSound);
	}

	// Soundbank
	if (Header->pVh1) {
		SonySoundbank * Buffer = Soundbank->VabReadVh(_File, Header->pVh1);
		if (Header->pVb0) { Soundbank->VabReadVb(Buffer, _File, Header->pVb1); }
		Vab.push_back(Buffer);
		Soundbank->VabWrite(Vab[1], (CHAR*)"%sROOM%X%02X%X\\sound\\sound1.vab", Directory, Stage_no, Room_no, Pl_id);
		tinyxml2::XMLElement * XmlMeta = Xml.NewElement("vab");
		XmlMeta->SetAttribute("file", "sound\\sound1.vab");
		XmlSound->InsertEndChild(XmlMeta);
	}

	// Complete
	fclose(_File);

	// Log
	if (DebugMode) {
		_iobuf * Log = Open(CREATE_FILE, (CHAR*)"%s\\ROOM%X%02X%X\\r%X%02X.log", Directory, Stage_no, Room_no, Pl_id, Stage_no, Room_no);
		if (!Log) {
			pOmodelTim.clear();
			pOmodelMd1.clear();
			Rcut.clear();
			delete[] Header;
			return FALSE;
		}
		Print(Log, (CHAR*)" %08X\t pRcut\r\n", Header->pRcut);
		Print(Log, (CHAR*)" %08X\t pOmodel\r\n", Header->pOmodel);
		Print(Log, (CHAR*)" %08X\t pVcut\r\n", Header->pVcut);
		Print(Log, (CHAR*)" %08X\t pLight\r\n", Header->pLight);
		for (UCHAR n = 0; n < Header->nCut; n++) {
			Print(Log, (CHAR*)" %08X\t Rcut[%d].pSp\r\n", Rcut[n].pSp, n);
		}
		Print(Log, (CHAR*)" %08X\t pSca\r\n", Header->pSca);
		Print(Log, (CHAR*)" %08X\t pBlock\r\n", Header->pBlock);
		Print(Log, (CHAR*)" %08X\t pFloor\r\n", Header->pFloor);
		Print(Log, (CHAR*)" %08X\t pScdx\r\n", Header->pScdx);
		Print(Log, (CHAR*)" %08X\t pScd\r\n", Header->pScd);
		Print(Log, (CHAR*)" %08X\t pScdOld\r\n", Header->pScdOld);
		Print(Log, (CHAR*)" %08X\t pMessage\r\n", Header->pMessage);
		for (UCHAR n = 0; n < Header->nOmodel; n++) {
			Print(Log, (CHAR*)" %08X\t pOmodelMd1[%d]\r\n", pOmodelMd1[n], n);
		}
		Print(Log, (CHAR*)" %08X\t pEsp_hed\r\n", Header->pEsp_hed);
		Print(Log, (CHAR*)" %08X\t pEsp_end\r\n", Header->pEsp_end);
		Print(Log, (CHAR*)" %08X\t pRbj\r\n", Header->pRbj);
		Print(Log, (CHAR*)" %08X\t pEdt0\r\n", Header->pEdt0);
		Print(Log, (CHAR*)" %08X\t pVh0\r\n", Header->pVh0);
		Print(Log, (CHAR*)" %08X\t pEdt1\r\n", Header->pEdt1);
		Print(Log, (CHAR*)" %08X\t pVh1\r\n", Header->pVh1);
		Print(Log, (CHAR*)" %08X\t pVb0\r\n", Header->pVb0);
		Print(Log, (CHAR*)" %08X\t pVb1\r\n", Header->pVb1);
		Print(Log, (CHAR*)" %08X\t pEsp_tim\r\n", Header->pEsp_tim);
		Print(Log, (CHAR*)" %08X\t pEsp_tim_end\r\n", Header->pEsp_tim_end);
		for (UCHAR n = 0; n < Header->nOmodel; n++) {
			Print(Log, (CHAR*)" %08X\t pOmodelTim[%d]\r\n", pOmodelTim[n], n);
		}
		fclose(Log);
	}

	// Flag
	RdtIsLoaded = TRUE;

	// Complete
	Xml.InsertFirstChild(pRoot);
	pRoot->InsertEndChild(XmlHeader);
	pRoot->InsertEndChild(XmlRid);
	pRoot->InsertEndChild(XmlRvd);
	pRoot->InsertEndChild(XmlLit);
	pRoot->InsertEndChild(XmlPri);
	pRoot->InsertEndChild(XmlSca);
	pRoot->InsertEndChild(XmlBlk);
	pRoot->InsertEndChild(XmlFlr);
	pRoot->InsertEndChild(XmlSound);
	CHAR * XmlName = Get((CHAR*)"%sROOM%X%02X%X\\r%X%02X.xml", Directory, Stage_no, Room_no, Pl_id, Stage_no, Room_no);
	Xml.SaveFile(XmlName);

	// Terminate
	return TRUE;
}
VOID Bio_Hazard_2_Proto_Room::RdtClear(VOID) {
	//Header
	Rcut.clear();
	pOmodelTim.clear();
	pOmodelMd1.clear();
	Rvd.clear();
	Lit.clear();
	Pri.clear();
	Sca->Data0.clear();
	Sca->Data1.clear();
	Sca->Data2.clear();
	Sca->Data3.clear();
	Sca->Data4.clear();
	//Blk
	//Flr
	ObjMd1.clear();
	Edt0.clear();
	Edt1.clear();
	Vab.clear();
	ObjTim.clear();
}
// Bio 2
BOOL Bio_Hazard_2_Room::RdtRead(CONST CHAR * _FileName, ...) {

	// Flag
	RdtIsLoaded = FALSE;

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
	if (!_File) {
		delete[] FileName;
		return FALSE;
	}

	// Meta
	Directory = GetDirectory(FileName);
	CHAR * NameBase = GetNamebase(FileName);
	Stage_no = ToInt((CHAR *)"0x%c\0", NameBase[4]);
	Room_no = ToInt((CHAR *)"0x%c%c\0", NameBase[5], NameBase[6]);
	Pl_id = ToInt((CHAR *)"0x%c\0", NameBase[7]);
	delete[] NameBase;
	delete[] FileName;
	ULONG RdtSize = Size(_File);

	// Xml
	XmlExists = FALSE;
	tinyxml2::XMLDocument Xml;
	tinyxml2::XMLNode * pRoot = NULL;
	tinyxml2::XMLDeclaration * Declare = Xml.NewDeclaration();
	tinyxml2::XMLElement * XmlPlayer = NULL;
	tinyxml2::XMLElement * XmlHeader = NULL;
	tinyxml2::XMLElement * XmlRid = NULL;
	tinyxml2::XMLElement * XmlRvd = NULL;
	tinyxml2::XMLElement * XmlLit = NULL;
	tinyxml2::XMLElement * XmlPri = NULL;
	tinyxml2::XMLElement * XmlSca = NULL;
	tinyxml2::XMLElement * XmlBlk = NULL;
	tinyxml2::XMLElement * XmlFlr = NULL;
	tinyxml2::XMLElement * XmlScd = NULL;
	tinyxml2::XMLElement * XmlMsg = NULL;
	tinyxml2::XMLElement * XmlScrl = NULL;
	tinyxml2::XMLElement * XmlObj = NULL;
	tinyxml2::XMLElement * XmlEsp = NULL;
	tinyxml2::XMLElement * XmlRbj = NULL;
	tinyxml2::XMLElement * XmlSound = NULL;
	Declare->SetValue((CHAR*)"xml version=\"1.0\" encoding=\"UTF-8\"");
	if (Exists((CHAR*)"%sr%X%02X\\r%X%02X.xml", Directory, Stage_no, Room_no, Stage_no, Room_no)) {
		CHAR * XmlName = Get((CHAR*)"%sr%X%02X\\r%X%02X.xml", Directory, Stage_no, Room_no, Stage_no, Room_no);
		Xml.LoadFile(XmlName);
		XmlExists = TRUE;

		// Hierarchy
		pRoot = Xml.FirstChild();
		if (!pRoot) {
			pRoot = Xml.NewElement("rdt");
		}
		XmlPlayer = pRoot->FirstChildElement(Get((CHAR*)"pl%02X", Pl_id));
		if (!XmlPlayer) {
			XmlPlayer = Xml.NewElement(Get((CHAR*)"pl%02X", Pl_id));
		}
		XmlHeader = XmlPlayer->FirstChildElement("header");
		if (!XmlHeader) {
			XmlHeader = Xml.NewElement("header");
		}
		XmlRid = XmlPlayer->FirstChildElement("rid");
		if (!XmlRid) {
			XmlRid = Xml.NewElement("rid");
		}
		XmlRvd = XmlPlayer->FirstChildElement("rvd");
		if (!XmlRvd) {
			XmlRvd = Xml.NewElement("rvd");
		}
		XmlLit = XmlPlayer->FirstChildElement("lit");
		if (!XmlLit) {
			XmlLit = Xml.NewElement("lit");
		}
		XmlPri = XmlPlayer->FirstChildElement("pri");
		if (!XmlPri) {
			XmlPri = Xml.NewElement("pri");
		}
		XmlSca = XmlPlayer->FirstChildElement("sca");
		if (!XmlSca) {
			XmlSca = Xml.NewElement("sca");
		}
		XmlBlk = XmlPlayer->FirstChildElement("blk");
		if (!XmlBlk) {
			XmlBlk = Xml.NewElement("blk");
		}
		XmlFlr = XmlPlayer->FirstChildElement("flr");
		if (!XmlFlr) {
			XmlFlr = Xml.NewElement("flr");
		}
		XmlScd = XmlPlayer->FirstChildElement("scd");
		if (!XmlScd) {
			XmlScd = Xml.NewElement("scd");
		}
		XmlMsg = XmlPlayer->FirstChildElement("msg");
		if (!XmlMsg) {
			XmlMsg = Xml.NewElement("msg");
		}
		XmlScrl = XmlPlayer->FirstChildElement("scroll");
		if (!XmlScrl) {
			XmlScrl = Xml.NewElement("scroll");
		}
		XmlObj = XmlPlayer->FirstChildElement("obj");
		if (!XmlObj) {
			XmlObj = Xml.NewElement("obj");
		}
		XmlEsp = XmlPlayer->FirstChildElement("esp");
		if (!XmlEsp) {
			XmlEsp = Xml.NewElement("esp");
		}
		XmlRbj = XmlPlayer->FirstChildElement("rbj");
		if (!XmlRbj) {
			XmlRbj = Xml.NewElement("rbj");
		}
		XmlSound = XmlPlayer->FirstChildElement("sound");
		if (!XmlSound) {
			XmlSound = Xml.NewElement("sound");
		}
	}
	else {
		pRoot = Xml.NewElement("rdt");
		XmlPlayer = Xml.NewElement(Get((CHAR*)"pl%02X", Pl_id));
		XmlRid = Xml.NewElement("rid");
		XmlHeader = Xml.NewElement("header");
		XmlRvd = Xml.NewElement("rvd");
		XmlLit = Xml.NewElement("lit");
		XmlPri = Xml.NewElement("pri");
		XmlSca = Xml.NewElement("sca");
		XmlBlk = Xml.NewElement("blk");
		XmlFlr = Xml.NewElement("flr");
		XmlScd = Xml.NewElement("scd");
		XmlMsg = Xml.NewElement("msg");
		XmlScrl = Xml.NewElement("scroll");
		XmlObj = Xml.NewElement("obj");
		XmlEsp = Xml.NewElement("esp");
		XmlRbj = Xml.NewElement("rbj");
		XmlSound = Xml.NewElement("sound");
	}
	//pRoot->SetAttribute("game", "Bio2");
	//pRoot->SetAttribute("version", "Final");
	//pRoot->SetAttribute("stage", Stage_no);
	//pRoot->SetAttribute("room", Room_no);
	//pRoot->SetAttribute("player", Pl_id);

	// Header
	Header = new tagBio2Room;
	Read(_File, 0x00, Header, sizeof(tagBio2Room));
	XmlHeader->SetAttribute("nCut", Header->nCut);
	XmlHeader->SetAttribute("nOmodel", Header->nOmodel);
	XmlHeader->SetAttribute("Reverb_lv", Header->Reverb_lv);

	// Camera Position
	if (Header->pRcut) {
		RdtRidRead(_File);
		RdtRidXml(&Xml, XmlRid);
		RidWrite(Rcut, (CHAR*)"%sr%X%02X\\camera\\position.rid", Directory, Stage_no, Room_no);
		XmlRid->SetAttribute("file", "camera\\position.rid");
	}

	// Camera Trigger
	if (Header->pVcut) {
		RdtRvdRead(_File);
		RdtRvdXml(Rvd, &Xml, XmlRvd);
		RvdArchive(Rvd, (CHAR*)"%sr%X%02X\\camera\\view.rvd", Directory, Stage_no, Room_no);
		XmlRvd->SetAttribute("file", "camera\\view.rvd");
	}

	// Camera Light
	if (Header->pLight) {
		RdtLitRead(_File);
		RdtLitXml(Lit, &Xml, XmlLit);
		LitWrite(Lit, "%sr%X%02X\\camera\\light.lit", Directory, Stage_no, Room_no);
		XmlLit->SetAttribute("file", "camera\\light.lit");
	}

	// Camera Sprite
	if (Header->pRcut && Rcut.size()) {
		Bio2RoomPri * Sprite = NULL;
		Pri.clear();
		for (UCHAR n = 0; n < Header->nCut; n++) {
			Sprite = PriRead(_File, Rcut[n].pSp);
			Pri.push_back(Sprite);
		}
		PriArchive(Pri, (CHAR*)"%sr%X%02X\\camera\\sprite.pri", Directory, Stage_no, Room_no);
		RdtPriXml(Rcut, Pri, &Xml, XmlPri);
		XmlPri->SetAttribute("file", "camera\\sprite.pri");
	}

	// Object Collision
	if (Header->pSca) {
		Sca = ScaRead(_File, Header->pSca);
		RdtScaXml(Sca, &Xml, XmlSca);
		ScaWrite(Sca, (CHAR*)"%sr%X%02X\\geometry\\collision.sca", Directory, Stage_no, Room_no);
		XmlSca->SetAttribute("file", "geometry\\collision.sca");
	}

	// Block Direction
	if (Header->pBlock) {
		Blk = BlkRead(_File, Header->pBlock);
		XmlBlk->SetAttribute("file", "geometry\\block.blk");
		XmlBlk->SetAttribute("nData", (UINT)Blk->nData);
		RdtBlkXml(Blk, &Xml, XmlBlk);
		BlkWrite(Blk, (CHAR*)"%sr%X%02X\\geometry\\block.blk", Directory, Stage_no, Room_no);
	}

	// Floor Data
	if (Header->pFloor) {
		Flr = FlrRead(_File, Header->pFloor);
		XmlFlr->SetAttribute("file", "geometry\\floor.flr");
		XmlFlr->SetAttribute("nData", Flr->nData);
		RdtFlrXml(Flr, &Xml, XmlFlr);
		FlrWrite(Flr, (CHAR*)"%sr%X%02X\\geometry\\floor.flr", Directory, Stage_no, Room_no);
	}

	// Script (Main)
	if (Header->pScdx) {
		USHORT nScd = NULL;
		Read(_File, Header->pScdx, &nScd, sizeof(unsigned short int));
		if (nScd) { nScd /= sizeof(unsigned short int); }
		if (nScd) {
			ULONG _Offset = NULL;
			ULONG _ElementSize = NULL;
			ULONG Pointer = Header->pScdx;
			tinyxml2::XMLElement * Xmlbuffer = XmlScd->FirstChildElement("main");
			if (!Xmlbuffer) { Xmlbuffer = Xml.NewElement("main"); }
			Xmlbuffer->SetAttribute("nFile", nScd);
			for (USHORT n = 0; n < nScd; n++, Pointer += sizeof(unsigned short int))
			{
				_Offset = NULL;
				Read(_File, Pointer, &_Offset, sizeof(unsigned short int));
				if (_Offset) {
					_Offset += Header->pScdx;
					_ElementSize = Script->InitScan(_File, _Offset);
					CreateFromSource(_File, _Offset, _ElementSize, "%sr%X%02X\\pl%02X\\event\\main%02d.scd", Directory, Stage_no, Room_no, Pl_id, n);
					Script->Disassemble("%sr%X%02X\\pl%02X\\event\\main%02d.scd", Directory, Stage_no, Room_no, Pl_id, n);
					tinyxml2::XMLElement * XmlData = Xmlbuffer->FirstChildElement(Get((CHAR*)"script%02d", n));
					if (!XmlData) { XmlData = Xml.NewElement(Get((CHAR*)"script%02d", n)); }
					XmlData->SetAttribute("file", Get((CHAR*)"pl%02X\\event\\main%02d.scd", Pl_id, n));
					XmlData->SetAttribute("disassembly", Get((CHAR*)"pl%02X\\event\\main%02d.cpp", Pl_id, n));
					Xmlbuffer->InsertEndChild(XmlData);
				}
			}
			XmlScd->InsertEndChild(Xmlbuffer);
		}
	}

	// Script (Sub)
	if (Header->pScd) {
		USHORT nScd = NULL;
		Read(_File, Header->pScd, &nScd, sizeof(unsigned short int));
		if (nScd) { nScd /= sizeof(unsigned short int); }
		if (nScd) {
			ULONG _Offset = NULL;
			ULONG _ElementSize = NULL;
			ULONG Pointer = Header->pScd;
			tinyxml2::XMLElement * Xmlbuffer = XmlScd->FirstChildElement("sub");
			if (!Xmlbuffer) { Xmlbuffer = Xml.NewElement("sub"); }
			Xmlbuffer->SetAttribute("nFile", nScd);
			for (USHORT n = 0; n < nScd; n++, Pointer += sizeof(unsigned short int))
			{
				_Offset = NULL;
				Read(_File, Pointer, &_Offset, sizeof(unsigned short int));
				if (_Offset) {
					_Offset += Header->pScd;
					_ElementSize = Script->InitScan(_File, _Offset);
					CreateFromSource(_File, _Offset, _ElementSize, "%sr%X%02X\\pl%02X\\event\\sub%02d.scd", Directory, Stage_no, Room_no, Pl_id, n);
					Script->Disassemble("%sr%X%02X\\pl%02X\\event\\sub%02d.scd", Directory, Stage_no, Room_no, Pl_id, n);
					tinyxml2::XMLElement * XmlData = Xmlbuffer->FirstChildElement(Get((CHAR*)"script%02d", n));
					if (!XmlData) { XmlData = Xml.NewElement(Get((CHAR*)"script%02d", n)); }
					XmlData->SetAttribute("file", Get((CHAR*)"pl%02X\\event\\sub%02d.scd", Pl_id, n));
					XmlData->SetAttribute("disassembly", Get((CHAR*)"pl%02X\\event\\sub%02d.cpp", Pl_id, n));
					Xmlbuffer->InsertEndChild(XmlData);
				}
			}
			XmlScd->InsertEndChild(Xmlbuffer);
		}
	}

	// Message Text (Main)
	if (Header->pMessage) {
		USHORT nMessage = NULL;
		Read(_File, Header->pMessage, &nMessage, sizeof(unsigned short int));
		if (nMessage) { nMessage /= sizeof(unsigned short int); }
		if (nMessage) {
			ULONG _Offset = NULL;
			ULONG _ElementSize = NULL;
			ULONG Pointer = Header->pMessage;
			tinyxml2::XMLElement * Xmlbuffer = XmlMsg->FirstChildElement("main");
			if (!Xmlbuffer) { Xmlbuffer = Xml.NewElement("main"); }
			Xmlbuffer->SetAttribute("nFile", nMessage);
			for (USHORT n = 0; n < nMessage; n++, Pointer += sizeof(unsigned short int))
			{
				_Offset = NULL;
				Read(_File, Pointer, &_Offset, sizeof(unsigned short int));
				if (_Offset) {
					_Offset += Header->pScd;
					//_ElementSize = Message->InitScan(_File, _Offset);
					//CreateFromSource(_File, _Offset, _ElementSize, "%sROOM%X%02X%X\\message\\main%02d.msg", Directory, Stage_no, Room_no, Pl_id, n);
					//Message->Disassemble("%sROOM%X%02X%X\\message\\main%02d.msg", Directory, Stage_no, Room_no, Pl_id, n);
					tinyxml2::XMLElement * XmlData = Xmlbuffer->FirstChildElement(Get((CHAR*)"text%02d", n));
					if (!XmlData) { XmlData = Xml.NewElement(Get((CHAR*)"text%02d", n)); }
					XmlData->SetAttribute("file", Get((CHAR*)"pl%02X\\message\\main%02d.msg", Pl_id, n));
					XmlData->SetAttribute("disassembly", Get((CHAR*)"pl%02X\\message\\main%02d.txt", Pl_id, n));
					Xmlbuffer->InsertEndChild(XmlData);
				}
			}
			XmlMsg->InsertEndChild(Xmlbuffer);
		}
	}

	// Message Text (Sub)
	if (Header->pMessage_sub) {
		USHORT nMessage = NULL;
		Read(_File, Header->pMessage, &nMessage, sizeof(unsigned short int));
		if (nMessage) { nMessage /= sizeof(unsigned short int); }
		if (nMessage) {
			ULONG _Offset = NULL;
			ULONG _ElementSize = NULL;
			ULONG Pointer = Header->pMessage;
			tinyxml2::XMLElement * Xmlbuffer = XmlMsg->FirstChildElement("sub");
			if (!Xmlbuffer) { Xmlbuffer = Xml.NewElement("sub"); }
			Xmlbuffer->SetAttribute("nFile", nMessage);
			for (USHORT n = 0; n < nMessage; n++, Pointer += sizeof(unsigned short int))
			{
				_Offset = NULL;
				Read(_File, Pointer, &_Offset, sizeof(unsigned short int));
				if (_Offset) {
					_Offset += Header->pScd;
					//_ElementSize = Message->InitScan(_File, _Offset);
					//CreateFromSource(_File, _Offset, _ElementSize, "%sROOM%X%02X%X\\message\\sub%02d.msg", Directory, Stage_no, Room_no, Pl_id, n);
					//Message->Disassemble("%sROOM%X%02X%X\\message\\sub%02d.msg", Directory, Stage_no, Room_no, Pl_id, n);
					tinyxml2::XMLElement * XmlData = Xmlbuffer->FirstChildElement(Get((CHAR*)"sub%02d", n));
					if (!XmlData) { XmlData = Xml.NewElement(Get((CHAR*)"sub%02d", n)); }
					XmlData->SetAttribute("file", Get((CHAR*)"pl%02X\\message\\sub%02d.msg", Pl_id, n));
					XmlData->SetAttribute("disassembly", Get((CHAR*)"pl%02X\\message\\sub%02d.txt", Pl_id, n));
					Xmlbuffer->InsertEndChild(XmlData);
				}
			}
			XmlMsg->InsertEndChild(Xmlbuffer);
		}
	}

	// Scroll Texture
	if (Header->pScrl) {
		Scrl = Texture->TimDummy();
		Scrl->Header.ID = 0x0010;
		Scrl->Header.Version = 0x0000;
		Scrl->Header.Mode = 0x00000002;
		Scrl->Data.Size = 0x0002580C;
		Scrl->Data.X = 0x0000;
		Scrl->Data.W = 0x0140;
		Scrl->Data.H = 0x00F0;
		UCHAR * Buffer = new UCHAR[0x00025800];
		Read(_File, Header->pScrl, Buffer, 0x00025800);
		Scrl->ABGR.push_back(Buffer);
		Texture->TimWrite(Scrl, (CHAR*)"%sr%X%02X\\camera\\scroll.tim", Directory, Stage_no, Room_no);
		XmlScrl->SetAttribute("file", "camera\\scroll.tim");
	}

	// Object Model
	if (Header->pOmodel) {
		ULONG Pointer = Header->pOmodel;
		ULONG OfsTim = NULL;
		ULONG OfsMd1 = NULL;
		ULONG Size = NULL;
		pOmodelTim.clear();
		pOmodelMd1.clear();
		ObjTim.clear();
		ObjMd1.clear();
		for (UCHAR n = 0; n < Header->nOmodel; n++, Pointer += 8) {
			Read(_File, Pointer, &OfsTim, sizeof(unsigned long int));
			pOmodelTim.push_back(OfsTim);
			Read(_File, Pointer + 4, &OfsMd1, sizeof(unsigned long int));
			pOmodelMd1.push_back(OfsMd1);
		}
		for (UCHAR n = 0; n < Header->nOmodel; n++) {

			// Xml
			tinyxml2::XMLElement * XmlData = XmlObj->FirstChildElement(Get((CHAR*)"data%02d", n));
			if (!XmlData) { XmlData = Xml.NewElement(Get((CHAR*)"data%02d", n)); }
			XmlData->SetAttribute("model", Get((CHAR*)"pl%02X\\object\\model%02d.md1", Pl_id, n));
			XmlData->SetAttribute("texture", Get((CHAR*)"pl%02X\\object\\model%02d.tim", Pl_id, n));

			// Model
			if (pOmodelMd1[n] != NULL && pOmodelMd1[n] != 0xFFFFFFFF) {
				Bio2Model * Buffer = Model->Md1Read(_File, pOmodelMd1[n]);
				ObjMd1.push_back(Buffer);
				if ((n + 1) >= Header->nOmodel) {
					if (Header->pEsp_hed) { Size = Header->pEsp_hed - pOmodelMd1[n]; }
					else if (Header->pEsp_end) { Size = Header->pEsp_end - pOmodelMd1[n]; }
					else if (Header->pRbj) { Size = Header->pRbj - pOmodelMd1[n]; }
					else if (Header->pRbj_end) { Size = Header->pRbj_end - pOmodelMd1[n]; }
					else if (Header->pEdt0) { Size = Header->pEdt0 - pOmodelMd1[n]; }
					else if (Header->pVh0) { Size = Header->pVh0 - pOmodelMd1[n]; }
					else if (Header->pVb0) { Size = Header->pVb0 - pOmodelMd1[n]; }
					else if (Header->pVh1) { Size = Header->pVh1 - pOmodelMd1[n]; }
					else if (Header->pVb1) { Size = Header->pVb1 - pOmodelMd1[n]; }
					else if (Header->pEsp_tim) { Size = Header->pEsp_tim - pOmodelMd1[n]; }
					else if (Header->pEsp_tim_end) { Size = Header->pEsp_tim_end - pOmodelMd1[n]; }
				} else {
					Size = pOmodelMd1[n + 1] - pOmodelMd1[n];
				}
				if (!Size) { XmlData->SetAttribute("md1_link", TRUE); }
				else { XmlData->SetAttribute("md1_link", FALSE); }
			} else {	// Dummy
				Bio2Model * Buffer = Model->Md1Dummy();
				ObjMd1.push_back(Buffer);
				XmlData->SetAttribute("md1_link", TRUE);
			}

			// Texture
			if (pOmodelTim[n] != NULL && pOmodelTim[n] != 0xFFFFFFFF) {
				SonyTexture * Buffer = Texture->TimRead(_File, pOmodelTim[n]);
				ObjTim.push_back(Buffer);
				if ((n + 1) >= Header->nOmodel) {
					Size = RdtSize - pOmodelTim[n];
				}
				else {
					Size = pOmodelTim[n + 1] - pOmodelTim[n];
				}
				if (!Size) { XmlData->SetAttribute("tim_link", TRUE); }
				else { XmlData->SetAttribute("tim_link", FALSE); }
			} else {	// Dummy
				SonyTexture * Buffer = Texture->TimDummy();
				ObjTim.push_back(Buffer);
				XmlData->SetAttribute("tim_link", TRUE);
			}

			// Write
			Model->Md1Write(ObjMd1[n], (CHAR*)"%sr%X%02X\\pl%02X\\object\\model%02d.md1", Directory, Stage_no, Room_no, Pl_id, n);
			Model->Md1DebugLog(ObjMd1[n], (CHAR*)"%sr%X%02X\\pl%02X\\object\\model%02d.log", Directory, Stage_no, Room_no, Pl_id, n);
			Texture->TimWrite(ObjTim[n], (CHAR*)"%sr%X%02X\\pl%02X\\object\\model%02d.tim", Directory, Stage_no, Room_no, Pl_id, n);

			// Complete
			XmlObj->InsertEndChild(XmlData);
		}
	}

	// Effect Sprite
	if (Header->pEsp_hed) {

		// Runtime
		ULONG Size = (Header->pEsp_end - Header->pEsp_hed);
		if (Header->pEsp_hed != 0xFFFFFFFF &&
			Header->pEsp_end != 0xFFFFFFFF &&
			Size != NULL) {
			Esp = EspReadArchive(_File, Header->pEsp_hed, Header->pEsp_end);
			EspWrite(Esp, (CHAR*)"%sr%X%02X\\effect\\", Directory, Stage_no, Room_no);
		} else { Esp = EspDummy(); }

		// Texture
		Size = (Header->pEsp_tim_end - Header->pEsp_tim);
		if (Header->pEsp_tim != 0xFFFFFFFF &&
			Header->pEsp_tim_end != 0xFFFFFFFF &&
			Size != NULL) {
			EspReadTexture(Esp, _File, Header->pEsp_tim, Header->pEsp_tim_end);
			EspWriteTexture(Esp, (CHAR*)"%sr%X%02X\\effect\\", Directory, Stage_no, Room_no);
		}

		// Xml
		RdtEspXml(Esp, &Xml, XmlEsp);

	}

	// Player Animation
	if (Header->pRbj) {
		if (Header->pRbj_end) {
			UCHAR * Buffer = new UCHAR[Header->pRbj_end - Header->pRbj];
			Read(_File, Header->pRbj, Buffer, Header->pRbj_end - Header->pRbj);
			CreateFromSource(Buffer, Header->pRbj_end - Header->pRbj, (CHAR*)"%sr%X%02X\\pl%02X\\animation.rbj", Directory, Stage_no, Room_no, Pl_id);
			XmlRbj->SetAttribute("data", Get((CHAR*)"pl%02X\\animation.rbj", Pl_id));
		}
	}

	// Soundbank
	if (Header->pEdt0) {
		ULONG nEdt = NULL;
		ULONG Pointer = Header->pEdt0;
		do {
			EDT_TABLE_WORK * Buffer = new tagEDT_TABLE_WORK;
			Read(_File, Pointer, Buffer, sizeof(tagEDT_TABLE_WORK));
			Edt.push_back(Buffer);
			Pointer += sizeof(unsigned long int);
			nEdt++;
		} while (nEdt != 48);
		EdtWrite(Edt, (CHAR*)"%sr%X%02X\\pl%02X\\sound\\sound0.edt", Directory, Stage_no, Room_no, Pl_id);
		RdtEdtXml(Edt, &Xml, XmlSound);
	}

	// Soundbank
	if (Header->pVh0) {
		SonySoundbank * Buffer = Soundbank->VabReadVh(_File, Header->pVh0);
		if (Header->pVb0) { Soundbank->VabReadVb(Buffer, _File, Header->pVb0); }
		Vab.push_back(Buffer);
		Soundbank->VabWrite(Vab[0], (CHAR*)"%sr%X%02X\\pl%02X\\sound\\sound0.vab", Directory, Stage_no, Room_no, Pl_id);
		tinyxml2::XMLElement * XmlMeta = XmlSound->FirstChildElement("vab0");
		if (!XmlMeta) { XmlMeta = Xml.NewElement("vab0"); }
		XmlMeta->SetAttribute("file", Get((CHAR*)"pl%02X\\sound\\sound0.vab", Pl_id));
		XmlSound->InsertEndChild(XmlMeta);
	}

	// Soundbank
	if (Header->pVh1) {
		SonySoundbank * Buffer = Soundbank->VabReadVh(_File, Header->pVh1);
		if (Header->pVb0) { Soundbank->VabReadVb(Buffer, _File, Header->pVb1); }
		Vab.push_back(Buffer);
		Soundbank->VabWrite(Vab[1], (CHAR*)"%sr%X%02X\\spl%02X\\ound\\sound1.vab", Directory, Stage_no, Room_no);
		tinyxml2::XMLElement * XmlMeta = XmlSound->FirstChildElement("vab1");
		if (!XmlMeta) { XmlMeta = Xml.NewElement("vab1"); }
		XmlMeta->SetAttribute("file", Get((CHAR*)"pl%02X\\sound\\sound1.vab", Pl_id));
		XmlSound->InsertEndChild(XmlMeta);
	}

	// Cleanup
	fclose(_File);

	// Log
	if (DebugMode) {
		_iobuf * Log = Open(CREATE_FILE, (CHAR*)"%s\\r%X%02X\\r%X%02X%X.log", Directory, Stage_no, Room_no, Stage_no, Room_no, Pl_id);
		if (!Log) {
			Rcut.clear();
			pOmodelTim.clear();
			pOmodelMd1.clear();
			Lit.clear();
			Pri.clear();
			return FAIL;
		}
		Print(Log, (CHAR*)" %08X\t pRcut\r\n", Header->pRcut);
		Print(Log, (CHAR*)" %08X\t pOmodel\r\n", Header->pOmodel);
		Print(Log, (CHAR*)" %08X\t pVcut\r\n", Header->pVcut);
		Print(Log, (CHAR*)" %08X\t pLight\r\n", Header->pLight);
		for (UCHAR n = 0; n < Header->nCut; n++) {
			Print(Log, (CHAR*)" %08X\t Rcut[%d].pSp\r\n", Rcut[n].pSp, n);
		}
		Print(Log, (CHAR*)" %08X\t pSca\r\n", Header->pSca);
		Print(Log, (CHAR*)" %08X\t pBlock\r\n", Header->pBlock);
		Print(Log, (CHAR*)" %08X\t pFloor\r\n", Header->pFloor);
		Print(Log, (CHAR*)" %08X\t pScdx\r\n", Header->pScdx);
		Print(Log, (CHAR*)" %08X\t pScd\r\n", Header->pScd);
		Print(Log, (CHAR*)" %08X\t pMessage\r\n", Header->pMessage);
		Print(Log, (CHAR*)" %08X\t pMessage_sub\r\n", Header->pMessage_sub);
		Print(Log, (CHAR*)" %08X\t pScrl\r\n", Header->pScrl);
		for (UCHAR n = 0; n < Header->nOmodel; n++) {
			Print(Log, (CHAR*)" %08X\t pOmodelMd1[%d]\r\n", pOmodelMd1[n], n);
		}
		Print(Log, (CHAR*)" %08X\t pEsp_hed\r\n", Header->pEsp_hed);
		Print(Log, (CHAR*)" %08X\t pEsp_end\r\n", Header->pEsp_end);
		Print(Log, (CHAR*)" %08X\t pRbj\r\n", Header->pRbj);
		Print(Log, (CHAR*)" %08X\t pRbj_end\r\n", Header->pRbj_end);
		Print(Log, (CHAR*)" %08X\t pEdt\r\n", Header->pEdt0);
		Print(Log, (CHAR*)" %08X\t pVh[0]\r\n", Header->pVh0);
		Print(Log, (CHAR*)" %08X\t pVb[0]\r\n", Header->pVb0);
		Print(Log, (CHAR*)" %08X\t pVh[1]\r\n", Header->pVh1);
		Print(Log, (CHAR*)" %08X\t pVb[1]\r\n", Header->pVb1);
		Print(Log, (CHAR*)" %08X\t pEsp_tim\r\n", Header->pEsp_tim);
		Print(Log, (CHAR*)" %08X\t pEsp_tim_end\r\n", Header->pEsp_tim_end);
		for (UCHAR n = 0; n < Header->nOmodel; n++) {
			Print(Log, (CHAR*)" %08X\t pOmodelTim[%d]\r\n", pOmodelTim[n], n);
		}
		fclose(Log);
	}

	// Flag
	RdtIsLoaded = TRUE;

	// Complete
	Xml.InsertFirstChild(Declare);
	Xml.LinkEndChild(pRoot);
	XmlPlayer->InsertEndChild(XmlHeader);
	XmlPlayer->InsertEndChild(XmlRid);
	XmlPlayer->InsertEndChild(XmlRvd);
	XmlPlayer->InsertEndChild(XmlLit);
	XmlPlayer->InsertEndChild(XmlPri);
	XmlPlayer->InsertEndChild(XmlSca);
	XmlPlayer->InsertEndChild(XmlBlk);
	XmlPlayer->InsertEndChild(XmlFlr);
	XmlPlayer->InsertEndChild(XmlScd);
	XmlPlayer->InsertEndChild(XmlMsg);
	XmlPlayer->InsertEndChild(XmlScrl);
	XmlPlayer->InsertEndChild(XmlObj);
	XmlPlayer->InsertEndChild(XmlEsp);
	XmlPlayer->InsertEndChild(XmlRbj);
	XmlPlayer->InsertEndChild(XmlSound);
	pRoot->InsertEndChild(XmlPlayer);
	CHAR * XmlName = Get((CHAR*)"%sr%X%02X\\r%X%02X.xml", Directory, Stage_no, Room_no, Stage_no, Room_no);
	Xml.SaveFile(XmlName);

	// Terminate
	return SUCCESS;
}
BOOL Bio_Hazard_2_Room::RdtWrite(CONST CHAR * _FileName, ...) {

	// Error
	if (!RdtIsLoaded) { return FAIL; }

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
	if (!_File) { return FALSE; }

	// Cleanup
	fclose(_File);

	// Terminate
	return SUCCESS;
}
VOID Bio_Hazard_2_Room::RdtClear(VOID) {
	RdtIsLoaded = FALSE;
	//Header
	Rcut.clear();
	pOmodelTim.clear();
	pOmodelMd1.clear();
	Rvd.clear();
	Lit.clear();
	Pri.clear();
	//Sca
	//Blk
	//Flr
	ObjMd1.clear();
	Edt.clear();
	Vab.clear();
	ObjTim.clear();
}