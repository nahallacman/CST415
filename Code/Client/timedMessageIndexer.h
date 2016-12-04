#ifndef TIMEDMESSAGEINDEXER_H
#define TIMEDMESSAGEINDEXER_H

#include "timedMessage.h"
#include <map>

#include "Message.h"

using std::map;

class timedMessageIndexer {

public:
timedMessageIndexer();

void addMessage( Message newMessage, milliseconds endTime );

bool checkIfIDExists(string ID);

int updateTime(milliseconds curTime);

int getResponseCount();

private:

map<string, timedMessage> m_timedMessages;
int m_responseCount;
};

#endif
