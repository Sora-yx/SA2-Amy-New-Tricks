#include "pch.h"

static Trampoline* Amy_Callback_t;
DataPointer(NJS_MATRIX, AmyRightHandMatrix, 0x1A51A3C);

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
			njTranslate(nullptr, -0.5f, 0.1f, 0.0f);
			DrawObject(hammer);
			njPopMatrixEx();
		}
	}

	njPushMatrixEx();
}

void Upgrades_Init() {


	WriteCall((void*)0x720698, DisplayUpgrade);
}