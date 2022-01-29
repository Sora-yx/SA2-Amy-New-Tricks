#include "pch.h"
#include "abilities.h"

signed int AmyCheckHammerAttack(EntityData1* data1, CharObj2Base* co2) {

	if ((Controllers[co2->PlayerNum].press & HammerAttackButton) == 0 || co2->CharID2 != Characters_Amy || !HammerAttackButton)
	{
		return 0;
	}

	data1->Action = HammerAttack;
	co2->AnimInfo.Next = HammerAttackAnim;

	if ((data1->Status & 0x20) != 0)
	{
		co2->DynColInfo->anaspdv.y = 0.0;
	}

	PlayCustomSoundVolume(Voice_AmyHammerAttack, 3);
	return 1;
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
				data1->Status &= ~Status_Attack;
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


	return;
}