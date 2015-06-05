/*
 * Event.cpp
 *
 *  Created on: May 20, 2015
 *      Author: ricardo
 */


#include "include/Event.h"
#include "include/DevicePool.h"
#include "include/Debug.h"
#include <sstream>

Event::Event(EventType event_type, int64_t real_time, Job* job)
	: _event_type(event_type), _real_time(real_time), _job(job)
{
	switch (event_type) {
		case EventType::BeginJob:
			_action = job->Name() + " started";
			break;
		case EventType::EndJob:
			_action = job->Name() + " finished";
			break;
		case EventType::RequestCPU:
			_action = job->Name() + " is waiting to run";
			break;
		case EventType::UseCPU:
			_action = job->Name() + " uses the CPU (" + SSTR(job->ExecutionTime() - job->MissingTime() + job->ReleaseCPUTime()) + "ns/" + SSTR(job->ExecutionTime()) + "ns)";
			break;
		case EventType::ReleaseCPU:
			_action = job->Name() + " stop running";
			break;
		case EventType::RequestMemory:
			_action = job->Name() + " is waiting for memory";
			break;
		case EventType::UseMemory:
			_action = job->Name() + " allocated";
			break;
		case EventType::ReleaseMemory:
			_action = job->Name() + " deallocated";
			break;
		case EventType::RequestIO:
				_action = job->Name() + " is waiting for I/O";
				break;
		case EventType::UseIO:
			_action = job->Name() + " uses " + DeviceNames[static_cast<int>(job->NextIOType())] + " doing I/O operation (" + SSTR(job->NIOs() - job->MissingIOs() + 1) + "/" + SSTR(job->NIOs()) + ")";
			break;
		case EventType::ReleaseIO:
			_action = job->Name() + " finished I/O";
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
