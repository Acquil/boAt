#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

int ls_i=0;
int ls_l=0;

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void
ls(char *path)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    printf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    printf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    if(ls_l == 1)
      printf(1, "%s \t %d \t %d \t %d \t  %d/%d/%d, %d:%d:%d\n", fmtname(buf), st.type, st.ino, st.size, st.day, st.month, st.year, st.hour, st.minute, st.second);
    else if(ls_i == 1)
      printf(1, "%s \t %d\n", fmtname(buf), st.ino);
    else
      printf(1, "%s \n", fmtname(buf));
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf(1, "ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf(1, "ls: cannot stat %s\n", buf);
        continue;
      }
      if(ls_l == 1)
      printf(1, "%s \t %d \t %d \t %d \t  %d/%d/%d, %d:%d:%d\n", fmtname(buf), st.type, st.ino, st.size, st.day, st.month, st.year, st.hour, st.minute, st.second);
      else if(ls_i == 1)
      printf(1, "%s \t %d\n", fmtname(buf), st.ino);
      else
      printf(1, "%s \n", fmtname(buf));

    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  int i,j;
  int ls_file=1;

  for (i=1; i<argc; i++){
    for (j=0; j<strlen(argv[i]); j++)
    {
      if(argv[i][j] == '-')
      {
        if(argv[i][j+1] == 'l')
        {
          ls_l = 1;
          ls_file = i+1;
        }
        else if(argv[i][j+1] == 'i')
        {
          ls_i = 1;
          ls_file = i+1;
        }
      }
    }
  }


  if(argc == ls_file){
    ls(".");
    exit();
  } 
  for(i=ls_file; i<argc; i++)
    ls(argv[i]);
  exit();
}
