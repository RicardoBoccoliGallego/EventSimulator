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
#include "EventQueue.h"

/**
 * Class to represent a simple segmented memory
 */
class Memory {
public:
	Memory(int64_t size);
	/**
	 * Requests memory. Return true if it was able to get
	 */
	void Request(Job* job, EventQueue& events, int64_t& curr_time);

	/**
	 * Releases memory of job
	 * returns if any new job was allocated
	 * new_allocated_jobs contains the list of new allocated jobs
	 */
	void Release(Job* job, EventQueue& events, int64_t& curr_time);

private:
	const static int64_t OVERHEAD = 100;
	const int64_t _memory_size;
	std::list<Job*> _queue_list;
	std::set<Job*> _alocated_jobs;
	int64_t _remaining_size;
};



#endif /* INCLUDE_MEMORY_H_ */
