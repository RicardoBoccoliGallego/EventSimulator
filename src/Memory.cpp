/*
 * Memory.cpp
 *
 *  Created on: May 26, 2015
 *      Author: ricardo
 */

#include "include/Memory.h"
#include "include/Debug.h"
Memory::Memory(int64_t size)
: _memory_size(size), _remaining_size(size)
{
	DEBUG("Memory Initialized: " << size << " bytes");
}

void Memory::Request(Job* job, EventQueue& events, int64_t& curr_time) {
	if (_remaining_size >= job->Size()) {
		_remaining_size -= job->Size();
		_alocated_jobs.insert(job);
		curr_time += OVERHEAD;
		events.InsertEvent(Event(EventType::UseMemory, curr_time, job));
	}
	else
		_queue_list.push_back(job);
}

void Memory::Release(Job* job, EventQueue& events, int64_t& curr_time) {
	if (_alocated_jobs.find(job) ==  _alocated_jobs.end())
		return;
	//Removes from allocated jobs and starts other jobs
	_alocated_jobs.erase(job);
	_remaining_size += job->Size();
	while (!_queue_list.empty()) {
		Job* start_job = _queue_list.front();
		if (_remaining_size >=  start_job->Size()) {
			_queue_list.pop_front();
			_alocated_jobs.insert(start_job);
			_remaining_size -= start_job->Size();
			curr_time += OVERHEAD;
			events.InsertEvent(Event(EventType::UseMemory, curr_time, start_job));
		}
		else
			break;
	}
}

