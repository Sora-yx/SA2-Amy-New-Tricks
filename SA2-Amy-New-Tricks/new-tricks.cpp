#include "pch.h"
#include "abilities.h"


static Buttons HammerPropButton = Buttons_X;

static float PropellerGravity = 0.011f;
static float PropellerInitialAccTreshold = 1.0f;
static float PropellerInitialAcc = 1.01f;
static float PropellerAirAccTreshold = 7.0f;
static float PropellerAirAcc = 1.005f;
static float DoubleJumpAcc = 1.12f;
static float MovingGroundSpinAccel = 0.025f;

bool BlockDoubleJump[MaxPlayers]{};
extern NJS_MATRIX AmyHammerMatrix;

void AmySetAttackColli(CharObj2Base* a1, EntityData1* data)
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

		data->Collision->CollisionArray->damage &= 0xFCu;
		data->Collision->CollisionArray->damage |= 0xCu;
		data->Collision->CollisionArray[1].attr &= 0xFFFFFFEF;
		data->Collision->CollisionArray[1].param1 = 9.0;

		njPushMatrixEx();

		njRotateZ_(CUR_MATRIX, (unsigned __int16)data->Rotation.z);
		njRotateX_(CUR_MATRIX, (unsigned __int16)data->Rotation.x);
		njRotateY_(CUR_MATRIX, (unsigned __int16)(0x8000 - data->Rotation.y));

		memcpy(_nj_current_matrix_ptr_, AmyHammerMatrix, 0x30u);
		a2a.x = 0;
		a2a.y = 0;
		a2a.z = 0;
		njTranslateV(CUR_MATRIX, &a3);
	
		data->Collision->CollisionArray[1].center = a3;
		AmyEffectPutSpdDwnHeart(&a3);
		njPopMatrixEx();

		break;
	default:
		return;
	}
}

static void AmyDoubleJump(EntityData1* data, CharObj2Base* co2)
{
	char pnum = co2->PlayerNum;

	if (EnableDoubleJump == true && CheckControl(co2->PlayerNum) && Controllers[pnum].press & Buttons_A && BlockDoubleJump[pnum] == false)
	{
		BlockDoubleJump[co2->PlayerNum] = true;
		co2->Speed.y = DoubleJumpAcc;
		//PlaySound(1286, 0, 0, 0);
		co2->AnimInfo.Next = 74;
	}
}

static void AmyMovingSpin(EntityData1* data, EntityData2* data2, CharObj2Base* co2)
{
	if (!(data->Status & Status_Ground))
	{
		if (HammerPropButton)
		{
			data->Action = HammerProp;
			return;
		}
	}

	auto RestoreSpeed = co2->PhysData.RunAccel;
	co2->PhysData.RunAccel = MovingGroundSpinAccel;

	if (co2->AnimInfo.Current == HammerSpinAnim)
	{
		PGetRotation(data, data2, co2);
		PResetAngle(data, co2);
		PGetAcceleration(data, data2, co2);
		PGetSpeed(data, co2, data2);
		PSetPosition(data, data2, co2);
		PResetPosition(data, data2, co2);
	}

	co2->PhysData.RunAccel = RestoreSpeed;

	if (!(data->Status & Status_Ground))
	{
		if (HammerPropButton)
		{
			data->Action = HammerProp;
		}
	}
}

#pragma region Propeller
static void AmyProp_Run(SonicCharObj2* sonicCO2, EntityData1* data, EntityData2* data2, CharObj2Base* co2)
{
	// If an object overrides the player action, stop
	if (Sonic_CheckNextAction(sonicCO2, data, data2, co2))
	{
		//co2->TailsFlightTime = 0.0f;
		return;
	}

	// If the player stops holding the button, stop
	if (!(Controllers[co2->PlayerNum].on & HammerPropButton))
	{
		data->Action = Action_Fall;
		co2->AnimInfo.Next = 15;
		//co2->TailsFlightTime = 0.0f;
		return;
	}

	// If the player touches the ground, stop
	if (data->Status & STATUS_FLOOR)
	{
		//PlaySound(33, 0, 0, 0);
		co2->AnimInfo.Next = 0;
		data->Action = Action_Run;
		//co2->TailsFlightTime = 0.0f;
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

	// Handle physics
	auto RestoreGravity = co2->PhysData.Weight;
	co2->PhysData.Weight = PropellerGravity;

	PGetRotation(data, data2, co2);
	PResetAngle(data, co2);
	PGetAcceleration(data, data2, co2);
	PGetSpeed(data, co2, data2);
	PSetPosition(data, data2, co2);
	PResetPosition(data, data2, co2);

	co2->PhysData.Weight = RestoreGravity;

	// Hammer Spin Animation
	co2->AnimInfo.Next = HammerSpinAnim;

	// Attack status
	data->Status |= Status_Attack;
}

static inline void AmyProp_Check(EntityData1* data, CharObj2Base* co2)
{
	if (CheckControl(co2->PlayerNum) && Controllers[co2->PlayerNum].press & HammerPropButton
		&& !(data->Status & STATUS_FLOOR) && co2->HeldObject == nullptr) //jump time?
	{
		data->Action = HammerProp;

		if (data->Rotation.x || data->Rotation.z)
		{
			PConvertVector_P2G(data, &co2->Speed);
		}

		data->Rotation.x = GravityAngle_X;
		data->Rotation.z = GravityAngle_Z;

		//PlaySound(1279, 0, 0, 0);
	}
}
#pragma endregion


void Amy_AbilitiesConfig(const IniFile* config, const IniFile* physics) {

	hammerJump = config->getBool("Abilities", "hammerJump", true);
	EnableDoubleJump = config->getBool("Abilities", "EnableDoubleJump", true);
	MovingGroundSpin = config->getBool("Abilities", "EnableMovingSpin", true);
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