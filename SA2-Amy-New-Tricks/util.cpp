#include "pch.h"

void njCnkAction(NJS_ACTION* action, float frame)
{
	*(int*)0x25EFE54 = 0x25EFE60;
	njSetMotion(action->motion, frame);
	MotionDrawCallback = (ObjectFuncPtr)0x42E660;
	DrawObjMotion(action->object);
}

void njCnkMotion(NJS_OBJECT* obj, NJS_MOTION* mot, float frame)
{
	*(int*)0x25EFE54 = 0x25EFE60;
	njSetMotion(mot, frame);
	MotionDrawCallback = (ObjectFuncPtr)0x42E660;
	DrawObjMotion(obj);
}

const char* ModelFormatStrings[]{
	"collision",
	"chunk",
	"battle"
};

//Load Object File
ModelInfo* LoadMDL(const char* name, ModelFormat format) {
	std::string fullPath;

	if (format == ModelFormat_Chunk) {
		fullPath = "resource\\gd_PC\\Models\\";
	}

	fullPath += name;

	switch (format) {
	case ModelFormat_Basic:
		fullPath += ".sa1mdl";
		break;
	case ModelFormat_Chunk:
		fullPath += ".sa2mdl";
		break;
	case ModelFormat_SA2B:
		fullPath += ".sa2bmdl";
		break;
	}

	const char* foo = fullPath.c_str();

	ModelInfo* temp = new ModelInfo(HelperFunctionsGlobal.GetReplaceablePath(foo));

	if (temp->getformat() == format) {
		PrintDebug("[Amy New Tricks Mod] Loaded %s model: %s.", ModelFormatStrings[(int)format - 1], name);
	}
	else {
		PrintDebug("[Amy New Tricks Mod] Failed loading %s model: %s.", ModelFormatStrings[(int)format - 1], name);
	}

	return temp;
}

void LoadAnimation(AnimationFile** info, const char* name, const HelperFunctions& helperFunctions) {
	std::string fullPath = "resource\\gd_PC\\Animations\\";
	fullPath = fullPath + name + ".saanim";

	AnimationFile* anm = new AnimationFile(helperFunctions.GetReplaceablePath(fullPath.c_str()));

	if (anm->getmodelcount() == 0) {
		delete anm;
		*info = nullptr;
	}
	else {
		*info = anm;
	}
};

AnimationFile* LoadAnim(const char* name) {
	std::string fullPath = "resource\\gd_PC\\Animations\\";

	fullPath = fullPath + name + ".saanim";

	AnimationFile* file = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath(fullPath.c_str()));

	if (file->getmotion() != nullptr) {
		PrintDebug("[Amy New Tricks Mod] Loaded animation: %s.", name);
	}
	else {
		PrintDebug("[Amy New Tricks Mod] Failed loading animation: %s.", name);
	}

	return file;
}

void FreeMDL(ModelInfo* pointer) {
	if (pointer) 
		delete(pointer);
}

void FreeAnim(AnimationFile* pointer) {
	if (pointer) 
		delete pointer;
}

int __cdecl BAMS_SubWrap(__int16 bams_a, unsigned __int16 bams_b, int limit)
{
	int result; // eax
	__int16 v4; // cx

	result = bams_b;
	v4 = bams_b - bams_a;
	if ((__int16)(bams_b - bams_a) > limit || v4 < -limit)
	{
		if (v4 >= 0)
		{
			result = (unsigned __int16)(limit + bams_a);
		}
		else
		{
			result = (unsigned __int16)(bams_a - limit);
		}
	}
	return result;
}

bool isCharaSelect() {
	HMODULE charaMod = GetModuleHandle(L"SA2CharSel");
	HMODULE charaModPlus = GetModuleHandle(L"CharacterSelectPlus");

	if (charaMod || charaModPlus)
		return true;

	return false;
}


bool CheckControl(int id)
{
	auto co2 = MainCharObj1[id];

	if (!co2)
		return false;


	return GameMode != GameMode_Event && ControllerEnabled[id] == 1;
}

void njGetTranslation(NJS_MATRIX_PTR matrix, NJS_VECTOR* out)
{
	if (!matrix)
	{
		matrix = CUR_MATRIX;
	}

	out->x = matrix[3];
	out->y = matrix[7];
	out->z = matrix[11];
}

bool isSA1Amy() {
	HMODULE sa1Amy = GetModuleHandle(L"SA1-Amy");

	if (sa1Amy)
		return true;

	return false;
}


bool isAmyAttacking(char pNum) {

	auto data1 = MainCharObj1[pNum];
	auto co2 = MainCharObj2[pNum];

	if (!data1 || !co2 || co2->CharID2 != Characters_Amy)
		return false;

	if (data1->Action >= HammerAttack) {

		return true;
	}

	return false;
}

bool checkAmyColAndAttack(EntityData1* data)
{
	if (data)
	{
		if (data->Collision) {

			if (data->Collision->CollidingObject && data->Collision->CollidingObject->Object) {

				int pnum = GetPlayerNumber(data->Collision->CollidingObject->Object);

				if (pnum >= 0) {
					if (isAmyAttacking(pnum))
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

