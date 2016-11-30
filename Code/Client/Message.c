#include "Message.h"

void Message::msgError(const char *msg)
{
	//FOR DEBUGGING, TAKE THE EXIT PARTS OUT SO I CAN RETURN AND SEE WHAT WENT WRONG!!
    //perror(msg);
    //exit(0);
}

Message::Message()
{
	m_FieldSeparator = '|';

	m_TCPHeader[0] = 0; 
	m_TCPHeader[1] = 0; 

}

Message::Message(ofstream* logFile)
{
	m_FieldSeparator = '|';

	m_TCPHeader[0] = 0; 
	m_TCPHeader[1] = 0; 

	m_logFile = logFile;
}

Message::Message(
	//char* logFileName,
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
	char ScenarioNum)
{
	m_FieldSeparator = '|';
	//strcpy(m_logFileName, logFileName);
	m_TCPHeader[0] = (unsigned char)TCPHeader[0];
	m_TCPHeader[1] = (unsigned char)TCPHeader[1];
	strcpy(m_MessageType, MessageType);
	strcpy(m_msTimeStamp, msTimeStamp);
	strcpy(m_RequestID, RequestID);
	strcpy(m_StudentName, StudentName);
	strcpy(m_StudentID, StudentID);
	strcpy(m_ResponseDelay, ResponseDelay);
	strcpy(m_ClientIPAddress, ClientIPAddress);
	strcpy(m_ClientServicePort, ClientServicePort);
	strcpy(m_ClientSocketNum, ClientSocketNum);
	strcpy(m_ForeignHostIPAddress, ForeignHostIPAddress);
	strcpy(m_ForeignHostServicePort, ForeignHostServicePort);
	strcpy(m_StudentData, StudentData);
	m_ScenarioNum = ScenarioNum;

	//m_logFile.open (logFileName);
	m_logFile = logFile;
}


char Message::getFieldSeparator()
{
	return m_FieldSeparator;
}

char * Message::getRequestMessage()
{

	return m_completeMessage;
}

char * Message::getTestRequestMessage()
{
	int curTime = 0;
	char Byte;

	//zero out the entire message just in case
	for(int i = 0; i < 146; i++)
	{
		m_completeMessage[i] = 0;
	}

	//2 bytes in Big Endian Order
	m_completeMessage[0] = 0; //double check that this is Big Endian byte Order
	m_completeMessage[1] = 144; // 144 bytes long (not including these two bytes)

	//MessageType fixed as "REQ"
	m_completeMessage[2] = 'R';
	m_completeMessage[3] = 'E';
	m_completeMessage[4] = 'Q';

	m_completeMessage[5] = m_FieldSeparator;

	//msTimeStamp 10 bytes right justified (probably collected by time)
	curTime = time(NULL);

	//printf("Time in ms (int): %d", curTime);
	//printf("Time in ms (hex): %X", curTime);
	for(int i =0; i < 4; i++)
	{
		Byte = (curTime >> ( i << 3)) & 0xFF;
		//printf("Byte #%d: %X\n", i, Byte);
		m_completeMessage[15-i] = Byte; //is this actually right justified?
	}

	m_completeMessage[16] = m_FieldSeparator;

	//RequestID 20 bytes Alphanumeric
	m_completeMessage[36] = '1'; //does Alphanumeric mean ASCII? or just binary?

	m_completeMessage[37] = m_FieldSeparator;

	//StudentName 20 bytes Alphanumeric
	char name[21] = "BarkmanC            ";

	for(int i = 0; i < 20; i++)
	{
		m_completeMessage[38+i] = name[i];
	}
	
	m_completeMessage[58] = m_FieldSeparator;

	//StudentID 7 bytes "dd-dddd" format
	char id[8] = "17-7918";

	for(int i = 0; i < 7; i++)
	{
		m_completeMessage[59+i] = id[i];
	}
	
	m_completeMessage[66] = m_FieldSeparator;

	//ResponseDelay 5 bytes right justified
	char delay[6] = "01000";

	for(int i = 0; i < 5; i++)
	{
		m_completeMessage[67+i] = delay[i];
	}
	
	m_completeMessage[72] = m_FieldSeparator;

	//ClientIPAddress 15 bytes left justified
	char ClientIP[16] = "10.0.3.200     ";

	for(int i = 0; i < strlen(ClientIP) ; i++)
	{
		m_completeMessage[73+i] = ClientIP[i];
	}
	
	m_completeMessage[88] = m_FieldSeparator;

	//ClientServicePort 5 bytes right justified
	char ClientPort[6] = "12345";

	for(int i = 0; i < 5; i++)
	{
		m_completeMessage[89+i] = ClientPort[i];
	}
	
	m_completeMessage[94] = m_FieldSeparator;

	//ClientSocketNo. 5 bytes right justified
	char ClientSocket[6] = "54321";

	for(int i = 0; i < 5; i++)
	{
		m_completeMessage[95+i] = ClientSocket[i];
	}
	
	m_completeMessage[100] = m_FieldSeparator;

	//ForeignHostIPAddress 15 bytes left justified
	char HostIP[16] = "192.168.101.222";

	for(int i = 0; i < 15; i++)
	{
		m_completeMessage[101+i] = HostIP[i];
	}
	
	m_completeMessage[116] = m_FieldSeparator;

	//ForeignHostServicePort 5 bytes right justified
	char HostPort[6] = "12345";

	for(int i = 0; i < 5; i++)
	{
		m_completeMessage[117+i] = HostPort[i];
	}
	
	m_completeMessage[122] = m_FieldSeparator;	

	//StudentData 20 bytes Basically Junk Data
	char StudentData[21] = "ABCDEFGHIJKLMNOPQRST";

	for(int i = 0; i < 20; i++)
	{
		m_completeMessage[123+i] = StudentData[i];
	}
	
	m_completeMessage[143] = m_FieldSeparator;

	//ScenarioNo. 1 byte Numeric
	m_completeMessage[144] = 1;

	m_completeMessage[145] = m_FieldSeparator;

	m_completeMessage[146] = 0;
}

void Message::displayTestRequestMessage()
{
	cout << "Begin display of test message" << endl;
	cout << getTestRequestMessage() << endl;

	for(int i = 0; i < 146; i++)
	{
		cout << m_completeMessage[i];
	}
	cout << endl;

}

void Message::displayRequestMessage()
{
	string test(m_completeMessage);
	string test2(m_completeMessage);
	char buf[146];

	cout << test << test2 << endl;

	cout << "Begin display of message" << endl;
	cout << getRequestMessage()[0] << getRequestMessage()[1] << getRequestMessage()[2] << endl;

	for(int i = 2; i < 146 && m_completeMessage[i] !=0; i++)
	{
		buf[i] = m_completeMessage[i];
	}

	cout << buf << endl;

	for(int i = 0; i < 146; i++)
	{
		cout << m_completeMessage[i];
	}
	cout << endl;

}

//gets the current time and returns a 10 character string containing the time in ms
char * Message::getCurrentMSTimeString()
{
	int curTime;
	char Byte;
	//char timeString[11];
/*	
	for(int i=0; i< 6; i++)
	{
		m_timeString[i] = ' ';
	}
*/
	//msTimeStamp 10 bytes right justified (probably collected by time)
	curTime = time(NULL);

	//itoa(curTime, m_timeString, 10); //non-standard, not portable
	snprintf(m_timeString, 11, "%d", curTime); //portable, and protected from buffer overflows
/*
	//printf("Time in ms (int): %d", curTime);
	//printf("Time in ms (hex): %X", curTime);
	for(int i =0; i < 4; i++)
	{
		Byte = (curTime >> ( i << 3)) & 0xFF;
		//printf("Byte #%d: %X\n", i, Byte);
		m_timeString[10-i] = Byte; //is this actually right justified?
	}
*/
	return m_timeString;
}

void Message::setMSTimeStamp(char * msTimeStamp)
{
	for(int i = 0; i < 10 && msTimeStamp[i] != 0; i++)
	{
		m_msTimeStamp[i] = msTimeStamp[i];
	}
}

void Message::setOutgoingPort(int portNum)
{
	char tempStr[6];

	snprintf(tempStr, 6, "%d", portNum); //portable, and protected from buffer overflows
	
	justifyRightZeroFillLeft(tempStr, 5);

	strcpy(m_ClientServicePort, tempStr);
}

void Message::setSocketNum(int socketNum)
{
	char tempStr[6];

	snprintf(tempStr, 6, "%d", socketNum); //portable, and protected from buffer overflows

	justifyRightZeroFillLeft(tempStr, 5);

	strcpy(m_ClientSocketNum, tempStr);
}

void Message::justifyRightZeroFillLeft(char * cString, int length)
{
	int strLen;
	strLen = strlen(cString);

	//make sure this copy goes first or else you will overwrite the data!
	for(int i = strLen, j = 0; i < length; i++, j++)
	{
		cString[i] = cString[j];
	}

	//add zeros until the string starts,  then print the string
	for(int i = 0; i < (length - strLen); i++)
	{
		cString[i] = '0';
	}

}

int Message::formRequestMessage()
{
	int curTime = 0;
	char Byte;
	int index = 0;

	//zero out the entire message just in case
	for(int i = 0; i < 146; i++)
	{
		m_completeMessage[i] = 0;
	}

	//2 bytes in Big Endian Order
	m_completeMessage[index] = m_TCPHeader[0]; //double check that this is Big Endian byte Order
	index++;
	m_completeMessage[index] = m_TCPHeader[1]; // 144 bytes long (not including these two bytes)
	index++;

	//MessageType fixed as "REQ"
	for(int i = 0; i < 3; i++, index++)
	{	
		m_completeMessage[index] = m_MessageType[i];
	}

	m_completeMessage[index] = m_FieldSeparator;
	index++;

	//msTimeStamp 10 bytes right justified (probably collected by time)
	for(int i = 0; i < 10; i++, index++)
	{	
		m_completeMessage[index] = m_msTimeStamp[i];
	}

	m_completeMessage[index] = m_FieldSeparator;
	index++;

	//RequestID 20 bytes Alphanumeric
	for(int i = 0; i < 20; i++, index++)
	{	
		m_completeMessage[index] = m_RequestID[i];
	}

	m_completeMessage[index] = m_FieldSeparator;
	index++;

	//StudentName 20 bytes Alphanumeric
	//char name[21] = "BarkmanC            ";

	for(int i = 0; i < 20; i++, index++)
	{
		m_completeMessage[index] = m_StudentName[i];
	}
	
	m_completeMessage[index] = m_FieldSeparator;
	index++;

	//StudentID 7 bytes "dd-dddd" format
	//char id[8] = "17-7918";

	for(int i = 0; i < 7; i++, index++)
	{
		m_completeMessage[index] = m_StudentID[i];
	}
	
	m_completeMessage[index] = m_FieldSeparator;
	index++;

	//ResponseDelay 5 bytes right justified
	//char delay[6] = "01000";

	for(int i = 0; i < 5; i++, index++)
	{
		m_completeMessage[index] = m_ResponseDelay[i];
	}
	
	m_completeMessage[index] = m_FieldSeparator;
	index++;

	//ClientIPAddress 15 bytes left justified
	//char ClientIP[16] = "10.1.20.29     ";

	/*strlen(ClientIP)*/ 
	for(int i = 0; i < 15 & m_ClientIPAddress[i] != 0 & m_ClientIPAddress[i] != '|' ; i++, index++)
	{
		m_completeMessage[index] = m_ClientIPAddress[i];
	}
	
	m_completeMessage[index] = m_FieldSeparator;
	index++;

	//ClientServicePort 5 bytes right justified
	//char ClientPort[6] = "12345";

	for(int i = 0; i < 5; i++, index++)
	{
		m_completeMessage[index] = m_ClientServicePort[i];
	}
	
	m_completeMessage[index] = m_FieldSeparator;
	index++;

	//ClientSocketNo. 5 bytes right justified
	//char ClientSocket[6] = "54321";

	for(int i = 0; i < 5; i++, index++)
	{
		m_completeMessage[index] = m_ClientSocketNum[i];
	}
	
	m_completeMessage[index] = m_FieldSeparator;
	index++;

	//ForeignHostIPAddress 15 bytes left justified
	char HostIP[16] = "192.168.101.220";

	for(int i = 0; i < 15; i++, index++)
	{	
		//TODO: Fix this!!!!
		//m_completeMessage[101+i] = m_ForeignHostIPAddress[i];
		m_completeMessage[index] = HostIP[i];
	}
	
	m_completeMessage[index] = m_FieldSeparator;
	index++;

	//ForeignHostServicePort 5 bytes right justified
	//char HostPort[6] = "12345";

	for(int i = 0; i < 5; i++, index++)
	{
		m_completeMessage[index] = m_ForeignHostServicePort[i];
	}
	
	m_completeMessage[index] = m_FieldSeparator;
	index++;

	//StudentData 20 bytes Basically Junk Data
	//char StudentData[21] = "ABCDEFGHIJKLMNOPQRST";

	for(int i = 0; i < 20; i++, index++)
	{
		m_completeMessage[index] = m_StudentData[i];
	}
	
	m_completeMessage[index] = m_FieldSeparator;
	index++;

	//ScenarioNo. 1 byte Numeric
	m_completeMessage[index] = m_ScenarioNum;
	index++;

	m_completeMessage[index] = m_FieldSeparator;
	index++;

	m_completeMessage[index] = 0;

	return index;

};

void Message::buildFromReturnString(char * returnString)
{
	int curTime = 0;
	char Byte;
	int index = 2;
	char errorMessage[100];

	//2 bytes in Big Endian Order
	m_TCPHeader[0] = returnString[0]; //double check that this is Big Endian byte Order
	m_TCPHeader[1] = returnString[1]; // 144 bytes long (not including these two bytes)

	//MessageType fixed as "REQ"
	//TODO: Make sure this actually says "REQ" first
	for(int i = 0; i < 3; i++, index++)
	{	
		m_MessageType[i] = returnString[index];
	}

	//m_completeMessage[5] = m_FieldSeparator;
	if(returnString[index] != '|')
	{
		msgError("Missing '|' at position 5 (0 index).");
	}
	index++;

	//msTimeStamp 10 bytes right justified (probably collected by time)
	for(int i = 0; i < 10 & returnString[index] != 0 & returnString[index] != '|' ; i++, index++)
	{	
		m_msTimeStamp[i] = returnString[index];
	}

	//m_completeMessage[16] = m_FieldSeparator;
	if(returnString[index] != '|')
	{
		sprintf(errorMessage, "Missing '|' at position %d? (0 index).", index);
		msgError(errorMessage);
	}
	index++;

	//RequestID 20 bytes Alphanumeric
	for(int i = 0; i < 20 & returnString[index] != 0 & returnString[index] != '|' ; i++, index++)
	{	
		m_RequestID[i] = returnString[index];
	}

	//m_completeMessage[37] = m_FieldSeparator;
	if(returnString[index] != '|')
	{
		sprintf(errorMessage, "Missing '|' at position %d? (0 index).", index);
		msgError(errorMessage);
	}
	index++;

	//StudentName 20 bytes Alphanumeric
	char name[21] = "BarkmanC            ";

	for(int i = 0; i < 20 & returnString[index] != 0 & returnString[index] != '|'; i++, index++)
	{
		m_StudentName[i] = returnString[index];
	}
	
	//m_completeMessage[58] = m_FieldSeparator;
	if(returnString[index] != '|')
	{
		sprintf(errorMessage, "Missing '|' at position %d? (0 index).", index);
		msgError(errorMessage);
	}
	index++;

	//StudentID 7 bytes "dd-dddd" format
	char id[8] = "17-7918";

	for(int i = 0; i < 7  & returnString[index] != 0 & returnString[index] != '|'; i++, index++)
	{
		m_StudentID[i]  = returnString[index];
	}
	
	//m_completeMessage[66] = m_FieldSeparator;
	if(returnString[index] != '|')
	{
		sprintf(errorMessage, "Missing '|' at position %d? (0 index).", index);
		msgError(errorMessage);
	}
	index++;

	//ResponseDelay 5 bytes right justified
	char delay[6] = "01000";

	for(int i = 0; i < 5  & returnString[index] != 0 & returnString[index] != '|'; i++, index++)
	{
		m_ResponseDelay[i] = returnString[index];
	}
	
	//m_completeMessage[72] = m_FieldSeparator;
	if(returnString[index] != '|')
	{
		sprintf(errorMessage, "Missing '|' at position %d? (0 index).", index);
		msgError(errorMessage);
	}
	index++;

	//ClientIPAddress 15 bytes left justified
	//char ClientIP[16] = "10.1.20.29     ";
	bool ClientIPLoopFlag = true;
	int ClientIPMaxStringLength = 15;
	//ClientIPStringLength = strlen(ClientIP);

	for(int i = 0; ClientIPLoopFlag == true; i++, index++) //TODO: bug here, index gets incremented one too many times
	{
		m_ClientIPAddress[i] = returnString[index];
		if(i > ClientIPMaxStringLength | returnString[index] == 0 | returnString[index] == '|' )
		{
			ClientIPLoopFlag = false;
			m_ClientIPAddress[i] = 0;
		}
	}
	
	//m_completeMessage[88] = m_FieldSeparator;
	if(returnString[index] != '|')
	{
		sprintf(errorMessage, "Missing '|' at position %d? (0 index).", index);
		msgError(errorMessage);
	}
	index++;

	//ClientServicePort 5 bytes right justified
	//char ClientPort[6] = "12345";
	bool ClientPortLoopFlag = true;

	for(int i = 0; ClientPortLoopFlag == true; i++, index++)
	{	
		if(i < 5  & returnString[index] != 0 & returnString[index] != '|')
		{
			ClientPortLoopFlag = false;
			m_ClientIPAddress[i] = 0;
		}
		else
		{
			m_ClientServicePort[i] = returnString[index];
		}	
	}
	
	//m_completeMessage[94] = m_FieldSeparator;
	if(returnString[index] != '|')
	{
		sprintf(errorMessage, "Missing '|' at position %d? (0 index).", index);
		msgError(errorMessage);
	}
	index++;

	//ClientSocketNo. 5 bytes right justified
	//char ClientSocket[6] = "54321";
	bool ClientSocketLoopFlag = true;

	for(int i = 0; ClientSocketLoopFlag == true ; i++, index++)
	{
		if(i < 5  & returnString[index] != 0 & returnString[index] != '|')
		{
			ClientSocketLoopFlag = false;
			m_ClientIPAddress[i] = 0;
		}
		else
		{
			m_ClientSocketNum[i] = returnString[index];
		}
	}
	
	//m_completeMessage[100] = m_FieldSeparator;
	if(returnString[index] != '|')
	{
		sprintf(errorMessage, "Missing '|' at position %d? (0 index).", index);
		msgError(errorMessage);
	}
	index++;

	//ForeignHostIPAddress 15 bytes left justified
	char HostIP[16] = "192.168.101.220";

	for(int i = 0; i < 15  & returnString[index] != 0 & returnString[index] != '|'; i++, index++)
	{
		//m_completeMessage[101+i] = m_ForeignHostIPAddress[i];
		HostIP[i] = returnString[index];
	}
	
	//m_completeMessage[116] = m_FieldSeparator;
	if(returnString[index] != '|')
	{
		sprintf(errorMessage, "Missing '|' at position %d? (0 index).", index);
		msgError(errorMessage);
	}
	index++;

	//ForeignHostServicePort 5 bytes right justified
	char HostPort[6] = "12345";

	for(int i = 0; i < 5 & returnString[index] != 0 & returnString[index] != '|'; i++, index++)
	{
		m_ForeignHostServicePort[i] = returnString[index];
	}
	
	//m_completeMessage[122] = m_FieldSeparator;	
	if(returnString[index] != '|')
	{
		sprintf(errorMessage, "Missing '|' at position %d? (0 index).", index);
		msgError(errorMessage);
	}
	index++;

	//StudentData 20 bytes Basically Junk Data
	char StudentData[21] = "ABCDEFGHIJKLMNOPQRST";

	for(int i = 0; i < 20  & returnString[index] != 0 & returnString[index] != '|'; i++, index++)
	{
		m_StudentData[i] = returnString[index];
	}
	
	//m_completeMessage[143] = m_FieldSeparator;
	if(returnString[index] != '|')
	{
		sprintf(errorMessage, "Missing '|' at position %d? (0 index).", index);
		msgError(errorMessage);
	}
	index++;

	//ScenarioNo. 1 byte Numeric
	m_ScenarioNum = returnString[index];
	index++;

	//m_completeMessage[145] = m_FieldSeparator;

	if(returnString[index] != '|')
	{
		sprintf(errorMessage, "Missing FINAL '|' at position %d? (0 index).", index);
		msgError(errorMessage);
	}
	index++;

	//m_completeMessage[146] = 0;


}

void Message::writeToLogFile()
{
	formRequestMessage(); //just in case
	//TODO: fix this so it actually prints out the correct message
	//*m_logFile << getRequestMessage() << endl;
	m_logFile->write(getRequestMessage(), sizeof(char) * 146);
	m_logFile->write("\n", sizeof(char));
}

void Message::writeRecordTrailerToLog()
{
	//TODO: Actually print the log trailer
	*m_logFile << "TODO: Make Record Trailer" << endl;
}

void Message::setResponseDelay(char * ResponseDelayString)
{
	for(int i=0; i < 4 | ResponseDelayString[i] == '|'; i++)
	{
		m_ResponseDelay[i] = ResponseDelayString[i];
	}
}

void Message::setRequestId(int Id)
{
	char tempStr[20];

	snprintf(tempStr, 20, "%d", Id); //portable, and protected from buffer overflows

	justifyRightZeroFillLeft(tempStr, 20);

	strncpy(m_RequestID, tempStr, 20);
}

