#include "pch.h"


FunctionHook<Bool, ObjectMaster*, ObjectMaster*> CheckBreakObject_t((intptr_t)CheckBreakObject);
TaskHook Dynamite_t(Dynamite_Main);
TaskHook DynamiteHiddenBase_t(DynamiteHiddenBase_Main);
TaskHook DynamiteSandOcean_t(DynamiteSandOcean_Main);
TaskHook MetalBox_t(MetalBox);
TaskHook MetalBoxGravity_t(MetalBoxGravity);
TaskHook RoadObjMain_t((intptr_t)0x5B0ED0);


Bool __cdecl CheckBreakObject_r(ObjectMaster* obj, ObjectMaster* other)
{
	auto data = obj->Data1.Entity;

	if (data && checkAmyColAndAttack(data))
		return 1;

	return CheckBreakObject_t.Original(obj, other);
}


void CheckBreakDynamite(ObjectMaster* obj) {

	auto data = obj->Data1.Entity;

	if (obj && data) {
		if (data->Action == 0 && checkAmyColAndAttack(data)) {
			data->Status |= 4u;
			obj->EntityData2->gap_44[0] = 0;
		}
	}

	Dynamite_t.Original(obj);
}

void CheckBreakDynamiteHiddenBase(ObjectMaster* obj) {

	auto data = obj->Data1.Entity;

	if (obj && data) {
		if (data->NextAction != 7 && (checkAmyColAndAttack(data))) {
			data->Timer = 0;
			data->NextAction = 7;
		}
	}

	DynamiteHiddenBase_t.Original(obj);
}

void CheckBreakDynamiteSandOcean(ObjectMaster* obj) {

	auto data = obj->Data1.Entity;

	if (obj && data) {
		if (data->Action == 0 && (checkAmyColAndAttack(data))) {
			data->Status |= 4u;
			obj->EntityData2->gap_44[0] = 0;
		}
	}

	DynamiteSandOcean_t.Original(obj);
}


void MetalBox_r(ObjectMaster* obj) {

	auto data = obj->Data1.Entity;

	if (data && checkAmyColAndAttack(data) && data->NextAction < 1)
	{
		data->Collision->CollisionArray->push |= 0x4000u;
		data->Timer = 1;
		AddScore(20);
		Play3DSound_Pos(4113, &data->Position, 1, 127, 80);
		data->NextAction = 1;
	}

	MetalBox_t.Original(obj);
}


void MetalBoxGravity_r(ObjectMaster* obj) {

	auto data = obj->Data1.Entity;

	if (data && checkAmyColAndAttack(data) && data->NextAction < 1)
	{
		data->Collision->CollisionArray->push |= 0x4000u;
		data->Timer = 1;
		AddScore(20);
		Play3DSound_Pos(4113, &data->Position, 1, 127, 80);
		data->NextAction = 1;
	}

	MetalBoxGravity_t.Original(obj);
}

void RoadObjMain_r(ObjectMaster* obj)
{
	auto data = obj->Data1.Entity;

	if (data && checkAmyColAndAttack(data) && !data->NextAction)
	{
		data->NextAction++;
	}

	RoadObjMain_t.Original(obj);
}

void Init_ObjectsHack() {
	CheckBreakObject_t.Hook(CheckBreakObject_r);
	Dynamite_t.Hook(CheckBreakDynamite);
	DynamiteHiddenBase_t.Hook(CheckBreakDynamiteHiddenBase);
	DynamiteSandOcean_t.Hook(CheckBreakDynamiteSandOcean);
	MetalBox_t.Hook(MetalBox_r);
	MetalBoxGravity_t.Hook(MetalBoxGravity_r);
	RoadObjMain_t.Hook(RoadObjMain_r);
}