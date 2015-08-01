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
#include <random>
#include <list>

#include "ProgramSegment.h"
#include "SystemFile.h"

enum class DiskOperation {
	Read, Write
};
class Disk;
class EventQueue;


enum class DeviceType : int {
	Disk,
	Printer,
	Reader
};

enum class JobAction {
	IO, SegmentReference, None
};

struct IO {
	DeviceType type;
	int64_t time;
	SystemFile file;
	DiskOperation oper;
	int n_tracks;//Number of tracks
};

class Job {

public:
	Job(const std::string& name, int64_t execution_time, std::list<IO> nios, int64_t priority, const ProgramSegment& head);

	std::string Name() const;
	int64_t ExecutionTime() const;
	int64_t NIOs() const;

	int64_t MissingTime() const;
	int64_t Priority() const;

	void AddExecutedTime(int64_t time);
	//Go to the next action
	void AdvanceAction();
	void AdvanceIO();
	std::pair<JobAction, int64_t> NextAction() const;
	IO* NextIO() const;
	int64_t MissingIOs() const;

	/* Methods of segment tree */
	ProgramSegment& SegmentHead();
	ProgramSegment* NextSegmentReference();
	void NextSegmentReference(ProgramSegment* next);
	void PrintSegmentTree();
	ProgramSegment* ActiveSegment();
	void ActiveSegment(ProgramSegment* active);

	bool operator<(const Job& sec) const;

	static void ReadJobsFile(std::string filename, std::set<Job>& job_list, EventQueue& events, Disk& disk);

private:
	/* Const values */
	static int64_t n_jobs;
	const int64_t _id;
	const std::string _name;
	const int64_t _execution_time;
	std::list<IO> _ios;
	const int64_t _priority;
	int64_t _done_ios;

	//Time the job was executed
	int64_t _executed_time;

	ProgramSegment _segment_head;
	ProgramSegment * _active_segment;
	int64_t n_segs;
	std::pair<JobAction, int64_t> _next_action;

	std::list<IO>::iterator _next_io;
	ProgramSegment* _next_segment;

};

#endif /* JOB_H_ */

