#include "pch.h"

int bird = flicky;
bool grunt = false;
bool hammerRot = false;
bool amyAdventure = false;

void ReadConfig(const char* path) {

	const IniFile* config = new IniFile(std::string(path) + "\\config.ini");
	grunt = config->getBool("General", "grunt", false);
	bird = config->getInt("General", "bird", randomFlicky);
	hammerRot = config->getBool("General", "hammerRot", false);
	amyAdventure = config->getBool("General", "amyAdventure", false);

	const auto physics = new IniFile(std::string(path) + "\\physics.ini");

	Amy_AbilitiesConfig(config, physics);

	delete config;


	if (bird != none) {
		Init_Bird();
	}

	if (!grunt)
		WriteData<5>((int*)0x71AF71, 0x90);

}