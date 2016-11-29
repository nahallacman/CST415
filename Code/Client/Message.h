#ifndef MESSAGE_H
#define MESSAGE_H
#include "stdint.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>

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

char * getCurrentMSTimeString();


void setMSTimeStamp(char * msTimeStamp);
void setOutgoingPort(int portNum);
void setSocketNum(int socketNum);
void justifyRightZeroFillLeft(char * cString, int length);

void formRequestMessage();

void buildFromReturnString(char * returnString);

void writeToLogFile();
void writeRecordTrailerToLog();

private:

char m_FieldSeparator;

char m_logFileName[1024];
char m_TCPHeader [2];
char m_MessageType [3];
char m_msTimeStamp [10];
char m_RequestID [20];
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

#endif
