#include "pch.h"

int bird = flicky;
bool grunt = false;
bool hammerRotFix = false;
bool amyAdventure = false;

void ReadConfig(const char* path) {

	const IniFile* config = new IniFile(std::string(path) + "\\config.ini");
	grunt = config->getBool("General", "grunt", false);
	bird = config->getInt("General", "bird", randomFlicky);
	hammerRotFix = config->getBool("General", "hammerRotFix", false);
	amyAdventure = config->getBool("General", "amyAdventure", false);

	const auto physics = new IniFile(std::string(path) + "\\physics.ini");

	Amy_AbilitiesConfig(config, physics);

	delete config;

	if (isSA1Amy())
		hammerRotFix = true;


	if (bird != none) {
		Init_Bird();
	}

	if (!grunt)
		WriteData<5>((int*)0x71AF71, 0x90);

}