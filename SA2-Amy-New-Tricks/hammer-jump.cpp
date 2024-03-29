#include "pch.h"
#include "abilities.h"

signed int AmyCheckHammerJump(EntityData1* data1, CharObj2Base* co2) {

	if ((Controllers[co2->PlayerNum].press & HammerJumpButton) == 0 || co2->CharID2 != Characters_Amy || !HammerJumpButton
		|| LightDashAllowed(data1, co2))
	{
		return 0;
	}

	data1->Status &= ~Status_Ball;
	data1->Action = HammerJump;
	co2->AnimInfo.Next = HammerJumpStartAnim;
	PlayCustomSoundVolume(Voice_AmyHammerJump, 0.6f);
	return 1;
}

void DoAmyHammerJump(SonicCharObj2* sonicCO2, EntityData1* data1, CharObj2Base* co2, EntityData2* data2) {

	if (Sonic_CheckNextAction(sonicCO2, data1, data2, co2)) {
		FreeAllCustomSounds();
		return;
	}

	if (co2->AnimInfo.Current == HammerJumpStartAnim && co2->AnimInfo.nframe >= 5.0f)
	{
		PlayCustomSoundVolume(SE_HammerAttack, 0.6f);
		data1->Status = Status_Attack;
		co2->Speed.y = co2->PhysData.JumpSpeed * 2.9f;
		Do_HammerWaveHeartEffect(data1);
		return;
	}

	if (co2->AnimInfo.Current > HammerJumpStartAnim)
	{
		if ((data1->Status & STATUS_FLOOR) != 0)
		{
			data1->Rotation.x = data2->Forward.x;
			data1->Rotation.z = data2->Forward.z;
			data1->Action = 1;
			co2->AnimInfo.Next = 1;
		}
		else
		{
			AmyAirAttack_Check(co2, data1);
		}
	}

	return;
}