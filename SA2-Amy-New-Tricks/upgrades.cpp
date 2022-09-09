#include "pch.h"

static Trampoline* Amy_Callback_t = nullptr;
static NJS_MATRIX AmyHeadMatrix;

NJS_MATRIX AmyHammerMatrix;

void Amy_Callback_r(NJS_OBJECT* model) {

	FunctionPointer(void, original, (NJS_OBJECT * model), Amy_Callback_t->Target());
	original(model);


	if (model == CharacterModels[403].Model) { //warrier feather sa1 amy support
		memcpy(&AmyHeadMatrix, _nj_current_matrix_ptr_, 0x30u);
	}

	NJS_OBJECT* hammer = CharacterModels[506].Model;

	if (hammer)
	{
		memcpy(&AmyHammerMatrix, _nj_current_matrix_ptr_, 0x30u);
	}
}

extern float hammerScale;
void DisplayUpgrade() {

	if (!SonicCO2PtrExtern || SonicCO2PtrExtern->base.CharID2 != Characters_Amy) {
		njPushMatrixEx();
		return;
	}

	NJS_OBJECT* hammer = CharacterModels[506].Model;

	char pnum = SonicCO2PtrExtern->base.PlayerNum;
	short curAnim = MainCharObj2[pnum]->AnimInfo.Current;

	if (hammer)
	{
		hammer->scl[0] = hammerScale;
		hammer->scl[1] = hammerScale;
		hammer->scl[2] = hammerScale;

		NJS_OBJECT* hammerChild = CharacterModels[506].Model->child;

		if (!hammerChild)
			return;

		hammerChild->scl[0] = hammerScale;
		hammerChild->scl[1] = hammerScale;
		hammerChild->scl[2] = hammerScale;

		if (curAnim == HammerSpinSetAnim) {

			hammer->ang[2] = MainCharObj1[pnum]->Rotation.z + isSA1Amy() ? 0x500 : 0x4000;
			hammerChild->ang[2] = hammer->ang[2];
		}
		else if (curAnim == HammerSpinAnim) {

			hammer->ang[0] = isSA1Amy() ? 0x4000 : 0x8000;
			hammer->ang[2] = MainCharObj1[pnum]->Rotation.z + isSA1Amy() ? 0x3000 : -0x1000;
			hammerChild->ang[0] = hammer->ang[0];
			hammerChild->ang[2] = hammer->ang[2];
		}

		else {
			if (!isSA1Amy()) {
				hammer->ang[0] = 0;
				hammer->ang[2] = 0;
				hammerChild->ang[0] = 0;
				hammerChild->ang[2] = 0x4000;
			}
			else
			{
				hammer->ang[0] = 0xFFFFFFD6;
				hammer->ang[2] = 161;
				hammerChild->ang[0] = 0;
				hammerChild->ang[2] = 0x0;
			}
		}

		if (MainCharObj2[pnum]->Speed.x >= 2.0f && MainCharObj1[pnum]->Action < 3 || curAnim == HammerJumpStartAnim || curAnim >= HammerAttackAnim && curAnim <= HammerSpinAnim) {
			njPushMatrixEx();
			memcpy(CUR_MATRIX, &AmyRightHandMatrix, 0x30u);

			njTranslate(nullptr, -0.5f, 0.1f, 0.0f);
			DrawObject(hammer);
			njPopMatrixEx();
		}
	}

	if (isSA1Amy()) {
		NJS_OBJECT* warrior = CharacterModels[500].Model;

		if (warrior) {
			njPushMatrixEx();
			memcpy(_nj_current_matrix_ptr_, &AmyHeadMatrix, 0x30u);
			njTranslate(nullptr, -0.5f, 0.1f, 0.0f);
			DrawObject(warrior);
			njPopMatrixEx();
		}
	}


	njPushMatrixEx();
}

void Upgrades_Init() {

	Amy_Callback_t = new Trampoline((int)0x71F040, (int)0x71F049, Amy_Callback_r);
	WriteCall((void*)0x720698, DisplayUpgrade);
}