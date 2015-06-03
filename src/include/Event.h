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

enum class ResourceType : int {
	None,
	Memory,
	CPU,
	Printer,
	Disk
};

enum class UsageType : int {
	None,
	Request,
	Use,
	Release,
};

enum class EventType : int {
	ResourceUsage,
	BeginJob,
	EndJob
};

class Event {
public:
	Event(EventType event_type, int64_t job_time = 0, int64_t real_time = -1, Job* job = nullptr,
			ResourceType resource_type = ResourceType::None, UsageType usage_type = UsageType::None);
	//virtual ~Event();


private:
	EventType _event_type;
	int64_t _job_time;//User time
	int64_t _real_time;//Real time


	ResourceType _resource_type;
	UsageType _usage_type;
	Job* _job;

};

#endif /* EVENT_H_ */
