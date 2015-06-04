/*
 * Job.cpp
 *
 *  Created on: May 14, 2015
 *      Author: ricardo
 */

#include "include/Job.h"

Job::Job(const std::string& name, int64_t execution_time, int64_t size, int64_t nios):
	_name(name), _execution_time(execution_time), _size(size), _nios(nios) {

	_executed_time = 0;
	_inter_request_time = nios == 0 ? _execution_time : _execution_time / _nios;
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
