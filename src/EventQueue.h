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
	EventQueue();
	Event& NextEvent();
	void InsertEvent(Event &&e);
	void PrintEventQueue() const;

private:
	std::list<Event>::iterator _current_position;
	std::list<Event> _events;
};

#endif /* EVENTQUEUE_H_ */
