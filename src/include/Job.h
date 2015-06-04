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
	Job(const std::string& name, int64_t execution_time, int64_t size, int64_t nios);

	std::string Name() const;
	int64_t ExecutionTime() const;
	int64_t Size() const;
	int64_t NIOs() const;


private:
	/* Const values */
	const std::string _name;
	const int64_t _execution_time;
	const int64_t _size;
	const int64_t _nios;

	//Time the job was executed
	int64_t _executed_time;
	//Time before the job ask for I/O or stops
	int64_t _inter_request_time;



};

#endif /* JOB_H_ */
