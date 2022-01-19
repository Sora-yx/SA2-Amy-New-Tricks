#include "pch.h"
#include "abilities.h"

int timerDizzy = 0;

int AmySpinAttack_Check(CharObj2Base* a1, EntityData1* a2)
{

    if (Action_Held[a1->PlayerNum] == 0)
    {
        return 0;
    }

    a2->Action = HammerSpin;
    a1->AnimInfo.Next = HammerSpinSetAnim;
    VibeThing(0, 15, 0, 6);
    return 1;
}


void DoAmySpinAttack(SonicCharObj2* sonicCO2, EntityData1* data1, CharObj2Base* co2, EntityData2* data2) {

    if (Sonic_CheckNextAction(sonicCO2, data1, data2, co2) || (data1->Status & STATUS_FLOOR) == 0) {

        return;
    }

    int curAnim = co2->AnimInfo.Current;

    if (Action_Held[co2->PlayerNum] == 0)
    {
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
            co2->Speed.z = 0.0;
            co2->Speed.y = 0.0;
            co2->Speed.x = 0.0;
            timerDizzy = 0;
            data1->Action = 0; //todo add dizzy stuff
            co2->AnimInfo.Next = 0;
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