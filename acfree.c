#include <stdio.h>
#include <stdlib.h>
#include "fcgi_stdio.h"

char *strbuf(char *filename)
{
  FILE *f = fopen(filename      , "rb");
  fseek(f, 0, SEEK_END);
  long fsize = ftell(f);
  fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

  char *string = (char*)calloc(fsize + 1,sizeof(char));
  fread(string, fsize, 1, f);
  fclose(f);

  string[fsize] = 0;
  return string;
}

int main()
{
// char *bname=(char*)calloc(20,sizeof(char));
//  char *ass;
//  bname="ass.html";
//  ass=strbuf(bname);
// printf("%s", ass); 
    while(FCGI_Accept() >= 0)
    {
             printf("Content-type: text/html\r\nStatus: 200 OK\r\n\r\nHello World!");
    }
  return 0;
}
