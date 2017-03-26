/* gcc -Wall -ansi -pedantic -std=c99 -o main main.c */

#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h> 
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>


int main(int argc, char** argv){
  struct sockaddr_in servAdress;
  if(argc!=4){
    printf("usage: ./main [IPV4] [port] [chaine à envoyer]\n");
    return 1;
  }


  int socketRecepteur=socket(PF_INET,SOCK_DGRAM,0);
  int desc;

  struct sockaddr_in a;

  a.sin_family=AF_INET;
  a.sin_addr.s_addr=INADDR_ANY;
  short int i=10000;
  a.sin_port=htons(i);

  while(-1!=(desc=bind(socketRecepteur,(struct sockaddr*)&a,sizeof(a)))){
    printf("socket attrib\n");
    a.sin_port=htons(++i);
  }
  printf("%d\n",ntohs(a.sin_port));
  /* desc=bind(socketRecepteur,(struct sockaddr*)&a,sizeof(a)); */
  /* if(desc==-1){ */
  /*   a.sin_port=htons((short)10001); */
  /*     desc=bind(socketRecepteur,(struct sockaddr*)&a,sizeof(a)); */
  /*     if(desc==-1){ */
  /* 	fprintf(stderr,"bind échoué : %s.\n",strerror(errno)); */
  /* 	return 5; */
  /*     } */
  /* } */

  printf("appuyez sur \r quand vous etes prets à lancer le programme.\n");
  char s;
  scanf("%c",&s);

  int socketWhoWillReceive=socket(PF_INET,SOCK_DGRAM,0);
  
  servAdress.sin_family=AF_INET;
  inet_pton(AF_INET,argv[1],&(servAdress.sin_addr));
  servAdress.sin_port=htons(atoi(argv[2]));
  socklen_t length=sizeof(struct sockaddr);
  
  ssize_t len=0;
  while(argv[3][len]!='\0'){//recuperation de la taille de la chaine de char
    len++;
  }
  char* chaine=argv[3];
  ssize_t test=sendto(socketWhoWillReceive,chaine,len,0,(struct sockaddr*)&servAdress,length);
  if(test==-1){
    fprintf(stderr,"L'envois s'est mal passé : %s.\n",strerror(errno));
  } 
  if(!test==len){
    printf("tout n'as pas été envoyé ! %zd/%zd\n",test,len);
  }
  else
    printf("Envoie %s.\n",chaine);
  

  char buffer[500];
  for(int i=0;i<500;i++){
    buffer[i]=(char)0;
  }

  printf("En attente d'un message!\n");
  struct sockaddr envoyeur;
  socklen_t envoyeurLen=sizeof(envoyeur);
  ssize_t recu=recvfrom(socketRecepteur,buffer,sizeof(buffer),0,&envoyeur,&envoyeurLen);
  int len2=0;


  if(recu==-1){
    fprintf(stderr,"La reception s'est mal passé : %s.\n",strerror(errno));
  }
  else{
    printf("%s",buffer);
    /* if(recu==sizeof(buffer)){ */
    printf("Reception du message:\n");
    while(buffer[len2]!='\0'){
      printf("%c",buffer[len2]);
      len2++;
    }
    printf(" \n from :\n");
    char ipEnChar[INET_ADDRSTRLEN];//on utilise la valeur de la taille d'une adresse définie par une macro dans le .h
    inet_ntop(AF_INET,(struct sockaddrin*)&envoyeur,ipEnChar,sizeof(ipEnChar));
    printf("%s\n",ipEnChar);

  }
    /* else{ */
    /*   fprintf(stderr,"reçu seulement %zd/%zd octets\n",recu,sizeof(buffer)); */
    /* }  */


  close(socketWhoWillReceive);
  close(socketRecepteur);
  close(desc);
  return 0;
}
