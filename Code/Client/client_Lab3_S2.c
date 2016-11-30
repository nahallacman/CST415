#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <fcntl.h>

#include "Message.h"



//#include <chrono>
//#include <thread>


void error(const char *msg)
{
    perror(msg);
    exit(0);
}


int main(int argc, char *argv[])
{
	bool doneSending = false;
	int sendCount = 0;
	int recieveCount = 0;



	ofstream logFile;
	logFile.open("Lab3_Scenario2.log"); //make sure to open the log before using it!

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

	//int i; i = 0;
	char message[256];
	int outgoingPortNum;

	int bytesRead;

    int sockfd, portno, n;
	//int sockfd, portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char inBuffer[1024];
    char outBuffer[256];

//	printf("Current time in MS: %s!\n", realMessage.getCurrentMSTimeString());

    	bzero(inBuffer,1024);
    	bzero(outBuffer,256);

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




	//sockfd = socket(PF_INET, SOCK_STREAM, 0);
	//set the socket to non-blocking so we don't wait forever on a read!
	fcntl(sockfd, F_SETFL, O_NONBLOCK);

		realMessage.setOutgoingPort(local_port);
		realMessage.setSocketNum(sockfd);


	//for(i = 0; i < 100; i++)
	//{
	while(!doneSending)
	{
		if(sendCount < 100)
		{
			printf("sending message iteration #: %d\n", sendCount+1);
			if(sendCount == 25 | sendCount == 75)
			{
				realMessage.setResponseDelay("03000");
			}
			else
			{
				realMessage.setResponseDelay("00000");
			}

			realMessage.setMSTimeStamp(realMessage.getCurrentMSTimeString());

			realMessage.setRequestId(sendCount+1);

			realMessage.formRequestMessage();

			// --- write output message log ---
			realMessage.writeToLogFile();

			memcpy(outBuffer, realMessage.getRequestMessage(), 146);

			// --- actual send ---
	    		n = write(sockfd,outBuffer,146);
	    		if (n < 0) 
		 		error("ERROR writing to socket");
	    		bzero(outBuffer,256);
			sendCount++;
		}
	
		if(recieveCount < 101)
		{
			//TODO: recieving only works on individual messages currently, needs to be able to read in multiple messages to the buffer on a single read.
			// --- receive ---
			bytesRead = read(sockfd,inBuffer,1024);
			//printf("After Read Debug Message");
			if (bytesRead < 0)
			{ 
				//error("ERROR reading from socket");
				cout << "Non-blocking read was empty." << endl;
			}
			else if(bytesRead > 1022) //7*146 = 1022
			{
				error("ERROR reading from socket, unballanced read performed.");
			}
			else
			{
				bool reading = true;
				int endOfLastMessage = 0;
				int readIndex = 0;
				char tempBuf[146];
				int numSeparators = 0;
				//TODO: Interpret the read as separate messages
				//13 '|' per message
				while(reading)
				{
					if(inBuffer[readIndex] == '|')
					{
						numSeparators++;
						if(numSeparators > 11)
						{
							numSeparators = 0;
							//finished with a message from endOfLastMessage to readIndex
							for(int i = endOfLastMessage; i < readIndex; i++)
							{
								tempBuf[i - endOfLastMessage] = inBuffer[i];
							}
							endOfLastMessage = readIndex;

							// --- Take buffer and convert into a Message
							returnMessage.buildFromReturnString(tempBuf);

							// --- write returned message to log
							returnMessage.writeToLogFile();

							recieveCount++;
							cout << "Recieved response #" << recieveCount << "." << endl;
						}
					}
					if(readIndex >= bytesRead)
					{
						reading = false; // we have reached the end of the file that has been read.
					}
					readIndex++;
				}

				



				// --- clear buffer just in case ----
		    		bzero(inBuffer,1024);
			}
		}

		//this could go just after recieveCount++ to save a few cycles if things are not threaded
		if(recieveCount >= 100)
		{
			doneSending = true;
		}

		//delay(50 milliseconds)
		usleep(50000);
		//better way to do this in C++ '11
		//std::this_thread::sleep_for(std::chrono::milliseconds(x));
	}




    close(sockfd);

	// --- clean up everything, finish log ---
	realMessage.writeRecordTrailerToLog();
	logFile.close();

    return 0;
}
