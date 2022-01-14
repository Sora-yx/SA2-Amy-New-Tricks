#include "pch.h"
#include "abilities.h"

static bool EnableDoubleJump = true;
static bool MovingGroundSpin = true;
static Buttons HammerPropButton = Buttons_X;

static float PropellerGravity = 0.011f;
static float PropellerInitialAccTreshold = 1.0f;
static float PropellerInitialAcc = 1.01f;
static float PropellerAirAccTreshold = 7.0f;
static float PropellerAirAcc = 1.005f;
static float DoubleJumpAcc = 1.12f;
static float MovingGroundSpinAccel = 0.025f;

static bool BlockDoubleJump[MaxPlayers]{};

static Trampoline* Amy_Exec_t = nullptr;
static Trampoline* Amy_runsActions_t = nullptr;
Trampoline* LoadCharacters_t = nullptr;

NJS_TEXNAME AmyEff_Tex[4]{};
NJS_TEXLIST AmyEff_TEXLIST = { arrayptrandlength(AmyEff_Tex, Uint32) };

ModelInfo* WaveMdl = nullptr;

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


static void Amy_NewActions(SonicCharObj2* sonicCO2, EntityData1* data, EntityData2* mwp, CharObj2Base* co2)
{

	if (!data)
		return;

	if (co2->CharID2 != Characters_Amy)
		return;

	if (EnableDoubleJump == true && data->Status & Status_Ground)
	{
		BlockDoubleJump[co2->PlayerNum] = false; // can double jump
	}

	switch (data->Action)
	{

	case Action_Jump:
		AmyProp_Check(data, co2);
		AmyDoubleJump(data, co2);
		break;
	case HammerJump:
		PResetAngle(data, co2);
		PGetAccelerationAir(data, co2, mwp);
		PGetSpeed(data, co2, mwp);
		PSetPosition(data, mwp, co2);
		PResetPosition(data, mwp, co2);
		//AmyProp_Check(data, co2);
		break;
	case Action_Spring:
	case Action_Launch:
		AmyProp_Check(data, co2);
		break;
	case HammerSpin:
		AmyMovingSpin(data, mwp, co2);
		break;
	case HammerProp:
		AmyProp_Run(sonicCO2, data, mwp, co2);
		break;
	}
}

void __cdecl Amy_runsActions_r(EntityData1* data1, EntityData2* data2, CharObj2Base* co2, SonicCharObj2* SonicCO2)
{
	FunctionPointer(void, original, (EntityData1 * data1, EntityData2 * data2, CharObj2Base * co2, SonicCharObj2 * SonicCO2), Amy_runsActions_t->Target());
	original(data1, data2, co2, SonicCO2);


	switch (data1->Action)
	{
	case Action_None:

		if (AmyCheckHammerAttack(data1, co2))
			return;

		break;
	case Action_Run:
		if (co2->Speed.x > 2.0)
		{
			if (AmyCheckHammerJump(data1, co2))
			{
				return;
			}
		}
		else {

			if (AmyCheckHammerAttack(data1, co2))
				return;
		}
		break;
	case Action_Jump:
	case Action_Fall:

		if (AmyAirAttack_Check(co2, data1))
			return;

		break;
	case HammerAttack:
		DoAmyHammerAttack(SonicCO2, data1, co2, data2);
		break;
	case HammerAir:
		DoAmyAirAttack(SonicCO2, data1, co2, data2);
		break;
	case HammerJump:
		DoAmyHammerJump(SonicCO2, data1, co2, data2);
		break;
	}

}

static void Amy_Exec_r(ObjectMaster* tsk)
{

	ObjectFunc(origin, Amy_Exec_t->Target());
	origin(tsk);

	char pnum = tsk->Data2.Character->PlayerNum;

	CharObj2Base* co2 = MainCharObj2[pnum];
	EntityData1* data = MainCharObj1[pnum];
	EntityData2* mwp = MainCharData2[pnum];

	//Amy_NewActions(sonicCO2, data, mwp, co2);

	switch (data->Action)
	{
	case HammerJump:
		PResetAngle(data, co2);

		if (!PResetAccelerationAir(data, mwp, co2)) {
			PGetAccelerationAir(data, co2, mwp);
		}
		PGetSpeed(data, co2, mwp);
		PSetPosition(data, mwp, co2);
		PResetPosition(data, mwp, co2);
		break;
	case HammerAttack:
		PGetRotation(data, mwp, co2);
		PGetFriction(data, mwp, co2);
		PGetSpeed(data, co2, mwp);
		PSetPosition(data, mwp, co2);
		PResetPosition(data, mwp, co2);
		break;
	case HammerAir:
		PResetAngle(data, co2);
		if (!PResetAccelerationAir(data, mwp, co2))
		{
			PGetAccelerationAir(data, co2, mwp);
		}
		PGetSpeed(data, co2, mwp);
		PSetPosition(data, mwp, co2);
		PResetPosition(data, mwp, co2);
		break;
	}
}

void Load_AmyEffText() {
	LoadTextureList("AMY_EFF_DC", &AmyEff_TEXLIST);
	WaveMdl = LoadMDL("WaveMdl", ModelFormat_Chunk);
}

void LoadCharacter_r() {

	auto original = reinterpret_cast<decltype(LoadCharacter_r)*>(LoadCharacters_t->Target());
	original();

	for (int i = 0; i < 2; i++) {
		if (MainCharObj2[i]) {
			if (MainCharObj2[i]->CharID2 == Characters_Amy)
			{
				Load_AmyNewAnim();
				Load_AmyEffText();
				break;
			}
		}
	}

	return;
}

void Amy_AbilitiesConfig(const IniFile* config, const IniFile* physics) {

	hammerJump = config->getBool("Abilities", "hammerJump", true);
	EnableDoubleJump = config->getBool( "Abilities", "EnableDoubleJump", true);
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

void Amy_Init()
{
	Amy_Exec_t = new Trampoline((int)Sonic_Main, (int)Sonic_Main + 0x6, Amy_Exec_r);
	Amy_runsActions_t = new Trampoline((int)Sonic_runsActions, (int)Sonic_runsActions + 0x8, Amy_runsActions_r);
	LoadCharacters_t = new Trampoline((int)LoadCharacters, (int)LoadCharacters + 0x6, LoadCharacter_r);
}