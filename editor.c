#include "editor.h"

void getEntry(char *entry, int size)
{
	char fileName[50] = "/tmp/ded.dedcache";

	char editCommand[200] = "vim ";
	strcat(editCommand, fileName);

	char rmCommand[200] = "shred -u ";	//We don't want to leave any trace of the clear entry on the disk
	strcat(rmCommand, fileName);

	system(editCommand);

	FILE *f = fopen(fileName, "r");

	if(f != NULL)
	{
		int nb = fread(entry, 1, size, f);
		entry[nb - 1] = '\0';
		fclose(f);
	}
	
	system(rmCommand);
}

void editEntry(char *entry, int size)
{
	char fileName[50] = "/tmp/ded.dedcache";

	FILE *f = fopen(fileName, "w");

	if(f != NULL)
	{
		fputs(entry, f);
		fclose(f);
		getEntry(entry, size);
	}
}
