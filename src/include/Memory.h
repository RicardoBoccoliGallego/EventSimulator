/*
 * Memory.h
 *
 *  Created on: May 26, 2015
 *      Author: ricardo
 */

#ifndef INCLUDE_MEMORY_H_
#define INCLUDE_MEMORY_H_


#include <set>
#include <map>
#include <list>
#include <iostream>

#include "Job.h"
#include "EventQueue.h"
#include "MemorySegment.h"

/**
 * Class to represent a simple segmented memory
 */
class Memory {
public:
	Memory(int64_t size);
	/**
	 * Request memory for job in memory
	 */
	void Request(Job* job, ProgramSegment* segment, EventQueue& events, int64_t& curr_time);

	/**
	 * Release of job
	 */
	void Release(Job* job, EventQueue& events, int64_t& curr_time);



private:
	void UnloadSegmentTree(ProgramSegment* seg);
	void FreeSegment(MemorySegment* seg);
	const static int64_t OVERHEAD = 100;
	//Programs segments to be loaded
	std::list<std::pair<Job*,ProgramSegment*>> _queue_list;
	//Memory segments
	std::map<int64_t, MemorySegment> _segments_table;
	std::set<MemorySegment*> _free_segments;
};



#endif /* INCLUDE_MEMORY_H_ */
