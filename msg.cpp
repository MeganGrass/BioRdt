
#include "stdafx.h"
#include FRAMEWORK_H

#include BIO2_H

// Bio 2
BOOL Bio_Hazard_2_Room::MsgSave(CONST CHAR * _FileName, ...) {

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

	// MSG
	if (Header->pMessage && Msg->Num) {
		ULONG iData = NULL;
		do {
			CreateFromSource(Msg->Data[iData].Buf, Msg->Data[iData].Size, (CHAR*)"E:\\Source\\Test\\x64\\Debug\\Message\\MSG%02d.MSG", iData);
			iData++;
		} while (iData != Msg->Num);
	}

	// Cleanup
	fclose(_File);

	// Terminate
	return TRUE;
}
ULONG Bio_Hazard_2_Room::MsgGetSize(_iobuf * FILE, ULONG _Offset) {

	// Test
	ULONG Pointer = _Offset;
	do {
		UCHAR Terminator = NULL;
		Read(FILE, Pointer, &Terminator, sizeof(unsigned char));
		if (Terminator != 0xFE) {}
		else { break; }
		Pointer++;
	} while (!feof(FILE));

	// Terminate
	return (Pointer - _Offset);
}