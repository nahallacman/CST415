#ifndef MESSAGE_H
#define MESSAGE_H
#include "stdint.h"
#include <stdio.h>
//#include <time.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
//#include <string>
#include <fstream>

//#include <sys/time.h>



//for stringstream
#include <sstream>
#include <string>
#include <iostream>



#include <chrono>
#include <thread>

using namespace std::chrono;

using namespace std;

using std::cout;
using std::endl;
using std::string;
using std::ofstream;

class Message {

public:

void msgError(const char *msg);

Message();
Message(ofstream* logFile);
Message(
	//char* logFile,
	ofstream* logFile,
	unsigned char* TCPHeader, 
	char* MessageType, 
	char* msTimeStamp, 
	char* RequestID, 
	char* StudentName, 
	char* StudentID, 
	char* ResponseDelay, 
	char* ClientIPAddress, 
	char* ClientServicePort, 
	char* ClientSocketNum, 
	char* ForeignHostIPAddress, 
	char* ForeignHostServicePort, 
	char* StudentData, 
	char ScenarioNum );

char getFieldSeparator(); 
char * getRequestMessage();
char * getTestRequestMessage();
void displayRequestMessage();
void displayTestRequestMessage();

char * getCurrentMSTimeString(milliseconds ms);

void setMSTimeStamp(char * msTimeStamp);
void setOutgoingPort(int portNum);
void setSocketNum(int socketNum);
void justifyRightZeroFillLeft(char * cString, int length);

int formRequestMessage();
void buildFromReturnString(char * returnString, int ResponseType);

void writeToLogFile();
void writeRecordTrailerToLog(int RcvShutdownStatus, int XmtShutdownStatus, int CloseStatus);

void setResponseDelay(char * ResponseDelayString);

void setRequestId(int Id);
char * getRequestID();
void setMessageType(char* typeStr);
char * getRequestId();

void setScenarioNum(char num);

private:

char m_FieldSeparator;

char m_logFileName[1024];
char m_TCPHeader [2];
char m_MessageType [3];
char m_msTimeStamp [10];
char m_RequestID [21];
char m_StudentName [20];
char m_StudentID [7];
char m_ResponseDelay [5];
char m_ClientIPAddress [15];
char m_ClientServicePort [5];
char m_ClientSocketNum [5];
char m_ForeignHostIPAddress [15];
char m_ForeignHostServicePort [5];
char m_StudentData [20];
char m_ScenarioNum; //use as binary

char m_completeMessage[146];

char m_timeString[11];

ofstream* m_logFile;

};
/*
*/
#endif
