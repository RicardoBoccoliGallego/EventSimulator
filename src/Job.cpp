/*
 * Job.cpp
 *
 *  Created on: May 14, 2015
 *      Author: ricardo
 */

#include "include/Job.h"
#include <fstream>
#include <iostream>
#include <tuple>

#include "include/EventQueue.h"
#include "include/Debug.h"

int64_t Job::n_jobs = 0;
Job::Job(const std::string& name, int64_t execution_time, int64_t size, int64_t nios):
		_id(++n_jobs), _name(name), _execution_time(execution_time), _size(size), _nios(nios) {

	_executed_time = 0;
	_inter_request_time = _execution_time / (_nios+1);
	_done_ios = 0;
	std::cout << "Created job " << name << std::endl;
}

std::string Job::Name() const {
	return _name;
}
int64_t Job::ExecutionTime() const {
	return _execution_time;
}
int64_t Job::Size() const {
	return _size;
}
int64_t Job::NIOs() const {
	return _nios;
}

int64_t Job::MissingTime() const {
	return _execution_time - _executed_time;
}

int64_t Job::MissingIOs() const {
	return _nios - _done_ios;
}

void Job::AddExecutedTime(int64_t time) {
	_executed_time += time;
	DEBUG("Job " << Name() << " - " << (100*_executed_time/_execution_time) << "%");
}

void Job::FinishIO() {
	_done_ios++;
}

int64_t Job::ReleaseCPUTime() const {
	if (MissingTime() < _inter_request_time)
		return MissingTime();
	return _inter_request_time;
}


bool Job::operator<(const Job& sec) const {
	return _id < sec._id;
}

void Job::ReadJobsFile(std::string filename, std::set<Job>& job_list, EventQueue& events) {
	std::ifstream file(filename);
	DEBUG("Opening File " << filename << "...");

	if (file.is_open()) {
		DEBUG("Opened File " << filename);
		std::string name;
		int64_t execution_time;
		int64_t size;
		int64_t nios;
		int64_t arrive_time;
		while ( file >> name >> execution_time >> size >> nios >> arrive_time) {
			auto it = job_list.emplace(name, execution_time, size, nios);
			events.InsertEvent(Event(EventType::BeginJob, arrive_time, const_cast<Job*>(&(*it.first))));
		}
	}
	else
		DEBUG("Couldn't open " << filename << "!");
}
