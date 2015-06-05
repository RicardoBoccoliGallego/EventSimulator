/*
 * EventQueue.h
 *
 *  Created on: May 14, 2015
 *      Author: ricardo
 */

#ifndef EVENTQUEUE_H_
#define EVENTQUEUE_H_

#include <list>
#include "Event.h"

class EventQueue {
public:
	EventQueue(int64_t simulation_start, int64_t simulation_end);
	//Returns next event
	Event& NextEvent();
	void AdvanceQueue();
	//Insert event
	void InsertEvent(Event e);
	//Cancels the next event of the job with the specific type
	void CancelNextEvent(Job* job, EventType type);
	void DelayNextEvent(Job* job, EventType type, int64_t delay);
	void PrintEventQueue() const;
	bool LastEvent() const;
	void GoToEnd();

private:
	std::list<Event>::iterator NextEvent(Job* job, EventType type);
	std::list<Event>::iterator _current_position;
	std::list<Event> _events;
};

#endif /* EVENTQUEUE_H_ */
