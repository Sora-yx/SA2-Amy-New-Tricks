#include "pch.h"
#include "abilities.h"

Buttons HammerPropButton = Buttons_X;
Buttons HammerAttackButton = (Buttons)0;
Buttons HammerAirButton = Buttons_B;
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

void AmySetAttackColli(SonicCharObj2* sonicCO2, CharObj2Base* a1, EntityData1* data)
{
	CollisionInfo* ColInfo; // eax
	CollisionData* v4; // ebp
	char v5; // cl
	char v6; // al
	Vector3 a3; // [esp+4h] [ebp-18h] BYREF
	Vector3 a2a; // [esp+10h] [ebp-Ch] BYREF

	switch (a1->AnimInfo.Current)
	{
	case HammerAttackAnim:
	case HammerSpinAnim:
	case HammerAirAnim:

		ColInfo = data->Collision;
		if (ColInfo)
		{
			v4 = ColInfo->CollisionArray;
			v4[1].attr |= 0x10u;
			data->Status &= 0xFBu;
			if (a1->Powerups >= 0)
			{
				v5 = 0;
				v6 = 0;
			}
			else
			{
				v5 = 3;
				v6 = 3;
			}
			v4->damage = v5 & 3 | v4->damage & 0xF0 | (4 * (v6 & 3));
		}

		njPushMatrixEx();
		memcpy(CUR_MATRIX, &AmyRightHandMatrix, 0x30u);
		njGetTranslation(CUR_MATRIX, &a2a);
		njTranslate(0, -0.5f, 0.1f, 0.0f);

		a3.x = sonicCO2->righthand_pos.x + sonicCO2->righthand_vec0.x * a2a.x;
		a3.y = sonicCO2->righthand_pos.y + sonicCO2->righthand_vec0.y * a2a.y;
		a3.z = sonicCO2->righthand_pos.z + sonicCO2->righthand_vec0.z * a2a.z;	

		data->Collision->CollisionArray->damage &= 0xFCu;
		data->Collision->CollisionArray->damage |= 0xCu;
		data->Collision->CollisionArray[1].attr &= 0xFFFFFFEF;
		data->Collision->CollisionArray[1].param1 = 9.0;

		njRotateZ_(CUR_MATRIX, (unsigned __int16)data->Rotation.z);
		njRotateX_(CUR_MATRIX, (unsigned __int16)data->Rotation.x);
		njRotateY_(CUR_MATRIX, (unsigned __int16)(0x8000 - data->Rotation.y));

		data->Status |= Status_Attack;
		data->Collision->CollisionArray[1].center = a3;
		AmyEffectPutSpdDwnHeart(&a3);
		Sonic_Afterimage(data, a1, sonicCO2);
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
		&& !(data->Status & STATUS_FLOOR) && co2->HeldObject == nullptr) //jump time?
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