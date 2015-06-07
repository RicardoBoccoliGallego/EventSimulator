/*
 * Processor.cpp
 *
 *  Created on: 03/06/2015
 *      Author: Ricardo
 */

#include <algorithm>

#include "include/Processor.h"

#include "include/Debug.h"

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
	_jobs_data[job] = std::tuple<int64_t, int64_t>(curr_time, 0);
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
		//TODO: Remove less priority job
		/*
		//Try to find less priority job to remove
		std::list<Job*>::iterator less_priority = _running_job;
		for (std::list<Job*>::iterator it = _running_job; it != _processing_jobs.end(); NextJob(it)) {
			if ((*it)->Priority() < (*less_priority)->Priority()) {
				less_priority = it;
			}
		}
		//Check if its priority is smaller
		if ((*less_priority)->Priority() < job->Priority()) {
			//Job to remove is running, need to simulate timeslice
			if (_running_job == less_priority) {
				Remove
				NewTimeslice(events, curr_time);
				if (_running_job->)
				events.CancelNextEvent(job, EventType::ReleaseCPU);
			}
			//Removes job

		}
		else {

		}*/

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
	//Schedules release if it will happen in this timeslice
	int64_t executed_time = std::get<1>(_jobs_data[*_running_job]);
	//int64_t total_time = std::get<2>(_jobs_data[*_running_job]);
	//if (executed_time + TIMESLICE >= total_time)
	//	events.InsertEvent(Event(EventType::ReleaseCPU, curr_time + total_time - executed_time, *_running_job));
	//Schedules next timeslice even if it will release CPU
	//else if (executed_time + TIMESLICE < total_time)
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
	//Didn't execute the time it should
	//if (std::get<1>(_jobs_data[job]) != std::get<2>(_jobs_data[job])) {
	//	DEBUG("Processor::Release error " << __LINE__);
	//	return;
	//}
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

void Processor::StopJob(Job* job, EventQueue& events, int64_t& curr_time) {
	if ((*_running_job) == job) {
		std::tuple<int64_t, int64_t>& last = _jobs_data[*_running_job];
		std::get<1>(last) += (curr_time - last_timeslice);
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
