/*
 * Job.cpp
 *
 *  Created on: May 14, 2015
 *      Author: ricardo
 */

#include "include/Job.h"
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <tuple>
#include <ctime>

#include "include/EventQueue.h"
#include "include/Debug.h"
#include "include/DevicePool.h"

int64_t Job::n_jobs = 0;
Job::Job(const std::string& name, int64_t execution_time, int64_t size, int64_t nios, int64_t priority):
		_id(++n_jobs), _name(name), _execution_time(execution_time), _size(size), _nios(nios), _priority(priority) {

	_executed_time = 0;
	_inter_request_time = _execution_time / (_nios+1);
	_done_ios = -1;
	//Seeds the rand
	std::srand(std::time(0));
	//Calculates next I/O device
	FinishIO();
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

int64_t Job::Priority() const {
	return _priority;
}

int64_t Job::MissingTime() const {
	return _execution_time - _executed_time;
}

int64_t Job::MissingIOs() const {
	return _nios - _done_ios;
}

int64_t Job::DoneIOs() const {
	return _done_ios;
}

DeviceType Job::NextIOType() const {
	return _next_io;
}

void Job::AddExecutedTime(int64_t time) {
	_executed_time += time;
	DEBUG("Job " << Name() << " - " << (100*_executed_time/_execution_time) << "%");
}

void Job::FinishIO() {
	_done_ios++;
	int device = std::rand() % 10;
	//50% disk
	if (device < 5)
		device = 0;
	//30% printer
	else if (device < 8)
		device = 1;
	//20%
	else
		device = 2;

	_next_io = static_cast<DeviceType>(device);
}

int64_t Job::ReleaseCPUTime() const {
	if (MissingTime() < _inter_request_time || MissingIOs() == 0)
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
		int64_t priority;
		while ( file >> name >> execution_time >> size >> nios >> arrive_time >> priority) {
			auto it = job_list.emplace(name, execution_time, size, nios, priority);
			events.InsertEvent(Event(EventType::BeginJob, arrive_time, const_cast<Job*>(&(*it.first))));
		}
	}
	else
		DEBUG("Couldn't open " << filename << "!");
}
