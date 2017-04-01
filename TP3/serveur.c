/* gcc -ansi -std=c99 -pedantic -Wall -o serveur serveur.c */

#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
//stringh pour errno...


int main(int argc,char** argv){
  int socketListen=socket(PF_INET,SOCK_STREAM,0);
  struct sockaddr_in addr;
  
  memset(&addr,0,sizeof(addr));
  addr.sin_family=AF_INET;
  addr.sin_addr.s_addr=INADDR_ANY;
  addr.sin_port=htons(12000);

  int descListen=bind(socketListen,(struct sockaddr*)&addr,sizeof(addr));
  if(-1==listen(socketListen,1)){
    fprintf(stderr,"problème listen : %s.\n",strerror(errno));
    exit(1);
  }
  printf("waiting for a message\n");
  
  struct sockaddr_in client;
  socklen_t sizeClient=sizeof(client);
  int descClient=accept(socketListen,(struct sockaddr*)&client,&sizeClient);
  if(-1==descClient){
    fprintf(stderr,"problème accept : %s.\n",strerror(errno));
    exit(1);
  } 
  printf("waiting for a message\n");
   
  char buffer[500];
  ssize_t res=recv(descClient,buffer,sizeof(buffer),0);
  if(-1==res){
    fprintf(stderr,"problème recv : %s.\n",strerror(errno));
    exit(1);
  }  
 char msg[res];
  int i=0;
  while(i<res){msg[i]=buffer[i];++i;}
  printf("%s\n",msg);
  int sizeMsg[1];
  sizeMsg[1]=res;
  ssize_t res2=send(descClient,sizeMsg,sizeof(sizeMsg),0);
  if(-1==res2){
    fprintf(stderr,"problème send : %s.\n",strerror(errno));
    exit(1);
  }


  close(descListen);
  close(socketListen);
  return 0;
}
