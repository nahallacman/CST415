#include "timedMessage.h"

timedMessage::timedMessage()
{
	setStartTime( 
		duration_cast< milliseconds >(
	    system_clock::now().time_since_epoch()
	) 
	);

	m_stateCount = 1;
}

timedMessage::timedMessage(Message message, milliseconds endTime):m_message{message},m_endTime{endTime}
{
	setStartTime( duration_cast< milliseconds >(
	    system_clock::now().time_since_epoch()
	) );
	m_stateCount = 1;
}

timedMessage::timedMessage(Message message, milliseconds startTime, milliseconds endTime):m_message{message},m_startTime{startTime},m_endTime{endTime}
{
	m_stateCount = 1;
}

int timedMessage::setStartTime(milliseconds startTime)
{
	m_startTime = startTime;
}
int timedMessage::setEndTime(milliseconds endTime)
{
	m_endTime = endTime;
}
milliseconds timedMessage::getStartTime()
{
	return m_startTime;
}
milliseconds timedMessage::getEndTime()
{
	return m_endTime;
}

milliseconds timedMessage::getTimeDifference()
{
	return m_startTime - m_endTime;
}

Message timedMessage::getMessage()
{
	return m_message;
}

int timedMessage::getStateCount()
{
	return m_stateCount;
}
void timedMessage::setStateCount(int count)
{
	m_stateCount = count;
}
