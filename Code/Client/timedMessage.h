#ifndef TIMEDMESSAGE_H
#define TIMEDMESSAGE_H


#include "Message.h"

class timedMessage {

public:
timedMessage();
timedMessage(Message message, milliseconds endTime);
timedMessage(Message message, milliseconds startTime, milliseconds endTime);


int setStartTime(milliseconds startTime);
int setEndTime(milliseconds endTime);
milliseconds getStartTime();
milliseconds getEndTime();

milliseconds getTimeDifference();
Message getMessage();
int getStateCount();
void setStateCount(int count);

private:

Message m_message;
milliseconds m_startTime;
milliseconds m_endTime;
int m_stateCount;
};

#endif
