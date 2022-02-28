#include <stdio.h>
#include <stdlib.h>
#include "fcgi_stdio.h"
#include <string.h>
#include <ctype.h>

const char *ENV_VARS[] = {
    "DOCUMENT_ROOT",
    "HTTP_COOKIE",
    "HTTP_HOST",
    "HTTP_REFERER",
    "HTTP_USER_AGENT",
    "HTTPS",
    "PATH",
    "QUERY_STRING",
    "REMOTE_ADDR",
    "REMOTE_HOST",
    "REMOTE_PORT",
    "REMOTE_USER",
    "REQUEST_METHOD",
    "REQUEST_URI",
    "SCRIPT_FILENAME",
    "SCRIPT_NAME",
    "SERVER_ADMIN",
    "SERVER_NAME",
    "SERVER_PORT",
    "SERVER_SOFTWARE"
};
/* Converts a hex character to its integer value */
char from_hex(char ch) {
  return isdigit(ch) ? ch - '0' : tolower(ch) - 'a' + 10;
}

/* Converts an integer value to its hex character*/
char to_hex(char code) {
  static char hex[] = "0123456789abcdef";
  return hex[code & 15];
}

/* Returns a url-encoded version of str */
/* IMPORTANT: be sure to free() the returned string after use */
char *url_encode(char *str) {
  char *pstr = str, *buf = malloc(strlen(str) * 3 + 1), *pbuf = buf;
  while (*pstr) {
    if (isalnum(*pstr) || *pstr == '-' || *pstr == '_' || *pstr == '.' || *pstr == '~') 
      *pbuf++ = *pstr;
    else if (*pstr == ' ') 
      *pbuf++ = '+';
    else 
      *pbuf++ = '%', *pbuf++ = to_hex(*pstr >> 4), *pbuf++ = to_hex(*pstr & 15);
    pstr++;
  }
  *pbuf = '\0';
  return buf;
}

/* Returns a url-decoded version of str */
/* IMPORTANT: be sure to free() the returned string after use */
char *url_decode(char *str) {
  char *pstr = str, *buf = malloc(strlen(str) + 1), *pbuf = buf;
  while (*pstr) {
    if (*pstr == '%') {
      if (pstr[1] && pstr[2]) {
        *pbuf++ = from_hex(pstr[1]) << 4 | from_hex(pstr[2]);
        pstr += 2;
      }
    } else if (*pstr == '+') { 
      *pbuf++ = ' ';
    } else {
      *pbuf++ = *pstr;
    }
    pstr++;
  }
  *pbuf = '\0';
  return buf;
}

void printEnvVars ()
{
    const char *format = "<p>%s : <strong>%s</strong></p>";
    int c = 0;
    for(; c < 20; ++c) {
        char *s = getenv(ENV_VARS[c]);
        if(s != NULL) {
            printf(format, ENV_VARS[c], getenv(ENV_VARS[c]));
        } else {
	    // no value
            printf(format, ENV_VARS[c], "(No value)");
        }
    }
}

int echofile(char *filename)
{
  FILE *f = fopen(filename      , "rb");
  fseek(f, 0, SEEK_END);
  long fsize = ftell(f);
  fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

  char *string = (char*)calloc(fsize + 1,sizeof(char));
  fread(string, fsize, 1, f);
  fclose(f);

  string[fsize] = 0;
  printf("%s",string);
  free(string);
  return 0;
}
int main()
{
    int count = 0;
    while(FCGI_Accept() >= 0)
    {
	    printf("Content-type: text/html\r\n"
	    "\r\n");
      char *ass;
      char *bname=(char*)calloc(32,sizeof(char));
      bname="header.tpl";
      echofile("header.tpl");
      printf("<h1>FastCGI Hello!</h1>"
               "Request number %d running on host <i>%s</i>\n",
                ++count, getenv("SERVER_NAME"));             ;
      echofile("new.html");
      char *page = (char*)calloc(64, sizeof(char));
      page = getenv("REQUEST_URI");
      char *left = (char*)calloc(64, sizeof(char));
      char *p1 = (char*)calloc(64, sizeof(char));
      p1 = strstr(page,"/"); 
//      int len = strlen(++p1);
      printf(p1);
//      p1[len] = 0;
      printf("%d",strlen(++p1));
      if (strcmp(p1,"post") == 0)
      {
        char * method = getenv("REQUEST_METHOD");
        if (!strcmp(method, "POST")) {
          int ilen = atoi(getenv("CONTENT_LENGTH"));
          char *bufp = malloc(ilen);
          fread(bufp, ilen, 1, stdin);
          printf("The POST data is<P>%s\n", url_decode(bufp));
          free(bufp);
        }
    }
      
	         else
      { 
    	printf("not found");
      }

//      p2 = strstr(found,"/");
//      char *found2 = ++p2;
//      printf("page:%s", found2);
       echofile("footer.tpl");
  }
    
  return 0;
} 
