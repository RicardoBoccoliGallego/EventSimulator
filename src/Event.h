/*
 * Event.h
 *
 *  Created on: May 14, 2015
 *      Author: ricardo
 */

#ifndef EVENT_H_
#define EVENT_H_

#include <cinttypes>

enum class EventType : int {
	BEGIN_JOB,
	END_JOB
};

class Event {
public:
	Event();
	virtual ~Event();

private:
	int64_t _time;
	int _job;
	EventType _type;


};

#endif /* EVENT_H_ */
