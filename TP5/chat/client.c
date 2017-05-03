// gcc -Wall -ansi -pedantic -std=c99 -o client client.c

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/select.h>
#include <signal.h>





#define SIZE_BUFF 100
//To be changed in the scanf. Can't do it if preproc

char* bufferSend;
char* bufferRecv;
int sock;


void sigINT_handler(int signo){
  if(signo == SIGINT){

    free(bufferRecv);
    printf("shutting down nicely, have a nice day :)\n");
    
    memset(bufferSend,0,SIZE_BUFF);
    bufferSend[0]='q';
    bufferSend[1]='u';
    bufferSend[2]='i';
    bufferSend[3]='t';
    send(sock,bufferSend,4,0);
    free(bufferSend);
    //close(sock);

  }
  exit(EXIT_SUCCESS);
}



int main(int argc,char** argv){

  if(argc!=3){
    fprintf(stderr,"usage : ./main adresse port\n");
    exit(EXIT_FAILURE);
  }

  if(signal(SIGINT, sigINT_handler) == SIG_ERR){
    fprintf(stderr,"Pas réussi à attribuer au signal INT sa trap\n");
    exit(EXIT_FAILURE);
  }

  

  sock=socket(PF_INET,SOCK_STREAM,0);

  struct sockaddr_in addr;
  memset(&addr,0,sizeof(addr));
  addr.sin_family=AF_INET;
  int checkReturn=inet_pton(AF_INET,argv[1],&(addr.sin_addr));
  if (checkReturn == 0){
    fprintf(stderr, "Adresse fournie format incorrect");
    exit(EXIT_FAILURE);
  }
  addr.sin_port=htons(atoi(argv[2]));
  
  
  int check=connect(sock,(struct sockaddr*)&addr,sizeof(struct sockaddr));
  if(-1==check){
    fprintf(stderr,"problème connect : %s.\n",strerror(errno));
    exit(EXIT_FAILURE);
  }


  fd_set set,settemp;
  FD_ZERO(&set);
  FD_SET(sock,&set);
  FD_SET(0,&set);

  bufferSend=malloc(sizeof(char)*SIZE_BUFF);
  bufferRecv=malloc(sizeof(char)*SIZE_BUFF);


  int max=sock;

  while(1){
    settemp=set;
    select(max+1,&settemp,NULL,NULL,NULL);
    
    if(FD_ISSET(0,&settemp)){

      memset(bufferSend,0,SIZE_BUFF);
      fgets(bufferSend,256,stdin);

      int i=0;
      while(i<SIZE_BUFF && bufferSend[i]!='\n'){//Enleve \n a la fin + envois de bit utiles
	i++;
	//printf("%d ",(int)bufferSend[i]);
      }
      /* printf(" %d\n",i); */

      //printf("%d",i);

      //printf("%s\n",bufferSend);
      ssize_t res2=send(sock,bufferSend,i,0);
      if(-1==res2){
	fprintf(stderr,"problème send : %s.\n",strerror(errno));
	//exit(EXIT_FAILURE);
      }

      //printf("%zd\n",res2);

      if(bufferSend[0]=='q' && bufferSend[1]=='u' && bufferSend[2]=='i' && bufferSend[3]=='t'){
	printf("leaving\n");
	sigINT_handler(SIGINT);
      }
      continue;
    }

      
    if(FD_ISSET(sock,&settemp)){
      memset(bufferRecv,0,SIZE_BUFF);
      ssize_t res=recv(sock,bufferRecv,SIZE_BUFF,0);
      if(-1==res){
	fprintf(stderr,"problème recv : %s.\n",strerror(errno));
	//exit(EXIT_FAILURE);
      }

      if(bufferRecv[0]=='1' && bufferRecv[1]=='5' && bufferRecv[2]=='4' && bufferRecv[3]=='2'){
	printf("Server just closed, have a nice day :) \n");
	exit(EXIT_SUCCESS);
      }
      

      /* int size=0; */
      /* while(bufferRecv[size]!='\0'){size++;} */
      /* char* buffer=malloc( */
        
      printf("%s\n",bufferRecv);
      exit(1);
    }
  }
}



  /* int c=fork(); */
  /* if(c==0){//ergo son, ergo sender */


  /*   bufferSend=malloc(sizeof(char)*SIZE_BUFF); */
  /*   while(1){ */
  /*     for(int j=0;j<SIZE_BUFF;++j){ */
  /* 	bufferSend[j]='\0'; */
  /*     } */
      
  /*     scanf("%99s",bufferSend); */

  /*     int i=0; */
  /*     while(i<100 && bufferSend[i]){i++;} */

  /*     printf("sending \"%s\"\n",bufferSend); */
  /*     ssize_t res2=send(sock,bufferSend,i,0); */
  /*     if(-1==res2){ */
  /* 	fprintf(stderr,"problème send : %s.\n",strerror(errno)); */
  /* 	exit(EXIT_FAILURE); */
  /*     } */

  /*     if(res2<1 && bufferSend[0]=='q'){ */
  /* 	printf("leaving\n"); */
  /* 	free(bufferSend); */
  /* 	exit(EXIT_SUCCESS); */
  /*     } */
  /*   } */
  /* } */
  /* else{//father */
/*     printf("receiving"); */
/*     int nbMessage=3;//timeoutstyle */
/*     bufferRecv=malloc(sizeof(char)*SIZE_BUFF); */
/*     while(nbMessage){ */
/*       printf("receiving"); */
/*       ssize_t res=recv(sock,bufferRecv,SIZE_BUFF,0); */
/*       if(-1==res){ */
/* 	fprintf(stderr,"problème recv : %s.\n",strerror(errno)); */
/* 	exit(EXIT_FAILURE); */
/*       } */
/*       printf("%s\n",bufferRecv); */
/*       --nbMessage; */
/*     } */
/*     printf("leaving2\n"); */
/*     close(sock); */
/*     return 0; */
/*   } */
/* } */




