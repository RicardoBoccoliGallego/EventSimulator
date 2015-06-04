/*
 * Event.h
 *
 *  Created on: May 14, 2015
 *      Author: ricardo
 */

#ifndef EVENT_H_
#define EVENT_H_

#include <cinttypes>
#include "Job.h"


enum class EventType : int {
	BeginSimulation = 0,
	EndSimulation,
	BeginJob,
	EndJob,
	RequestMemory,
	ReleaseMemory,
	RequestCPU,
	ReleaseCPU,
	RequestIO,
	ReleaseIO
};

std::string EventDescriptions[] ={"BeginSimulation",
		"EndSimulation",
		"BeginJob",
		"EndJob",
		"RequestMemory",
		"ReleaseMemory",
		"RequestCPU",
		"ReleaseCPU",
		"RequestIO",
		"ReleaseIO"
};

std::string EventRoutines[] ={
		"?",
		"?",
		"?",
		"?",
		"Memory::Request",
		"Memory::Release",
		"Processor::Request",
		"Processor::Release",
		"IO::Request",
		"IO::Release"
};

class Event {
public:
	Event(EventType event_type, int64_t real_time = -1, Job* job = nullptr);

	int64_t Time() const;
	EventType Type() const;
	Job* EventJob() const;

private:
	EventType _event_type;
	int64_t _real_time;//Real time
	Job* _job;

};

#endif /* EVENT_H_ */
