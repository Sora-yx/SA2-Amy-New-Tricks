#pragma once

void DoAmyHammerJump(SonicCharObj2* sonicCO2, EntityData1* data1, CharObj2Base* co2, EntityData2* data2);
signed int AmyCheckHammerJump(EntityData1* data1, CharObj2Base* co2);
void Do_HammerWaveHeartEffect(EntityData1* data1);
int AmyAirAttack_Check(CharObj2Base* a1, EntityData1* a2);
void DoAmyAirAttack(SonicCharObj2* sonicCO2, EntityData1* data1, CharObj2Base* co2, EntityData2* data2);
void __cdecl AmyEffectPutSpdDwnHeart(NJS_POINT3* pos);
void __cdecl AmyPutHammerWave(ObjectMaster* obj);
int AmySpinAttack_Check(CharObj2Base* a1, EntityData1* a2);
void DoAmySpinAttack(SonicCharObj2* sonicCO2, EntityData1* data1, CharObj2Base* co2, EntityData2* data2);
void AmySetAttackColli(CharObj2Base* a1, EntityData1* data);

extern bool BlockDoubleJump[MaxPlayers];