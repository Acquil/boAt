//Custom system call
#include "types.h"
#include "user.h"
#include "date.h"

int
main(int argc, char *argv[])
{
  struct rtcdate r;
  
  if (date(&r)) {
    printf(2, "date failed\n");
    exit();
  }
    cmostime(&r);
    time_t total_time;
    struct tm *date;
    time(&total_time);
    date = gmtime(&total_time);

    r.second = date->tm_sec;
    r.minute = (date->tm_min + 30)%60;
    r.hour = (date->tm_hour + 5)%24;
    r.day = date->tm_mday;
    r.month = date->tm_mon;
    r.year = date->tm_year;

  
  printf(1, "Time\t%d/%d/%d\n",r.day,r.month,r.year);

  exit();
}
