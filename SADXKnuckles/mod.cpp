
#include "pch.h"
#include "IniFile.hpp"
#include "SADXModLoader.h"
#include "ModelInfo.h"
#include "AnimationFile.h"
#include "..\CommonFunctions\CommonFunctions.h"
#include "..\CommonFunctions\CommonSADXFunctions.h"
#include "UsercallFunctionHandler.h"

using std::unordered_map;
using std::vector;
using std::string;

unordered_map<int, NJS_OBJECT*> modelmap;
unordered_map<NJS_OBJECT*, NJS_OBJECT*> modelmap2;
char currentplayer;
static NJS_MATRIX KnuxLeftHandMatrix;
static NJS_MATRIX KnuxRightHandMatrix;

void NodeCallback2(NJS_OBJECT* obj)
{
	float* v1 = _nj_current_matrix_ptr_;
	if (obj == modelmap[5])
	{
		NJS_VECTOR v = { 0, 0, 0 };
		njCalcPoint(v1, &v, &CharObj2Ptrs[currentplayer]->SoManyVectors[0]);
		v.z = 1;
		njCalcVector(v1, &v, &CharObj2Ptrs[currentplayer]->SoManyVectors[6]);
	}
	else if (obj == modelmap[9])
	{
		NJS_VECTOR v = { 0, 0, 0 };
		njCalcPoint(v1, &v, &CharObj2Ptrs[currentplayer]->SoManyVectors[1]);
		v.z = 1;
		njCalcVector(v1, &v, &CharObj2Ptrs[currentplayer]->SoManyVectors[7]);
	}
}

void(__cdecl** NodeCallbackFuncPtr)(NJS_OBJECT* obj) = (decltype(NodeCallbackFuncPtr))0x3AB9908;
void DrawKnuxModel(CharObj2* a2, int animNum, NJS_ACTION* action)
{
	njPushMatrix(nullptr);
	SetupWorldMatrix();
	Direct3D_SetChunkModelRenderState();

	NJS_ACTION act2 = *action;
	act2.object = modelmap2[act2.object];

	if (*(int*)0x3ABD9CC)
	{
		DrawQueueDepthBias = -5952.0;
		njCnkAction_Queue(&act2, a2->AnimationThing.Frame, QueuedModelFlagsB_EnableZWrite);
		DrawQueueDepthBias = 0.0;
	}
	else
	{
		njCnkAction(&act2, a2->AnimationThing.Frame);
	}

	Direct3D_UnsetChunkModelRenderState();
	njPopMatrix(1);
}

void CallBackStuff(NJS_ACTION* action, CharObj2* data2) {
	NJS_ACTION act2 = *action;
	act2.object = modelmap2[act2.object];
	*NodeCallbackFuncPtr = NodeCallback2;
	njPushMatrix(_nj_unit_matrix_);
	njNullAction(&act2, data2->AnimationThing.Frame);
	njPopMatrix(1);
	*NodeCallbackFuncPtr = nullptr;
}

DataArray(AnimData, KnuxAnimData, 0x3C532A0, 115);
void __cdecl Knuckles_Display_r(ObjectMaster* obj)
{
	CharObj2* data2; // edi
	EntityData1* data1_ptr; // esi
	int currentAnimation; // ebx
	int v4; // eax
	_BOOL1 v5; // zf
	Angle v6; // eax
	double v7; // st7
	Angle v8; // eax
	char action; // al
	Angle v11; // eax
	Angle v12; // eax
	Angle v13; // eax
	int v14; // ebp
	long double v15; // st7
	__int16 v16; // ax
	unsigned __int16 v17; // ax
	int animation; // ebp
	int i; // ebx
	double v20; // st7
	float frameCount; // [esp+14h] [ebp-1Ch]
	NJS_VECTOR a2; // [esp+18h] [ebp-18h] BYREF
	NJS_VECTOR vs; // [esp+24h] [ebp-Ch] BYREF
	motionwk2* field_24_dupea; // [esp+34h] [ebp+4h]
	float frame; // [esp+34h] [ebp+4h]
	data1_ptr = obj->Data1;
	data2 = ((EntityData2*)obj->Data2)->CharacterData;
	csts* v10 = (csts*)data2->array_1x132;
	field_24_dupea = (motionwk2*)obj->Data2;
	taskwk* mwk = (taskwk*)data1_ptr;

	if (IsVisible(&data1_ptr->Position, 20.0))
	{
		Direct3D_SetZFunc(1u);
		currentAnimation = (unsigned __int16)data2->AnimationThing.Index;
		if (data2->AnimationThing.State == 2)
		{
			currentAnimation = (unsigned __int16)data2->AnimationThing.LastIndex;
		}
		if ((data1_ptr->Status & Status_Ball) != 0 && !IsGamePaused())
		{
			++(data2->field_84);
			v4 = LOBYTE(data2->field_84);
			if (data2->AnimationThing.Index == 34)
			{
				v5 = (v4 & 1) == 0;
			}
			else
			{
				v5 = (v4 & 0x11) == 0;
			}
			if (!v5)
			{
				currentAnimation = 16;
			}
		}
		if ((data1_ptr->InvulnerableTime & 2) == 0)
		{
			njSetTexture(&KNUCKLES_TEXLIST);
			Direct3D_PerformLighting(2);
			njControl3D_Backup();
			njControl3D(NJD_CONTROL_3D_CONSTANT_MATERIAL);
			SetMaterialAndSpriteColor_Float(1.0, 1.0, 1.0, 1.0);
			njPushMatrix(0);
			v7 = data2->PhysicsData.CollisionSize * 0.5;
			vs.z = 0.0;
			vs.x = 0.0;
			vs.y = v7;
			njPushMatrix(_nj_unit_matrix_);
			v6 = data1_ptr->Rotation.z;
			if (v6)
			{
				njRotateZ(0, (unsigned __int16)v6);
			}
			v8 = data1_ptr->Rotation.x;
			if (v8)
			{
				njRotateX(0, (unsigned __int16)v8);
			}
			if (data1_ptr->Rotation.y)
			{
				njRotateY(0, (unsigned __int16)-LOWORD(data1_ptr->Rotation.y));
			}
			njCalcVector(0, &vs, &a2);
			njPopMatrix(1u);
			action = data1_ptr->Action;
			v5 = data1_ptr->Action == 19;
			a2.x = a2.x + data1_ptr->Position.x;
			a2.y = a2.y + data1_ptr->Position.y;
			a2.z = a2.z + data1_ptr->Position.z;
			if (v5 || action == 20) //digging stuff
			{

				a2.x = v10->normal2.x * mwk->value.f + a2.x;
				a2.y = v10->normal2.y * mwk->value.f + a2.y;
				a2.z = v10->normal2.z * mwk->value.f + a2.z;
			}
			njTranslateV(0, &a2);
			data1_ptr->CollisionInfo->CollisionArray->center = a2;
			if (currentAnimation == 51)
			{
				v16 = SubAngle(data1_ptr->Rotation.y, field_24_dupea->ang_aim.y);
				data2->LightdashTimer = BAMS_SubWrap(data2->LightdashTimer, -v16, 2048);
				if (data1_ptr->Rotation.y != 0x8000)
				{
					njRotateY(0, (unsigned __int16)(0x8000 - LOWORD(data1_ptr->Rotation.y)));
				}
				v17 = data2->LightdashTimer;
				if (v17)
				{
					njRotateX(0, v17);
				}
			}
			else if (currentAnimation == 52)
			{
				v14 = (unsigned __int64)((1.0
					- data2->AnimationThing.Frame
					/ (double)KnuxAnimData[(unsigned __int16)data2->AnimationThing.Index].Animation->motion->nbFrame)
					* (double)data2->LightdashTimer);
				v13 = data1_ptr->Rotation.y;
				if (v13 != 0x8000)
				{
					njRotateY(0, (unsigned __int16)(0x8000 - v13));
				}
				if (v14)
				{
					njRotateX(0, (unsigned __int16)v14);
				}


				v15 = acos(v10->normal2.y) * 65536.0 * -0.1591549762031479;
				if ((unsigned int)(unsigned __int64)v15)
				{
					njRotateZ(0, (unsigned __int16)(unsigned __int64)v15 + 0x8000);
				}
			}
			else
			{
				v11 = data1_ptr->Rotation.z;
				if (v11)
				{
					njRotateZ(0, (unsigned __int16)v11);
				}
				v12 = data1_ptr->Rotation.x;
				if (v12)
				{
					njRotateX(0, (unsigned __int16)v12);
				}
				if (data1_ptr->Rotation.y != 0x8000)
				{
					njRotateY(0, (unsigned __int16)(0x8000 - LOWORD(data1_ptr->Rotation.y)));
				}
				if (currentAnimation == 14 && (data1_ptr->Status & (Status_Unknown1 | Status_Ground)) != 0)
				{
					a2.x = 0.0;
					a2.y = -1.0;
					a2.z = 0.0;
					njTranslateV(0, &a2);
					njRotateZ(0, 0x2000);
					a2.x = 0.69999999;
					a2.y = 1.1;
					a2.z = 0.80000001;
					njScaleV(0, &a2);
				}
			}

			if (*((_DWORD*)data1_ptr->field_3C + 16))
			{
				DrawEventAction(data1_ptr);
				//DrawEventActionPL(data1_ptr, dspl);
			}
			else
			{
				NJS_ACTION* action;
				if (data2->AnimationThing.State == 2)
					action = data2->AnimationThing.action;
				else
					action = data2->AnimationThing.AnimData[currentAnimation].Animation;
				animation = currentAnimation;
				DrawKnuxModel(data2, currentAnimation, action);
				CallBackStuff(action, data2);


				if (currentAnimation == 13
					|| currentAnimation == 14 && (data1_ptr->Status & (Status_Unknown1 | Status_Ground)) != 0)
				{
					frame = data2->AnimationThing.Frame;
					frameCount = (float)data2->AnimationThing.AnimData[(unsigned __int16)data2->AnimationThing.Index].Animation->motion->nbFrame;
					i = 2;
					do
					{
						v20 = frame - 2.0;
						frame = v20;
						if (v20 < 0.0)
						{
							frame = frameCount + frame;
						}
						//njAction(data2->AnimationThing.AnimData[animation].Animation, frame);
						DrawKnuxModel(data2, animation, action);
						CallBackStuff(action, data2);
						--i;
					} while (i);
					ClampGlobalColorThing_Thing();
				}
			}
			njPopMatrix(1u);
			ClampGlobalColorThing_Thing();
			njControl3D_Restore();
			Direct3D_PerformLighting(0);
		}
		Direct3D_ResetZFunc();
		if (IsGamePaused())
		{
			DrawCharacterShadow(data1_ptr, &data2->_struct_a3);
		}
	}
}



extern "C"
{
	__declspec(dllexport) void Init(const char* path, const HelperFunctions& helperFunctions)
	{
		string mdlpath = string(path) + "\\models\\";
		unordered_map<string, void*> labels;
		if (!FindModels(mdlpath, labels))
			return;
		const IniFile* mdlini = new IniFile(mdlpath + "models.ini");
		const IniGroup* mdlgrp = mdlini->getGroup("");
		for (auto i = mdlgrp->cbegin(); i != mdlgrp->cend(); i++)
			if (labels.find(i->second) != labels.cend())
				modelmap[std::stol(i->first)] = (NJS_OBJECT*)labels[i->second];
		delete mdlini;
		for (int i = 0; i < 77; i++) {
			if (modelmap.find(i) != modelmap.cend())
				modelmap2[KNUCKLES_OBJECTS[i]] = modelmap[i];
		}

		if (modelmap.find(74) != modelmap.cend()) //assign spin form to the new chunk model.
			modelmap2[KNUCKLES_ACTIONS[61]->object] = modelmap[74];

		if (modelmap.find(75) != modelmap.cend()) //assign ball form to the new chunk model.
			modelmap2[KNUCKLES_ACTIONS[62]->object] = modelmap[75];

		WriteJump(Knuckles_Display, Knuckles_Display_r);

		//WriteCall((void*)0x47255A, DrawEventActionPL);
	}

	__declspec(dllexport) ModInfo SADXModInfo = { ModLoaderVer };
}