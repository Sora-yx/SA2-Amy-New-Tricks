#include "pch.h"
#include "abilities.h"

static int timerDizzy = 0;

int AmySpinAttack_Check(CharObj2Base* co2, EntityData1* data)
{

	if ((Controllers[co2->PlayerNum].on & HammerAttackButton) == 0 || co2->CharID2 != Characters_Amy || !HammerAttackButton)
	{
		return 0;
	}

	data->Status &= ~Status_Ball;
	data->Action = HammerSpin;
	co2->AnimInfo.Next = HammerSpinSetAnim;
	VibeThing(0, 15, 0, 6);
	return 1;
}

void DoAmySpinAttack(SonicCharObj2* sonicCO2, EntityData1* data1, CharObj2Base* co2, EntityData2* data2) {

	if ((data1->Status & STATUS_FLOOR) == 0) {
		FreeAllCustomSounds();
		return;
	}

	int curAnim = co2->AnimInfo.Current;

	if ((Controllers[co2->PlayerNum].on & HammerAttackButton) == 0)
	{
		FreeAllCustomSounds();
		data1->Status &= ~Status_Attack;
		data1->Action = Action_None;
		co2->AnimInfo.Next = 0;
		return;
	}


	if (!GetAnalog(data1, co2, 0, 0))
	{
		return;
	}

	if (curAnim == HammerSpinSetAnim) {

		timerDizzy = 0;
		co2->AnimInfo.Next = HammerSpinAnim;
		PlayCustomSoundVolume(SE_HammerSpin, 0.3f);
	}

	if (curAnim == HammerSpinAnim) {

		timerDizzy++;
		if (timerDizzy > 300)
		{
			data1->Status &= ~Status_Attack;
			co2->Speed = { 0.0f };
			timerDizzy = 0;
			data1->Action = 0;
			co2->AnimInfo.Next = 0;
			FreeAllCustomSounds();
			return;
		}

		if ((timerDizzy & 7) == 0)
		{
			ObjectMaster* wave = LoadObject(2, "AmyWaveEffect", AmyPutHammerWave, LoadObj_Data1);
			if (wave)
			{
				EntityData1* waveData = wave->Data1.Entity;
				waveData->Position = data1->Position;
				waveData->Position.y = co2->PhysData.Height * 0.5 + waveData->Position.y;
				waveData->Rotation = data1->Rotation;
				waveData->Timer = 4;
				waveData->Index = 0;
			}
		}
	}

	return;
}