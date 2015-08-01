/*
 * Event.cpp
 *
 *  Created on: May 20, 2015
 *      Author: ricardo
 */


#include "include/Event.h"
#include "include/DevicePool.h"
#include "include/Processor.h"
#include "include/Debug.h"
#include <sstream>

Event::Event(EventType event_type, int64_t real_time, Job* job, int64_t sum_time)
	: _event_type(event_type), _real_time(real_time), _job(job)
{
	switch (event_type) {
		case EventType::BeginJob:
			_action = job->Name() + " started";
			break;
		case EventType::EndJob:
			_action = job->Name() + " finished " + SSTR(job->MissingTime());
			break;
		case EventType::RequestCPU:
			_action = job->Name() + " is waiting to run";
			break;
		case EventType::UseCPU:
			_action = job->Name() + " will use the CPU";
			break;
		case EventType::ReleaseCPU:
			_action = job->Name() + " stopped running (run for " + SSTR(job->ExecutionTime() - job->MissingTime() + job->NextAction().second + sum_time) + "ns/" + SSTR(job->ExecutionTime()) + "ns)";
			break;
		case EventType::RequestMemory:
			_action = job->Name() + " is waiting for memory";
			break;
		case EventType::SegmentLoaded:
			_action = job->Name() + " segment #" + SSTR(job->NextSegmentReference()->Number()) + " allocated";
			break;
		case EventType::ReleaseMemory:
			_action = job->Name() + " deallocated";
			break;
		case EventType::RequestIO:
			_action = job->Name() + " is waiting for I/O";
			break;
		case EventType::UseIO:
			_action = job->Name() + " uses " + DeviceNames[static_cast<int>(job->NextIO()->type)] + " doing I/O operation (" + SSTR(job->NIOs() - job->MissingIOs() + 1) + "/" + SSTR(job->NIOs()) + ")";
			break;
		case EventType::ReleaseIO:
			_action = job->Name() + " finished I/O";
			break;
		case EventType::BeginTimeSlice:
			_action = job->Name() + " will run for its timeslice";
			break;
		case EventType::EndTimeSlice:
			_action = job->Name() + " finished its timeslice ";
			break;
		case EventType::SegmentReference:
			_action = job->Name() + " referenced segment #" + SSTR(job->NextSegmentReference()->Number());
			break;
		case EventType::SegmentFault:
			_action = job->Name() + " segment #" + SSTR(job->NextSegmentReference()->Number()) + " is not in memory";
			break;
		case EventType::UseFile:
			_action = job->Name() + " does " + (job->NextIO()->oper == DiskOperation::Read ? "read" : "write") + " of file " + job->NextIO()->file.Name() + " of " + SSTR(job->NextIO()->n_tracks) + " sectors";
			break;
		case EventType::ReleaseFile:
			_action = job->Name() + " finished file access";
			break;
		case EventType::RequestFile:
			_action = job->Name() + " is waiting for file access";
			break;
		default:
			_action = "-";
		break;

	}
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

const std::string Event::Action() const {
	return _action;
}

void Event::AddDelay(int64_t delay) {
	_real_time += delay;
}
