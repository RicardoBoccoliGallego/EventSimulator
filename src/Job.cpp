/*
 * Job.cpp
 *
 *  Created on: May 14, 2015
 *      Author: ricardo
 */

#include "include/Job.h"
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <tuple>
#include <ctime>
#include <random>
#include <cmath>


#include "include/EventQueue.h"
#include "include/Debug.h"
#include "include/DevicePool.h"
#include "include/Processor.h"

int64_t Job::n_jobs = 0;
Job::Job(const std::string& name, int64_t execution_time, int64_t nios, int64_t priority, const ProgramSegment& head) :
		_id(++n_jobs), _name(name), _execution_time(execution_time), _nios(nios), _priority(priority), _segment_head(head) {

	_executed_time = 0;
	_inter_request_time = _execution_time / (_nios+1);
	_done_ios = -1;
	//No segment running
	_active_segment = nullptr;
	srand(time(NULL));
	//Calculates next I/O device
	FinishIO();
	_next_action = std::pair<JobAction, int64_t>(JobAction::None, 0);
	_segment_head.SegmentJob(this);
}

std::string Job::Name() const {
	return _name;
}
int64_t Job::ExecutionTime() const {
	return _execution_time;
}
int64_t Job::NIOs() const {
	return _nios;
}

int64_t Job::Priority() const {
	return _priority;
}

int64_t Job::MissingTime() const {
	return _execution_time - _executed_time;
}

int64_t Job::MissingIOs() const {
	return _nios - _done_ios;
}

int64_t Job::DoneIOs() const {
	return _done_ios;
}

DeviceType Job::NextIOType() const {
	return _next_io;
}

void Job::AddExecutedTime(int64_t time) {
	DEBUG("[Job::AddExecutedTime] " << Name() << " Executed time " << (_executed_time + time) << "/" << _execution_time);
	_executed_time += time;
	_next_action.second -= time;
}

void Job::FinishIO() {
	_done_ios++;
	int device = std::rand() % 10;
	//50% disk
	if (device < 5)
		device = 0;
	//30% printer
	else if (device < 8)
		device = 1;
	//20%
	else
		device = 2;

	_next_io = static_cast<DeviceType>(device);
}

std::pair<JobAction, int64_t> Job::NextAction() const {
	return _next_action;
}

void Job::AdvanceAction() {
	//There is 33% of chance of memory reference
	int reference = rand() % 3;
	static std::mt19937 generator;
	static std::normal_distribution<> d(Processor::TIMESLICE / 2, Processor::TIMESLICE/5);
	int64_t reference_time = std::abs(d(generator));
	//Memory reference

	_next_action = std::pair<JobAction, int64_t>(JobAction::None, 0);

	if (reference == 0 && reference_time < MissingTime()) {
		if (_active_segment->Children().size() > 0) {
			_next_action = std::pair<JobAction, int64_t>(JobAction::SegmentReference, reference_time);
			int random = rand() % _active_segment->Children().size();
			auto it = _active_segment->Children().begin();
			while (random-- > 0)
				it++;
			_next_segment = const_cast<ProgramSegment*>((&(*it)));
		}
		else if (_active_segment->Parent() != nullptr) {
			_next_action = std::pair<JobAction, int64_t>(JobAction::SegmentReference, reference_time);
			_next_segment = _active_segment->Parent();
		}
	}
	DEBUG("[Job::AdvanceAction] " << Name() <<  " next action is at "  << _next_action.second << " of " << (int) _next_action.first);
	//TODO: I/O
}

ProgramSegment* Job::NextSegmentReference() {
	return _next_segment;
}

void Job::NextSegmentReference(ProgramSegment* next) {
	_next_segment = next;
}
ProgramSegment& Job::SegmentHead() {
	return _segment_head;
}

ProgramSegment* Job::ActiveSegment() {
	return _active_segment;
}

void Job::ActiveSegment(ProgramSegment* active) {
	_active_segment = active;
}


bool Job::operator<(const Job& sec) const {
	return _id < sec._id;
}

namespace {

void PrintSegment(ProgramSegment* seg, const std::string& ident) {
	if (seg != nullptr) {
		DEBUG(ident << " Segment #" << seg->Number() << " (" << seg->Size() << " bytes) - " << seg->Children().size() << " children ");
		for (auto& it : seg->Children()) {
			ProgramSegment* curr = &const_cast<ProgramSegment&>(it);
			if (curr->Parent() != seg)
				DEBUG("ERROR - CHILDEN PARENT DOESN'T MATCH");
			PrintSegment(curr, ident + "    ");
		}
	}
}

}
void Job::PrintSegmentTree() {
	DEBUG(Name() << " Segment Tree: ");
	PrintSegment(&_segment_head, "");
}

void Job::ReadJobsFile(std::string filename, std::set<Job>& job_list, EventQueue& events) {
	std::ifstream file(filename);
	DEBUG("Opening File " << filename << "...");

	if (file.is_open()) {
		DEBUG("Opened File " << filename);
		std::string name;
		int64_t execution_time;
		int64_t nios;
		int64_t arrive_time;
		int64_t priority;
		while ( file >> name >> execution_time >> nios >> arrive_time >> priority) {
			//Stack with segments. Each position contains the segment and number of children
			std::list<std::pair<ProgramSegment&, int64_t>> total_segs;
			//Read first segment
			int64_t segs, n_seg, size;
			file >> n_seg >> size >> segs;
			ProgramSegment temp_head_seg(n_seg, size);
			auto it = job_list.emplace(name, execution_time, nios, priority, temp_head_seg);
			Job* job = const_cast<Job*>(&(*it.first));
			job->n_segs = 1;
			if (segs > 0)
				total_segs.push_back(std::pair<ProgramSegment&, int64_t>(job->SegmentHead(), segs));

			while (!total_segs.empty()) {
				file >> n_seg >> size >> segs;
				ProgramSegment temp_seg(n_seg, size);
				//Add child
				ProgramSegment& inserted = total_segs.back().first.AddChild(temp_seg);
				//Decrement
				total_segs.back().second--;
				//if it isn't leaf, stack it
				if (segs > 0) {
					total_segs.push_back(std::pair<ProgramSegment&, int64_t>(inserted, segs));
				}
				//unstack segs
				while (!total_segs.empty() && total_segs.back().second == 0)
					total_segs.pop_back();
				job->n_segs++;
			}
			job->PrintSegmentTree();
			//Insert event
			events.InsertEvent(Event(EventType::BeginJob, arrive_time, const_cast<Job*>(&(*it.first))));

		}
	}
	else
		DEBUG("Couldn't open " << filename << "!");
}
