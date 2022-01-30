#include "pch.h"
#include "abilities.h"

Buttons HammerPropButton = Buttons_X;
Buttons HammerAttackButton = Buttons_Y;
Buttons HammerAirButton = Buttons_Y;
Buttons HammerJumpButton = Buttons_B;

float PropellerGravity = 0.011f;
static float PropellerInitialAccTreshold = 1.0f;
static float PropellerInitialAcc = 1.01f;
static float PropellerAirAccTreshold = 7.0f;
static float PropellerAirAcc = 1.005f;
static float DoubleJumpAcc = 1.12f;
float MovingGroundSpinAccel = 0.025f;

bool BlockDoubleJump[MaxPlayers]{};
extern NJS_MATRIX AmyHammerMatrix;

int timerWaveProper = 0;
float hammerScale = 1.0f;

void AmySetHammerScale(CharObj2Base* a1) {

	switch (a1->AnimInfo.Current)
	{
	default:

		if (hammerScale > 1.0f)
			hammerScale -= 0.1f;

		break;
	case HammerAttackAnim:
	case HammerAirAnim:
	case HammerSpinSetAnim:

		if (hammerScale < 1.2f)
			hammerScale += 0.1f;
		break;
	case HammerSpinAnim:

		if (hammerScale < 1.5f)
			hammerScale += 0.1f;
		break;
	}
}

void AmySetAttackColli(SonicCharObj2* sonicCO2, CharObj2Base* a1, EntityData1* data)
{
	CollisionInfo* ColInfo; // eax
	CollisionData* v4; // ebp
	char v5; // cl
	char v6; // al
	Vector3 a2a; // [esp+10h] [ebp-Ch] BYREF
	NJS_VECTOR pos;

	int curAnim = a1->AnimInfo.Current;

	switch (curAnim)
	{
	case HammerAttackAnim:
	case HammerSpinSetAnim:
	case HammerSpinAnim:
	case HammerAirAnim:

		data->Collision->CollisionArray->damage &= 0xFCu;
		data->Collision->CollisionArray->damage |= 0xCu;
		data->Collision->CollisionArray[1].attr &= 0xFFFFFFEF;
		data->Collision->CollisionArray[1].param1 = 10.0;
		data->Status |= Status_Attack;

		//move Amy's collision to her hammer Credits: Kell 

		pos = { -8.0f, -1.0f, 0.0f };

		if (curAnim == HammerSpinAnim || curAnim == HammerSpinSetAnim) {
			pos.y -= 10.0f;
		}
		
		
		njPushMatrix((NJS_MATRIX_PTR)0x1A51A3C); // Right hand matrix
		njCalcPointR(CUR_MATRIX, &pos, &pos, 0);
		njCalcPointR((NJS_MATRIX_PTR)0x1A51A00, &pos, &pos, 0); // Base matrix
		njPopMatrixEx();

		data->Collision->CollisionArray[1].center = pos;

		AmyEffectPutSpdDwnHeart(&pos);
		njPopMatrixEx();

		break;
	default:
		return;
	}
}


void AmyMovingSpin(EntityData1* data, EntityData2* data2, CharObj2Base* co2)
{
	if (!(data->Status & Status_Ground))
	{
		if (HammerPropButton)
		{
			timerWaveProper = 0;
			data->Action = HammerProp;
			return;
		}
	}
}

#pragma region Propeller
void AmyProp_Run(SonicCharObj2* sonicCO2, EntityData1* data, EntityData2* data2, CharObj2Base* co2)
{
	// If an object overrides the player action, stop
	if (Sonic_CheckNextAction(sonicCO2, data, data2, co2))
	{
		FreeAllCustomSounds();
		return;
	}

	// If the player stops holding the button, stop
	if (!(Controllers[co2->PlayerNum].on & HammerPropButton))
	{
		data->Action = Action_Fall;
		co2->AnimInfo.Next = 15;
		FreeAllCustomSounds();
		return;
	}

	// If the player touches the ground, stop
	if (data->Status & STATUS_FLOOR)
	{
		co2->AnimInfo.Next = 0;
		data->Action = Action_Run;
		FreeAllCustomSounds();
		return;
	}

	// Initial acceleration if close to no input speed
	if (co2->Speed.x < PropellerInitialAccTreshold)
	{
		co2->Speed.x *= PropellerInitialAcc;
	}

	// Hammer Air acceleration
	if (co2->Speed.x < PropellerAirAccTreshold)
	{
		co2->Speed.x *= PropellerAirAcc;
	}

	// Fix velocity bug
	if (njScalor(&co2->Speed) == 0)
	{
		co2->Speed.y -= 0.1f;
	}

	timerWaveProper++;

	if ((timerWaveProper & 7) == 0)
	{
		ObjectMaster* wave = LoadObject(2, "AmyWaveEffect", AmyPutHammerWave, LoadObj_Data1);
		if (wave)
		{
			EntityData1* waveData = wave->Data1.Entity;
			waveData->Position = data->Position;
			waveData->Position.y = co2->PhysData.Height * 0.5 + waveData->Position.y;
			waveData->Rotation = data->Rotation;
			waveData->field_6 = 4;
			waveData->Index = 0;
		}
	}

	// Hammer Spin Animation
	co2->AnimInfo.Next = HammerSpinAnim;
	// Attack status
	data->Status |= Status_Attack;
}

signed int AmyProp_Check(EntityData1* data, CharObj2Base* co2)
{
	if (CheckControl(co2->PlayerNum) && Controllers[co2->PlayerNum].press & HammerPropButton
		&& !(data->Status & STATUS_FLOOR) && co2->HeldObject == nullptr) 
	{
		timerWaveProper = 0;
		data->Action = HammerProp;

		if (data->Rotation.x || data->Rotation.z)
		{
			PConvertVector_P2G(data, &co2->Speed);
		}

		data->Rotation.x = GravityAngle_X;
		data->Rotation.z = GravityAngle_Z;

		//PlaySound(1279, 0, 0, 0);
		if (co2->Speed.x > 5)
		{
			PlayVoice(2, 2677);
		}

		return 1;
	}

	return 0;
}
#pragma endregion


void Amy_AbilitiesConfig(const IniFile* config, const IniFile* physics) {

	HammerJumpButton = (Buttons)config->getInt("Abilities", "HammerJumpButton", HammerJumpButton);
	HammerAttackButton = (Buttons)config->getInt("Abilities", "HammerAttackButton", HammerAttackButton);
	HammerAirButton = (Buttons)config->getInt("Abilities", "HammerAirButton", HammerAirButton);
	HammerPropButton = (Buttons)config->getInt("Abilities", "HammerPropButton", HammerPropButton);

	auto physgrp = physics->getGroup("Amy");

	if (physgrp)
	{
		PropellerGravity = physgrp->getFloat("PropellerGravity", PropellerGravity);
		PropellerInitialAccTreshold = physgrp->getFloat("PropellerInitialAccTreshold", PropellerInitialAccTreshold);
		PropellerInitialAcc = physgrp->getFloat("PropellerInitialAcc", PropellerInitialAcc);
		PropellerAirAccTreshold = physgrp->getFloat("PropellerAirAccTreshold", PropellerAirAccTreshold);
		PropellerAirAcc = physgrp->getFloat("PropellerAirAcc", PropellerAirAcc);
		DoubleJumpAcc = physgrp->getFloat("DoubleJumpAcc", DoubleJumpAcc);
		MovingGroundSpinAccel = physgrp->getFloat("MovingGroundSpinAccel", MovingGroundSpinAccel);
	}
}