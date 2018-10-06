#pragma once

#pragma pack(push, 1)
typedef struct tagGENERIC_DATA {
	ULONG Offset;
	ULONG Size;
	UCHAR * Buf;
} GENERIC_DATA;
typedef struct tagGENERIC {
	USHORT Num;
	std::vector <GENERIC_DATA> Data;
} GENERIC;
typedef struct tagBio2RoomProto {
	UCHAR nSprite;		// XX	// 0x00
	UCHAR nCut;			// XX	// 0x01
	UCHAR nOmodel;		// XX	// 0x02
	UCHAR nItem;		// XX	// 0x03
	UCHAR nDoor;		// XX	// 0x04
	UCHAR nRoom_at;		// XX	// 0x05
	UCHAR Reverb_lv;	// XX	// 0x06
	UCHAR nSprite_max;	// XX	// 0x07
	ULONG pEdt0;		// 00	// 0x08
	ULONG pVh0;			// 01	// 0x0C
	ULONG pVb0;			// 02	// 0x10
	ULONG pEdt1;		// 03	// 0x14
	ULONG pVh1;			// 04	// 0x18
	ULONG pVb1;			// 05	// 0x1C
	ULONG pSca;			// 06	// 0x20
	ULONG pRcut;		// 07	// 0x24
	ULONG pVcut;		// 08	// 0x28
	ULONG pLight;		// 09	// 0x2C
	ULONG pOmodel;		// 10	// 0x30
	ULONG pFloor;		// 11	// 0x34
	ULONG pBlock;		// 12	// 0x38
	ULONG pMessage;		// 13	// 0x3C
	ULONG pScdx;		// 14	// 0x40	// Main
	ULONG pScd;			// 15	// 0x44	// Sub
	ULONG pScdOld;		// 16	// 0x48	// There are zero (0) instances of these script filesize > 2 (ALWAYS Evt_end, ONLY!!!)
	ULONG pEsp_hed;		// 17	// 0x4C
	ULONG pEsp_end;		// 18	// 0x50
	ULONG pEsp_tim;		// 19	// 0x54
	ULONG pEsp_tim_end;	// 20	// 0x58
	ULONG pRbj;			// 21	// 0x5C
} Bio2RoomProto;
typedef struct tagBio2Room {
	UCHAR nSprite;		// XX // 0x00
	UCHAR nCut;			// XX // 0x01
	UCHAR nOmodel;		// XX // 0x02
	UCHAR nItem;		// XX // 0x03
	UCHAR nDoor;		// XX // 0x04
	UCHAR nRoom_at;		// XX // 0x05
	UCHAR Reverb_lv;	// XX // 0x06
	UCHAR nSprite_max;	// XX // 0x07
	ULONG pEdt0;		//  0 // 0x08
	ULONG pVh0;			//  1 // 0x0C // VH 0
	ULONG pVb0;			//  2 // 0x10 // VB 0
	ULONG pVh1;			//  3 // 0x14 // VH 1 for Trial Edition, only (pVh1=0 otherwise)
	ULONG pVb1;			//  4 // 0x18 // VB 1 for Trial Edition, only (pVb1=0 otherwise)
	ULONG pRbj_end;		//  5 // 0x1C
	ULONG pSca;			//  6 // 0x20
	ULONG pRcut;		//  7 // 0x24
	ULONG pVcut;		//  8 // 0x28
	ULONG pLight;		//  9 // 0x2C
	ULONG pOmodel;		// 10 // 0x30	// tagMODEL_LINK_INFO
	ULONG pFloor;		// 11 // 0x34
	ULONG pBlock;		// 12 // 0x38
	ULONG pMessage;		// 13 // 0x3C
	ULONG pMessage_sub;	// 14 // 0x40
	ULONG pScrl;		// 15 // 0x44
	ULONG pScdx;		// 16 // 0x48
	ULONG pScd;			// 17 // 0x4C
	ULONG pEsp_hed;		// 18 // 0x50
	ULONG pEsp_end;		// 19 // 0x54
	ULONG pEsp_tim;		// 20 // 0x58
	ULONG pEsp_tim_end;	// 21 // 0x5C
	ULONG pRbj;			// 22 // 0x60
} Bio2Room;
#pragma pack(pop)

class Bio_Hazard_2_Proto_Room :
	private System_File {
private:
public:

	// Boot
	Bio_Hazard_2_Proto_Room(VOID);
	virtual ~Bio_Hazard_2_Proto_Room(VOID);

	// Class
	Sony_PlayStation_Texture * Texture;
	Sony_PlayStation_Soundbank * Soundbank;
	Bio_Hazard_2_Model * Model;
	Bio_Hazard_2_Script * Script;

	// Flag
	BOOL DebugMode;			// Print some disassembly statistics during RdtRead()
	BOOL RdtIsLoaded;		// Rdt has been read, all data in buffers
	BOOL ConvertToFinal;	// Convert Rdt data to Final version format

	// Meta
	CHAR * Directory;
	SHORT Stage_no;
	SHORT Room_no;
	UCHAR Pl_id;

	// Data Buffer
	Bio2RoomProto * Header;
	std::vector <RCUT> Rcut;
	std::vector <ULONG> pOmodelTim;
	std::vector <ULONG> pOmodelMd1;
	std::vector <Bio2RoomRvd*> Rvd;
	std::vector <LIGHT_CUT_DATA> Lit;
	std::vector <Bio2RoomPri*> Pri;
	Bio2RoomScaProto * Sca;
	Bio2RoomBlk * Blk;
	Bio2RoomFlr * Flr;
	GENERIC * Msg;
	std::vector <Bio2Model*> ObjMd1;
	std::vector <EDT_TABLE_WORK*> Edt0;
	std::vector <EDT_TABLE_WORK*> Edt1;
	std::vector <SonySoundbank*> Vab;
	Bio2EffectSprite * Esp;
	std::vector <SonyTexture*> ObjTim;

	// Camera Position
	RCUT RidRead(_iobuf * FILE, ULONG _Offset);
	RCUT RidRead(CONST CHAR * _FileName, ...);
	BOOL RidWrite(std::vector <RCUT> Position, _iobuf * FILE, ULONG _Offset);
	BOOL RidWrite(std::vector <RCUT> Position, CONST CHAR * _FileName, ...);
	BOOL RdtRidRead(_iobuf * FILE);
	BOOL RdtRidXml(tinyxml2::XMLDocument * Xml, tinyxml2::XMLElement * Element);

	// Camera Trigger
	Bio2RoomRvdData * RvdRead(_iobuf * FILE, ULONG _Offset);
	Bio2RoomRvdData * RvdRead(CONST CHAR * _FileName, ...);
	BOOL RvdWrite(Bio2RoomRvdData * Trigger, _iobuf * FILE, ULONG _Offset);
	BOOL RvdWrite(Bio2RoomRvdData * Trigger, CONST CHAR * _FileName, ...);
	BOOL RvdArchive(std::vector <Bio2RoomRvd*> Trigger, _iobuf * FILE, ULONG _Offset);
	BOOL RvdArchive(std::vector <Bio2RoomRvd*> Trigger, CONST CHAR * _FileName, ...);
	Bio2RoomRvd * RvdSort(Bio2RoomRvd * Trigger);
	std::vector <Bio2RoomRvd*> RvdSort(std::vector <Bio2RoomRvd*> Trigger);
	std::vector <Bio2RoomRvdData*> RvdSortData(std::vector <Bio2RoomRvdData*> Trigger);
	BOOL RdtRvdRead(_iobuf * FILE);
	BOOL RdtRvdXml(std::vector <Bio2RoomRvd*> Trigger, tinyxml2::XMLDocument * Xml, tinyxml2::XMLElement * Element);

	// Camera Light
	LIGHT_CUT_DATA LitRead(_iobuf * FILE, ULONG _Offset);
	LIGHT_CUT_DATA LitRead(CONST CHAR * _FileName, ...);
	BOOL LitWrite(std::vector <LIGHT_CUT_DATA> Light, _iobuf * FILE, ULONG _Offset);
	BOOL LitWrite(std::vector <LIGHT_CUT_DATA> Light, CONST CHAR * _FileName, ...);
	BOOL RdtLitRead(_iobuf * FILE);
	BOOL RdtLitXml(std::vector <LIGHT_CUT_DATA> Light, tinyxml2::XMLDocument * Xml, tinyxml2::XMLElement * Element);

	// Camera Sprite
	Bio2RoomPri * PriRead(_iobuf * FILE, ULONG _Offset);
	Bio2RoomPri * PriRead(CONST CHAR * _FileName, ...);
	BOOL PriWrite(Bio2RoomPri * Sprite, BOOL Convert, _iobuf * FILE, ULONG _Offset);
	BOOL PriWrite(Bio2RoomPri * Sprite, BOOL Convert, CONST CHAR * _FileName, ...);
	BOOL PriArchive(std::vector <Bio2RoomPri*> Sprite, BOOL Convert, _iobuf * FILE, ULONG _Offset);
	BOOL PriArchive(std::vector <Bio2RoomPri*> Sprite, BOOL Convert, CONST CHAR * _FileName, ...);
	BOOL RdtPriXml(std::vector <RCUT> Rcut, std::vector <Bio2RoomPri*> Sprite, BOOL Convert, tinyxml2::XMLDocument * Xml, tinyxml2::XMLElement * Element);

	// Object Collision
	Bio2RoomScaProto * ScaRead(_iobuf * FILE, ULONG _Offset);
	Bio2RoomScaProto * ScaRead(CONST CHAR * _FileName, ...);
	BOOL ScaWrite(Bio2RoomScaProto * Collision, BOOL Convert, _iobuf * FILE, ULONG _Offset);
	BOOL ScaWrite(Bio2RoomScaProto * Collision, BOOL Convert, CONST CHAR * _FileName, ...);
	Bio2RoomScaProto * ScaCleanup(Bio2RoomScaProto * Collision);
	BOOL RdtScaXml(Bio2RoomScaProto * Collision, BOOL Convert, tinyxml2::XMLDocument * Xml, tinyxml2::XMLElement * Element);

	// Block Direction
	Bio2RoomBlk * BlkRead(_iobuf * FILE, ULONG _Offset);
	Bio2RoomBlk * BlkRead(CONST CHAR * _FileName, ...);
	BOOL BlkWrite(Bio2RoomBlk * Block, _iobuf * FILE, ULONG _Offset);
	BOOL BlkWrite(Bio2RoomBlk * Block, CONST CHAR * _FileName, ...);
	BOOL RdtBlkXml(Bio2RoomBlk * Block, tinyxml2::XMLDocument * Xml, tinyxml2::XMLElement * Element);

	// Floor Data
	Bio2RoomFlr * FlrRead(_iobuf * FILE, ULONG _Offset);
	Bio2RoomFlr * FlrRead(CONST CHAR * _FileName, ...);
	BOOL FlrWrite(Bio2RoomFlr * Floor, _iobuf * FILE, ULONG _Offset);
	BOOL FlrWrite(Bio2RoomFlr * Floor, CONST CHAR * _FileName, ...);
	BOOL RdtFlrXml(Bio2RoomFlr * Floor, tinyxml2::XMLDocument * Xml, tinyxml2::XMLElement * Element);

	// Effect Sprite
	Bio2EffectSprite * EspDummy(VOID);
	BOOL EspWriteEff(Bio2EffectSprite * Sprite, UCHAR &Id, _iobuf * FILE, ULONG _Offset);
	BOOL EspWriteEff(Bio2EffectSprite * Sprite, UCHAR &Id, CONST CHAR * _FileName, ...);
	BOOL EspWrite(Bio2EffectSprite * Sprite, CONST CHAR * _Directory, ...);
	Bio2EffectSprite * EspReadArchive(_iobuf * FILE, ULONG pEsp_hed, ULONG pEsp_end);
	BOOL EspReadTexture(Bio2EffectSprite * Sprite, _iobuf * FILE, ULONG pEsp_tim, ULONG pEsp_tim_end);
	BOOL EspWriteTexture(Bio2EffectSprite * Sprite, CONST CHAR * _Directory, ...);

	// Soundbank
	BOOL EdtWrite(std::vector <EDT_TABLE_WORK*> Data, _iobuf * FILE, ULONG _Offset);
	BOOL EdtWrite(std::vector <EDT_TABLE_WORK*> Data, CONST CHAR * _FileName, ...);
	BOOL RdtEdtXml(std::vector <EDT_TABLE_WORK*> Data, tinyxml2::XMLDocument * Xml, tinyxml2::XMLElement * Element);

	// 
	BOOL RdtRead(CONST CHAR * _FileName, ...);
	BOOL RdtWrite(CONST CHAR * _FileName, ...);
	VOID RdtClear(VOID);

};
class Bio_Hazard_2_Room :
	private System_File {
private:
public:

	// Boot
	Bio_Hazard_2_Room(VOID);
	~Bio_Hazard_2_Room(VOID);

	// Class
	Sony_PlayStation_Texture * Texture;
	Sony_PlayStation_Soundbank * Soundbank;
	Bio_Hazard_2_Model * Model;
	Bio_Hazard_2_Script * Script;

	// Flag
	BOOL DebugMode;
	BOOL RdtIsLoaded;
	BOOL XmlExists;

	// Meta
	CHAR * Directory;
	SHORT Stage_no;
	SHORT Room_no;
	UCHAR Pl_id;

	// Data Buffer
	Bio2Room * Header;
	std::vector <RCUT> Rcut;
	std::vector <ULONG> pOmodelTim;
	std::vector <ULONG> pOmodelMd1;
	std::vector <Bio2RoomRvd*> Rvd;
	std::vector <LIGHT_CUT_DATA> Lit;
	std::vector <Bio2RoomPri*> Pri;
	Bio2RoomSca * Sca;
	Bio2RoomBlk * Blk;
	Bio2RoomFlr * Flr;
	GENERIC * Msg;
	SonyTexture * Scrl;
	std::vector <Bio2Model*> ObjMd1;
	std::vector <EDT_TABLE_WORK*> Edt;
	std::vector <SonySoundbank*> Vab;
	Bio2EffectSprite * Esp;
	std::vector <SonyTexture*> ObjTim;

	// Camera Position
	RCUT RidRead(_iobuf * FILE, ULONG _Offset);
	RCUT RidRead(CONST CHAR * _FileName, ...);
	BOOL RidWrite(std::vector <RCUT> Position, _iobuf * FILE, ULONG _Offset);
	BOOL RidWrite(std::vector <RCUT> Position, CONST CHAR * _FileName, ...);
	BOOL RdtRidRead(_iobuf * FILE);
	BOOL RdtRidXml(tinyxml2::XMLDocument * Xml, tinyxml2::XMLElement * Element);

	// Camera Trigger
	Bio2RoomRvdData * RvdRead(_iobuf * FILE, ULONG _Offset);
	Bio2RoomRvdData * RvdRead(CONST CHAR * _FileName, ...);
	BOOL RvdWrite(Bio2RoomRvdData * Trigger, _iobuf * FILE, ULONG _Offset);
	BOOL RvdWrite(Bio2RoomRvdData * Trigger, CONST CHAR * _FileName, ...);
	BOOL RvdArchive(std::vector <Bio2RoomRvd*> Trigger, _iobuf * FILE, ULONG _Offset);
	BOOL RvdArchive(std::vector <Bio2RoomRvd*> Trigger, CONST CHAR * _FileName, ...);
	Bio2RoomRvd * RvdSort(Bio2RoomRvd * Trigger);
	std::vector <Bio2RoomRvd*> RvdSort(std::vector <Bio2RoomRvd*> Trigger);
	std::vector <Bio2RoomRvdData*> RvdSortData(std::vector <Bio2RoomRvdData*> Trigger);
	BOOL RdtRvdRead(_iobuf * FILE);
	BOOL RdtRvdXml(std::vector <Bio2RoomRvd*> Trigger, tinyxml2::XMLDocument * Xml, tinyxml2::XMLElement * Element);

	// Camera Light
	LIGHT_CUT_DATA LitRead(_iobuf * FILE, ULONG _Offset);
	LIGHT_CUT_DATA LitRead(CONST CHAR * _FileName, ...);
	BOOL LitWrite(std::vector <LIGHT_CUT_DATA> Light, _iobuf * FILE, ULONG _Offset);
	BOOL LitWrite(std::vector <LIGHT_CUT_DATA> Light, CONST CHAR * _FileName, ...);
	BOOL RdtLitRead(_iobuf * FILE);
	BOOL RdtLitXml(std::vector <LIGHT_CUT_DATA> Light, tinyxml2::XMLDocument * Xml, tinyxml2::XMLElement * Element);

	// Camera Sprite
	Bio2RoomPri * PriRead(_iobuf * FILE, ULONG _Offset);
	Bio2RoomPri * PriRead(CONST CHAR * _FileName, ...);
	BOOL PriWrite(Bio2RoomPri * Sprite, _iobuf * FILE, ULONG _Offset);
	BOOL PriWrite(Bio2RoomPri * Sprite, CONST CHAR * _FileName, ...);
	BOOL PriArchive(std::vector <Bio2RoomPri*> Sprite, _iobuf * FILE, ULONG _Offset);
	BOOL PriArchive(std::vector <Bio2RoomPri*> Sprite, CONST CHAR * _FileName, ...);
	BOOL RdtPriXml(std::vector <RCUT> Rcut, std::vector <Bio2RoomPri*> Sprite, tinyxml2::XMLDocument * Xml, tinyxml2::XMLElement * Element);

	// Object Collision
	Bio2RoomSca * ScaRead(_iobuf * FILE, ULONG _Offset);
	Bio2RoomSca * ScaRead(CONST CHAR * _FileName, ...);
	BOOL ScaWrite(Bio2RoomSca * Collision, _iobuf * FILE, ULONG _Offset);
	BOOL ScaWrite(Bio2RoomSca * Collision, CONST CHAR * _FileName, ...);
	BOOL RdtScaXml(Bio2RoomSca * Collision, tinyxml2::XMLDocument * Xml, tinyxml2::XMLElement * Element);

	// Block Direction
	Bio2RoomBlk * BlkRead(_iobuf * FILE, ULONG _Offset);
	Bio2RoomBlk * BlkRead(CONST CHAR * _FileName, ...);
	BOOL BlkWrite(Bio2RoomBlk * Block, _iobuf * FILE, ULONG _Offset);
	BOOL BlkWrite(Bio2RoomBlk * Block, CONST CHAR * _FileName, ...);
	BOOL RdtBlkXml(Bio2RoomBlk * Block, tinyxml2::XMLDocument * Xml, tinyxml2::XMLElement * Element);

	// Floor Data
	Bio2RoomFlr * FlrRead(_iobuf * FILE, ULONG _Offset);
	Bio2RoomFlr * FlrRead(CONST CHAR * _FileName, ...);
	BOOL FlrWrite(Bio2RoomFlr * Floor, _iobuf * FILE, ULONG _Offset);
	BOOL FlrWrite(Bio2RoomFlr * Floor, CONST CHAR * _FileName, ...);
	BOOL RdtFlrXml(Bio2RoomFlr * Floor, tinyxml2::XMLDocument * Xml, tinyxml2::XMLElement * Element);

	// Message Text
	BOOL MsgSave(CONST CHAR * _FileName, ...);
	ULONG MsgGetSize(_iobuf * FILE, ULONG _Offset);

	// Effect Sprite
	Bio2EffectSprite * EspDummy(VOID);
	BOOL EspWriteEff(Bio2EffectSprite * Sprite, UCHAR &Id, _iobuf * FILE, ULONG _Offset);
	BOOL EspWriteEff(Bio2EffectSprite * Sprite, UCHAR &Id, CONST CHAR * _FileName, ...);
	BOOL EspWrite(Bio2EffectSprite * Sprite, CONST CHAR * _Directory, ...);
	Bio2EffectSprite * EspReadArchive(_iobuf * FILE, ULONG pEsp_hed, ULONG pEsp_end);
	BOOL EspReadTexture(Bio2EffectSprite * Sprite, _iobuf * FILE, ULONG pEsp_tim, ULONG pEsp_tim_end);
	BOOL EspWriteTexture(Bio2EffectSprite * Sprite, CONST CHAR * _Directory, ...);
	BOOL RdtEspXml(Bio2EffectSprite * Sprite, tinyxml2::XMLDocument * Xml, tinyxml2::XMLElement * Element);

	// Player Animation

	// Soundbank
	BOOL EdtWrite(std::vector <EDT_TABLE_WORK*> Data, _iobuf * FILE, ULONG _Offset);
	BOOL EdtWrite(std::vector <EDT_TABLE_WORK*> Data, CONST CHAR * _FileName, ...);
	BOOL RdtEdtXml(std::vector <EDT_TABLE_WORK*> Data, tinyxml2::XMLDocument * Xml, tinyxml2::XMLElement * Element);

	// 
	BOOL RdtRead(CONST CHAR * _FileName, ...);
	BOOL RdtWrite(CONST CHAR * _FileName, ...);
	VOID RdtClear(VOID);

};