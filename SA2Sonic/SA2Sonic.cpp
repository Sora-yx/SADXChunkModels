// SA2Sonic.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "IniFile.hpp"
#include "SADXModLoader.h"
#include "ModelInfo.h"

using std::unordered_map;
using std::string;

unordered_map<int, NJS_OBJECT *> modelmap;
void DrawSonicModel_i(int a1, CharObj2 *a2)
{
	DrawChunkObject(modelmap[0]);
}

static void __declspec(naked) DrawSonicModel()
{
	__asm
	{
		push esi // a2
		push eax // a1

		// Call your __cdecl function here:
		call DrawSonicModel_i

		pop eax // a1
		pop esi // a2
		retn
	}
}

void DrawSonicModelAlt_i(CharObj2 *a1)
{
	DrawChunkObject(modelmap[0]);
}

static void __declspec(naked) DrawSonicModelAlt()
{
	__asm
	{
		push esi // a1

		// Call your __cdecl function here:
		call DrawSonicModelAlt_i

		pop esi // a1
		retn
	}
}

extern "C"
{
	__declspec(dllexport) void Init(const char *path, const HelperFunctions &helperFunctions)
	{
		const string mdlpath = string(path) + "\\system\\sonicmdl\\";
		unordered_map<string, void*> labels;
		WIN32_FIND_DATAA data;
		const string srcPathSearch = mdlpath + "*.sa2mdl";
		const HANDLE hFind = FindFirstFileA(srcPathSearch.c_str(), &data);
		if (hFind == INVALID_HANDLE_VALUE)
		{
			// No files found.
			return;
		}

		do
		{
			const string modFile = mdlpath + string(data.cFileName);
			ModelInfo *mdl = new ModelInfo(modFile.c_str());
			auto map = mdl->getlabels();
			for (auto i = map->cbegin(); i != map->cend(); ++i)
				labels[i->first] = i->second;
		} while (FindNextFileA(hFind, &data) != 0);
		FindClose(hFind);
		const IniFile *mdlini = new IniFile(mdlpath + "sonicmdl.ini");
		const IniGroup *mdlgrp = mdlini->getGroup("");
		for (auto i = mdlgrp->cbegin(); i != mdlgrp->cend(); ++i)
			modelmap[std::stol(i->first)] = (NJS_OBJECT *)labels[i->second];
		delete mdlini;
		WriteJump((void*)0x494400, DrawSonicModel);
		WriteJump((void*)0x494570, DrawSonicModelAlt);
	}

	__declspec(dllexport) ModInfo SADXModInfo = { ModLoaderVer };
}