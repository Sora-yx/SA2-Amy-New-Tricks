#include "pch.h"
#include "abilities.h"

NJS_TEXANIM anim_amy_heart = { 4, 4, 2, 2, 0, 0, 0xFF, 0xFF, 1, 0 };
NJS_SPRITE sprite_amy_heart = { {0}, 1.0, 1.0, 0, &AmyEff_TEXLIST, &anim_amy_heart };

signed int AmyCheckHammerAttack(EntityData1* data1, CharObj2Base* co2) {

	if ((Controllers[co2->PlayerNum].press & Buttons_Y) == 0 || co2->CharID2 != Characters_Amy || !hammerJump)
	{
		return 0;
	}

	co2->Speed.x = 0.0;
	co2->Speed.z = 0.0;

	data1->Action = HammerAttack;
	co2->AnimInfo.Next = HammerAttackAnim;
	PlayCustomSoundVolume(Voice_AmyHammerAttack, 3);

	if ((data1->Status & 0x20) != 0)
	{
		co2->DynColInfo->anaspdv.y = 0.0;
	}

	return 1;
}

void __cdecl AmyEffectSpdDwnHeartDisplayer(ObjectMaster* a1)
{
	EntityData1* data; // esi
	float r; // [esp+0h] [ebp-14h]

	data = a1->Data1.Entity;
	EntityData2* data2 = a1->Data2.Entity;

	njSetTexture(&AmyEff_TEXLIST);
	r = 0.25
		- (data2->Velocity.x - 0.5) * (data2->Velocity.x - 0.5)
		+ 0.25
		- (data2->Velocity.x - 0.5) * (data2->Velocity.x - 0.5);
	SetMaterial(r, 1.0, 1.0, 1.0);

	njPushMatrix(0);
	njTranslateV(0, &data->Position);
	NJDrawSprite3D(0, &sprite_amy_heart, NJD_SPRITE_ALPHA | NJD_SPRITE_SCALE | NJD_SPRITE_COLOR);
	njPopMatrix(1u);
	ResetMaterial();
}

void __cdecl AmyEffectSpdDwnHeart(ObjectMaster* a1)
{
	EntityData1* data; // ecx
	double v3; // st7
	double v4; // st7
	double v5; // st6
	float v6; // [esp+4h] [ebp+4h]

	data = a1->Data1.Entity;
	EntityData2* data2 = a1->Data2.Entity;

	v3 = data2->Velocity.x + 0.06666667;
	data2->Velocity.x = v3;
	if (v3 < 1.0)
	{
		v4 = data->Scale.x * 0.89999998;
		data->Scale.x = v4;
		v5 = data->Scale.y * 0.89999998;
		data->Scale.y = v5;
		v6 = data->Scale.z * 0.89999998;
		data->Scale.z = v6;
		data->Position.x = v4 + data->Position.x;
		data->Position.y = v5 + data->Position.y;
		data->Position.z = v6 + data->Position.z;
	}
	else
	{
		DeleteObject_(a1);
	}
}

void __cdecl AmyEffectPutSpdDwnHeart(NJS_POINT3* pos)
{
	ObjectMaster* HeartObj; // esi
	double v2; // st7
	long double v3; // st7
	float v5; // [esp+4h] [ebp-10h]
	Vector3 a1; // [esp+8h] [ebp-Ch] BYREF

	HeartObj = LoadObject(2, "AmyEffectSpdHeart", AmyEffectSpdDwnHeart, 10);
	if (HeartObj)
	{
		a1.x = (double)rand() * 0.000030517578 - 0.5;
		a1.y = (double)rand() * 0.000030517578 - 0.5;
		a1.z = (double)rand() * 0.000030517578 - 0.5;
		if (njScalor(&a1) == 0.0)
		{
			v2 = 1.0;
			a1.z = 0.0;
			a1.y = 0.0;
		}
		else
		{
			v3 = sqrt(a1.z * a1.z + a1.y * a1.y + a1.x * a1.x);
			if (v3 == 0.0)
			{
				a1.z = 0.0;
				v2 = 0.0;
				a1.y = 0.0;
			}
			else
			{
				v5 = 1.0 / v3;
				v2 = v5 * a1.x;
				a1.y = v5 * a1.y;
				a1.z = v5 * a1.z;
			}
		}

		EntityData1* data = HeartObj->Data1.Entity;
		data->Scale.x = v2 * 0.25;
		data->Scale.y = a1.y * 0.25;
		data->Scale.z = a1.z * 0.25;
		HeartObj->DisplaySub_Delayed2 = AmyEffectSpdDwnHeartDisplayer;
		data->Position = *pos;
	}
}



extern ModelInfo* WaveMdl;
void __cdecl HammerWaveColorDisplayer(ObjectMaster* a1)
{
	EntityData1* data; // esi
	Angle v2; // eax
	Angle v3; // eax

	data = a1->Data1.Entity;

	njSetTexture(&AmyEff_TEXLIST);
	ResetMaterial();
	njPushMatrix(0);
	njTranslateV(0, &data->Position);
	v2 = data->Rotation.z;
	if (v2)
	{
		njRotateZ(0, (unsigned __int16)v2);
	}
	v3 = data->Rotation.x;
	if (v3)
	{
		njRotateX(0, (unsigned __int16)v3);
	}
	njScaleV_(&data->Scale);
	DrawObject(WaveMdl->getmodel());
	njPopMatrix(1u);
}

void AmyEffectHammerWaveColor(ObjectMaster* tp)
{

	double v3; // fp0
	double v4; // fp1
	double v5; // fp8

	EntityData1* data = tp->Data1.Entity;
	EntityData2* data2 = tp->Data2.Entity;

	if (!data->Action)
	{
		data->Scale.x = 3.0;
		data->Action = 1;
		data2->Velocity.x = 1.0;
		tp->DisplaySub_Delayed2 = HammerWaveColorDisplayer;
	}
	v3 = (float)(data2->Velocity.x - (float)0.033333335);
	data2->Velocity.x = data2->Velocity.x - (float)0.033333335;
	if (v3 >= 0.0)
	{
		data->Scale.x = data->Scale.x + (float)0.5;
		v4 = njSin((int)(float)((float)v3 * (float)32768.0));
		v5 = data->Scale.x;
		data->Scale.y = (float)v4 * (float)3.0;
		data->Scale.z = v5;
	}
	else
	{
		DeleteObject_(tp);
	}
}


void __cdecl AmyEffectFallHeartDisplayer(ObjectMaster* a1)
{

	Angle rotY;
	Angle rotZ;
	EntityData1* data = a1->Data1.Entity;
	EntityData2* data2 = a1->Data2.Entity;

	njSetTexture(&AmyEff_TEXLIST);
	float r = 1.0 - data2->Velocity.x;
	SetMaterial(r, 1.0, 1.0, 1.0);

	njPushMatrix(0);
	njTranslateV(0, &data->Position);
	rotY = data->Rotation.y;
	if (rotY)
	{
		njRotateY(0, (unsigned __int16)rotY);
	}
	rotZ = data->Rotation.z;
	if (rotZ)
	{
		njRotateZ(0, (unsigned __int16)rotZ);
	}
	NJDrawSprite3D(0, &sprite_amy_heart, NJD_SPRITE_ALPHA | NJD_SPRITE_COLOR);
	njPopMatrix(1u);
	ResetMaterial();
}

void __cdecl AmyEffectFallHeart(ObjectMaster* a1)
{


	double sclY; // st7
	Angle rotZ; // eax
	Angle rotX; // esi
	Angle result; // esi

	EntityData1* data = a1->Data1.Entity;
	EntityData2* data2 = a1->Data2.Entity;

	double v2 = data2->Velocity.x + 0.033333335;
	data2->Velocity.x = v2;

	if (v2 < 1.0)
	{
		sclY = data->Scale.y - 0.050000001;
		data->Rotation.y += 1024;
		rotZ = data->Rotation.z;
		data->Scale.y = sclY;
		rotX = data->Rotation.x;
		data->Position.x = data->Scale.x + data->Position.x;
		data->Position.y = sclY + data->Position.y;
		data->Position.z = data->Scale.z + data->Position.z;
		if ((unsigned __int16)rotZ <= 0x8000u)
		{
			result = rotX + 512;
		}
		else
		{
			result = rotX - 256;
		}
		data->Rotation.x = result;
		data->Rotation.z = rotZ + result;
	}
	else
	{
		DeleteObject_(a1);
	}
}


void __cdecl AmyPutHammerWave(ObjectMaster* obj)
{
	EntityData1* data; // esi
	unsigned __int8 index; // al
	ObjectMaster* v3; // eax
	int address[2]; // [esp+4h] [ebp-8h]

	data = obj->Data1.Entity;
	if (--data->field_6)
	{
		index = data->Index;
		//address[0] = (int)AmyEffectHammerWave;
		//address[1] = (int)AmyEffectHammerWaveColor;
		if (index < 2u)
		{
			v3 = LoadObject(2, "AmyEffectHammerWave", (void(__cdecl*)(ObjectMaster*))AmyEffectHammerWaveColor, 10);
			if (v3)
			{
				v3->Data1.Entity->Position = data->Position;
				v3->Data1.Entity->Rotation.x = data->Rotation.x;
				v3->Data1.Entity->Rotation.z = data->Rotation.z;
			}
		}
	}
	else
	{
		DeleteObject_(obj);
	}
}

void __cdecl AmyEffectPutFallHeart(NJS_VECTOR a1, int rotX, int rotZ)
{
	int i; // esi
	ObjectMaster* obj; // eax
	EntityData1* data; // ecx
	Vector3 a3; // [esp+0h] [ebp-18h] BYREF
	Vector3 pos; // [esp+Ch] [ebp-Ch] BYREF

	njPushMatrix(_nj_unit_matrix_);
	if (rotZ)
	{
		njRotateZ(0, (unsigned __int16)rotZ);
	}
	if (rotX)
	{
		njRotateX(0, (unsigned __int16)rotX);
	}
	pos.x = 0.43579999;
	pos.y = 0.89999998;
	pos.z = 0.0;
	for (i = 0; i < 0x10000; i += 0x2000)
	{
		njRotateY(0, 0x2000);
		njCalcPointR(CUR_MATRIX, &a3, &pos, 0);
		obj = LoadObject(2, "AmyEffectFallHeart", AmyEffectFallHeart, 10);
		if (obj)
		{
			data = obj->Data1.Entity;
			data->Position.x = a1.x;
			data->Position.y = a1.y;
			data->Position.z = a1.z;
			data->Position.x = a3.x * 5.0 + data->Position.x;
			data->Position.y = a3.y * 5.0 + data->Position.y;
			data->Position.z = a3.z * 5.0 + data->Position.z;
			data->Scale = a3;
			data->Rotation.y = i;
			data->Rotation.z = 28672;
			obj->DisplaySub_Delayed1 = AmyEffectFallHeartDisplayer;
		}
	}
	njPopMatrix(1u);
}

void Do_HammerWaveHeartEffect(EntityData1* data1) {


	NJS_VECTOR vs;
	NJS_VECTOR result;

	ObjectMaster* hammerWave = LoadObject(2, "AmyHammerWave", AmyPutHammerWave, 10);

	if (hammerWave)
	{
		vs.x = 15.0;
		vs.z = 0.0;
		vs.y = 0.0;
		njPushMatrix(_nj_unit_matrix_);
		njRotateZ_(CUR_MATRIX, data1->Rotation.z);
		njRotateX_(CUR_MATRIX, data1->Rotation.x);
		njRotateY_(CUR_MATRIX, (unsigned __int16)-(data1->Rotation.y));

		njCalcVector_(&vs, &result, CUR_MATRIX);

		njPopMatrix(1u);
		hammerWave->Data1.Entity->Position.x = result.x + data1->Position.x;
		hammerWave->Data1.Entity->Position.y = result.y + data1->Position.y;
		hammerWave->Data1.Entity->Position.z = result.z + data1->Position.z;
		hammerWave->Data1.Entity->Rotation.x = data1->Rotation.x;
		hammerWave->Data1.Entity->Rotation.z = data1->Rotation.z;
		hammerWave->Data1.Entity->field_6 = 10;
		hammerWave->Data1.Entity->Index = 1;
		AmyEffectPutFallHeart(hammerWave->Data1.Entity->Position, data1->Rotation.x, data1->Rotation.z);
	}

	//PlaySound(796, 0, 0, 0);
	PlayDelayedCustomSound(SE_HammerAttack, 2, 0);
	VibeThing(0, 15, 0, 6);
}

void DoAmyHammerAttack(SonicCharObj2* sonicCO2, EntityData1* data1, CharObj2Base* co2, EntityData2* data2) {

	if (Sonic_CheckNextAction(sonicCO2, data1, data2, co2) || ((data1->Status & STATUS_FLOOR) == 0)) {
		return;
	}

	short curAnimHammer = co2->AnimInfo.Current;

	if (curAnimHammer == HammerAttackAnim && co2->AnimInfo.field_10 >= 3.0 && co2->AnimInfo.field_10 < 4.0)
	{
		Do_HammerWaveHeartEffect(data1);
	}

	if (co2->AnimInfo.Current != 0) {

		if (co2->AnimInfo.Current == HammerSpinSetAnim) {

			if (AmySpinAttack_Check(co2, data1)) 
				return;
			else {
				data1->Action = Action_None;
				co2->AnimInfo.Next = 0;
				return;
			}
		}
		else {

			if (AmyCheckHammerAttack(data1, co2))
			{
				return;
			}
		}
	}
	else {
		data1->Action = Action_None;
		co2->AnimInfo.Next = 0;
	}

	return;
}