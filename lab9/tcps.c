#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>



char * ShowTCPAdd( struct sockaddr_in * a )
{
   static char tcpadd [ 16+6 ];
   sprintf( tcpadd, "%s:%d", inet_ntoa( a->sin_addr ), a->sin_port );
   return tcpadd;
}

void processConnection( int sock )
{
//Funkcja do obslugi nawiazanego polaczenia. Tutaj ma mala funkcjonalnosc - tylko odbiera dane i wyswietla na ekranie. Mozna napisac czesc odpowiedzialna za odpowiadanie do klienta uzywajac funkcji write. Serwer moze np zamieniac duze znaki na male i tak poprawiony tekst odsylac do klienta.

//  int n;

   // Bufor do przechowywania zawartosci pliku HTML
   char buffer[1024];

   // Naglowek HTTP
   char *http_interpret = "HTTP/1.0 200 OK\nContent-Type: text/html\n\n";
  
   // Otwarcie pliku HTML do odczytu
   int file_fd = open("site.html", O_RDONLY);

   // Zmienna do przechowywania ilosci odczytanych bajtow
   ssize_t bytes_read;

   // Interpretacja naglowka HTTP i wyslanie go do klienta
   ssize_t bytes_sent = send(sock, http_interpret, strlen(http_interpret), 0);

   // Odczytanie kodu HTML z pliku do bufora
   bytes_read = read(file_fd, buffer, 1024);

   // Wyslanie zawartosci pliku HTML do klienta
   ssize_t bytes_sent2 = send(sock, buffer, bytes_read, 0);
   
   // Zamknięcie pliku HTML
   close(file_fd);

   // do {
   //   n = read( sock, line, 1024 );
   //   if( n < 0 ) {
   //      printf( "Server child: read error\n" );
   //   } else if( n == 0 ) {
   //      printf( "Server child: connection terminated\n" );
   //   } else {
   //      line[n] = '\0';
   // printf( "Server child: received line: \n%s\n", line );

   //   }
   // }
   // while( n );


   // int bytes_sent = send(sock, page_buffer, nbytes, 0);
}


int main( int argc, char * argv [] )
{
   int Sock;

//Domyslnie serwer slucha na porcie 13333 i kazdym interfejsie sieciowym

   int Server_Port = 8888;

   int koniec = 0;
   struct sockaddr_in srvadd;
   struct sockaddr_in kliadd;

   printf("You can run this program as follows: %s [portnumber]\n", argv[0] );

//Utworzenie gniazda

   if( ( Sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP) ) < 0 ) {   // zamiast IPPROTO_TCP 0
      perror( "Can't create socket Sock" );
      return 1;
   }
   printf("Socket created, handle=%d\n", Sock );


//Jesli podano port na ktorym ma nasluchiwac serwer to bedzie wczytany

   if( argc > 1 ) Server_Port = atoi ( argv[ 1 ] );

//Przygotowanie struktury przechowujacej adres, na ktory bedzie dowiazany serwer (interfejs i port na ktorym nasluchuje)

   memset(&srvadd, 0, sizeof(struct sockaddr_in) );
   srvadd.sin_family = AF_INET;
   srvadd.sin_addr.s_addr = htonl(INADDR_ANY);
   srvadd.sin_port = htons((unsigned short) Server_Port);

//Dowiazanie gniazda

   if(bind(Sock, (struct sockaddr*) &srvadd, sizeof(srvadd) ) < 0) {
      perror("Error. Socket not bound.");
      close(Sock);
      return 1;
   }
   printf("Socket is bound.\n");


//Przjescie w stan oczekiwania na zgloszenia

   printf( "Listening ...\n" );
   listen( Sock, 2 );

   while(! koniec) {

      int childPid;
      int childSock;
      int kliaddlen;

      kliaddlen = sizeof( kliadd );

//Wywolanie funkcji accept, w ktorej proces serwera "zawisa" az do momentu pojawienia sie zadania obslugi (polaczenia od klienta)

      childSock = accept( Sock, (struct sockaddr *) &kliadd, &kliaddlen );

//Funkcja accept zwraca deskryptor nowego gniazda dedykowanego do obslugi polaczenia (zmienna childSock)

      printf( "Server accepted connection from %s\n", ShowTCPAdd( &kliadd ) );

      if( childSock < 0 ) {
         printf( "Server main: accept error\n" );
         koniec = 1;
      } else {

//Utworzenie procesu potomnego do obslugi polaczenia

         childPid = fork();
         if( childPid < 0 ) {
            printf( "Server child: fork error\n" );
            close( childSock );
            koniec = 1;
         }
         if( childPid == 0 ) {

//Potomek. Zamyka gniazdo "glowne", na ktorym nasluchuje serwer. Bedzie uzywal gniazda dedykowanego.

            close( Sock );

//Wejdz do funkcji obslugujacej polaczenie

            processConnection( childSock );
            close( childSock );
            printf( "Server child closed connection, exiting\n" );
            exit( 0 );
         } else {

//Proces macierzysty. Zamyka gniazdo dedykowane do obslugi polaczenia, po czym wraca w petli do funkcji accept i czeka na kolejne zgloszenia
            close( childSock );
         }
      }
   }
   close( Sock );
   while(wait(0)!=-1){};
   return 0;
}
