/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <iostream>

using std::string;
using std::cout;
using std::endl;

const int bufferRetrieveSize = 1500;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
	char tempBuffer[150] = {0};
	int i = 0;
	string testString;

     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n = 1; //non-zero inialization so first check won't fail
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }

	//char *buff = "this is a test string";
	//printf("%.*s", 4, buff + 10);

     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");


	//for (i = 0; i < 4; i++)
	while(n > 0)
	{
	     bzero(buffer,bufferRetrieveSize);
	     n = read(newsockfd,buffer,bufferRetrieveSize); // reads from the socket. If it returns a 0, there is no more data.
	     
		
	     if (n < 0) error("ERROR reading from socket");
		//testString = buffer;
		cout << "Here is the message: (";
		fwrite(buffer, sizeof(char), 146, stdout);
		cout << endl;

//<< buffer[0] << buffer[1] << " bytes) " << string(testString.begin()+2, testString.begin()+146) << endl;
	     //printf("Here is the message: (%d%d bytes) %s\n",buffer[0],buffer[1],printf("%s", buffer));

	     n = write(newsockfd,"I got your message",18);
	     if (n < 0) error("ERROR writing to socket");
	}

     close(newsockfd);
     close(sockfd);
     return 0; 
}
