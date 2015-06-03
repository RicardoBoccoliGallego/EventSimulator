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

class Memory {
public:
	Memory(int64_t size);
	void RequestMemory(const Job* job);
	void ReleaseMemory(const Job* job);

private:
	int64_t _memory_size;
	std::list<const Job*> _jobs_queue;
	std::set<const Job*> _alocated_jobs;

};



#endif /* INCLUDE_MEMORY_H_ */
