#include <stdio.h>
#include <string.h>

#include "KTest.h"

#define PKT_KEYBOARD 24

  int buftoint(unsigned char* buf) {
    return (int)(((unsigned)buf[0] << 24) 
                 | ((unsigned)buf[1] << 16) 
                 | ((unsigned)buf[2] << 8) 
                 | ((unsigned)buf[3]));
  }

int main(int argc, char* argv[]) {

  if (argc < 2) {
    fprintf(stderr, "Usage: %s [ktest-file]\n",argv[0]);
    return 1;
  }

  KTest *kt = kTest_fromFile(argv[1]);

  int round=0;
  int c2s=0;
  int s2c=0;
  int key=0;
  unsigned i;

  for (i=0; i<kt->numObjects; ++i) {
    //printf("i=%d, name=%s\n",i,kt->objects[i].name);
    unsigned char* buf = kt->objects[i].bytes;
    /*
       int j=0;
       for (;j<kt->objects[i].numBytes;++j)
       printf("%d:", kt->objects[i].bytes[j]);
       printf("\n");
    /**/

    if (!strcmp("s2c", kt->objects[i].name)) s2c++;

    int index;
    unsigned char type;
    if (!strcmp("c2s", kt->objects[i].name)) {
      c2s++;
      index = ((unsigned char)buftoint(buf+8))+12;
      type = buf[index];
      if (type == PKT_KEYBOARD) {
        //printf("KEYBOARD!\n");
        key++;
      }
    }

    if (buftoint(buf) != round) {
      printf("STATS %d %d %d %d\n", round, s2c, c2s, key);
      round = buftoint(buf);
      c2s = s2c = key = 0;
    }
  }
  return 0;
}
