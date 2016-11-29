#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


#include "Message.h"


void error(const char *msg)
{
    perror(msg);
    exit(0);
}


int main(int argc, char *argv[])
{
	ofstream logFile;
	logFile.open("Lab3_Scenario1.log"); //make sure to open the log before using it!

	int local_port;
	sockaddr_in sin;
	socklen_t addrlen;


	string messageToSend;
	string testString;
	string testString2;
	unsigned char length[3];
	length[0] = 0;
	length[1] = 144;
	length[2] = 0;
	char timeStamp[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	Message testMessage;
	Message returnMessage(&logFile);
	Message realMessage(
			//"testLog.log",
			&logFile,
			length,
			"REQ",
			timeStamp,
			"11111111111111111111",
			"BarkmanC            ",
			"17-7918",
			"00000",
			//"10.1.20.29     ",
			"10.1.20.29     ", 
			"17784",
			"    3",
			"192.168.101.220",
			"02605",
			"ABCDEFGHIJKLMNOPQRST",
			'1' );

	int i; i = 0;
	char message[256];
	int outgoingPortNum;

    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];

//	printf("Current time in MS: %s!\n", realMessage.getCurrentMSTimeString());

    	bzero(buffer,256);

    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
	outgoingPortNum = connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) ;
    if ( outgoingPortNum < 0) 
        error("ERROR connecting");


	addrlen = sizeof(sin);
	if(getsockname(sockfd, (struct sockaddr *)&sin, &addrlen) == 0 &&
	   sin.sin_family == AF_INET &&
	   addrlen == sizeof(sin))
	{
	    local_port = ntohs(sin.sin_port);
	}
	else
		error("ERROR getting local port number"); // handle error

	for(i = 0; i < 100; i++)
	{
		printf("sending message iteration #: %d\n", i);

		realMessage.setMSTimeStamp(realMessage.getCurrentMSTimeString());
		realMessage.setOutgoingPort(local_port);
		realMessage.setSocketNum(sockfd);

		realMessage.formRequestMessage();

	// --- TODO: write output message log ---
		realMessage.writeToLogFile();

		cout << "Socket file descriptor:" << sockfd << endl;
		cout << "Outgoing Port Number:" << local_port << endl;

		memcpy(buffer, realMessage.getRequestMessage(), 146);

		printf("buffer[0]: 0x%.2X", (unsigned char)buffer[0]);
		printf(" buffer[1]: 0x%.2X", (unsigned char)buffer[1]);
		printf(" buffer[144]: 0x%.2X\n", (unsigned char)buffer[144]);

		cout << "Buffer to be sent:\n";
		fwrite(buffer, sizeof(char), 146, stdout);
		cout << endl;

		cout << "Buffer to be sent in hex:\n";
		for(int i =0; i < 146; i++)
		{
			printf("%.2X ", (unsigned char)buffer[i]);
		}
		cout << endl;

	// --- actual send ---
	    n = write(sockfd,buffer,146);
	    if (n < 0) 
		 error("ERROR writing to socket");
	    bzero(buffer,256);

	// --- receive ---
	    n = read(sockfd,buffer,255);
		//printf("After Read Debug Message");
	    if (n < 0) 
		 error("ERROR reading from socket");
	    //printf("%s\n",buffer);

	// --- display results for debugging ---
		printf("buffer[0]: 0x%.2X", (unsigned char)buffer[0]);
		printf(" buffer[1]: 0x%.2X", (unsigned char)buffer[1]);
		printf(" buffer[%d]: 0x%.2X\n", (unsigned char)buffer[1], (unsigned char)buffer[buffer[1]]+1);

		fwrite(buffer, sizeof(char), (buffer[1] > 255) ? buffer[1] : 255 , stdout); //a bit sketchy but at least it has a max size check. If the target size is less than 255 there could be issues.
		cout << endl;

	// --- Take buffer and convert into a Message
		returnMessage.buildFromReturnString(buffer);

	// --- TODO: write returned message to log
		returnMessage.writeToLogFile();

	// --- clear buffer just in case ----
    		bzero(buffer,256);
	}
    close(sockfd);

	realMessage.writeRecordTrailerToLog();
	logFile.close();

    return 0;
}
