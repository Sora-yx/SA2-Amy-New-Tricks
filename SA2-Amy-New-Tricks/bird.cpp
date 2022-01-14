#include "pch.h"


ModelInfo* Bird = nullptr;
AnimationFile* BirdMotion = nullptr;

NJS_TEXNAME Amy_texname[64];
NJS_TEXLIST Amy_texlist = { arrayptrandlengthT(Amy_texname, Uint32) };

NJS_TEXNAME WING_P_texname[7];
NJS_TEXLIST WING_P_texlist = { arrayptrandlengthT(WING_P_texname, Uint32) };

int randFlicky = 0;



void Bird_Delete(ObjectMaster* obj) {
	FreeMDL(Bird);
	FreeAnim(BirdMotion);
}

void __cdecl Bird_Display(ObjectMaster* obj) {
	EntityData1* data = obj->Data1.Entity;

	if (bird == flicky || randFlicky == flicky) {
		njSetTexture(&Amy_texlist);
	}

	if (bird == gammaFlicky || randFlicky == gammaFlicky) {
		njSetTexture(&WING_P_texlist);
	}

	njPushMatrixEx();
	njTranslateV(0, &data->Position);
	njRotateY(0, data->Rotation.y);
	njCnkMotion(Bird->getmodel(), BirdMotion->getmotion(), FrameCountIngame % BirdMotion->getmotion()->nbFrame);
	njPopMatrixEx();
}

void __cdecl AmyBird_Main(ObjectMaster* obj) {
	EntityData1* data = obj->Data1.Entity;
	EntityData1* player = obj->Parent->Data1.Entity;
	CharObj2Base* co2 = obj->Parent->Data2.Character;

	if (!player) {
		DeleteObject_(obj);
		return;
	}

	float posx = data->Position.x;
	float posy = data->Position.y;
	float posz = data->Position.z;

	data->Rotation.y = BAMS_SubWrap(data->Rotation.y, 0x8000 - player->Rotation.y, 1024);
	data->Rotation.x += 16;
	data->Rotation.z += 1024;

	NJS_VECTOR dest;

	dest.x = njCos(data->Rotation.x) * 3.0f + player->Position.x;
	dest.y = njSin(data->Rotation.z) + 12.0f + player->Position.y;
	dest.z = njSin(data->Rotation.x) * 3.0f + player->Position.z;

	float distance = sqrtf(powf(dest.x - data->Position.x, 2) + powf(dest.y - data->Position.y, 2) + powf(dest.z - data->Position.z, 2));

	if (distance >= 200.0f) {
		data->Position.x = dest.x;
		data->Position.y = dest.y;
		data->Position.z = dest.z;
	}
	else
	{
		data->Position.x = (dest.x - data->Position.x) * 0.25f + data->Position.x;
		data->Position.y = (dest.y - data->Position.y) * 0.25f + data->Position.y;
		data->Position.z = (dest.z - data->Position.z) * 0.25f + data->Position.z;
	}
}

void __cdecl Load_AmyBird(ObjectMaster* obj) {
	EntityData1* data = obj->Data1.Entity;
	EntityData1* player = obj->Parent->Data1.Entity;

	if (!player) {
		DeleteObject_(obj);
		return;
	}

	data->Position.x = player->Position.x;
	data->Position.y = player->Position.y;
	data->Position.z = player->Position.z;
	data->Position.y = data->Position.y + 12.0;
	data->Rotation.y = 0x8000 - player->Rotation.y;

	obj->DisplaySub = Bird_Display;
	obj->MainSub = AmyBird_Main;
	obj->DeleteSub = Bird_Delete;
}

void __cdecl AmyHook(ObjectMaster* obj) {
	obj->MainSub = Sonic_Main;

	if (TwoPlayerMode) {
		return;
	}

	if (bird == randomFlicky) {
		randFlicky = rand() % 2 + 1;
	}

	if (bird == flicky || randFlicky == flicky) {
		Bird = LoadMDL("bird", ModelFormat_Chunk);
		BirdMotion = LoadAnim("wing_act.nam");
		LoadTextureList("AMYTEX", &Amy_texlist);
	}

	if (bird == gammaFlicky || randFlicky == gammaFlicky) {
		Bird = LoadMDL("pinkBird", ModelFormat_Chunk);
		BirdMotion = LoadAnim("w_p0001_wing_p.nam");
		LoadTextureList("WING_P", &WING_P_texlist);
	}

	LoadChildObject(LoadObj_Data1, Load_AmyBird, obj);
}

void Init_Bird() {
	WriteData((ObjectFuncPtr*)0x7170ED, AmyHook); //When loading Amy, attach the birdie as a child object.
	return;
}