#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/ip.h> 
#include <netinet/in.h> 
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>


int main(int argc, char** argv){
  struct sockaddr_in servAdress;

  if(argc!=4){
    printf("usage: ./main [IPV4] [port] [entier à envoyer]\n");
    return 1;
  }



  int sockServ=socket(PF_INET,SOCK_DGRAM,0);
  
  servAdress.sin_family=AF_INET;
  inet_pton(AF_INET,argv[1],&(servAdress.sin_addr));
  servAdress.sin_port=htons(atoi(argv[2]));
  socklen_t length=sizeof(struct sockaddr);
  
  int end=atoi(argv[3]);
  int somme=0;
  for(int i=0;i<end;i++){
    somme+=i;

    int number=i;   
    ssize_t test=sendto(sockServ,&number,sizeof(number),0,(struct sockaddr*)&servAdress,length);
    
    if(test==-1){
      fprintf(stderr,"L'envois s'est mal passé : %s.\n",strerror(errno));
      break;
    } 
    if(!test==sizeof(number)){
      printf("tout n'as pas été envoyé ! %zd/%zd\n",test,sizeof(number));
      break;
    }
    else
      printf("Envoie #%d : Somme=%d\n",i,somme);
  }
  
  close(sockServ);
  return 0;
}
