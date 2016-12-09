//for file output stuff
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
//#include "stdafx.h"
#include <iostream>


//#include "D:\School\CST415\Code\Common/Message.hpp"
#include "..\..\..\..\Common\Message.hpp"
//#include <Common/Message.h>


//for strtok
//#include <cstring>//
//#include <iostream>

//for list
#include <list>



//#include <unistd.h>

//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <netdb.h>





//#include <chrono>
//#include <thread>



/*
Initialise Winsock
*/

#include<stdio.h>
#include<winsock2.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library










//Global for testing and debugging, TODO: REMOVE or put into a class
char strPtrArray[13][21];
int strArrayIndex;
char messageBuffer[146];
std::list<Message> MessageQueue;
std::ofstream logFile;
milliseconds startTime;
int delimiterCount = 0;
int lastCount;
//bool endNull;
//char lastDelimiter;

using namespace std;

void error(const char *msg)
{
	perror(msg);
	exit(0);
}

int buildTokensIntoString()
{
	int index = 1;

	//cout << "Begin buildTokensIntoString()" << endl;

	messageBuffer[0] = 0;
	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; (j < 20)&(strPtrArray[i][j] != 0); j++, index++)
		{
			messageBuffer[index] = strPtrArray[i][j];
		}
		messageBuffer[index] = '|';
		index++;
	}

	cout << messageBuffer[0] << messageBuffer+1 << endl;

	Message localMessage(&logFile);
	// --- Take buffer and convert into a Message
	localMessage.buildFromReturnString(messageBuffer, '1');
	localMessage.setMessageType("REQ");
	MessageQueue.push_back(localMessage);

	//cout << "End buildTokensIntoString()" << endl;
	return index;
}

char * myStrTok(char * inStr, char delimiter)
{
	static char * lastPtr;
	if (inStr != NULL)
	{
		lastPtr = inStr;
	}
	else
	{
		if (lastPtr != NULL)
		{
			lastPtr += strlen(lastPtr) + 1; //since the string should now be null terminated
		}
	}

	//manual hard limit of 146
	int i;
	for (i = 0; (i < 146) & (lastPtr[i] != delimiter) & (lastPtr[i] != 0); i++)
	{
	}
	if (lastPtr[i] == delimiter)
	{
		if (delimiterCount < 12)
		{
			delimiterCount++;
		}
		else
		{
			delimiterCount = 0;
		}
	}
	
	//lastDelimiter = lastPtr[i];

	//put a null at the end of the new cstring
	lastPtr[i] = 0;

	return lastPtr;
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
	char *token;
	bool middleRead = false;

	if (inBuffer[0] != 0)
	{
		//if (lastDelimiter == '\0')
		//{
			//may need to check strArrayIndex as well here
			//concatenate two strings now
		//	cout << "test" << endl;
		//}
		//we have started reading in the middle of a token. This means we are concatenating onto an old token.
		//token = std::strtok(inBuffer, "|");
		if (lastCount < strArrayIndex)
		{
			cout << "test" << endl;
		}
		token = myStrTok(inBuffer, '|');

		//middleRead = true;
		//strArrayIndex--; //go back to the older string
	}
	else
	{ 
		//token = std::strtok(inBuffer + 1, "|");
		token = myStrTok(inBuffer + 1, '|');
	}

	while (amountRead < bytesRead)
	{
		//there is still stuff left, get the remaining tokens processed
		if (token == NULL)
		{
			//cout << "Null read at amountRead=" << amountRead << endl;
			//token = std::strtok(inBuffer + amountRead + 1, "|");
			token = myStrTok(inBuffer + amountRead + 1, '|');
			//token = std::strtok(NULL, "|\0");
			amountRead++;
		}
		else
		{
			if (*token == '\0')
			{
				//cout << "Null read at amountRead=" << amountRead << endl;
				//token = std::strtok(inBuffer + amountRead + 1, "|");
				token = myStrTok(inBuffer + amountRead + 1, '|');
				//token = std::strtok(NULL, "|\0");
				amountRead++;
			}
			else
			{
				//std::cout << token << '\t' << "strArrayIndex=" << strArrayIndex << '\n';
				strncpy(strPtrArray[strArrayIndex], token, 20);
				amountRead += strlen(token);
				//cout << "Normal read at amountRead=" << amountRead << endl;
				//increase the index
				if (strArrayIndex < 12)
				{
					strArrayIndex++;
					amountRead++;
				}
				else
				{
					strArrayIndex = 0;// if we are done, process everything
					amountProcessed += buildTokensIntoString();
					recieveCount += 1;
					//cout << "recieveCount=" << recieveCount << endl;
				}
				//prepare for the next loop
				//token = std::strtok(NULL, "|");
				token = myStrTok(NULL, '|');
			}
		}
		//cout << "End of single while loop iteration in processReadData(). amountRead=" << amountRead << " bytesRead=" << bytesRead << endl;
	}

	lastCount += delimiterCount;
	if (lastCount > 12) // if the count goes over, roll back
	{
		lastCount -= 12;
	}
	delimiterCount = 0;//reset this

	//cout << "End processReadData()" << endl;
	return recieveCount;
}

void testProcessReadData()
{
	int processedCount = 0;
	strArrayIndex = 0;

	char testString[] = "\0�RSP|0000045007|00000000000000000001|BarkmanC            |17-7918|00000|10.1.20.29     |45994|00004|192.168.101.220|02605|ABCDEFGHIJKLMNOPQRST|1|\0";
	char testString2[] = "\0�RSP|1234567890|000000000000000000002|BarkmanC            |17-7918|00000|10.1.20.29     |45994|00004|192.168.101.220|02605|ABCDEFGHIJKLMNOPQRST|1|\0�RSP|9999999999|\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

	char testString3[] = "00000000000000000003|BarkmanC            |17-7918|00000|10.1.20.29     |45994|00004|192.168.101.220|02605|ABCDEFGHIJKLMNOPQRST|1|\0�RSP|0000045007|00000000000000000004|BarkmanC            |17-7918|00000|10.1.20.29     |45994|00004|192.168.101.220|02605|ABCDEFGHIJKLMNOPQRST|1|\0";

	//TODO: Fix this test case if it ends up happeneing to me!
	char testString4[] = "\0�RSP|0000045007|00000000000000000001|BarkmanC            |17-7918|00000|10.1.20.29     |45994|00004|192.168.101.220|02605|ABCDEFGHIJKLMNOPQRST|1|\0�RSP|0000045007|1234567890";

	char testString5[] = "1234567890|BarkmanC            |17-7918|00000|10.1.20.29     |45994|00004|192.168.101.220|02605|ABCDEFGHIJKLMNOPQRST|1|\0";



	logFile.open("Lab4.Scenario1.BarkmanC.txt"); //make sure to open the log before using it!

	processedCount += processReadData(testString, 146);

	if (processedCount < 1) {
		cout << "error in number of strings procesed" << endl;
	}

	processedCount += processReadData(testString2, 163);

	if (processedCount < 2) {
		cout << "error in number of strings procesed" << endl;
	}

	processedCount += processReadData(testString3, 275);

	if (processedCount < 4) {
		cout << "error in number of strings procesed" << endl;
	}

	if (lastCount != 0)
	{
		cout << "error in number delimiters counted" << endl;
		lastCount = 0;
	}


	processedCount += processReadData(testString4, 172);

	processedCount += processReadData(testString5, 120);


	if (processedCount < 6)
	//if (processedCount < 4)
	{
		cout << "error in number of strings procesed" << endl;
	}
//	else
//	{
//		for (Message cur : MessageQueue) //write all the messages to file
//		{
//			cur.writeToLogFile();
//		}
//	}
}

int client()
{

	//For the Winsock
	WSADATA wsa;
	//For the socket
	SOCKET s;
	//For the connection information
	struct sockaddr_in server;
	//For recieving
	char server_reply[2000 + 1];
	int recv_size;

	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}

	printf("Initialised.");

	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}

	printf("Socket created.\n");

	server.sin_addr.s_addr = inet_addr("192.168.11.47");
	server.sin_family = AF_INET;
	server.sin_port = htons(2605);

	//Connect to remote server
	if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		puts("connect error");
		return 1;
	}

	puts("Connected");

	//Send some data
	char message[] = "GET / HTTP/1.1\r\n\r\n";
	if (send(s, message, strlen(message), 0) < 0)
	{
		puts("Send failed");
		return 1;
	}
	puts("Data Send\n");

	//Receive a reply from the server
	if ((recv_size = recv(s, server_reply, 2000, 0)) == SOCKET_ERROR)
	{
		puts("recv failed");
	}
	else
	{
		puts("Reply received\n");

		//Add a NULL terminating character to make it a proper string before printing
		server_reply[recv_size] = '\0';
		puts(server_reply);

	}


	//Clean up
	closesocket(s);
	WSACleanup();
}

int server()
{
	WSADATA wsa;
	SOCKET s, new_socket;
	struct sockaddr_in server, client;
	int c;

	char server_reply[2000 + 1] = { 0 };
	int recv_size;

	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}

	printf("Initialised.\n");

	//Create a socket
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}

	printf("Socket created.\n");

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(2605);

	//Bind
	if (::bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)// the :: means pull from gobal namespace. using namespace std; causes this need.
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
	}

	puts("Bind done");

	//Listen to incoming connections
	listen(s, 3);

	//Accept and incoming connection
	puts("Waiting for incoming connections...");

	c = sizeof(struct sockaddr_in);
	new_socket = accept(s, (struct sockaddr *)&client, &c);
	if (new_socket == INVALID_SOCKET)
	{
		printf("accept failed with error code : %d", WSAGetLastError());
	}

	puts("Connection accepted");

	int recieveCount = 0;
	while (recieveCount < 10000)
	{
		//Receive a reply from the server
		if ((recv_size = recv(new_socket, server_reply, 2000, 0)) == SOCKET_ERROR)
		{//bad read for whatever reason
			//puts("recv failed");
		}
		else if(recv_size == 0)
		{// empty read 
		}
		else
		{
			int thisRecieveCount = 0;
			//puts("Reply received\n");
			thisRecieveCount = processReadData(server_reply, recv_size);
			recieveCount += thisRecieveCount;

			std::list<Message>::iterator iterator = MessageQueue.end();
			//std::list<Message>::iterator oldEnd = MessageQueue.end();
			for (int i = 0; i < thisRecieveCount; i++)//move the iterator back to where the just recieved Messages are kept
			{
				--iterator;
			}

			int loopI = 0;
			while (loopI < thisRecieveCount)
			{
				//get a copy of the message that was recieved
				Message localMessage(*iterator);

				localMessage.setMSTimeStamp(localMessage.getCurrentMSTimeString(startTime));
				localMessage.setMessageType("RSP");
				//returnMessage.setOutgoingPort(local_port);
				//returnMessage.setSocketNum(newsockfd);
				localMessage.setScenarioNum('1');

				int writeSize = 0;
				localMessage.formRequestMessage(); //ensure the message is built and ready to send
				char* msgPtr = localMessage.getRequestMessage();
				for (writeSize = 2; writeSize < 146 & msgPtr[writeSize] != 0; writeSize++)
				{

				}

				send(new_socket, msgPtr, writeSize, 0);

				MessageQueue.push_back(localMessage);

//				//For Debug, this is put here. For real operations, put it at the end of the program.
//				for (Message cur : MessageQueue) //write all the messages to file
//				{
//					cur.writeToLogFile();
//					//MessageQueue.remove(cur);
//					//MessageQueue.pop_front(); // remove the message so it won't be printed again
//				}


				iterator++;
				loopI++;
			}
		}
	}

	closesocket(s);
	WSACleanup();

	return 0;
}

int main(int argc, char *argv[])
{

	startTime = duration_cast< milliseconds >(
		system_clock::now().time_since_epoch()
		);

	logFile.open("Lab4.Scenario1.BarkmanC.txt");
	if(logFile.is_open())
	{ 
		//logFile << "TEST" << endl;
		testProcessReadData();
		//server();
		//client();
	}

	//For Debug, this is put here. For real operations, put it at the end of the program.
	for (Message cur : MessageQueue) //write all the messages to file
	{
		cur.writeToLogFile();
		//MessageQueue.remove(cur);
		//MessageQueue.pop_front(); // remove the message so it won't be printed again
	}

	logFile.close();
	return 0;
}






/*
int _tmain(int argc, _TCHAR* argv[])
{
	testProcessReadData();
	return 0;
}
*/