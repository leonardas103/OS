#include "pm.h"
#include <minix/callnr.h>
#include <minix/com.h>
#include <signal.h>
#include "mproc.h"
#include "param.h"

#define SECS_IN_DAY    (24 * 3600)
#define DAYS_IN_YEAR   365

int cumulativeLeaps[49][2] = {
    {0,0}, //1970
    {0,0}, //1971
    {1,2}, //1972
    {2,3}, //1973
    {3,4}, //1974
    {4,5}, //1975
    {5,6}, //1976
    {6,7}, //1977
    {7,8}, //1978
    {8,9}, //1979
    {9,9}, //1980
    {10,10}, //1981
    {11,11}, //1982
    {12,12}, //1983
    {12,12}, //1984
    {13,13}, //1985
    {13,13}, //1986
    {13,14}, //1987
    {14,14}, //1988
    {14,15}, //1989
    {15,16}, //1990
    {16,16}, //1991
    {17,17}, //1992
    {18,18}, //1993
    {19,19}, //1994
    {19,20}, //1995
    {20,20}, //1996
    {21,21}, //1997
    {21,22}, //1998
    {22,22}, //1999
    {22,22}, //2000
    {22,22}, //2001
    {22,22}, //2002
    {22,22}, //2003
    {22,22}, //2004
    {22,23}, //2005
    {23,23}, //2006
    {23,23}, //2007
    {23,24}, //2008
    {24,24}, //2009
    {24,24}, //2010
    {24,24}, //2011
    {25,25}, //2012
    {25,25}, //2013
    {25,25}, //2014
    {26,26}, //2015
    {26,27}, //2016
    {27,27}, //2017
    {27,27}, //2018
};

//Returns 1 is a year is a leap year
static int isLeapYear (int x) {
    if ((x%400) == 0) {
        return 1;
    } 
    if ((x%100) == 0) {
       return 0;
    }
    return ((x%4) == 0);
}

static int secondsInYear (int y) {
    return SECS_IN_DAY * (DAYS_IN_YEAR + isLeapYear(y));
}

// Returns the number of seconds that have occured since 1970 given the unix time
static int leapSecs (int unixTime) {
    int leapSeconds = 0;
    int year = 1970;
    while (year < 2019 && unixTime > secondsInYear(year)) {
        unixTime -= secondsInYear(year);
        year++;
    }
    return cumulativeLeaps[year-1970][unixTime > secondsInYear(year)/2];
}

int do_utctime(void) {
    clock_t uptime, boottime;
    int s;

    //Get Time in seconds since 1970 - As in do_time()
    if ((s = getuptime2(&uptime, &boottime)) != OK) {
        panic("do_utctime couldn't get uptime: %d", s);
    }
    time_t unixTime = (time_t)(boottime + (uptime/system_hz));

    // Save time in message reply
    mp->mp_reply.reply_time = unixTime + leapSecs(unixTime);

    return(OK);
}