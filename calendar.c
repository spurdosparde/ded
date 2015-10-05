#include "calendar.h"

struct date today()
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	struct date td;
	td.year = tm.tm_year + 1900;
	td.month = tm.tm_mon + 1;
	td.day = tm.tm_mday;
	return td;
}

int daysInMonth(int year, int month)
{
	switch(month)
	{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			return 31;
			break;

		case 4:
		case 6:
		case 9: 
		case 11:
			return 30;
			break;
		case 2:
			if(daysInYear(year) == 366)
				return 29;
			else
				return 28;
			break;
		default:
			//printf("%d is not a valid month\n", month);
			break;
	}
}

int daysInYear(int year)
{
	return 365 + 1 * ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
}

int firstDayOfMonth(struct date d)
{
	int nbDays = 0;
	for(int i = 1970; i < d.year; i++)
		nbDays += daysInYear(i);
	for(int i = 1; i < d.month; i++)
		nbDays += daysInMonth(d.year, i);
	nbDays ++;
	
	return (nbDays + 2) % 7;
}
