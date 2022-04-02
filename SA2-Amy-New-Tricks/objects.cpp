#include "pch.h"

Trampoline* CheckBreakObject_t;
Trampoline* Dynamite_t;
Trampoline* DynamiteHiddenBase_t;
Trampoline* DynamiteSandOcean_t;
Trampoline* MetalBox_t;
Trampoline* MetalBoxGravity_t;
Trampoline* RoadObjMain_t = nullptr;


Bool __cdecl CheckBreakObject_r(ObjectMaster* obj, ObjectMaster* other)
{
	EntityData1* data = obj->Data1.Entity;

	if (checkAmyColAndAttack(obj, data))
		return 1;

	FunctionPointer(Bool, original, (ObjectMaster * obj, ObjectMaster * other), CheckBreakObject_t->Target());
	return original(obj, other);
}


void CheckBreakDynamite(ObjectMaster* obj) {

	EntityData1* data = obj->Data1.Entity;

	if (obj) {
		if (data->Action == 0 && checkAmyColAndAttack(obj, data)) {
			data->Status |= 4u;
			obj->EntityData2->gap_44[0] = 0;
		}
	}

	ObjectFunc(origin, Dynamite_t->Target());
	origin(obj);
}

void CheckBreakDynamiteHiddenBase(ObjectMaster* obj) {

	EntityData1* data = obj->Data1.Entity;

	if (obj) {
		if (data->NextAction != 7 && (checkAmyColAndAttack(obj, data))) {
			data->field_6 = 0;
			data->NextAction = 7;
		}
	}

	ObjectFunc(origin, DynamiteHiddenBase_t->Target());
	origin(obj);
}

void CheckBreakDynamiteSandOcean(ObjectMaster* obj) {

	EntityData1* data = obj->Data1.Entity;

	if (obj) {
		if (data->Action == 0 && (checkAmyColAndAttack(obj, data))) {
			data->Status |= 4u;
			obj->EntityData2->gap_44[0] = 0;
		}
	}

	ObjectFunc(origin, DynamiteSandOcean_t->Target());
	origin(obj);
}


void MetalBox_r(ObjectMaster* obj) {

	EntityData1* data = obj->Data1.Entity;

	if (checkAmyColAndAttack(obj, data) && data->NextAction < 1)
	{
		data->Collision->CollisionArray->push |= 0x4000u;
		data->field_6 = 1;
		AddScore(20);
		Play3DSound_Pos(4113, &data->Position, 1, 127, 80);
		data->NextAction = 1;
	}

	ObjectFunc(origin, MetalBox_t->Target());
	origin(obj);
}


void MetalBoxGravity_r(ObjectMaster* obj) {

	EntityData1* data = obj->Data1.Entity;

	if (checkAmyColAndAttack(obj, data) && data->NextAction < 1)
	{
		data->Collision->CollisionArray->push |= 0x4000u;
		data->field_6 = 1;
		AddScore(20);
		Play3DSound_Pos(4113, &data->Position, 1, 127, 80);
		data->NextAction = 1;
	}

	ObjectFunc(origin, MetalBoxGravity_t->Target());
	origin(obj);
}

void RoadObjMain_r(ObjectMaster* obj)
{
	EntityData1* data = obj->Data1.Entity;

	if (checkAmyColAndAttack(obj, data) && !data->NextAction)
	{
		data->NextAction++;
	}

	ObjectFunc(origin, RoadObjMain_t->Target());
	origin(obj);
}

void Init_ObjectsHack() {
	CheckBreakObject_t = new Trampoline((int)CheckBreakObject, (int)CheckBreakObject + 0x7, CheckBreakObject_r);
	Dynamite_t = new Trampoline((int)Dynamite_Main, (int)Dynamite_Main + 0x5, CheckBreakDynamite);
	DynamiteHiddenBase_t = new Trampoline((int)DynamiteHiddenBase_Main, (int)DynamiteHiddenBase_Main + 0x5, CheckBreakDynamiteHiddenBase);
	DynamiteSandOcean_t = new Trampoline((int)DynamiteSandOcean_Main, (int)DynamiteSandOcean_Main + 0x6, CheckBreakDynamiteSandOcean);


	MetalBox_t = new Trampoline((int)MetalBox, (int)MetalBox + 0x6, MetalBox_r);
	MetalBoxGravity_t = new Trampoline((int)MetalBoxGravity, (int)MetalBoxGravity + 0x6, MetalBoxGravity_r);

	RoadObjMain_t = new Trampoline((int)0x5B0ED0, (int)0x5B0ED5, RoadObjMain_r);

}