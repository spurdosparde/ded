#include "config.h"

config_t conf;

void cfgInit(char* confFile)
{
	config_init(&conf);

	if(!config_read_file(&conf, confFile))
	{
		printf("No config file found, creating a new one\n");
		cfgCreateConfigFile();
	}
}

void cfgGetDbPath(const char** path)
{
	if(!config_lookup_string(&conf, "database", path))
	{
		printf("Error: database not defined\n");
	}
}

int cfgCreateConfigFile()
{
	static const char* out = "ded.cfg";
	config_setting_t *root, *setting;
	root = config_root_setting(&conf);

	setting = config_setting_add(root, "database", CONFIG_TYPE_STRING);
	config_setting_set_string(setting, "entries.db");

	if(!config_write_file(&conf, out))
	{
		printf("Error creating config file\n");
		return 0;
	}
}
