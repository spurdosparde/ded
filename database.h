#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include "cryptolayer.h"
#include "config.h"
#define ENTRY_SIZE 5000


struct entryHeader
{
	char date[11];
	int ID;
};

struct entryHeaderArray
{
	struct entryHeader buffer[31];
	int bufferIndex;
};

struct entry
{
	char content[ENTRY_SIZE*4/3];
	struct entryHeader header;
};


sqlite3* openDatabase(unsigned char* key);
void createDatabase(sqlite3* db);
void closeDatabase(sqlite3* db);

void readEntry(int ID, struct entry* buffer, sqlite3* db, unsigned char* key);

void addEntry(struct entry* buffer, sqlite3* db, unsigned char* key);
void updateEntry(struct entry* buffer, sqlite3* db, unsigned char* key);

void getEntriesByMonth(int year, int month, struct entryHeaderArray* buffer, sqlite3* db);

void delEntry(int ID, sqlite3* db);

static int readCallback(void *data, int argc, char** argv, char** colNames);
static int getCallback(void *data, int argc, char** argv, char** colNames);

