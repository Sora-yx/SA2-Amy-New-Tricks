#include "pch.h"

int bird = flicky;
bool grunt = false;

void ReadConfig(const char* path) {

	const IniFile* config = new IniFile(std::string(path) + "\\config.ini");
	grunt = config->getBool("General", "grunt", false);
	bird = config->getInt("General", "bird", randomFlicky);

	const auto physics = new IniFile(std::string(path) + "\\physics.ini");

	Amy_AbilitiesConfig(config, physics);

	if (bird != none) {
		Init_Bird();
	}

	delete config;

}