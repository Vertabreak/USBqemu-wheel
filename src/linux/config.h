#ifndef LINUXCONFIG_H
#define LINUXCONFIG_H
#include <sstream>
#include <string>
#include "osdebugout.h"

extern std::string IniDir;
extern std::string LogDir;
extern const char* iniFile;

bool LoadSettingValue(const std::string& ini, const std::string& section, const char* param, std::string& value);
bool LoadSettingValue(const std::string& ini, const std::string& section, const char* param, int32_t& value);
bool LoadSettingValue(const std::string& ini, const std::string& section, const char* param, bool& value);

bool SaveSettingValue(const std::string& ini, const std::string& section, const char* param, const std::string& value);
bool SaveSettingValue(const std::string& ini, const std::string& section, const char* param, const int32_t value);
bool SaveSettingValue(const std::string& ini, const std::string& section, const char* param, const bool value);

template<typename Type>
bool LoadSetting(int port, const std::string& key, const char* name, Type& var)
{
	bool ret = false;
	if (key.empty())
	{
		OSDebugOut("Key is empty for '%s' on port %d\n", name, port);
		return false;
	}

	std::stringstream section;
	section << key << " " << port;

	std::string ini(IniDir);
	ini.append(iniFile);

	OSDebugOut("[%s %d] '%s'=", key.c_str(), port, name);
	ret = LoadSettingValue(ini, section.str(), name, var);
	OSDebugOutStream_noprfx(var);
	return ret;
}

/**
 * 
 * [devices]
 * portX = pad
 * 
 * [pad X]
 * api = joydev
 * 
 * [joydev X]
 * button0 = 1
 * button1 = 2
 * ...
 * 
 * */
template<typename Type>
bool SaveSetting(int port, const std::string& key, const char* name, const Type var)
{
	bool ret = false;
	if (key.empty())
	{
		OSDebugOut("Key is empty for '%s' on port %d\n", name, port);
		return false;
	}

	std::stringstream section;
	section << key << " " << port;

	std::string ini(IniDir);
	ini.append(iniFile);

	OSDebugOut("[%s %d] '%s'=", key.c_str(), port, name);

	ret = SaveSettingValue(ini, section.str(), name, var);
	OSDebugOutStream_noprfx(var);
	return ret;
}

#endif
