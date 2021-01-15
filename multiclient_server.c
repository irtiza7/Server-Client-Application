#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg) {
    perror(msg);
    exit(1);
}


int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    int input, product;
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    if (argc < 2) {
         fprintf(stderr, "ERROR, no port provided\n");
         exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
         error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = portno; // htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    int pid;
    while (1) 
    {
         newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
         if (newsockfd < 0)
                error("ERROR on accept");
         pid = fork();
         if (pid < 0) {
              error("ERROR in new process creation");
         }
         if (pid == 0) 
	 {
            //child process
            close(sockfd);
	    do
	    {
              product = 0;
		    n = read(newsockfd, &input, 1);
		    printf("Client Request: %d\n", input);
              
              product = input;

              for(int junk = 0; junk < 2; junk++){
                   product = product*input;
              }
              printf("Server Response: %d\n", product);
		    n = write(newsockfd, &product, 1);
	    } while (input != 0);
            close(newsockfd);
	    return 0;
          } 
	  else       //parent process
             close(newsockfd);
    }
   return 0;
}

