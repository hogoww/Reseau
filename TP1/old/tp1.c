

//Pour la compilation utilise la phrase suivante, normalement j'pense qu'il est installé de base sur mac
//gcc -Wall -o tp tp1.c

#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>

int main(int argc,char** argv){
  struct sockaddr *sa;    /* input */
  socklen_t len;         /* input */
  char hbuf[256], sbuf[256];
  
  if (getnameinfo(sa, len, hbuf, sizeof(hbuf), sbuf,
		  sizeof(sbuf), NI_NUMERICHOST | NI_NUMERICSERV) == 0)
    printf("host=%s, serv=%s\n", hbuf, sbuf);
  return 0;
}


/* struct sockaddr *sa;    /\\* input *\/ */
/* socklent_t len;         /\* input *\/ */
/* char hbuf[NI_MAXHOST]; */

/* if (getnameinfo(sa, len, hbuf, sizeof(hbuf), */
/*                 NULL, 0, NI_NAMEREQD)) */
/*     printf("could not resolve hostname"); */
/* else */
/*     printf("host=%s\n", hbuf); */
