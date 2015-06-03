/*
 * Event.cpp
 *
 *  Created on: May 20, 2015
 *      Author: ricardo
 */


#include "include/Event.h"


Event::Event(EventType event_type, int64_t job_time, int64_t real_time, Job* job,
			ResourceType resource_type, UsageType usage_type)
	: _event_type(event_type),  _job_time(job_time), _real_time(real_time),
	  _resource_type(resource_type), _usage_type(usage_type), _job(job)
{
}

