#include "database.h"

sqlite3* openDatabase(unsigned char* key)
{
	sqlite3 *db;

	if(sqlite3_open("/home/sylvain/data/sylvain/Documents/developpement/ded/entries.db", &db))
	{
		printf("Error, can't open database\n");
		return NULL;
	}
	else
	{
		printf("Database succesfully opened\n");
		return db;
	}
}

void closeDatabase(sqlite3* db)
{
	printf("Closing database\n");
	sqlite3_close(db);
}

void readEntry(int ID, struct entry* buffer, sqlite3* db, unsigned char* key)
{
	char *err = 0;
	char query[100] = "SELECT ID,date,content FROM entries WHERE ID=\"";
	struct entry tmp;

	char chid[15] = "";
	sprintf(chid, "%d", ID);

	strcat(query, chid);
	strcat(query, "\";");

	int resp = sqlite3_exec(db, query, readCallback, (void*)&tmp, &err);
	
	decrypt(tmp.content, buffer->content, key);
	strncpy(buffer->header.date, tmp.header.date, 11);
	buffer->header.date[10] = '\0';
	buffer->header.ID = tmp.header.ID;
	
	if(resp)
	{
		printf("Error reading database: %s\n", err);
	}
}

static int readCallback(void *data, int argc, char** argv, char** colNames)
{
	if(argc == 3)
	{
		struct entry* ref = (struct entry*) data;
		ref->header.ID = atoi(argv[0]);
		strncpy(ref->header.date, argv[1], 10);
		ref->header.date[10] = '\0';
		strcpy(ref->content, argv[2]);
		ref->header.ID = atoi(argv[0]);
		//printf("Date in callback: %s", ref->header.date);
		return 0;
	}
	else
	{
		return -1;
	}
}

void addEntry(struct entry* buffer, sqlite3* db, unsigned char* key)
{
	char *err = 0;
	char encEntry[(ENTRY_SIZE + 64)*4/3];
	int num = 0;

	encrypt(buffer->content, encEntry, key);
	
	char query[100 + ENTRY_SIZE*4/3] = "INSERT INTO entries (content, date) VALUES (\"";
	strcat(query, encEntry);
	strcat(query, "\", ");

	if(!strcmp(buffer->header.date, "now"))
		strcat(query, "date(\"now\"));");
	else
	{
		strcat(query, "\"");
		strcat(query, buffer->header.date);
		strcat(query, "\");");
	}

	int resp = sqlite3_exec(db, query, NULL, NULL, &err);
	if(resp)
	{
		printf(err);
	}
}

void updateEntry(struct entry* buffer, sqlite3* db, unsigned char* key)
{
	char *err = 0;
	char encEntry[(ENTRY_SIZE + 64)*4/3];
	int num = 0;

	encrypt(buffer->content, encEntry, key);
	printf("encrypted\n");	
	char query[100 + ENTRY_SIZE*4/3]; 
	sprintf(query, "UPDATE entries SET content = \"%s\" WHERE ID=\"%d\";", encEntry, buffer->header.ID);

	int resp = sqlite3_exec(db, query, NULL, NULL, &err);
	if(resp)
	{
		printf(err);
	}
}

void getEntriesByMonth(int year, int month, struct entryHeaderArray* buffer, sqlite3* db)
{
	char *err = 0;
	char mth[15] = "";
	sprintf(mth, "%02d", month);

	buffer->bufferIndex = 0;

	char query[100] = {0};
	
	sprintf(query, "SELECT ID,date FROM entries WHERE date LIKE \"%d-%02d-%%\"", year, month);

	int resp = sqlite3_exec(db, query, getCallback, (void*)buffer, &err);

	if(resp)
		printf("SQL error: %s\n", err);
}

static int getCallback(void *data, int argc, char** argv, char** colNames)
{
	if(argc > 1)
	{
		struct entryHeaderArray* ref = (struct entryHeaderArray*) data;	//Avoid to make static cast everytime
		struct entryHeader element;		//Declaring a local temporary header variable
		int nb = ref->bufferIndex;
		
		if(nb < 31)
		{
			ref->buffer[nb].ID = atoi(argv[0]);
			strncpy(ref->buffer[nb].date, argv[1], 10);
			ref->buffer[nb].date[11] = '\0';
			ref->bufferIndex = nb + 1;

			return 0;
		}
		else
		{
			printf("WARNING: more than 31 entries in one month, skipping some\n");
			return -1;
		}
	}
	else
	{
		return -1;
	}
}

void delEntry(int ID, sqlite3* db)
{
	char* err = 0;
	char query[100] = {0};

	sprintf(query, "DELETE FROM entries WHERE ID=\"%d\";", ID);

	int resp = sqlite3_exec(db, query, NULL, NULL, &err);

	if(resp)
		printf("SQL error: %s\n", err);
}
