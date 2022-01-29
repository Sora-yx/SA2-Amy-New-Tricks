#include "pch.h"
#include "abilities.h"

int timerDizzy = 0;

int AmySpinAttack_Check(CharObj2Base* co2, EntityData1* data)
{

    if ((Controllers[co2->PlayerNum].on & HammerAttackButton) == 0 || co2->CharID2 != Characters_Amy || !HammerAttackButton)
    {
        return 0;
    }

    data->Action = HammerSpin;
    co2->AnimInfo.Next = HammerSpinSetAnim;
    VibeThing(0, 15, 0, 6);
    return 1;
}

void DoAmySpinAttack(SonicCharObj2* sonicCO2, EntityData1* data1, CharObj2Base* co2, EntityData2* data2) {

    if (Sonic_CheckNextAction(sonicCO2, data1, data2, co2) || (data1->Status & STATUS_FLOOR) == 0) {
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

    if (curAnim == HammerSpinSetAnim) {

        if (GetAnalog(data1, co2, 0, 0))
        {
            timerDizzy = 0;
            co2->AnimInfo.Next = HammerSpinAnim;
            PlayCustomSoundVolume(SE_HammerSpin, 1);
        }
    }

    if (curAnim == HammerSpinAnim) {

        timerDizzy++;
        if (timerDizzy > 300.0)
        {
            data1->Status &= ~Status_Attack;
            co2->Speed.z = 0.0;
            co2->Speed.y = 0.0;
            co2->Speed.x = 0.0;
            timerDizzy = 0;
            data1->Action = 0; //todo add dizzy stuff
            co2->AnimInfo.Next = 0;
            FreeAllCustomSounds();
            return;
        }
     
        if ( (timerDizzy & 7) == 0)
        {
            ObjectMaster* wave = LoadObject(2, "AmyWaveEffect", AmyPutHammerWave, LoadObj_Data1);
            if (wave)
            {
                EntityData1* waveData = wave->Data1.Entity;
                waveData->Position = data1->Position;
                waveData->Position.y = co2->PhysData.Height * 0.5 + waveData->Position.y;
                waveData->Rotation = data1->Rotation;
                waveData->field_6 = 4;
                waveData->Index = 0;
            }
        }
    }

    return;
}