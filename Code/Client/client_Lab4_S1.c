#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <fcntl.h>

//#include "Message.h"
#include <Common/Message.h>


//for strtok
#include <cstring>
#include <iostream>

//for list
#include <list>

//#include <chrono>
//#include <thread>

//Global for testing and debugging, TODO: REMOVE
char strPtrArray[13][21];
int strArrayIndex;
char messageBuffer[146];
std::list<Message> MessageQueue;
ofstream logFile;


void error(const char *msg)
{
    perror(msg);
    exit(0);
}



int buildTokensIntoString()
{
	int index = 1;

	messageBuffer[0] = 0;
	for(int i = 0;  i < 13; i++)
	{
		for(int j = 0; j < 21 & strPtrArray[i][j] != 0; j++, index++)
		{
			messageBuffer[index] = strPtrArray[i][j];
		}
		messageBuffer[index] = '|';
		index++;
	}

	cout << messageBuffer[0] << messageBuffer+1 << endl;

	Message localMessage(&logFile);
	//MessageQueue.push_back(localMessage);
	
	return index;
}

/*
int processReadData(int bytesRead, int bufferSize, int maxMessageSize, char * inBuffer, int &unfinishedReadIndex, int &endOfLastMessage)
*/
int processReadData(char * inBuffer, int bytesRead)
{
	int recieveCount = 0;
	//int tokenIndex = 0;
	int amountProcessed = 0;
	int amountRead = 0;

	char *token = std::strtok(inBuffer+1, "|\0");

	while(amountRead < bytesRead - 1)
	{
		std::cout << token << '\t' << "strArrayIndex=" << strArrayIndex << '\n';
		//there is still stuff left, get the remaining tokens processed
		if(token == NULL)
		{
			
			//token = std::strtok(inBuffer+amountRead+1, "|\0");
			token = std::strtok(NULL, "|\0");
			amountRead++;
			//DEBUG!!:
			//return -1;
		}
		else
		{
			strncpy(strPtrArray[strArrayIndex], token, 20);
			amountRead += strlen(token);
			//increase the index
			if(strArrayIndex < 12){strArrayIndex++; amountRead++;}
			else{
				strArrayIndex = 0;// if we are done, process everything
				amountProcessed += buildTokensIntoString();
				recieveCount += 1;
			}
			//prepare for the next loop
			token = std::strtok(NULL, "|\0");
		}
		
	}

	//std::cout << token << '\t' << "strArrayIndex=" << strArrayIndex << '\n';

	return recieveCount;
}

void testProcessReadData()
{
	int processedCount = 0;
	strArrayIndex = 0;


	char testString2[] = "\0�RSP|1234567890|000000000000000000002|BarkmanC            |17-7918|00000|10.1.20.29     |45994|00004|192.168.101.220|02605|ABCDEFGHIJKLMNOPQRST|1|\0�RSP|9999999999|\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

	char testString3[] = "00000000000000000003|BarkmanC            |17-7918|00000|10.1.20.29     |45994|00004|192.168.101.220|02605|ABCDEFGHIJKLMNOPQRST|1|\0�RSP|0000045007|00000000000000000004|BarkmanC            |17-7918|00000|10.1.20.29     |45994|00004|192.168.101.220|02605|ABCDEFGHIJKLMNOPQRST|1|\0";





	logFile.open("Lab4.Scenario1.BarkmanC.txt"); //make sure to open the log before using it!

	//char testString[] = "\0�RSP|0000045007|00000000000000000001|BarkmanC            |17-7918|00000|10.1.20.29     |45994|00004|192.168.101.220|02605|ABCDEFGHIJKLMNOPQRST|1|\0";

	//processedCount += processReadData(testString, 146);
	//processReadData(bytesRead, bufferSize, maxMessageSize, testString, unfinishedReadIndex, endOfLastMessage);



	processedCount += processReadData(testString2, 163);

	cout << "Finished with read 1" << endl;

	processedCount += processReadData(testString3, 275);

	cout << "Finished with read 2" << endl;

	if(processedCount < 3)
	{
		cout << "error in number of strings procesed" << endl;
	}
}

int main(int argc, char *argv[])
{
	testProcessReadData();
}



/*
int main(int argc, char *argv[])
{
<<<<<<< HEAD
	const int maxMessageSize = 146;
	const int MaxNumberOfRuns = 10000;




	int unfinishedReadIndex  = 0;
	int endOfLastMessage = 0;













=======
>>>>>>> parent of 119a5f1... The actual traffic is correct, my interpretation of the traffic is incorrect on the client end.

	//char Times[100][10];





	milliseconds startTime = duration_cast< milliseconds >(
	    system_clock::now().time_since_epoch()
	);
	bool doneSending = false;
	int sendCount = 0;
	int recieveCount = 0;
	int lastRecieveCount = 0;



	ofstream logFile;
	logFile.open("Lab4.Scenario1.BarkmanC.txt"); //make sure to open the log before using it!

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

    //char inBuffer[1024];
	const int bufferSize = 128*1024;
	char inBuffer[bufferSize+1];
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
		if(sendCount < 5000)
		{
			printf("sending message iteration #: %d\n", sendCount+1); // TODO: remove this printing to improve performance
			if(sendCount == 25 | sendCount == 75)
			{
				realMessage.setResponseDelay("03000");
			}
			else
			{
				realMessage.setResponseDelay("00000");
			}

			//strncpy(Times[sendCount], realMessage.getCurrentMSTimeString(startTime), 10);
			realMessage.setMSTimeStamp(realMessage.getCurrentMSTimeString(startTime));

			realMessage.setRequestId(sendCount+1);

			realMessage.setScenarioNum('1');


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
	
		if(recieveCount < 5001)
		{
			//TODO: recieving only works on individual messages currently, needs to be able to read in multiple messages to the buffer on a single read.
			// --- receive ---
			bytesRead = read(sockfd,inBuffer,bufferSize);
			recieveCount += processReadData(bytesRead, bufferSize, maxMessageSize, inBuffer, unfinishedReadIndex, endOfLastMessage);

			if(unfinishedReadIndex > 0)
			{
				cout << "Partially read string detected" << endl;
				//endOfLastMessage to unfinished read index is now a partial string
				//bytesRead = read(sockfd,inBuffer+unfinishedReadIndex,bufferSize-unfinishedReadIndex);
				//recieveCount += processReadData(bytesRead+unfinishedReadIndex, bufferSize-unfinishedReadIndex, maxMessageSize-unfinishedReadIndex,  inBuffer+endOfLastMessage, unfinishedReadIndex, endOfLastMessage);
				//TODO: do I need to loop here?
			}
<<<<<<< HEAD

			if(recieveCount != lastRecieveCount)
			{
				cout << "Recieved response #" << recieveCount << "." << endl;
				lastRecieveCount = recieveCount;
=======
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
						if(numSeparators > 12)
						{
							numSeparators = 0;
							//finished with a message from endOfLastMessage to readIndex
							for(int i = endOfLastMessage; i < readIndex+1; i++)
							{
								tempBuf[i - endOfLastMessage] = inBuffer[i];
							}
							endOfLastMessage = readIndex;//+2;

							// --- Take buffer and convert into a Message
							returnMessage.buildFromReturnString(tempBuf, '1');
							
							// --- write returned message to log
							returnMessage.writeToLogFile();

							//TODO: remove debug printing
							//fwrite(returnMessage.getRequestMessage(), sizeof(char), (returnMessage.getRequestMessage()[1] < 255) ? returnMessage.getRequestMessage()[1] : 255 , stdout); //a bit sketchy but at least it has a max size check. If the target size is less than 255 there could be issues.
							//cout << endl;

							recieveCount++;
							cout << "Recieved response #" << recieveCount << "." << endl;
						}
					}

					readIndex++;
					if(readIndex >= bytesRead)
					{
						reading = false; // we have reached the end of the file that has been read.
					}
				}

				



				// --- clear buffer just in case ----
		    		bzero(inBuffer,1024);
>>>>>>> parent of 119a5f1... The actual traffic is correct, my interpretation of the traffic is incorrect on the client end.
			}
		} // end if(recieveCount < 101)

		//this could go just after recieveCount++ to save a few cycles if things are not threaded
		if(recieveCount >= MaxNumberOfRuns)
		{
			doneSending = true;
		}

		//delay(50 milliseconds)
		//usleep(50000);
		//better way to do this in C++ '11
		//std::this_thread::sleep_for(std::chrono::milliseconds(2));
		//std::this_thread::sleep_for(std::chrono::microseconds(10));
	} // end WHILE

	int recieveShutdownStatus, sendShutdownStatus, closeStatus = 0;

	recieveShutdownStatus = shutdown(sockfd, 0); 
	sendShutdownStatus = shutdown(sockfd, 1); 

	closeStatus = close(sockfd);

	// --- clean up everything, finish log ---
	realMessage.writeRecordTrailerToLog(recieveShutdownStatus, sendShutdownStatus, closeStatus);
	logFile.close();

    return 0;
}
*/

