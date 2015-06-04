/*
 * Processor.h
 *
 *  Created on: 03/06/2015
 *      Author: Ricardo
 */

#ifndef INCLUDE_PROCESSOR_H_
#define INCLUDE_PROCESSOR_H_


#include <list>
#include <map>
#include "EventQueue.h"

/**
 * Class to represent a n-core processor
 */
class Processor {
public:
	Processor(int64_t ncores = 1);
	/**
	 * Request CPU for job and assign new events
	 */
	void Request(Job* job, EventQueue& events, int64_t& curr_time);
	/**
	 * Release CPU for job and assign new events and returns the time the job was executing
	 */
	int64_t Release(Job* job, EventQueue& events, int64_t& curr_time);


private:
	const static int64_t OVERHEAD = 0;
	//Queue of waiting jobs
	std::list<Job*> _queue_list;
	//Executing jobs with the begining time
	std::map<Job*, int64_t> _processing_jobs;
	const int64_t _ncores;
};


#endif /* INCLUDE_PROCESSOR_H_ */
