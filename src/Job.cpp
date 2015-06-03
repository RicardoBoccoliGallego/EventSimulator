/*
 * Job.cpp
 *
 *  Created on: May 14, 2015
 *      Author: ricardo
 */

#include "include/Job.h"

Job::Job(const std::string& name, int64_t execution_time, int64_t needed_memory, int64_t nios):
	_name(name), _execution_time(execution_time), _needed_memory(needed_memory), _nios(nios) {

}

std::string Job::Name() const {
	return _name;
}
int64_t Job::ExecutionTime() const {
	return _execution_time;
}
int64_t Job::NeededMemory() const {
	return _needed_memory;
}
int64_t Job::NIOs() const {
	return _nios;
}
