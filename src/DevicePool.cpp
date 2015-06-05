/*
 * DevicePool.cpp
 *
 *  Created on: 04/06/2015
 *      Author: Ricardo
 */

#include "include/DevicePool.h"
#include "include/Debug.h"

DevicePool::DevicePool(DeviceType type, int64_t io_time, unsigned int n_devices)
	: _type(type), _io_time(io_time), _n_devices(n_devices), _used_devices(0)
{
}

void DevicePool::Request(Job* job, EventQueue& events, int64_t& curr_time) {
	//All devices being used
	if (_jobs_using_devices.size() == _n_devices) {
		_queue_list.push_back(job);
		DEBUG("Job " << job->Name() << " waiting for device");
	}
	else {
		curr_time += OVERHEAD;
		_jobs_using_devices.insert(job);
		events.InsertEvent(Event(EventType::UseIO, curr_time, job));
		DEBUG("Job " << job->Name() << " got device");
	}
}

void DevicePool::Release(Job* job, EventQueue& events, int64_t& curr_time) {
	_jobs_using_devices.erase(job);
	DEBUG("Job " << job->Name() << " release device");
	if (!_queue_list.empty()) {
		 Job* start_job = _queue_list.front();
		_queue_list.pop_front();
		_jobs_using_devices.insert(start_job);
		curr_time += OVERHEAD;
		events.InsertEvent(Event(EventType::UseIO, curr_time, start_job));
		DEBUG("Job " << start_job->Name() << " got device");

	}
}

DeviceType DevicePool::Type() const {
	return _type;
}

int64_t DevicePool::IOTime() const {
	return _io_time;
}


