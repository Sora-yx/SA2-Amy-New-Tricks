#include "pch.h"


int AmyAirAttack_Check(CharObj2Base* a1, EntityData1* a2)
{
    if (Action_Pressed[a1->PlayerNum] == 0)
    {
        return 0;
    }
 
    a2->Action = HammerAir;
    a1->AnimInfo.Next = HammerAirAnim;
    PlayCustomSoundVolume(Voice_AmyHammerAir, 1);
    VibeThing(0, 15, 0, 6);
    return 1;
}


void DoAmyAirAttack(SonicCharObj2* sonicCO2, EntityData1* data1, CharObj2Base* co2, EntityData2* data2) {

    if (Sonic_CheckNextAction(sonicCO2, data1, data2, co2)) {

        return;
    }

    int curAnim = co2->AnimInfo.Current;

    if (curAnim == 15)
    {
        data1->Action = (data1->Status & STATUS_FLOOR) != 0 ? (char)Action_None : (char)Action_Fall;
    }
    else if ((data1->Status & STATUS_FLOOR) != 0)
    {
        data1->Rotation.x = data2->Forward.x;
        data1->Rotation.z = data2->Forward.z;
        data1->Action = (char)Action_None;
        co2->AnimInfo.Next = 1;
    }

    return;
}