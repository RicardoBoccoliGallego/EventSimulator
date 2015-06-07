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
	ReleaseIO,
	SegmentLoaded,
	UseCPU,
	UseIO,
	BeginTimeSlice,
	EndTimeSlice,
	SegmentReference,
	SegmentFault
};

enum class ActionType : int {
	None
};


const std::string EventDescriptions[] = {
		"BeginSimulation",
		"EndSimulation",
		"BeginJob",
		"EndJob",
		"RequestMemory",
		"ReleaseMemory",
		"RequestCPU",
		"ReleaseCPU",
		"RequestIO",
		"ReleaseIO",
		"SegmentLoaded",
		"UseCPU",
		"UseIO",
		"BeginTimeSlice",
		"EndTimeSlice",
		"SegmentReference",
		"SegmentFault",
};

const std::string EventRoutines[] = {
		"Job::ReadJobsFile",
		"EventQueue::InsertEvent",
		"None",
		"None",
		"Memory::Request",
		"Memory::Release",
		"Processor::Request",
		"Processor::Release",
		"DevicePool::Request",
		"DevicePool::Release",
		"EventQueue::InsertEvent",
		"EventQueue::InsertEvent",
		"EventQueue::InsertEvent",
		"Processor::BeginTimeslice",
		"Processor::EndTimeslice",
		"Memory::Segment",
		"Memory::Segment"
};

class Event {
public:
	Event(EventType event_type, int64_t real_time, Job* job);


	int64_t Time() const;
	EventType Type() const;
	Job* EventJob() const;
	const std::string Action() const;
	void AddDelay(int64_t delay);

private:

	EventType _event_type;
	int64_t _real_time;//Real time
	Job* _job;
	std::string _action = "";

};

#endif /* EVENT_H_ */
