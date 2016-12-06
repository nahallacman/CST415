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

//#include "Message.h"
#include <Common/Message.h>


#include <algorithm>    // std::min



#include <fcntl.h>

using std::string;
using std::cout;
using std::endl;


using std::min;

const int bufferRetrieveSize = 1500;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
	int bytesRead = 1; //non-zero inialization so first check won't fail
	//char inBuffer[1024];

	const int bufferSize = 128*1024;
	char inBuffer[bufferSize];
	char outBuffer[256];

	ofstream logFile;
	logFile.open("Lab4.Scenario1.BarkmanC.txt"); //make sure to open the log before using it!

	Message recievedMessage(&logFile);
	Message returnMessage;


	bool doneSending = false;






	const int maxMessageSize = 146;
	int sendCountTotal = 0;










	int writeSize = 0;









	milliseconds startTime = duration_cast< milliseconds >(
	    system_clock::now().time_since_epoch()
	);















	char tempBuffer[150] = {0};
	int i = 0;
	string testString;

     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n = 0; 
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






	//set the socket to non-blocking so we don't wait forever on a read!
	//fcntl(newsockfd, F_SETFL, O_NONBLOCK);


	//variables for getting the local port number
	int local_port;
	sockaddr_in sin;
	socklen_t addrlen;

	//get the local port number
	addrlen = sizeof(sin);
	if(getsockname(sockfd, (struct sockaddr *)&sin, &addrlen) == 0 &&
	   sin.sin_family == AF_INET &&
	   addrlen == sizeof(sin))
	{
	    local_port = ntohs(sin.sin_port);
	}
	else
		error("ERROR getting local port number"); // handle error







	//for (i = 0; i < 4; i++)
	//while(bytesRead > 0)
	//{
	while(!doneSending)
	{
		int sendCount = 0;
/*
	     bzero(buffer,bufferRetrieveSize);
	     n = read(newsockfd,buffer,bufferRetrieveSize); // reads from the socket. If it returns a 0, there is no more data.
		
	     if (n < 0) error("ERROR reading from socket");
		//testString = buffer;
		//cout << "Here is the message: (";
		//fwrite(buffer, sizeof(char), 146, stdout);
		//cout << endl;

//<< buffer[0] << buffer[1] << " bytes) " << string(testString.begin()+2, testString.begin()+146) << endl;
	     //printf("Here is the message: (%d%d bytes) %s\n",buffer[0],buffer[1],printf("%s", buffer));

	     n = write(newsockfd,"I got your message",18);
	     if (n < 0) error("ERROR writing to socket")
*/

		// --- receive ---
		bytesRead = read(newsockfd,inBuffer, bufferSize);
		//printf("After Read Debug Message");
		if (bytesRead < 0)
		{ 
			//error("ERROR reading from socket");
			cout << "Non-blocking read was empty." << endl;
		}
		else if(bytesRead > bufferSize) //7*146 = 1022
		{
			error("ERROR reading from socket, over read performed.");
		}
		else
		{
			bool reading = true;
			int endOfLastMessage = 0;
			int readIndex = 0;
			char tempBuf[147];
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
						int loopIndex = endOfLastMessage;
						numSeparators = 0;
						//finished with a message from endOfLastMessage to readIndex
						//for(int loopIndex = endOfLastMessage; loopIndex < readIndex+1; loopIndex++)

						while(loopIndex < readIndex+1)
						{
							tempBuf[loopIndex - endOfLastMessage] = inBuffer[loopIndex];
							loopIndex++;
						}
						endOfLastMessage = loopIndex+2;
						//if(endOfLastMessage > 2048) //make buffer circular
						//{
						//	endOfLastMessage = endOfLastMessage - 2048;
						//}

						

						// --- Take buffer and convert into a Message
						recievedMessage.buildFromReturnString(tempBuf, '1');
						
						// --- write returned message to log
						recievedMessage.writeToLogFile();

						//TODO: remove debug printing
						//fwrite(recievedMessage.getRequestMessage(), sizeof(char), (recievedMessage.getRequestMessage()[1] < 255) ? recievedMessage.getRequestMessage()[1] : 255 , stdout); //a bit sketchy but at least it has a max size check. If the target size is less than 255 there could be issues.
						//cout << endl;

						returnMessage = recievedMessage;
						//TODO: update time stamp, plus others probably

						returnMessage.setMSTimeStamp(returnMessage.getCurrentMSTimeString(startTime));
						returnMessage.setMessageType("RSP");
						//returnMessage.setOutgoingPort(local_port);
						//returnMessage.setSocketNum(newsockfd);
						returnMessage.formRequestMessage();//TODO: am I accidentally calling this twice?

						// --- write output message log ---
						returnMessage.writeToLogFile();


						if( strlen(returnMessage.getRequestMessage()) < 146 )
						{
							writeSize = strlen(returnMessage.getRequestMessage());//there will possibly be a null in the first spot...
						}
						else
						{
							writeSize = 146;
						}

						// --- form message to send ---
						memcpy( outBuffer, returnMessage.getRequestMessage(), 146 );
						// --- actual send ---
	

						n = write(newsockfd,outBuffer,writeSize);
						if (n < 0) error("ERROR writing to socket");

						//TODO: fix this feature
						//increase the send count so packets can be sent back when there is time.
						sendCount++;

						//TODO: remove this debug output.
						//cout << "Recieved response #" << sendCount << "." << endl;
					}
				}

				readIndex++;
				if(readIndex >= endOfLastMessage+maxMessageSize)
				{
					reading = false; // we have (probably) reached the end of the last message we will find in the buffer
				}
			}

			// --- clear buffer just in case ----
	    		bzero(inBuffer,bufferSize);
		}
		if(sendCount > 0)
		{
/*
			// --- form message to send ---
			memcpy(outBuffer, returnMessage.getRequestMessage(), 146);
			// --- actual send ---
	    		n = write(newsockfd,outBuffer,146);
	    		if (n < 0) error("ERROR writing to socket");
*/

			sendCountTotal++;
			sendCount--;
		}

		//TODO: If too much time has passed, or there has been 5,000 transactions, stop.
		if(sendCountTotal >= 5000)
		{
			doneSending = true;
		}

	}

     close(newsockfd);
     close(sockfd);
     return 0; 
}
