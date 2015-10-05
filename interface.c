#include "interface.h"

static char* monthesNames[12] =
{"January", "February", "March", "April", "May", "June", "July", "August", "September", "October","November", "December"};
static char* daysNames[7] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

void ncInterface(sqlite3* database, char* key)
{
	db = database;
	currentDate = today();

	updateEntriesOfMonth();

	initNcurses();

	bool cont = true;
	int input;

	while(cont)
	{
		erase();
		drawCalendar();
		loadEntry(key);
		printCurrentEntry();
		refresh();
		input = getch();

		switch(input)
		{
			case 113:
				cont = false;
				break;

			case 72:
				currentDate.year --;
				updateEntriesOfMonth();
				break;

			case 76:
				currentDate.year ++;
				updateEntriesOfMonth();
				break;

			case 104:
				currentDate.month = currentDate.month - 1;
				if(currentDate.month < 1)
				{
					currentDate.month = 12;
					currentDate.year --;
				}
				updateEntriesOfMonth();
				break;

			case 108:
				currentDate.month = currentDate.month + 1;
				if(currentDate.month > 12)
				{
					currentDate.month = 1;
					currentDate.year ++;
				}
				updateEntriesOfMonth();
				break;

			case 68:
				currentDate.day --;
				if(currentDate.day < 1)
					currentDate.day = daysInMonth(currentDate.year, currentDate.month);
				break;

			case 67:
				currentDate.day ++;
				if(currentDate.day > daysInMonth(currentDate.year, currentDate.month))
					currentDate.day = 1;
				break;

			case 65:
				if(currentDate.day > 7)
					currentDate.day -= 7;
				break;

			case 66:
				if(currentDate.day + 7 <= daysInMonth(currentDate.year, currentDate.month))
					currentDate.day += 7;
				break;

			case 126:
				deleteEntry();
				break;

			case 10:
				if(entryToDate(currentDate))
				{
					modifyEntry(key);
				}
				else
				{
					newEntry(key);
				}
				break;

			default:
				break;
		}
	}
	endwin();
}

static void dateToChar(struct date d, char* dc)
{
	char tmp[15 * 3 + 2];	//This way we are sure there won't be any overflow

	sprintf(tmp, "%d-%02d-%02d", d.year, d.month, d.day);
	strncpy(dc, tmp, 10);
	dc[11] = '\0';
}

static void updateEntriesOfMonth()
{
	getEntriesByMonth(currentDate.year, currentDate.month, &entriesOfMonth, db);
}

static void drawCalendar()
{
	/*char lineTmp[35];
	  for(int i = 0; i < entriesOfMonth.bufferIndex; i++)
	  {
	  sprintf(lineTmp, "ID: %d - Date: %s", entriesOfMonth.buffer[i].ID, entriesOfMonth.buffer[i].date);
	  mvprintw(i, 5, lineTmp);
	  }*/

	mvprintw(0,0,"%s - %d", monthesNames[currentDate.month - 1], currentDate.year);

	for(int i = 0; i < 7; i++)
		mvprintw(2, 6 * i + 2, "%s", daysNames[i]);

	int n = daysInMonth(currentDate.year, currentDate.month);
	int offset = firstDayOfMonth(currentDate);

	for(int i = 1; i <= n; i ++)
	{
		struct date d;
		d.year = currentDate.year;
		d.month = currentDate.month;
		d.day = i;

		if(i == currentDate.day)
			attron(A_REVERSE);
		mvprintw(3 + (i + offset - 1)/7, ((i + offset - 1)%7) * 6 + 2, "%d", i);
		attroff(A_REVERSE);
		if(entryToDate(d))
		{
			mvprintw(3 + (i + offset - 1)/7, ((i + offset - 1)%7) * 6 + 1, "%c", '*');
		}

	}
}

static void printCurrentEntry()
{
	mvprintw(10,0,"---------- %d-%02d-%02d ----------", currentDate.year, currentDate.month, currentDate.day);
	mvprintw(12,0,"%s", currentEntry.content);
}

static void loadEntry(char* key)
{
	int id = entryToDate(currentDate);

	if(id)
	{
		readEntry(id, &currentEntry, db, key);
	}
	else
	{
		strcpy(currentEntry.content,"*no entry at this date*\0");
	}
}

static int entryToDate(struct date d)
{
	for(int i = 0; i < entriesOfMonth.bufferIndex; i++)
	{
		int tmp1 = 0, tmp2 = 0, tmp3 = 0;
		sscanf(entriesOfMonth.buffer[i].date, "%d-%d-%d", &tmp1, &tmp2, &tmp3);
		if(tmp3 == d.day)
			return entriesOfMonth.buffer[i].ID;
	}
	return 0;
}

static void newEntry(char* key)
{
	endwin();
	struct entry buffer;
	
	dateToChar(currentDate, buffer.header.date);
	getEntry(buffer.content, ENTRY_SIZE);

	addEntry(&buffer, db, key);
	initNcurses();	//the editor is likely to destroy the ncurses context so we have to rebuild it
	updateEntriesOfMonth();
}

static void modifyEntry(char* key)
{
	endwin();
	editEntry(currentEntry.content, ENTRY_SIZE);

	updateEntry(&currentEntry, db, key);
	initNcurses();
	updateEntriesOfMonth();
}

static void deleteEntry()
{
	int id = entryToDate(currentDate);
	if(id)
	{
		delEntry(id, db);
	}

	updateEntriesOfMonth();
}

static void initNcurses()
{
	setlocale(LC_CTYPE, "fr_FR.UTF-8");
	initscr();
	//raw();
	noecho();
	cbreak();
}
