#include "pch.h"
#include "abilities.h"

signed int AmyCheckHammerAttack(EntityData1* data1, CharObj2Base* co2) {

	if ((Controllers[co2->PlayerNum].press & HammerAttackButton) == 0 || co2->CharID2 != Characters_Amy || !HammerAttackButton
		|| LightDashAllowed(data1, co2))
	{
		return 0;
	}

	data1->Action = HammerAttack;
	data1->Status &= ~Status_Ball;
	co2->AnimInfo.Next = HammerAttackAnim;

	if ((data1->Status & 0x20) != 0)
	{
		co2->DynColInfo->anaspdv.y = 0.0f;
	}

	PlayCustomSoundVolume(Voice_AmyHammerAttack, 0.3f);
	return 1;
}

void amyCheckSpin(EntityData1* data1, CharObj2Base* co2)
{
	if (co2->AnimInfo.Current == HammerSpinSetAnim) {

		if (AmySpinAttack_Check(co2, data1))
		{
			return;
		}
		else {
			data1->Status &= ~Status_Attack;
			data1->Action = Action_None;
			co2->AnimInfo.Next = 0;
			return;
		}
	}
}

void DoAmyHammerAttack(SonicCharObj2* sonicCO2, EntityData1* data1, CharObj2Base* co2, EntityData2* data2) {

	short curAnimHammer = co2->AnimInfo.Current;

	if (curAnimHammer == HammerAttackAnim && co2->AnimInfo.nframe >= 3.0f && co2->AnimInfo.nframe < 4.0f)
	{
		Do_HammerWaveHeartEffect(data1);
	}

	amyCheckSpin(data1, co2);
	return;
}