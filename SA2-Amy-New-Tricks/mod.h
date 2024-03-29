#pragma once

extern HelperFunctions HelperFunctionsGlobal;
void ReadConfig(const char* path);
#define TaskHook FunctionHook<void, ObjectMaster*>


extern int bird;
extern bool hammerRotFix;
extern bool grunt;
extern bool amyAdventure;

extern Buttons HammerPropButton;
extern Buttons HammerAttackButton;
extern Buttons HammerAirButton;
extern Buttons HammerJumpButton;

enum AmyAction {
	HammerAttack = 95,
	HammerAir,
	HammerJump,
	HammerSpin,
	HammerProp,
};


enum AmyAnim {
	HammerJumpStartAnim = 249,
	HammerJumpAnim,
	HammerJumpLoopAnim,
	HammerAttackStartAnim,
	HammerAttackAnim,
	HammerAirAnim,
	HammerSpinSetAnim,
	HammerSpinAnim,
	DoubleJumpAnim
};

enum BirdSettings {
	none,
	flicky,
	gammaFlicky,
	randomFlicky
};

const char MaxPlayers = 2;

#define CUR_MATRIX _nj_current_matrix_ptr_

extern NJS_TEXLIST AmyEff_TEXLIST;
