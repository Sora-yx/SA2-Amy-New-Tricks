#pragma once

//void njCnkAction(NJS_OBJECT* obj, NJS_MOTION* mot, float frame);
void njCnkAction(NJS_ACTION* action, float frame);
void njCnkMotion(NJS_OBJECT* obj, NJS_MOTION* mot, float frame);
ModelInfo* LoadMDL(const char* name, ModelFormat format);
AnimationFile* LoadAnim(const char* name);
int __cdecl BAMS_SubWrap(__int16 bams_a, unsigned __int16 bams_b, int limit);
void FreeMDL(ModelInfo* pointer);
void LoadAnimation(AnimationFile** info, const char* name, const HelperFunctions& helperFunctions);
void FreeAnim(AnimationFile* pointer);
int IsPlayerInsideSphere(NJS_VECTOR* position, float a2);
bool CheckControl(int id);
void Amy_Init();
void Amy_AbilitiesConfig(const IniFile* config, const IniFile* physics);
void Init_Bird();
void Load_AmyNewAnim();
void Init_AmyAnim();
void SetNewAnimList(int player);
signed int AmyCheckHammerAttack(EntityData1* data1, CharObj2Base* co2);
void DoAmyHammerAttack(SonicCharObj2* sonicCO2, EntityData1* data1, CharObj2Base* co2, EntityData2* data2);
void Upgrades_Init();
void njGetTranslation(NJS_MATRIX_PTR matrix, NJS_VECTOR* out);

#define STATUS_FLOOR (Status_Ground | Status_OnObjectColli)