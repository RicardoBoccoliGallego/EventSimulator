/*
 * Processor.cpp
 *
 *  Created on: 03/06/2015
 *      Author: Ricardo
 */


#include "Processor.h"

Processor::Processor(int ncores) : _ncores(ncores) {
}

void Processor::Request(const Job& job, EventQueue& events) {
	if (_processing_jobs.find(job) !=  std::set::end)
		return;

	//Try to execute if it has CPU
	if (_processing_jobs.size() < _ncores) {
		_processing_jobs.insert(job);
		events.InsertEvent(Event())
	}
	else {
		_queue_list.push_back(job);
		return false;
	}
}


void Processor::Release(const Job& job, EventQueue& events) {
	if (_processing_jobs.find(job) ==  std::set::end)
		return nullptr;
	//Removes from processing jobs and starts other job
	_processing_jobs.erase(job);
	if (!_queue_list.empty()) {
		Job& start_job = _queue_list.front();
		_processing_jobs.insert(start_job);
		_queue_list.pop_front();
		return &start_job;
	}
	return nullptr;

}

