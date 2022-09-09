#include "pch.h"
#include "abilities.h"

TaskHook Amy_Exec_t(Sonic_Main);
FunctionHook<void, EntityData1*, EntityData2*, CharObj2Base*, SonicCharObj2*> Amy_runsActions_t((intptr_t)Sonic_runsActions);
TaskHook Amy_Delete_t(Sonic_Delete);
FunctionHook<void> LoadCharacters_t((intptr_t)LoadCharacters);
FunctionHook<void, int> LoadSonic_t((intptr_t)LoadSonic);

NJS_TEXNAME AmyEff_Tex[4]{};
NJS_TEXLIST AmyEff_TEXLIST = { arrayptrandlength(AmyEff_Tex, Uint32) };

ModelInfo* WaveMdl = nullptr;

static bool CheckHomingAttack(EntityData1* data, CharObj2Base* co2, SonicCharObj2* SonicCO2, EntityData2* data2)
{
	if (!data || data->Action != HammerJump)
		return false;

	if ((data->Status & Status_DisableControl) == 0)
	{
		if (Jump_Pressed[co2->PlayerNum])
		{

			float gravity = fabs(Gravity.y);
			if (gravity == flt_1283704)
			{
				float spd = fabs(co2->SurfaceInfo.BottomSurfaceDist - data->Position.y);

				if (spd <= 8.0f)
				{
					return false;
				}
			}
			DoHomingAttackEffect(SonicCO2, data, data2, co2);
			data->Action = Action_HomingAttack;
			if (SonicCO2->gap35E[6] <= 0) {

				data->Status |= Status_Attack | Status_Ball;
			}
			else
			{
				co2->AnimInfo.Next = 100;
				data->Status &= 0xFEFFu;
			}
			return true;
		}
	}

	return false;
}

static void Amy_NewActions(SonicCharObj2* SonicCO2, EntityData1* data, EntityData2* mwp, CharObj2Base* co2)
{
	if ((!data) || co2->CharID2 != Characters_Amy || CurrentLevel == LevelIDs_ChaoWorld)
		return;

	switch (data->Action)
	{
	case Action_None:

		if (AmyCheckHammerAttack(data, co2))
			return;

		break;
	case Action_Run:
		if (co2->Speed.x > 2.0f)
		{
			if (AmyCheckHammerJump(data, co2))
			{
				return;
			}

			if (HammerJumpButton != HammerAttackButton)
			{
				if (AmyCheckHammerAttack(data, co2))
					return;
			}
		}
		else {

			if (AmyCheckHammerAttack(data, co2))
				return;
		}
		break;
	case Action_Spring:
	case Action_Launch:


		if (AmyProp_Check(data, co2))
			return;

		break;
	case Action_Jump:

		if (AmyProp_Check(data, co2))
			return;

		if (AmyAirAttack_Check(co2, data))
			return;

		break;
	case Action_Fall:

		if (Sonic_CheckNextAction(SonicCO2, data, mwp, co2))
			return;

		if (AmyProp_Check(data, co2))
			return;

		if (AmyAirAttack_Check(co2, data))
			return;

		break;
	case HammerAttack:
	{

		if (Sonic_CheckNextAction(SonicCO2, data, mwp, co2))
			return;

		DoAmyHammerAttack(SonicCO2, data, co2, mwp);
	}
	break;
	case HammerAir:

		if (Sonic_CheckNextAction(SonicCO2, data, mwp, co2))
			return;

		DoAmyAirAttack(SonicCO2, data, co2, mwp);
		break;
	case HammerJump:

		if (Sonic_CheckNextAction(SonicCO2, data, mwp, co2) || AmyProp_Check(data, co2) || CheckHomingAttack(data, co2, SonicCO2, mwp))
			return;

		DoAmyHammerJump(SonicCO2, data, co2, mwp);
		break;
	case HammerSpin:

		if (Sonic_CheckNextAction(SonicCO2, data, mwp, co2))
			return;

		DoAmySpinAttack(SonicCO2, data, co2, mwp);
		AmyMovingSpin(data, mwp, co2);
		break;
	case HammerProp:

		if (Sonic_CheckNextAction(SonicCO2, data, mwp, co2))
			return;

		AmyProp_Run(SonicCO2, data, mwp, co2);
		break;
	}
}

void __cdecl Amy_runsActions_r(EntityData1* data1, EntityData2* data2, CharObj2Base* co2, SonicCharObj2* SonicCO2)
{

	Amy_NewActions(SonicCO2, data1, data2, co2);

	return Amy_runsActions_t.Original(data1, data2, co2, SonicCO2);
}

void Amy_NewMoves_Main(ObjectMaster* tsk) {

	char pnum = tsk->Data2.Character->PlayerNum;

	auto co2 = MainCharObj2[pnum];
	auto data = MainCharObj1[pnum];
	auto mwp = MainCharData2[pnum];
	auto SonicCO2 = (SonicCharObj2*)tsk->Data2.Undefined;

	if (co2->CharID2 != Characters_Amy)
		return;

	int curAnim = co2->AnimInfo.Current;

	switch (data->Action)
	{
	case HammerAttack:
		PGetRotation(data, mwp, co2);
		PGetFriction(data, mwp, co2);
		PGetSpeed(data, co2, mwp);
		PSetPosition(data, mwp, co2);
		PResetPosition(data, mwp, co2);
		break;
	case HammerAir:
		PResetAngle(data, co2);
		if (!PResetAccelerationAir(mwp, data, co2))
		{
			PGetAccelerationAir(data, co2, mwp);
		}
		PGetSpeed(data, co2, mwp);
		PSetPosition(data, mwp, co2);
		PResetPosition(data, mwp, co2);
		break;
	case HammerJump:
		PResetAngle(data, co2);

		if (!PResetAccelerationAir(mwp, data, co2)) {
			PGetAccelerationAir(data, co2, mwp);
		}
		PGetSpeed(data, co2, mwp);
		PSetPosition(data, mwp, co2);
		PResetPosition(data, mwp, co2);
		break;
	case HammerSpin:
		if (curAnim == HammerSpinSetAnim || curAnim == HammerSpinAnim)
		{
			PGetRotation(data, mwp, co2);
		}
		PResetAngle(data, co2);
		PGetAcceleration(data, mwp, co2);
		PGetSpeed(data, co2, mwp);
		PSetPosition(data, mwp, co2);
		PResetPosition(data, mwp, co2);
		break;
	case HammerProp:
		auto RestoreSpeed = co2->PhysData.Weight;
		co2->PhysData.Weight = PropellerGravity;

		PGetRotation(data, mwp, co2);
		PResetAngle(data, co2);
		PGetAcceleration(data, mwp, co2);
		PGetSpeed(data, co2, mwp);
		PSetPosition(data, mwp, co2);
		PResetPosition(data, mwp, co2);

		co2->PhysData.Weight = RestoreSpeed;
		break;
	}
}

static void Amy_Exec_r(ObjectMaster* tsk)
{
	Amy_NewMoves_Main(tsk);

	Amy_Exec_t.Original(tsk);

	char pnum = tsk->Data2.Character->PlayerNum;
	auto co2 = MainCharObj2[pnum];
	auto data = MainCharObj1[pnum];
	auto SonicCO2 = (SonicCharObj2*)tsk->Data2.Undefined;

	if (co2->CharID2 == Characters_Amy) {
		AmySetHammerScale(co2);
		AmySetAttackColli(SonicCO2, co2, data);
	}
}

void Load_AmyEffText() {
	LoadTextureList("AMY_EFF_DC", &AmyEff_TEXLIST);
	WaveMdl = LoadMDL("WaveMdl", ModelFormat_Chunk);
	return;
}

void Amy_Delete_R(ObjectMaster* obj)
{
	SonicCharObj2* sonicCO2 = (SonicCharObj2*)obj->Data2.Undefined;

	if (sonicCO2->base.CharID2 == Characters_Amy) {
		FreeMDL(WaveMdl);
		FreeTexList(&AmyEff_TEXLIST);
	}

	Amy_Delete_t.Original(obj);
}

int BannedAmyLevel[6] = { LevelIDs_FinalHazard, LevelIDs_Route101280,
LevelIDs_EggGolemE, LevelIDs_KartRace, LevelIDs_TailsVsEggman1, LevelIDs_TailsVsEggman2 };

bool isLevelBanned() {
	for (int i = 0; i < LengthOfArray(BannedAmyLevel); i++)
	{
		if (CurrentLevel == BannedAmyLevel[i])
			return true;
	}

	return false;
}

void LoadCharacter_r() {

	if (!isLevelBanned() && amyAdventure && !TwoPlayerMode)
	{
		CurrentCharacter = Characters_Sonic;
	}

	LoadCharacters_t.Original();

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

void LoadSonic_r(int playNum)
{
	if (!TwoPlayerMode && !playNum && !isLevelBanned())
	{
		LoadAmy(playNum);
		return;
	}

	LoadSonic_t.Original(playNum);
}

void Amy_Init()
{
	Amy_Exec_t.Hook(Amy_Exec_r);
	Amy_runsActions_t.Hook(Amy_runsActions_r);
	Amy_Delete_t.Hook(Amy_Delete_R);

	LoadCharacters_t.Hook(LoadCharacter_r);

	if (amyAdventure)
		LoadSonic_t.Hook(LoadSonic_r);

	return;
}