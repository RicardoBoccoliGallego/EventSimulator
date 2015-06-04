/*
 * Event.cpp
 *
 *  Created on: May 20, 2015
 *      Author: ricardo
 */


#include "Event.h"


Event::Event(EventType event_type, int64_t real_time, Job* job)
	: _event_type(event_type), _real_time(real_time), _job(job)
{
}

int64_t Event::Time() const {
	return _real_time;
}

EventType Event::Type() const {
	return _event_type;
}


Job* Event::EventJob() const {
	return _job;
}

