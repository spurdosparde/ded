#include <libconfig.h>
#include <stdio.h>

void cfgInit(char* confFile);
void cfgGetDbPath(const char** path);
int cfgCreateConfigFile();
