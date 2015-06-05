/*
 * Job.h
 *
 *  Created on: May 18, 2015
 *      Author: ricardo
 */

#ifndef JOB_H_
#define JOB_H_

#include <cinttypes>
#include <string>
#include <set>

class EventQueue;
enum class DeviceType;
class Job {

public:
	Job(const std::string& name, int64_t execution_time, int64_t size, int64_t nios, int64_t priority);

	std::string Name() const;
	int64_t ExecutionTime() const;
	int64_t Size() const;
	int64_t NIOs() const;

	int64_t MissingTime() const;
	int64_t MissingIOs() const;
	int64_t DoneIOs() const;
	DeviceType NextIOType() const;
	int64_t Priority() const;

	void AddExecutedTime(int64_t time);
	void FinishIO();
	int64_t ReleaseCPUTime() const;

	bool operator<(const Job& sec) const;

	static void ReadJobsFile(std::string filename, std::set<Job>& job_list, EventQueue& events);

private:
	/* Const values */
	static int64_t n_jobs;
	const int64_t _id;
	const std::string _name;
	const int64_t _execution_time;
	const int64_t _size;
	const int64_t _nios;
	const int64_t _priority;

	DeviceType _next_io;
	//Time the job was executed
	int64_t _executed_time;
	//Time before the job ask for I/O or stops
	int64_t _inter_request_time;
	//Number of IOs done
	int64_t _done_ios;



};

#endif /* JOB_H_ */

