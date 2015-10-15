#include "database.h"
#include "calendar.h"
#include "editor.h"
#include <ncurses.h>
#include <locale.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

static struct entry currentEntry;
static struct date currentDate;
static struct entryHeaderArray entriesOfMonth;
static int scrollOffset;
static sqlite3* db;

static void dateToChar(struct date d, char* dc);
static void drawCalendar(WINDOW* w);
static void printCurrentEntry(WINDOW* w);
static void loadEntry(char* key);
static void updateEntriesOfMonth();
static int entryToDate(struct date d);
static void newEntry(char* key);
static void modifyEntry(char* key);
static void deleteEntry();
static void initNcurses();

void ncInterface(sqlite3* database, char* key);

