#include <time.h>

struct date
{
	int year, month, day;
};

struct date today();
int daysInMonth(int year,int month); 
int daysInYear(int year);
int firstDayOfMonth(struct date d);
