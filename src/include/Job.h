/*
 * Job.h
 *
 *  Created on: May 18, 2015
 *      Author: ricardo
 */

#ifndef JOB_H_
#define JOB_H_

#include <vector>
#include <cinttypes>
#include <string>

class Job {

public:
	Job(const std::string& name, int64_t execution_time, int64_t needed_memory, int64_t nios);

	std::string Name() const;
	int64_t ExecutionTime() const;
	int64_t NeededMemory() const;
	int64_t NIOs() const;


private:
	const std::string _name;
	const int64_t _execution_time;
	const int64_t _needed_memory;
	const int64_t _nios;
};

#endif /* JOB_H_ */
