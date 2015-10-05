#include "editor.h"
#include "interface.h"

#define NCURSES true

void createEntry(sqlite3 *db, char* key);
void openEntry(sqlite3 *db, char* key);
void listEntries(sqlite3* db);
void modifEntry(sqlite3* db, char* key);

int main(int argc, char** argv)
{
	if(argc != 1 && 1 == 0)
	{
		printf("Usage: semavy <path_to_diary>");
		return -1;
	}
	else
	{
		char dbFile[101] = "";
		unsigned char key[57] = "";

		printf("Enter key (max 56 characters): ");
		scanf("%56s", key);

		int l = sizeof(argv[0]);
		//strncpy(dbFile, argv[0], max(100,l));
		sqlite3 *db;
		db = openDatabase(key);

		if(NCURSES)
		{
			ncInterface(db, key);
		}
		else
		{

			int choice = 0;

			while(choice != 4)
			{
				printf("1: Add new entry, 2: Read Entry, 3: List all entries, 4: Quit, 5: edit entry\n");
				scanf("%d", &choice);

				switch(choice)
				{
					case 1:
						createEntry(db, key);
						break;

					case 2:
						openEntry(db, key);
						break;

					case 3:
						listEntries(db);
						break;

					case 5:
						modifEntry(db, key);
						break;

					default:
						break;
				}
			}
		}

		closeDatabase(db);

		return 0;
	}
}

void createEntry(sqlite3 *db, char* key)
{
	char date[11] = "";
	struct entry buffer;

	printf("Date of the entry (type \"now\" for today): ");
	scanf("%10s", buffer.header.date);

	getEntry(buffer.content, ENTRY_SIZE);

	addEntry(&buffer, db, key);
}

void openEntry(sqlite3* db, char* key)
{
	int ID;
	struct entry buffer;
	buffer.header.ID = 0;

	printf("ID of the entry you want to read: ");
	scanf("%d", &ID);

	readEntry(ID, &buffer, db, key);

	printf("-----%s-----\n\n%s\n\n--------------------\n", buffer.header.date, buffer.content);
}

void listEntries(sqlite3* db)
{
	struct entryHeaderArray array;
	int month;

	do
	{
		printf("Enter month (integer 1 - 12): ");
		scanf("%d", &month);
	}while(month < 1 || month > 12);

	getEntriesByMonth(2015, month, &array, db);

	printf("%d Entr(y)(ies) this month: \n", array.bufferIndex);

	for(int i = 0; i < array.bufferIndex; i++)
		printf("ID: %d; date: %s\n", array.buffer[i].ID, array.buffer[i].date);
}

void modifEntry(sqlite3* db, char* key)
{
	int id = 0;
	printf("ID of the entry you want to edit: ");

	scanf("%d", &id);

	struct entry e;
	e.header.ID = 0;
	readEntry(id, &e, db, key);

	editEntry(e.content, ENTRY_SIZE);
	updateEntry(&e, db, key);
}
