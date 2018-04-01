#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main (void) {
    time_t now = time(NULL);
    time_t utc = utctime(NULL);
	
	printf("time    = %ld\n", (long)time);
	printf("utctime = %ld\n", (long)utctime);
    return 0;
}
