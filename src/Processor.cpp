/*
 * Processor.cpp
 *
 *  Created on: 03/06/2015
 *      Author: Ricardo
 */


#include "include/Processor.h"

#include "include/Debug.h"

Processor::Processor(int64_t ncores) : _ncores(ncores) {
}

void Processor::Request(Job* job, EventQueue& events, int64_t& curr_time) {
	if (_processing_jobs.find(job) !=  _processing_jobs.end())
		return;
	//Try to execute if it has CPU
	if (_processing_jobs.size() < _ncores) {
		curr_time += OVERHEAD;
		_processing_jobs[job] = curr_time;
		events.InsertEvent(Event(EventType::UseCPU, curr_time, job));
	}
	else {
		_queue_list.push_back(job);
	}
}


int64_t Processor::Release(Job* job, EventQueue& events, int64_t& curr_time) {
	if (_processing_jobs.find(job) ==   _processing_jobs.end()) {
		DEBUG("Processor::Release error " << __LINE__);
		return 0;
	}
	//Removes from processing jobs and starts other job
	int64_t ret = curr_time - _processing_jobs[job];
	DEBUG("Release CPU for " << job->Name() << " (" << ret << " ns)");
	_processing_jobs.erase(job);
	if (!_queue_list.empty()) {
		Job* start_job = _queue_list.front();

		curr_time += OVERHEAD;
		_processing_jobs[start_job] = curr_time;
		events.InsertEvent(Event(EventType::UseCPU, curr_time, start_job));
		_queue_list.pop_front();
	}
	return ret;
}

