#include "pch.h"

static Trampoline* Amy_Callback_t;
static NJS_MATRIX AmyHeadMatrix;


NJS_MATRIX AmyHammerMatrix;

void Amy_Callback_r(NJS_OBJECT* model) {

	if (model == CharacterModels[403].Model) {
		memcpy(&AmyHeadMatrix, _nj_current_matrix_ptr_, 0x30u);
	}

	FunctionPointer(void, original, (NJS_OBJECT * model), Amy_Callback_t->Target());
	original(model);
}


void DisplayUpgrade() {

	if (SonicCO2PtrExtern->base.CharID2 != Characters_Amy)
		return;

	NJS_OBJECT* hammer = CharacterModels[506].Model;
	char pnum = SonicCO2PtrExtern->base.PlayerNum;
	short curAnim = MainCharObj2[pnum]->AnimInfo.Current;

	if (hammer)
	{
		if (MainCharObj2[pnum]->Speed.x >= 2.0 && MainCharObj1[pnum]->Action < 3 || curAnim == HammerJumpStartAnim || curAnim >= HammerAttackAnim && curAnim <= HammerSpinAnim) {
			njPushMatrixEx();
			memcpy(CUR_MATRIX, &AmyRightHandMatrix, 0x30u);

			if (curAnim == HammerSpinSetAnim || curAnim == HammerSpinAnim)
				njRotateZ_(CUR_MATRIX, MainCharObj1[pnum]->Rotation.z + 0x3000);

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