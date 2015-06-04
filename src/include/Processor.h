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

/**
 * Class to represent a n-core processor
 */
class Processor {
public:
	Processor(int ncores = 1);
	/**
	 * Request CPU for job and assign new events
	 */
	void Request(const Job& job, EventQueue& events);
	/**
	 * Release CPU for job and assign new events
	 */
	void Release(const Job& job, EventQueue& events);


private:
	//Queue of waiting jobs
	std::list<const Job&> _queue_list;
	//Executing jobs with the begining time
	std::set<const Job&> _processing_jobs;
	const int _ncores;
};


#endif /* INCLUDE_PROCESSOR_H_ */
