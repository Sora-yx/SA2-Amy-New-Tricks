#include "pch.h"

static Trampoline* Amy_Callback_t;
static NJS_MATRIX AmyHeadMatrix;


NJS_MATRIX AmyHammerMatrix;

void Amy_Callback_r(NJS_OBJECT* model) {

	if (model == CharacterModels[403].Model) {
		memcpy(&AmyHeadMatrix, _nj_current_matrix_ptr_, 0x30u);
	}


	NJS_OBJECT* hammer = CharacterModels[506].Model;

	if (hammer)
	{
		memcpy(&AmyHammerMatrix, _nj_current_matrix_ptr_, 0x30u);
	}

	FunctionPointer(void, original, (NJS_OBJECT * model), Amy_Callback_t->Target());
	original(model);
}

extern float hammerScale;
void DisplayUpgrade() {

	if (SonicCO2PtrExtern->base.CharID2 != Characters_Amy)
		return;

	NJS_OBJECT* hammer = CharacterModels[506].Model;
	NJS_OBJECT* hammerChild = CharacterModels[506].Model->child;
	char pnum = SonicCO2PtrExtern->base.PlayerNum;
	short curAnim = MainCharObj2[pnum]->AnimInfo.Current;

	if (hammer)
	{

		hammer->scl[0] = hammerScale;
		hammer->scl[1] = hammerScale;
		hammer->scl[2] = hammerScale;
		hammerChild->scl[0] = hammerScale;
		hammerChild->scl[1] = hammerScale;
		hammerChild->scl[2] = hammerScale;

		if (curAnim == HammerSpinSetAnim) {
			hammer->ang[2] = MainCharObj1[pnum]->Rotation.z + 0x500;
		}

		if (curAnim == HammerSpinAnim) {
			hammer->ang[2] = MainCharObj1[pnum]->Rotation.z + 0x3000;
			hammerChild->ang[0] = 0x4000;
		}

		if (MainCharObj2[pnum]->Speed.x >= 2.0 && MainCharObj1[pnum]->Action < 3 || curAnim == HammerJumpStartAnim || curAnim >= HammerAttackAnim && curAnim <= HammerSpinAnim) {
			njPushMatrixEx();
			memcpy(CUR_MATRIX, &AmyRightHandMatrix, 0x30u);

			njTranslate(nullptr, -0.5f, 0.1f, 0.0f);
			DrawObject(hammer);
			njPopMatrixEx();
		}
	}

	NJS_OBJECT* warrior = CharacterModels[500].Model;

	if (warrior) {
		njPushMatrixEx();
		memcpy(_nj_current_matrix_ptr_, &AmyHeadMatrix, 0x30u);
		njTranslate(nullptr, -0.5f, 0.1f, 0.0f);
		DrawObject(warrior);
		njPopMatrixEx();
	}


	njPushMatrixEx();
}

void Upgrades_Init() {


	Amy_Callback_t = new Trampoline((int)0x71F040, (int)0x71F049, Amy_Callback_r);
	WriteCall((void*)0x720698, DisplayUpgrade);
}