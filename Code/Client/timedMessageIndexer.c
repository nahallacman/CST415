#include "timedMessageIndexer.h"


timedMessageIndexer::timedMessageIndexer()
{
	m_responseCount = 0;
}

void timedMessageIndexer::addMessage( Message newMessage, milliseconds endTime )
{
	string reqID(newMessage.getRequestId());
	//if it already exists, check what state the ID is in
	if( checkIfIDExists(reqID) )
	{
		int stateCount = m_timedMessages[reqID].getStateCount();
		switch(stateCount)
		{
			case 0:
				m_timedMessages[reqID].setStateCount(1);
			case 1: //we just recieved a regular response
				//log request and response
					//-- request --
				//auto a = m_timedMessages[reqID];
				m_timedMessages[reqID].getMessage().formRequestMessage();
				// --- write output message log ---
				m_timedMessages[reqID].getMessage().writeToLogFile();
				m_timedMessages[reqID].setStateCount(5); //set the state to not looking for more responses

					//-- response --
				newMessage.formRequestMessage();
				newMessage.setScenarioNum('1');
				// --- write output message log ---
				newMessage.writeToLogFile();

				m_responseCount++;
				break;
			case 2: //we just recieved a stand in response
				// THIS SHOULDN'T HAPPEN! Stand in's are handled internally, not actually sent
				break;
			case 3:	//we just recieved a latent response
				//log original request and response
					//-- request --
				m_timedMessages[reqID].getMessage().formRequestMessage();
				// --- write output message log ---
				m_timedMessages[reqID].getMessage().writeToLogFile();

					//-- response --
				newMessage.formRequestMessage();
				newMessage.setScenarioNum('3');
				// --- write output message log ---
				newMessage.writeToLogFile();
				m_responseCount++;
				break;
			case 4: //we just recieved a spurious response
			case 5:	//we just recieved a spurious response after getting a real response
				// log response, but NOT the matching request
					//-- response --
				newMessage.formRequestMessage();
				newMessage.setScenarioNum('4');
				// --- write output message log ---
				newMessage.writeToLogFile();

				break;
			default:
				//Error? This should never happen.
				int i = 0;
		}
	}
	else //if it doesn't exist, then add it to the struture
	{
		string messageID(reqID);
		timedMessage a(newMessage, endTime);
		std::pair<string,timedMessage> ins(messageID,a);
		m_timedMessages.insert(ins);
	}
}

bool timedMessageIndexer::checkIfIDExists(string ID)
{
	int count = m_timedMessages.count(ID);

	if(count > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int timedMessageIndexer::updateTime(milliseconds curTime)
{
	Message tempMessage;
	milliseconds oldTime;
	int state = 0;
	//go through each member of the map and check if the curTime is later than the end time
	map <string, timedMessage>::iterator iter;
	for(iter = m_timedMessages.begin() ; iter != m_timedMessages.end(); iter++)
	{
		oldTime = iter->second.getEndTime();
		if( ( oldTime - curTime ) <= milliseconds(0) ) //if it is, update the state number appropriately
		{
			//also update to a new end time that matches the new state
			state = iter->second.getStateCount();
			if(state == 1) //if the state number would be changed to 2, issue a stand in response log, then increase the state to 3
			{
				iter->second.setStateCount(3);
				iter->second.setEndTime( curTime + duration_cast< milliseconds >(std::chrono::seconds(20)) );
				//TODO: Stand in reponse! TOP PRIORITY!

				iter->second.getMessage().formRequestMessage();
				// --- write output message log ---
				iter->second.getMessage().writeToLogFile();

					//-- stand in --
				tempMessage = iter->second.getMessage();
				tempMessage.formRequestMessage();
				tempMessage.setScenarioNum('2');
				// --- write output message log ---
				tempMessage.writeToLogFile();
			}
			else if(state == 3)
			{
				iter->second.setStateCount(4);
				m_responseCount++; //no longer waiting for the response to come, so we have technically recieved a response of nothing and are waiting for a spurious response
			}
			else if(state == 4)
			{
			
			}
			else if(state == 5)
			{

			}
			else
			{			
				//error!!
			}


		}
	}

}

int timedMessageIndexer::getResponseCount()
{
	return m_responseCount;
}
