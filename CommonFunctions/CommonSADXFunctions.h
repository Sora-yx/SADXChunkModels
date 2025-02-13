#pragma once


struct CharMdlPrmT
{
	void(__cdecl* BornFnc)(task*);
	NJS_TEXLIST* TlsPtr;
	NJS_ACTION** ActPP;
	int* FrameP;
};



struct __declspec(align(4)) CharMdlWk
{
	task* PlTskPtrs[7];
	task* CamTskPtr;
	float LocX;
	float LocY;
	float Scale;
	char MdlDspCnt;
	char MotCnts[7];
	char SelFlg;
};


FunctionPointer(void, DrawCharacterShadow, (EntityData1* a1, struct_a3* a2), 0x49F0B0);
FunctionPointer(void, DrawEventAction, (EntityData1* a1), 0x4187D0);
FunctionPointer(void, DrawEventActionPL, (EntityData1* data1, int light), 0x417FB0);
FunctionPointer(void, NjAction_QueueLinkEx, (NJS_ACTION* anim, float frame, QueuedModelFlagsB flg), 0x408330);
FunctionPointer(int, SubAngle, (__int16 ang0, __int16 ang1), 0x4383A0);
FunctionPointer(void, sub_4084D0, (NJS_ACTION_LINK* a1, int a2, int a3), 0x4084D0);
FunctionPointer(void, sub_406EE0, (NJS_ACTION_LINK* a1, int a2, int a3), 0x406EE0);
FunctionPointer(void, sub_4084B0, (NJS_ACTION_LINK* a1, int a2, int a3), 0x4084B0);

DataPointer(int, dspl, 0x90C4E8);
