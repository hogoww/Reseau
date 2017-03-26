/*gcc -Wall -ansi -std=c99 -pedantic serveur.c -o serveur*/

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


#include <signal.h>
/*pour arreter la boucle proprement.*/


typedef int boolean;

static int theSocket;
static int desc;
static int somme=0;
static int reception=0;

int buffer[1];

void sigINT_handler(int signo)
{
  if(signo == SIGINT){
    
    close(theSocket);
    close(desc);
    printf("\nLa socket à bien été fermée. Bonne journée!\n");
    exit(EXIT_SUCCESS);
  }
  else{
    printf("Le programme n'as pas été fermé correctement!\n");
    exit(EXIT_FAILURE);
  }
}

void sigQUIT_handler(int signo)
{
  if(signo == SIGQUIT){
    somme=0;
    reception=0;
    printf("\nLes compteurs ont bien été réinitialisés.\n");
  }
  else{
    printf("dafuck?\n");
    exit(EXIT_FAILURE);
  }
}



int main(int argc, char** argv){

  struct sockaddr_in a;

  if(argc!=1){
    fprintf(stderr,"L'activation du serveur ne demande pas d'arguments!\n");
    return 1;

  }
  if(signal(SIGINT, sigINT_handler) == SIG_ERR){
    fprintf(stderr,"Pas réussi à attribuer au signal INT sa trap\n");
    return 2;
  }    
  if(signal(SIGQUIT, sigQUIT_handler) == SIG_ERR){
    fprintf(stderr,"Pas réussi à attribuer au signal QUIT sa trap\n");
    return 3;
  }    



  theSocket=socket(PF_INET,SOCK_DGRAM,0);
  if(theSocket==-1){
    fprintf(stderr,"attribution du socket échouée.\n");
    return 4;
  }    
  

  a.sin_family=AF_INET;
  a.sin_addr.s_addr=INADDR_ANY;
  a.sin_port=htons((short)10000);

  desc=bind(theSocket,(struct sockaddr*)&a,sizeof(a));
  if(desc==-1){
    fprintf(stderr,"bind échouée.\n");
    return 5;
  }    

  printf("Utilisez ctrl-c pour arreter le programme proprement!\nUtiliser ctrl-\\ pour reset la somme.\n");
  
  
  while(1){
    printf("En attente d'un message!\n");
    struct sockaddr envoyeur;
    socklen_t len=sizeof(envoyeur);
    ssize_t recu=recvfrom(theSocket,buffer,sizeof(buffer),0,&envoyeur,&len);

    if(recu==-1){
      fprintf(stderr,"La reception s'est mal passé : %s.\n",strerror(errno));
    }
    else
      if(recu==sizeof(buffer)){
	somme+=buffer[0];	
	printf("Reception #%d : Somme=%d\n",reception,somme);
	reception++;
      }
      else{
	fprintf(stderr,"reçu seulement %zd/%zd octets\n",recu,sizeof(buffer));
	sigINT_handler(SIGINT);//ergo fin du prog
      }
  } 

  printf("Le programme n'as pas été fermé correctement!\n");
  return 3;
}
