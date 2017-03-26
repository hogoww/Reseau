/* gcc -Wall -ansi -pedantic -std=c99 -o client client.c */
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>


int main(int argc,char**argv){
  if(argc!=4){
    fprintf(stderr,"usage : ./main adresse port message\n");
    return 1;
  }
  
  int sock=socket(PF_INET,SOCK_STREAM,0);
  
  
  struct sockaddr_in addr;
  memset(&addr,0,sizeof(addr));
  addr.sin_family=AF_INET;
  inet_pton(AF_INET,argv[1],&(addr.sin_addr));
  addr.sin_port=htons(atoi(argv[2]));


  int servDesc=connect(sock,(struct sockaddr*)&addr,sizeof(addr));
  if(-1==servDesc){
    fprintf(stderr,"problème connect : %s.\n",strerror(errno));
    exit(1);
  }

  int taille=0;
  while(argv[3][taille]!='\0'){++taille;}  
  if(taille>=500){
    printf("nop");
    return 2;
  }
  

  ssize_t res=send(sock,argv[3],taille,0);
  if(-1==res){
    fprintf(stderr,"problème send : %s.\n",strerror(errno));
    exit(1);    
  } 

  printf("send : %s\n",argv[3]);

  int buffer[1];
  res=recv(sock,buffer,sizeof(buffer),0);
  if(-1==res){
    fprintf(stderr,"problème reception : %s.\n",strerror(errno));
    exit(1);
  }
  printf("%d",buffer[1]);
  
  close(sock);
  return 0;
}
