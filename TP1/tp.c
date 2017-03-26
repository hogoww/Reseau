#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

int main(int argc, char** argv)
{ 
  if (argc != 2) {
    fprintf(stderr,"Nombre d'argument incorrect !\n");
    return 1;
  }
  
  //Ex_3
  struct addrinfo *res;
  char ipEnChar[INET6_ADDRSTRLEN];//on utilise la valeur de la taille d'une adresse définie par une macro dans le .h
  

  int s=getaddrinfo(argv[1], NULL,NULL, &res); /* Nom du domaine, services (non nécessaires) , flags de masquages (non nécessaires), resultat récupéré dans la structure*/

  if (s != 0) {//gestion d'erreur standart
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
    return 1;
  }

  printf("\nAdresses IP du nom de domaine %s:\n\n", argv[1]);
  
  struct addrinfo *iterator;
  bool done=false;
  while(!done)
    {
      
      for(iterator = res;iterator != NULL; iterator = iterator->ai_next) {/*On itère sur la structure récursive du champ ai_addr qui renvois également une structure addrinfo*/
	void *addr;/* permet de récuperer une structure dont on ne connait pas le type en dehors d'un blon*/
	char* typeIP;
    

	struct sockaddr *sa;
	socklen_sa_t len_sa=sizeof(struct sockaddr);
	char hbuf[NI_MAXHOST];//macro qui contient la taille maximale de l'host, prédéfinie dans le .h
    
	if (iterator->ai_family == AF_INET)
	  {
	    struct sockaddr_in *ipv4 = (struct sockaddr_in *)iterator->ai_addr;
	    addr=&(ipv4->sin_addr);
	    typeIP="IPv4";
	  } 
	else 
	  {
	    struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)iterator->ai_addr;
	    addr = &(ipv6->sin6_addr);
	    typeIP="IPv6";
	  }
	sa=iterator->ai_addr;//Pour chaque nom on recupère la structure la qui contient l'adresse IP, permet de la passer directement à getnameinfo.



	if(getnameinfo(sa, len_sa, hbuf, sizeof(hbuf),NULL, 0, NI_NAMEREQD))//Renvois 0 si ça s'est bien passé seulement.
	  printf("GetNameInfo n'as pas réussi à trouver l'adresse correspondant à l'ip.");
	else
	  printf("host=%s\n", hbuf);
        
	//Ex_7
	inet_ntop(iterator->ai_family, addr, ipEnChar, sizeof ipEnChar);//transforme l'adresse IP depuis binaire en entier.(Comme quoi ça sert de lire tout le tp!)  
	printf("  %s: %s\n", typeIP,ipEnChar);
      }
  
      printf("\n");
      freeaddrinfo(res);//pas besoin de free iterator, il finit à NULL. En plus, il itérait sur res, donc toute les ai_next sont libérées aussi par freeaddrinfo.
    }  
  
  return 0;
}



