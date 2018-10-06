
#include "stdafx.h"
#include FRAMEWORK_H

#include BIO2_H

// Boot
Bio_Hazard_2_Model::Bio_Hazard_2_Model(VOID) {
}
Bio_Hazard_2_Model::~Bio_Hazard_2_Model(VOID) {
}
// Debug Mode
BOOL Bio_Hazard_2_Model::Md1DebugLog(Bio2Model * Model, CONST CHAR * _FileName, ...) {

	// Error
	if (!DebugMode) { return FAIL; }

	// _FileName
	va_list _ArgList = { NULL };
	__crt_va_start(_ArgList, _FileName);
	_StrLen = (_vscprintf(_FileName, _ArgList) + 2);
	CHAR * FileName = new CHAR[(_StrLen * 2)];
	RtlSecureZeroMemory(FileName, (_StrLen * 2));
	vsprintf_s(FileName, _StrLen, _FileName, _ArgList);
	__crt_va_end(_ArgList);

	// Open
	_iobuf * Log = Open(CREATE_FILE, FileName);
	CHAR * Directory = GetDirectory(FileName);
	CHAR * NameBase = GetNamebase(FileName);
	delete[] FileName;
	if (!Log) { return FAIL; }

	// Statistical
	Print(Log, (CHAR*)" %s%s.md1\r\n\r\n", Directory, NameBase);
	Print(Log, (CHAR*)" 0x%X bytes\r\n\r\n", Model->Size);
	Print(Log, (CHAR*)" %d Vertices\r\n", Model->nVertices);
	Print(Log, (CHAR*)" %d Normals\r\n", Model->nNormals);
	Print(Log, (CHAR*)" %d Primitives (%d Triangles, %d Quadrilaterals)\r\n\r\n", Model->nPrimT + Model->nPrimQ, Model->nPrimT, Model->nPrimQ);

	// Runtime Data
	Print(Log, (CHAR*)" %08X\tModel->Header.pUV\r\n", Model->Header.pUV);
	Print(Log, (CHAR*)" %d\t\tModel->Header.unknown\r\n", Model->Header.unknown);
	Print(Log, (CHAR*)" %d\t\tModel->Header.nObj\r\n\r\n", Model->Header.nObj / 2);
	for (ULONG i = 0; i < (Model->Header.nObj / 2); i++) {
		Print(Log, (CHAR*)" %d\t\tModel->Obj[%d]->Index.nTriVert\r\n", Model->Obj[i]->Index.nTriVert, i);
		Print(Log, (CHAR*)" %d\t\tModel->Obj[%d]->Index.nQuadVert\r\n", Model->Obj[i]->Index.nQuadVert, i);

		Print(Log, (CHAR*)" %d\t\tModel->Obj[%d]->Index.nTriNorm\r\n", Model->Obj[i]->Index.nTriNorm, i);
		Print(Log, (CHAR*)" %d\t\tModel->Obj[%d]->Index.nQuadNorm\r\n", Model->Obj[i]->Index.nQuadNorm, i);

		Print(Log, (CHAR*)" %d\t\tModel->Obj[%d]->Index.nTriPrim\r\n", Model->Obj[i]->Index.nTriPrim, i);
		Print(Log, (CHAR*)" %d\t\tModel->Obj[%d]->Index.nQuadPrim\r\n", Model->Obj[i]->Index.nQuadPrim, i);

		Print(Log, (CHAR*)" %08X\tModel->Obj[%d]->Index.pTriVert\r\n", Model->Obj[i]->Index.pTriVert + sizeof(tagBio2ModelHeader), i);
		Print(Log, (CHAR*)" %08X\tModel->Obj[%d]->Index.pQuadVert\r\n", Model->Obj[i]->Index.pQuadVert + sizeof(tagBio2ModelHeader), i);

		Print(Log, (CHAR*)" %08X\tModel->Obj[%d]->Index.pTriNorm\r\n", Model->Obj[i]->Index.pTriNorm + sizeof(tagBio2ModelHeader), i);
		Print(Log, (CHAR*)" %08X\tModel->Obj[%d]->Index.pQuadNorm\r\n", Model->Obj[i]->Index.pQuadNorm + sizeof(tagBio2ModelHeader), i);

		Print(Log, (CHAR*)" %08X\tModel->Obj[%d]->Index.pTriPrim\r\n", Model->Obj[i]->Index.pTriPrim + sizeof(tagBio2ModelHeader), i);
		Print(Log, (CHAR*)" %08X\tModel->Obj[%d]->Index.pQuadPrim\r\n", Model->Obj[i]->Index.pQuadPrim + sizeof(tagBio2ModelHeader), i);

		Print(Log, (CHAR*)" %08X\tModel->Obj[%d]->Index.pTriUV\r\n", Model->Obj[i]->Index.pTriUV + sizeof(tagBio2ModelHeader), i);
		Print(Log, (CHAR*)" %08X\tModel->Obj[%d]->Index.pQuadUV\r\n\r\n", Model->Obj[i]->Index.pQuadUV + sizeof(tagBio2ModelHeader), i);
	}

	// Cleanup
	fclose(Log);

	// Terminate
	return SUCCESS;
}
// Statistical
ULONG Bio_Hazard_2_Model::Md1Size(Bio2Model * Model) {

	// Error
	if (!Model->Obj.size()) { return NULL; }

	// Return expected sizeof file (*.md1)
	ULONG Size =	sizeof(tagBio2ModelHeader) +
					Model->Obj.size() * sizeof(tagBio2ModelIndex) +
					Model->nVertices * sizeof(tagBio2ModelVertice) +
					Model->nNormals * sizeof(tagBio2ModelNormal) +
					Model->nPrimT * sizeof(tagBio2ModelTriPrim) +
					Model->nPrimQ * sizeof(tagBio2ModelQuadPrim) +
					Model->nPrimT * sizeof(tagBio2ModelTriPrimUV) +
					Model->nPrimQ * sizeof(tagBio2ModelQuadPrimUV);

	// Terminate
	return Size;
}
ULONG Bio_Hazard_2_Model::Md1VertPointer(Bio2Model * Model) {

	// Error
	if (!Model->Obj.size()) { return NULL; }

	// Return pointer to start of Vertice data
	ULONG Pointer =	sizeof(tagBio2ModelHeader) +
					Model->Obj.size() * sizeof(tagBio2ModelIndex);

	// Terminate
	return Pointer;
}
ULONG Bio_Hazard_2_Model::Md1NormPointer(Bio2Model * Model) {

	// Error
	if (!Model->Obj.size()) { return NULL; }

	// Return pointer to start of Normal data
	ULONG Pointer =	sizeof(tagBio2ModelHeader) +
					Model->Obj.size() * sizeof(tagBio2ModelIndex) +
					Model->nVertices * sizeof(tagBio2ModelVertice);

	// Terminate
	return Pointer;
}
ULONG Bio_Hazard_2_Model::Md1PrimPointer(Bio2Model * Model) {

	// Error
	if (!Model->Obj.size()) { return NULL; }

	// Return pointer to start of Primitive data
	ULONG Pointer =	sizeof(tagBio2ModelHeader) +
					Model->Obj.size() * sizeof(tagBio2ModelIndex) +
					Model->nVertices * sizeof(tagBio2ModelVertice) +
					Model->nNormals * sizeof(tagBio2ModelNormal);

	// Terminate
	return Pointer;
}
ULONG Bio_Hazard_2_Model::Md1PrimUVPointer(Bio2Model * Model) {

	// Error
	if (!Model->Obj.size()) { return NULL; }

	// Return pointer to start of Primitive data
	ULONG Pointer = sizeof(tagBio2ModelHeader) +
					Model->Obj.size() * sizeof(tagBio2ModelIndex) +
					Model->nVertices * sizeof(tagBio2ModelVertice) +
					Model->nNormals * sizeof(tagBio2ModelNormal) +
					Model->nPrimT * sizeof(tagBio2ModelTriPrim) +
					Model->nPrimQ * sizeof(tagBio2ModelQuadPrim);

	// Terminate
	return Pointer;
}
// Data Buffer
Bio2Model * Bio_Hazard_2_Model::Md1Dummy(VOID) {

	// Structure
	Bio2Model * Model = new tagBio2Model;
	Model->Header.pUV = NULL;
	Model->Header.unknown = NULL;
	Model->Header.nObj = NULL;
	Model->Obj.clear();
	Model->Size = NULL;
	Model->nVertices = NULL;
	Model->nNormals = NULL;
	Model->nPrimT = NULL;
	Model->nPrimQ = NULL;

	// Terminate
	return Model;
}
Bio2Model * Bio_Hazard_2_Model::Md1Read(_iobuf * FILE, ULONG _Offset) {

	// Structure
	Bio2Model * Model = Md1Dummy();

	// Error
	if (!IsOpen(FILE)) { return Model; }

	// Header
	ULONG Pointer = _Offset;
	Read(FILE, Pointer, &Model->Header, sizeof(tagBio2ModelHeader));

	// Object
	ULONG pIndex = _Offset + sizeof(tagBio2ModelHeader);
	ULONG pVertice = NULL;
	ULONG pNormal = NULL;
	ULONG pPrimitive = NULL;
	ULONG pPrimitiveUV = NULL;
	for (ULONG i = 0; i < (Model->Header.nObj/2); i++) {

		// Structure
		Bio2ModelObject * Object = new tagBio2ModelObject;
		Model->Obj.push_back(Object);

		// Index
		Read(FILE, pIndex, &Model->Obj[i]->Index, sizeof(tagBio2ModelIndex));
		pIndex += sizeof(tagBio2ModelIndex);

		// Triangle
		if (Model->Obj[i]->Index.nTriPrim) {

			// Pointer
			pVertice = Model->Obj[i]->Index.pTriVert + (_Offset + sizeof(tagBio2ModelHeader));
			pNormal = Model->Obj[i]->Index.pTriNorm + (_Offset + sizeof(tagBio2ModelHeader));
			pPrimitive = Model->Obj[i]->Index.pTriPrim + (_Offset + sizeof(tagBio2ModelHeader));
			pPrimitiveUV = Model->Obj[i]->Index.pTriUV + (_Offset + sizeof(tagBio2ModelHeader));

			// Vertice
			for (ULONG x = 0; x < Model->Obj[i]->Index.nTriVert; x++) {
				Bio2ModelVertice * Vertice = new tagBio2ModelVertice;
				Read(FILE, pVertice, Vertice, sizeof(tagBio2ModelVertice));
				Model->Obj[i]->TriV.push_back(Vertice);
				pVertice += sizeof(tagBio2ModelVertice);
			}

			// Normal
			for (ULONG x = 0; x < Model->Obj[i]->Index.nTriNorm; x++) {
				Bio2ModelNormal * Normal = new tagBio2ModelNormal;
				Read(FILE, pNormal, Normal, sizeof(tagBio2ModelNormal));
				Model->Obj[i]->TriN.push_back(Normal);
				pNormal += sizeof(tagBio2ModelNormal);
			}

			// Primitive
			for (ULONG x = 0; x < Model->Obj[i]->Index.nTriPrim; x++) {

				// Texture Mapping
				Bio2ModelTriPrim * Primitive = new tagBio2ModelTriPrim;
				Read(FILE, pPrimitive, Primitive, sizeof(tagBio2ModelTriPrim));
				Model->Obj[i]->TriP.push_back(Primitive);
				pPrimitive += sizeof(tagBio2ModelTriPrim);
				Model->nPrimT++;

				// Texture Mapping
				tagBio2ModelTriPrimUV * PrimitiveUV = new tagBio2ModelTriPrimUV;
				Read(FILE, pPrimitiveUV, PrimitiveUV, sizeof(tagBio2ModelTriPrimUV));
				Model->Obj[i]->TriUV.push_back(PrimitiveUV);
				pPrimitiveUV += sizeof(tagBio2ModelTriPrimUV);

			}

		}

		// Quadrilateral
		if (Model->Obj[i]->Index.nQuadPrim) {

			// Pointer
			pVertice = Model->Obj[i]->Index.pQuadVert + (_Offset + sizeof(tagBio2ModelHeader));
			pNormal = Model->Obj[i]->Index.pQuadNorm + (_Offset + sizeof(tagBio2ModelHeader));
			pPrimitive = Model->Obj[i]->Index.pQuadPrim + (_Offset + sizeof(tagBio2ModelHeader));
			pPrimitiveUV = Model->Obj[i]->Index.pQuadUV + (_Offset + sizeof(tagBio2ModelHeader));

			// Vertice
			for (ULONG x = 0; x < Model->Obj[i]->Index.nQuadVert; x++) {
				Bio2ModelVertice * Vertice = new tagBio2ModelVertice;
				Read(FILE, pVertice, Vertice, sizeof(tagBio2ModelVertice));
				Model->Obj[i]->QuadV.push_back(Vertice);
				pVertice += sizeof(tagBio2ModelVertice);
			}

			// Normal
			for (ULONG x = 0; x < Model->Obj[i]->Index.nQuadNorm; x++) {
				Bio2ModelNormal * Normal = new tagBio2ModelNormal;
				Read(FILE, pNormal, Normal, sizeof(tagBio2ModelNormal));
				Model->Obj[i]->QuadN.push_back(Normal);
				pNormal += sizeof(tagBio2ModelNormal);
			}

			// Primitive
			for (ULONG x = 0; x < Model->Obj[i]->Index.nQuadPrim; x++) {

				// Texture Mapping
				Bio2ModelQuadPrim * Primitive = new tagBio2ModelQuadPrim;
				Read(FILE, pPrimitive, Primitive, sizeof(tagBio2ModelQuadPrim));
				Model->Obj[i]->QuadP.push_back(Primitive);
				pPrimitive += sizeof(tagBio2ModelQuadPrim);
				Model->nPrimQ++;

				// Texture Mapping
				tagBio2ModelQuadPrimUV * PrimitiveUV = new tagBio2ModelQuadPrimUV;
				Read(FILE, pPrimitiveUV, PrimitiveUV, sizeof(tagBio2ModelQuadPrimUV));
				Model->Obj[i]->QuadUV.push_back(PrimitiveUV);
				pPrimitiveUV += sizeof(tagBio2ModelQuadPrimUV);

			}

		}

		// Statistical
		if (Model->Obj[i]->Index.nQuadVert) { Model->nVertices += Model->Obj[i]->Index.nQuadVert; }
		else { Model->nVertices += Model->Obj[i]->Index.nTriVert; }
		if (Model->Obj[i]->Index.nQuadNorm) { Model->nNormals += Model->Obj[i]->Index.nQuadNorm; }
		else { Model->nNormals += Model->Obj[i]->Index.nTriNorm; }

	}

	// Statistical
	Model->Size = Md1Size(Model);

	// Terminate
	return Model;
}
Bio2Model * Bio_Hazard_2_Model::Md1Read(CONST CHAR * _FileName, ...) {

	// Structure
	Bio2Model * Model = Md1Dummy();

	// _FileName
	va_list _ArgList = { NULL };
	__crt_va_start(_ArgList, _FileName);
	_StrLen = (_vscprintf(_FileName, _ArgList) + 2);
	CHAR * FileName = new CHAR[(_StrLen * 2)];
	RtlSecureZeroMemory(FileName, (_StrLen * 2));
	vsprintf_s(FileName, _StrLen, _FileName, _ArgList);
	__crt_va_end(_ArgList);

	// Error
	if (!Exists(FileName)) { delete[] FileName; return Model; }

	// Open
	_iobuf * _File = Open(READ_FILE, FileName);
	CHAR * Directory = GetDirectory(FileName);
	CHAR * NameBase = GetNamebase(FileName);
	delete[] FileName;
	if (!_File) { return FAIL; }

	// Structure
	Model = Md1Read(_File, 0x00);
	Md1DebugLog(Model, (CHAR*)"%s%s.log", Directory, NameBase);

	// Cleanup
	fclose(_File);

	// Terminate
	return Model;
}
BOOL Bio_Hazard_2_Model::Md1Write(Bio2Model * Model, _iobuf * FILE, ULONG _Offset) {

	// Error
	if (!IsOpen(FILE)) { return FAIL; }
	if (!Model->Obj.size()) { return FAIL; }

	// Texture
	ULONG pppUV = _Offset + Md1PrimUVPointer(Model);
	for (ULONG i = 0; i < Model->Obj.size(); i++) {
		Model->Obj[i]->Index.pTriUV = pppUV - _Offset - sizeof(tagBio2ModelHeader);
		if (Model->Obj[i]->Index.nTriPrim) {
			for (ULONG x = 0; x < Model->Obj[i]->Index.nTriPrim; x++) {
				Write(FILE, pppUV, Model->Obj[i]->TriUV[x], sizeof(tagBio2ModelTriPrimUV));
				pppUV += sizeof(tagBio2ModelTriPrimUV);
			}
		}
		Model->Obj[i]->Index.pQuadUV = pppUV - _Offset - sizeof(tagBio2ModelHeader);
		if (Model->Obj[i]->Index.nQuadPrim) {
			for (ULONG x = 0; x < Model->Obj[i]->Index.nQuadPrim; x++) {
				Write(FILE, pppUV, Model->Obj[i]->QuadUV[x], sizeof(tagBio2ModelQuadPrimUV));
				pppUV += sizeof(tagBio2ModelQuadPrimUV);
			}
		}
	}

	// Primitive
	ULONG pppPrim = _Offset + Md1PrimPointer(Model);
	for (ULONG i = 0; i < Model->Obj.size(); i++) {
		Model->Obj[i]->Index.pTriPrim = pppPrim - _Offset - sizeof(tagBio2ModelHeader);
		if (Model->Obj[i]->Index.nTriPrim) {
			for (ULONG x = 0; x < Model->Obj[i]->Index.nTriPrim; x++) {
				Write(FILE, pppPrim, Model->Obj[i]->TriP[x], sizeof(tagBio2ModelTriPrim));
				pppPrim += sizeof(tagBio2ModelTriPrim);
			}
		}
		Model->Obj[i]->Index.pQuadPrim = pppPrim - _Offset - sizeof(tagBio2ModelHeader);
		if (Model->Obj[i]->Index.nQuadPrim) {
			for (ULONG x = 0; x < Model->Obj[i]->Index.nQuadPrim; x++) {
				Write(FILE, pppPrim, Model->Obj[i]->QuadP[x], sizeof(tagBio2ModelQuadPrim));
				pppPrim += sizeof(tagBio2ModelQuadPrim);
			}
		}
	}

	// Normal
	ULONG pppNormal = _Offset + Md1NormPointer(Model);
	for (ULONG i = 0; i < Model->Obj.size(); i++) {
		ULONG Pointer = pppNormal;
		Model->Obj[i]->Index.pTriNorm = pppNormal - _Offset - sizeof(tagBio2ModelHeader);
		if (Model->Obj[i]->Index.nTriPrim) {
			for (ULONG x = 0; x < Model->Obj[i]->Index.nTriNorm; x++) {
				Write(FILE, pppNormal, Model->Obj[i]->TriN[x], sizeof(tagBio2ModelNormal));
				pppNormal += sizeof(tagBio2ModelNormal);
			}
		}
		pppNormal = Pointer;
		Model->Obj[i]->Index.pQuadNorm = pppNormal - _Offset - sizeof(tagBio2ModelHeader);
		if (Model->Obj[i]->Index.nQuadPrim) {
			for (ULONG x = 0; x < Model->Obj[i]->Index.nQuadNorm; x++) {
				Write(FILE, pppNormal, Model->Obj[i]->QuadN[x], sizeof(tagBio2ModelNormal));
				pppNormal += sizeof(tagBio2ModelNormal);
			}
		}
	}

	// Vertice
	ULONG pppVertice = _Offset + Md1VertPointer(Model);
	for (ULONG i = 0; i < Model->Obj.size(); i++) {
		ULONG Pointer = pppVertice;
		Model->Obj[i]->Index.pTriVert = pppVertice - _Offset - sizeof(tagBio2ModelHeader);
		if (Model->Obj[i]->Index.nTriPrim) {
			for (ULONG x = 0; x < Model->Obj[i]->Index.nTriVert; x++) {
				Write(FILE, pppVertice, Model->Obj[i]->TriV[x], sizeof(tagBio2ModelVertice));
				pppVertice += sizeof(tagBio2ModelVertice);
			}
		}
		pppVertice = Pointer;
		Model->Obj[i]->Index.pQuadVert = pppVertice - _Offset - sizeof(tagBio2ModelHeader);
		if (Model->Obj[i]->Index.nQuadPrim) {
			for (ULONG x = 0; x < Model->Obj[i]->Index.nQuadVert; x++) {
				Write(FILE, pppVertice, Model->Obj[i]->QuadV[x], sizeof(tagBio2ModelVertice));
				pppVertice += sizeof(tagBio2ModelVertice);
			}
		}
	}

	// Index
	ULONG pppIndex = _Offset + sizeof(tagBio2ModelHeader);
	for (ULONG i = 0; i < Model->Obj.size(); i++) {
		Write(FILE, pppIndex, &Model->Obj[i]->Index, sizeof(tagBio2ModelIndex));
		pppIndex += sizeof(tagBio2ModelIndex);
	}

	// Header
	Write(FILE, _Offset, &Model->Header, sizeof(tagBio2ModelHeader));

	// Terminate
	return SUCCESS;
}
BOOL Bio_Hazard_2_Model::Md1Write(Bio2Model * Model, CONST CHAR * _FileName, ...) {

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
	BOOL bResult = NULL;
	if (Model->Header.nObj) {
		bResult = Md1Write(Model, _File, 0x00);
	}

	// Cleanup
	fclose(_File);

	// Terminate
	return bResult;
}
// Extra
Bio2ModelObject * Bio_Hazard_2_Model::Md1CombineObj(Bio2ModelObject * Obj0, Bio2ModelObject * Obj1) {

	// Structure
	Bio2ModelObject * Object = new tagBio2ModelObject;

	// Terminate
	return Object;
}