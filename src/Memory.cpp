/*
 * Memory.cpp
 *
 *  Created on: May 26, 2015
 *      Author: ricardo
 */

#include "Memory.h"

Memory::Memory(int64_t size)
: _memory_size(size), _used_size(0)
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

bool Memory::Release(const Job& job, std::list<const Job&>& new_allocated_jobs) {
	_used_size -= job.NeededMemory()
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
