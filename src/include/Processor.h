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
	Processor(int64_t max_programs, int64_t ncores = 1);
	/**
	 * Request CPU for job and assign new events
	 */
	void Request(Job* job, EventQueue& events, int64_t& curr_time);
	/**
	 * Release CPU for job and assign new events and returns the time the job was executing
	 */
	void Release(Job* job, EventQueue& events, int64_t& curr_time);

	/**
	 * Changes the program running
	 */
	Job* BeginTimeslice(EventQueue& events, int64_t& curr_time);
	void EndTimeslice(EventQueue& events, int64_t& curr_time);

	Job* Running();
	const static int64_t TIMESLICE = 10 * 1000; /* 10 us of time slice */

private:
	/* Add and remove job from executing jobs */
	void InsertJob(Job* job, EventQueue& events, int64_t& curr_time);
	void RemoveJob(Job* job, EventQueue& events, int64_t& curr_time);
	/* Check if job is running and stop it */
	void StopJob(Job* job, EventQueue& events, int64_t& curr_time);

	void NextJob(std::list<Job*>::iterator& next);

	const static int64_t OVERHEAD = 0;
	const static int64_t OVERHEAD_TIMESLICE = 0;
	const static int64_t OVERHEAD_PRIORITY = 0;

	//All jobs with the begining time, the executed time until the moment
	std::map<Job*, std::tuple<int64_t, int64_t>> _jobs_data;
	//Queue of waiting jobs
	std::list<Job*> _queue_list;
	//Jobs being executed
	std::list<Job*> _processing_jobs;
	//Current running job
	std::list<Job*>::iterator _running_job;
	//Next job to run
	std::list<Job*>::iterator _to_run_job;
	const int64_t _max_programs;
	const int64_t _ncores;
	int64_t last_timeslice;

};


#endif /* INCLUDE_PROCESSOR_H_ */
