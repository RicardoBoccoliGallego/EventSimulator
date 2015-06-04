/*
 * Memory.h
 *
 *  Created on: May 26, 2015
 *      Author: ricardo
 */

#ifndef INCLUDE_MEMORY_H_
#define INCLUDE_MEMORY_H_


#include <set>
#include <list>
#include <iostream>

#include "Job.h"

/**
 * Class to represent a simple segmented memory
 */
class Memory {
public:
	Memory(int64_t size);
	/**
	 * Requests memory. Return true if it was able to get
	 */
	bool Request(const Job& job);

	/**
	 * Releases memory of job
	 * returns if any new job was allocated
	 * new_allocated_jobs contains the list of new allocated jobs
	 */
	bool Release(const Job& job, std::list<const Job&>& new_allocated_jobs);

private:
	const int64_t _memory_size;
	std::list<const Job&> _jobs_queue;
	std::set<const Job&> _alocated_jobs;
	int64_t _used_size;
};



#endif /* INCLUDE_MEMORY_H_ */
