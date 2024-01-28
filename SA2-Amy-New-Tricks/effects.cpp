#include "pch.h"

NJS_TEXANIM anim_amy_heart = { 4, 4, 2, 2, 0, 0, 0xFF, 0xFF, 1, 0 };
NJS_SPRITE sprite_amy_heart = { {0}, 1.0, 1.0, 0, &AmyEff_TEXLIST, &anim_amy_heart };

//used when you wait spin
void __cdecl AmyEffectSpdDwnHeartDisplayer(ObjectMaster* a1)
{	 

	auto data = a1->Data1.Entity;
	auto data2 = a1->Data2.Entity;
	char pnum = data2->SpeedAngle.x;

	njSetTexture(&AmyEff_TEXLIST);
	float r = 0.25f - (data2->Velocity.x - 0.5f) * (data2->Velocity.x - 0.5f) + 0.25f
		- (data2->Velocity.x - 0.5f) * (data2->Velocity.x - 0.5f);

	SetMaterial(r, 1.0f, 1.0f, 1.0f);

	njPushMatrix(0);
	njTranslateV(0, &data->Position);
	njDrawSprite3D(0, &sprite_amy_heart, NJD_SPRITE_ALPHA | NJD_SPRITE_SCALE | NJD_SPRITE_COLOR);
	njPopMatrix(1u);
	ResetMaterial();
}

void __cdecl AmyEffectSpdDwnHeart(ObjectMaster* a1)
{
	const float v4 = 0.89999998f;
	auto data = a1->Data1.Entity;
	auto data2 = a1->Data2.Entity;

	float velRes = data2->Velocity.x + 0.06666667f;

	data2->Velocity.x = velRes;
	if (velRes < 1.0f)
	{
		data->Scale.x *= v4;
		data->Scale.y *= v4;
		data->Scale.z *= v4;
		data->Position.x += v4;
		data->Position.y += v4;
		data->Position.z += v4;
	}
	else
	{
		DeleteObject_(a1);
	}
}

void __cdecl AmyEffectPutSpdDwnHeart(NJS_POINT3* pos, char pnum)
{
	float v2 = 0.0f;
	long float v3 = 0.0f;
	float v5 = 0.0f;
	NJS_VECTOR result = { 0 };

	auto HeartObj = LoadObject(2, "AmyEffectSpdHeart", AmyEffectSpdDwnHeart, 10);
	if (HeartObj)
	{
		result.x = (float)rand() * 0.000030517578f - 0.5f;
		result.y = (float)rand() * 0.000030517578f - 0.5f;
		result.z = (float)rand() * 0.000030517578f - 0.5f;
		if (njScalor(&result) == 0.0f)
		{
			v2 = 1.0f;
			result.z = 0.0f;
			result.y = 0.0f;
		}
		else
		{
			v3 = sqrt(result.z * result.z + result.y * result.y + result.x * result.x);
			if (v3 == 0.0f)
			{
				result.z = 0.0f;
				v2 = 0.0f;
				result.y = 0.0f;
			}
			else
			{
				v5 = 1.0f / v3;
				v2 = v5 * result.x;
				result.y = v5 * result.y;
				result.z = v5 * result.z;
			}
		}

		auto data = HeartObj->Data1.Entity;
		data->Scale.x = v2 * 0.25f;
		data->Scale.y = result.y * 0.25f;
		data->Scale.z = result.z * 0.25f;
		HeartObj->Data2.Entity->SpeedAngle.x = pnum;
		HeartObj->DisplaySub_Delayed2 = AmyEffectSpdDwnHeartDisplayer;
		data->Position = *pos;
	}
}

extern ModelInfo* WaveMdl;
void __cdecl HammerWaveColorDisplayer(ObjectMaster* a1)
{	
	if (!WaveMdl)
		return;

	auto data = a1->Data1.Entity;

	njSetTexture(&AmyEff_TEXLIST);
	ResetMaterial();
	njPushMatrix(0);
	njTranslateV(0, &data->Position);
	Angle rotZ = data->Rotation.z;
	njRotateZ_(0, (unsigned __int16)rotZ);	
	Angle rotX = data->Rotation.x;
	njRotateX_(0, (unsigned __int16)rotX);	
	njScaleV_(&data->Scale);
	DrawObject(WaveMdl->getmodel());
	njPopMatrix(1u);
}

void AmyEffectHammerWaveColor(ObjectMaster* tp)
{

	float calcWave = 0.0f;
	float result = 0.0f;

	auto data = tp->Data1.Entity;
	auto data2 = tp->Data2.Entity;

	if (!data->Action)
	{
		data->Scale.x = 3.0f;
		data->Action = 1;
		data2->Velocity.x = 1.0f;
		tp->DisplaySub_Delayed2 = HammerWaveColorDisplayer;
	}
	calcWave = (data2->Velocity.x - 0.033333335f);
	data2->Velocity.x = data2->Velocity.x - 0.033333335f;
	if (calcWave >= 0.0f)
	{
		data->Scale.x += 0.5f;
		result = njSin((int)(float)((float)calcWave * 32768.0f));
		data->Scale.y = result * 3.0f;
		data->Scale.z = data->Scale.x;
	}
	else
	{
		DeleteObject_(tp);
	}
}

void __cdecl AmyEffectFallHeartDisplayer(ObjectMaster* a1)
{
	auto data = a1->Data1.Entity;
	auto data2 = a1->Data2.Entity;

	njSetTexture(&AmyEff_TEXLIST);
	float r = 1.0f - data2->Velocity.x;
	SetMaterial(r, 1.0f, 1.0f, 1.0f);

	njPushMatrix(0);
	njTranslateV(0, &data->Position);

	njRotateY_(0, (unsigned __int16)data->Rotation.y);
	njRotateZ_(0, (unsigned __int16)data->Rotation.z);
	
	njDrawSprite3D(0, &sprite_amy_heart, NJD_SPRITE_ALPHA | NJD_SPRITE_COLOR);
	njPopMatrix(1u);
	ResetMaterial();
}

void __cdecl AmyEffectFallHeart(ObjectMaster* a1)
{
	float sclY = 0.0f;
	Angle rotZ = 0;
	Angle rotX = 0;
	Angle result = 0;

	auto data = a1->Data1.Entity;
	auto data2 = a1->Data2.Entity;

	float resultEff = data2->Velocity.x + 0.033333335f;
	data2->Velocity.x = resultEff;

	if (resultEff < 1.0)
	{
		sclY = data->Scale.y - 0.050000001f;
		data->Rotation.y += 1024;
		rotZ = data->Rotation.z;
		data->Scale.y = sclY;
		rotX = data->Rotation.x;
		data->Position.x += data->Scale.x;
		data->Position.y += sclY;
		data->Position.z += data->Scale.z;

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
	auto data = obj->Data1.Entity;

	if (--data->Timer)
	{
		unsigned __int8 index = data->Index;
		if (index < 2u)
		{
			ObjectMaster* wave = LoadObject(2, "AmyEffectHammerWave", (void(__cdecl*)(ObjectMaster*))AmyEffectHammerWaveColor, 10);
			if (wave)
			{
				wave->Data1.Entity->Position = data->Position;
				wave->Data1.Entity->Rotation.x = data->Rotation.x;
				wave->Data1.Entity->Rotation.z = data->Rotation.z;
			}
		}
	}
	else
	{
		DeleteObject_(obj);
	}
}

void __cdecl AmyEffectPutFallHeart(NJS_VECTOR heartPos, int rotX, int rotZ)
{

	NJS_VECTOR result = { 0 };
	NJS_VECTOR pos = { 0 };

	njPushMatrix(_nj_unit_matrix_);
	njRotateZ_(0, (unsigned __int16)rotZ);
	njRotateX_(0, (unsigned __int16)rotX);
	
	pos.x = 0.43579999f;
	pos.y = 0.89999998f;
	pos.z = 0.0f;

	for (int i = 0; i < 0x10000; i += 0x2000)
	{
		njRotateY(0, 0x2000);
		njCalcPoint(CUR_MATRIX, &result, &pos, 0);
		ObjectMaster* obj = LoadObject(2, "AmyEffectFallHeart", AmyEffectFallHeart, 10);
		if (obj)
		{
			EntityData1* data = obj->Data1.Entity;
			data->Position = heartPos;
			data->Position.x = result.x * 5.0f + data->Position.x;
			data->Position.y = result.y * 5.0f + data->Position.y;
			data->Position.z = result.z * 5.0f + data->Position.z;
			data->Scale = result;
			data->Rotation.y = i;
			data->Rotation.z = 28672;
			obj->DisplaySub_Delayed1 = AmyEffectFallHeartDisplayer;
		}
	}
	njPopMatrix(1u);
}

void Do_HammerWaveHeartEffect(EntityData1* data1) {

	NJS_VECTOR vs = { 0 };
	NJS_VECTOR result = { 0 };

	auto hammerWave = LoadObject(2, "AmyHammerWave", AmyPutHammerWave, 10);

	if (hammerWave)
	{
		vs.x = 15.0f;
		vs.z = 0.0f;
		vs.y = 0.0f;
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
		hammerWave->Data1.Entity->Timer = 10;
		hammerWave->Data1.Entity->Index = 1;
		AmyEffectPutFallHeart(hammerWave->Data1.Entity->Position, data1->Rotation.x, data1->Rotation.z);
	}

	PlayDelayedCustomSound(SE_HammerAttack, 2, 0.3f);
	VibeThing(0, 15, 0, 6);
}
