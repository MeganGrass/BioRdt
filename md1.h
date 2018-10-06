#pragma once

#pragma pack(push, 1)
typedef struct tagBio2ModelHeader {
	ULONG pUV;			// 0x00	// Pointer to UV data
	ULONG unknown;		// 0x04	// Always 0?
	ULONG nObj;			// 0x08	// Must be divisible by 2
} Bio2ModelHeader;
typedef struct tagBio2ModelIndex {
	ULONG pTriVert;		// 0x00
	ULONG nTriVert;		// 0x04
	ULONG pTriNorm;		// 0x08
	ULONG nTriNorm;		// 0x0C
	ULONG pTriPrim;		// 0x10
	ULONG nTriPrim;		// 0x14
	ULONG pTriUV;		// 0x18
	ULONG pQuadVert;	// 0x1C
	ULONG nQuadVert;	// 0x20
	ULONG pQuadNorm;	// 0x24
	ULONG nQuadNorm;	// 0x28
	ULONG pQuadPrim;	// 0x2C
	ULONG nQuadPrim;	// 0x30
	ULONG pQuadUV;		// 0x34
} Bio2ModelIndex;
typedef struct tagBio2ModelVertice {
	SHORT X;
	SHORT Y;
	SHORT Z;
	USHORT Pad;
} Bio2ModelVertice;
typedef struct tagBio2ModelNormal {
	SHORT X;
	SHORT Y;
	SHORT Z;
	USHORT Pad;
} Bio2ModelNormal;
typedef struct tagBio2ModelTriPrim {
	USHORT n0;
	USHORT v0;
	USHORT n1;
	USHORT v1;
	USHORT n2;
	USHORT v2;
} Bio2ModelTriPrim;
typedef struct tagBio2ModelTriPrimUV {
	UCHAR tu0;
	UCHAR tv0;
	USHORT Clut;
	UCHAR tu1;
	UCHAR tv1;
	USHORT tPage;
	UCHAR tu2;
	UCHAR tv2;
	USHORT Pad0;
} Bio2ModelTriPrimUV;
typedef struct tagBio2ModelQuadPrim {
	USHORT n0;
	USHORT v0;
	USHORT n1;
	USHORT v1;
	USHORT n2;
	USHORT v2;
	USHORT n3;
	USHORT v3;
} Bio2ModelQuadPrim;
typedef struct tagBio2ModelQuadPrimUV {
	UCHAR tu0;
	UCHAR tv0;
	USHORT Clut;
	UCHAR tu1;
	UCHAR tv1;
	USHORT tPage;
	UCHAR tu2;
	UCHAR tv2;
	USHORT Pad0;
	UCHAR tu3;
	UCHAR tv3;
	USHORT Pad1;
} Bio2ModelQuadPrimUV;
typedef struct tagBio2ModelObject {
	Bio2ModelIndex Index;
	std::vector <Bio2ModelVertice*> TriV;
	std::vector <Bio2ModelNormal*> TriN;
	std::vector <Bio2ModelTriPrim*> TriP;
	std::vector <Bio2ModelTriPrimUV*> TriUV;
	std::vector <Bio2ModelVertice*> QuadV;
	std::vector <Bio2ModelNormal*> QuadN;
	std::vector <tagBio2ModelQuadPrim*> QuadP;
	std::vector <tagBio2ModelQuadPrimUV*> QuadUV;
} Bio2ModelObject;
typedef struct tagBio2Model {

	// Runtime File
	Bio2ModelHeader Header;
	std::vector <Bio2ModelObject*> Obj;

	// Statistical
	ULONG Size;			// sizeof file (*.md1)
	ULONG nVertices;	// Total Vertice Count
	ULONG nNormals;		// Total Normal Count
	ULONG nPrimT;		// Total Primitive Triangle Count
	ULONG nPrimQ;		// Total Primitive Quadrangle Count

} Bio2Model;
#pragma pack(pop)

class Bio_Hazard_2_Model :
	private System_File {
private:
public:

	// Boot
	Bio_Hazard_2_Model(VOID);
	virtual ~Bio_Hazard_2_Model(VOID);

	// Flag
	BOOL DebugMode;

	// Debug Mode
	BOOL Md1DebugLog(Bio2Model * Model, CONST CHAR * _FileName, ...);

	// Statistical
	ULONG Md1Size(Bio2Model * Model);
	ULONG Md1VertPointer(Bio2Model * Model);
	ULONG Md1NormPointer(Bio2Model * Model);
	ULONG Md1PrimPointer(Bio2Model * Model);
	ULONG Md1PrimUVPointer(Bio2Model * Model);

	// Data Buffer
	Bio2Model * Md1Dummy(VOID);
	Bio2Model * Md1Read(_iobuf * FILE, ULONG _Offset);
	Bio2Model * Md1Read(CONST CHAR * _FileName, ...);
	BOOL Md1Write(Bio2Model * Model, _iobuf * FILE, ULONG _Offset);
	BOOL Md1Write(Bio2Model * Model, CONST CHAR * _FileName, ...);

	// Extra
	Bio2ModelObject * Md1CombineObj(Bio2ModelObject * Obj0, Bio2ModelObject * Obj1);

};