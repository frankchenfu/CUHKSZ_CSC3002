/* P3:
 * File: calendar.cpp
 * ------------------
 * This file implements the p1calendar.h interface
 */

#include <string>
#include <iostream>
//#include "CSC3002OJActive/assignment2/lib.h" // for OJ test
//#include "CSC3002OJActive/assignment2/calendar.h" // for OJ test
//#include "lib.h" // For local test
#include "calendar.h" // For local test
using namespace std;

Month stringToMonth(string s)
{
   if (s == "JANUARY") {
      return JANUARY;
   }
   if (s == "FEBRUARY") {
      return FEBRUARY;
   }
   if (s == "MARCH") {
      return MARCH;
   }
   if (s == "APRIL") {
      return APRIL;
   }
   if (s == "MAY") {
      return MAY;
   }
   if (s == "JUNE") {
      return JUNE;
   }
   if (s == "JULY") {
      return JULY;
   }
   if (s == "AUGUST") {
      return AUGUST;
   }
   if (s == "SEPTEMBER") {
      return SEPTEMBER;
   }
   if (s == "OCTOBER") {
      return OCTOBER;
   }
   if (s == "NOVEMBER") {
      return NOVEMBER;
   }
   if (s == "DECEMBER") {
      return DECEMBER;
   }
}

/* TODO PART:
 * Implementation notes: monthToString
 * -----------------------------------
 * The monthToString function must return some value if the month does not
 * match any of the enumeration constants.  Here, as in the Direction
 * type, the function returns ???.
 */
string monthToString(Month month)
{
	string months[]={"JANUARY", "FEBRUARY", "MARCH", "APRIL", "MAY", "JUNE",
		"JULY", "AUGUST", "SEPTEMBER", "OCTOBER", "NOVEMBER", "DECEMBER"};
	if(month>=1&&month<=12)
		return months[month-1];
	else
		return "???";
}

Month operator++(Month &month, int)
{
	Month tmp=month;
	if (month == DECEMBER)
		month = JANUARY;
	else
		month = Month(month + 1);
	return tmp;
}


/*
 * Implementation notes: Constructors
 * ----------------------------------
 * There are three constructors for the Date class.  The default
 * constructor creates a Date with a zero internal value that must
 * be assigned a new value before it is used.  The others initialize
 * the date from the arguments by calling the private initDate method.
 */

Date::Date()
{
   initDate(1, JANUARY, 1900);
}

Date::Date(int day, Month month, int year)
{
   initDate(day, month, year);
}

Date::Date(Month month, int day, int year)
{
   initDate(day, month, year);
}

/*
 * TODO function
 * Implementation notes: getDay, getMonth
 * --------------------------------------
 * 
 */

int Date::getDay()
{
	return day;
}

/*
 * TODO function
 * Method: getMonth
 * Usage: Month month = date.getMonth();
 * -------------------------------------
 * Returns the month.
 */

Month Date::getMonth()
{
	return month;
}

int Date::getYear()
{
	return year;
}

/*
 * TODO function
 * Implementation notes: toString
 * ------------------------------
 * The toString method uses the getters to perform the translation into
 * day/month/year values.
 */

string Date::toString()
{
	string abbr[]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
	return to_string(day)+"-"+abbr[month-1]+"-"+to_string(year);
}

void Date::initDate(int day, Month month, int yyyy)
{
	this->day=day;
	this->month=month;
	this->year=yyyy;
}

inline int days_passed(Date date)
{
	int day=date.getDay(), month=date.getMonth(), year=date.getYear();
	int days=day;
	for(int i=1;i<month;i++)
		days+=daysInMonth(Month(i),year);
	return days-1;
}
Date operator+(Date date, int delta)
{
	if(delta<0)
		return date-(-delta);
	delta+=days_passed(date);
	date=Date(JANUARY,1,date.getYear());
	int day=date.getDay(), month=date.getMonth(), year=date.getYear();
	while(delta>=(isLeapYear(year)?366:365)){
		delta-=(isLeapYear(year)?366:365);
		year++;
	}
	while(delta>=daysInMonth(Month(month),year)){
		delta-=daysInMonth(Month(month),year);
		month++;
	}
	day+=delta;
	return Date(day,Month(month),year);
}
Date operator-(Date date, int delta)
{
	delta-=days_passed(date);
	date=Date(JANUARY,1,date.getYear());
	while(delta>0){
		date=Date(JANUARY,1,date.getYear()-1);
		delta-=(isLeapYear(date.getYear())?366:365);
	}
	return date+(-delta);
}

int operator-(Date d1, Date d2)
{
	if(d1<d2)
		return -(d2-d1);
	int days=days_passed(d1)-days_passed(d2);
	int y1=d1.getYear(), y2=d2.getYear();
	for(int i=y2;i<y1;i++)
		days+=(isLeapYear(i)?366:365);
	return days;
}

Date &operator+=(Date &date, int delta)
{
	return date=date+delta;
}

Date &operator-=(Date &date, int delta)
{
	return date=date-delta;
}

Date operator++(Date &date)
{
	return date=date+1;
}

Date operator++(Date &date, int)
{
	Date tmp=date;
	date=date+1;
	return tmp;
}

Date operator--(Date &date)
{
	return date=date-1;
}

Date operator--(Date &date, int)
{
	Date tmp=date;
	date=date-1;
	return tmp;
}

bool operator==(Date d1, Date d2)
{
	return d1.getDay()==d2.getDay()&&d1.getMonth()==d2.getMonth()&&d1.getYear()==d2.getYear();
}

bool operator!=(Date d1, Date d2)
{
	return !(d1==d2);
}

bool operator<(Date d1, Date d2)
{
	if(d1.getYear()!=d2.getYear())
		return d1.getYear()<d2.getYear();
	if(d1.getMonth()!=d2.getMonth())
		return d1.getMonth()<d2.getMonth();
	return d1.getDay()<d2.getDay();
}

bool operator<=(Date d1, Date d2)
{
	return d1<d2||d1==d2;
}

bool operator>(Date d1, Date d2)
{
	return !(d1<=d2);
}

bool operator>=(Date d1, Date d2)
{
	return !(d1<d2);
}

std::ostream &operator<<(std::ostream &os, Date date){
	return os<<date.toString();
}

/*
 * TODO function
 * Implementation notes: daysInMonth
 * ---------------------------------
 * This function is a reasonably literal translation of the old rhyme:
 *
 *    Thirty days has September
 *    April, June, and November
 *    All the rest have 31
 *    Excepting February alone
 *    Which has 28 in fine
 *    And each leap year 29
 */

int daysInMonth(Month month, int year)
{
	int days[]={31,28,31,30,31,30,31,31,30,31,30,31};
	if(month==FEBRUARY&&isLeapYear(year))
		return 29;
	else
		return days[month-1];
}

/* TODO PART:
 * Implementation notes: isLeapYear
 * --------------------------------
 * This function simply encodes the rule for determining leap years:
 * a leap year is any year divisible by 4, except for years ending in 00,
 * in which case the year must be divisible by 400.
 */

bool isLeapYear(int year)
{
	return year%4==0&&year%100!=0||year%400==0;
}

/* DO NOT modify this main() part */
int main()
{
   int id;
   cin >> id;
   int day, year;
   string mon;
   cin >> day >> mon >> year;

   if (id == 1)
   {
      for (Month month = JANUARY; month <= DECEMBER; month = Month(month + 1))
      {
         cout << monthToString(month) << " has " << daysInMonth(month, year)
              << " days." << endl;
      }
   }
   else if (id == 2)
   {
      Date moonLanding(day, stringToMonth(mon), year);
      cout << "moonLanding = " << moonLanding.toString() << endl;
   }
   else if (id == 3)
   {
      Date moonLanding(day, stringToMonth(mon), year);
      cin >> mon >> day >> year;
      Date kennedyAssassination(stringToMonth(mon), day, year);
      cin >> mon >> day >> year;
      Date newYearsEve(stringToMonth(mon), day, year);
      cin >> day >> mon >> year;
      Date inaugurationDay(day, stringToMonth(mon), year);
      cin >> day >> mon >> year;
      Date electionDay(day, stringToMonth(mon), year);
      cout << "moonLanding = " << moonLanding << endl;
      cout << "kennedyAssassination = " << kennedyAssassination << endl;
      cout << boolalpha;
      cout << "moonLanding < kennedyAssassination = "
           << (moonLanding < kennedyAssassination) << endl;
      cout << "moonLanding > kennedyAssassination = "
           << (moonLanding > kennedyAssassination) << endl;
      cout << "moonLanding == kennedyAssassination = "
           << (moonLanding == kennedyAssassination) << endl;
      cout << "moonLanding == moonLanding = "
           << (moonLanding == moonLanding) << endl;
      cout << "inaugurationDay - electionDay = "
           << (inaugurationDay - electionDay) << endl;
      Date day = newYearsEve;
      cout << "New Year's Eve = " << day++ << endl;
      cout << "New Year's Day = " << day << endl;
      for (Date d = electionDay; d <= inaugurationDay; d++)
      {
         cout << d << endl;
      }
   }
}
