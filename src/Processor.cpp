/*
 * Processor.cpp
 *
 *  Created on: 03/06/2015
 *      Author: Ricardo
 */

#include <algorithm>

#include "include/Processor.h"

#include "include/Debug.h"


const int64_t Processor::TIMESLICE = 10 * 1000;
Processor::Processor(int64_t max_programs, int64_t ncores) : _max_programs(max_programs), _ncores(ncores) {
	_running_job = _processing_jobs.end();
	_to_run_job = _processing_jobs.end();
	last_timeslice = 0;
}

void Processor::Request(Job* job, EventQueue& events, int64_t& curr_time) {
	if (_jobs_data.find(job) !=  _jobs_data.end()) {
		DEBUG("Processor::Request of existing Job");
		return;
	}
	//Record job data
	_jobs_data[job] = curr_time;
	//Try to execute if it has CPU
	if (_processing_jobs.size() < _ncores * _max_programs)
		InsertJob(job, events, curr_time);
	else {
		//Put job in the queue
		std::list<Job*>::iterator walk = _queue_list.begin();
		//Walk until find less priority
		while (walk != _queue_list.end() && (*walk)->Priority() >= job->Priority())
			walk++;
		_queue_list.insert(walk, job);
	}
}

Job* Processor::Running() {
	if (_running_job == _processing_jobs.end())
		return nullptr;
	return const_cast<Job*>((*_running_job));
}

void Processor::EndTimeslice(EventQueue& events, int64_t& curr_time) {
	//little fix
	Event* next = events.NextEventOf(*_running_job, EventType::EndJob);
	if (next != nullptr && next->Time() == curr_time)
		return;
	//Removes executing job
	_to_run_job = _running_job;
	if (_running_job != _processing_jobs.end()) {
		(*_running_job)->AddExecutedTime(curr_time - last_timeslice);
	}
	//Next job
	NextJob(_to_run_job);
	if (_to_run_job != _processing_jobs.end())
		events.InsertEvent(Event(EventType::BeginTimeSlice, curr_time, *_to_run_job));
	last_timeslice = curr_time;
}

Job* Processor::BeginTimeslice(EventQueue& events, int64_t& curr_time) {
	//Removes executing job
	last_timeslice = curr_time;
	_running_job = _to_run_job;
	_to_run_job = _processing_jobs.end();
	events.InsertEvent(Event(EventType::EndTimeSlice, curr_time + TIMESLICE, *_running_job));
	return *_running_job;
}


void Processor::Release(Job* job, EventQueue& events, int64_t& curr_time) {
	if (std::find(_processing_jobs.begin(), _processing_jobs.end(), job) == _processing_jobs.end()) {
		DEBUG("Processor::Release error : Job doesn't exist ");
		return;
	}


	if (*_running_job == job) {
		//Cancel timeslice
		events.CancelNextEvent(job, EventType::EndTimeSlice);
		if (_processing_jobs.size() == 1) {
			events.CancelNextEvent(job, EventType::BeginTimeSlice);
			(*_running_job)->AddExecutedTime(curr_time - last_timeslice);
			_running_job = _processing_jobs.end();
		}
		else
			EndTimeslice(events, curr_time);
	}
	//Remove from processing jobs
	RemoveJob(job, events, curr_time);
	//Removes data
	_jobs_data.erase(job);

	if (!_queue_list.empty()) {
		Job* start_job = _queue_list.front();
		InsertJob(start_job, events, curr_time);
		_queue_list.pop_front();
	}
}


void Processor::InsertJob(Job* job, EventQueue& events, int64_t& curr_time) {
	auto new_job = _processing_jobs.insert(_running_job, job);
	events.InsertEvent(Event(EventType::UseCPU, curr_time, job));
	//No job running, start
	if (_running_job == _processing_jobs.end() && _to_run_job == _processing_jobs.end()) {
		_to_run_job = new_job;
		events.InsertEvent(Event(EventType::BeginTimeSlice, curr_time, *_to_run_job));
	}
}

void Processor::RemoveJob(Job* job, EventQueue& events, int64_t& curr_time) {
	auto it = std::find(_processing_jobs.begin(), _processing_jobs.end(), job);
	if (it != _processing_jobs.end()) {
		_processing_jobs.remove(*it);
	}
}

void Processor::NextJob(std::list<Job*>::iterator& next) {
	if (next == _processing_jobs.end())
		next = _processing_jobs.begin();
	else {
		next++;
		if (next == _processing_jobs.end())
			next = _processing_jobs.begin();
	}

}
