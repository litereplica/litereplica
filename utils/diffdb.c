/*
** A utility for printing the differences between two SQLite database files.
*/
#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>


#define PAGESIZE 1024
FILE *db1;
FILE *db2;

int main(int argc, char **argv){
  int iPg, nAdd1=0, nAdd2=0, extra_info=0, i;
  unsigned char a1[PAGESIZE], a2[PAGESIZE];
  if( argc<3 ){
    fprintf(stderr,"Usage: %s FILENAME FILENAME\n", argv[0]);
    exit(1);
  }
  if( argc==4 ) extra_info = 1;
  db1 = fopen(argv[1], "r+b");
  if( db1 == 0 ){
    fprintf(stderr,"%s: can't open %s\n", argv[0], argv[1]);
    exit(1);
  }
  db2 = fopen(argv[2], "r+b");
  if( db2 == 0 ){
    fprintf(stderr,"%s: can't open %s\n", argv[0], argv[2]);
    exit(1);
  }
  iPg = 1;
  while( fread(a1, 1, PAGESIZE, db1)==PAGESIZE ){
    if ( fread(a2, 1, PAGESIZE, db2)==PAGESIZE ){
      if( memcmp(a1,a2,PAGESIZE) ){
        printf("Page %d\n", iPg);
        if (extra_info) {
          for (i=0; i < PAGESIZE; i++) {
            if (a1[i] != a2[i])
              printf("  %d %x (%c) %x (%c)\n", i, a1[i], a1[i], a2[i], a2[i]);
          }
        }
      }
      iPg++;
    } else {
      nAdd1++;
    }
  }
  while( fread(a2, 1, PAGESIZE, db2)==PAGESIZE ){
    nAdd2++;
  }
  printf("%d pages checked\n", iPg-1);
  if (nAdd1)
    printf("file 1 [%s] has %d more page(s)\n", argv[1], nAdd1);
  if (nAdd2)
    printf("file 2 [%s] has %d more page(s)\n", argv[2], nAdd2);
  fclose(db1);
  fclose(db2);
}
