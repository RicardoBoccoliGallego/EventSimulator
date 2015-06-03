/*
 * Memory.cpp
 *
 *  Created on: May 26, 2015
 *      Author: ricardo
 */

#include "include/Memory.h"

Memory::Memory(int64_t size)
: _memory_size(size)
{
}

void Memory::RequestMemory(const Job* job) {
	if (_memory_size >= job->NeededMemory()) {
		_memory_size -= job->NeededMemory();
		//_alocated_jobs.insert(job);
	}
//	else
		//_jobs_queue.push_back(job);
}

void Memory::ReleaseMemory(const Job* job) {
	if (true) {
		_memory_size += job->NeededMemory();
	//	if (!_jobs_queue.empty() && _jobs_queue.front()->NeededMemory() <= _memory_size) {
		//	_alocated_jobs.insert(*_jobs_queue.front());
	//		_jobs_queue.pop_front();
//		}
	//	_alocated_jobs.erase(job);
	}
}
