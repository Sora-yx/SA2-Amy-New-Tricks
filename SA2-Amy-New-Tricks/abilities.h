#pragma once

extern float MovingGroundSpinAccel;
extern float PropellerGravity;

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
signed int AmyProp_Check(EntityData1* data, CharObj2Base* co2);
void AmyProp_Run(SonicCharObj2* sonicCO2, EntityData1* data, EntityData2* data2, CharObj2Base* co2);
signed int AmyDoubleJump(EntityData1* data, CharObj2Base* co2);
void AmyMovingSpin(EntityData1* data, EntityData2* data2, CharObj2Base* co2);

extern bool BlockDoubleJump[MaxPlayers];