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


#include "include/EventQueue.h"
#include "include/Debug.h"
#include "include/DevicePool.h"

int64_t Job::n_jobs = 0;
Job::Job(const std::string& name, int64_t execution_time, int64_t size, int64_t nios, int64_t priority, const ProgramSegment& head) :
		_id(++n_jobs), _name(name), _execution_time(execution_time), _size(size), _nios(nios), _priority(priority), _segment_head(head) {

	_executed_time = 0;
	_inter_request_time = _execution_time / (_nios+1);
	_done_ios = -1;
	//No segment running
	_active_segment = nullptr;
	srand(time(NULL));
	//Calculates next I/O device
	FinishIO();
	_next_action = std::pair<JobAction, int64_t>(JobAction::None, 0);
}

std::string Job::Name() const {
	return _name;
}
int64_t Job::ExecutionTime() const {
	return _execution_time;
}
int64_t Job::Size() const {
	return _size;
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
	DEBUG("[Job::AddExecutedTime] " << Name() << " Active Segment " << (_active_segment->Executed() + time) << "/" << _active_segment->MaxExecution());
	_executed_time += time;
	_next_action.second -= time;
	_discount += time;
	if (_discount > 0)
		_active_segment->Executed(_active_segment->Executed() + _discount);
}

void Job::DiscountSegment(int64_t time) {
	_discount = -time;
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

void Job::AdvanceAction(bool already_reference, int64_t delay) {
	int64_t max_seg_execution = _active_segment->MaxExecution() - (_active_segment->Executed() + delay);
	int64_t max_execution = _execution_time - (_executed_time + delay);
	int64_t reference_time = std::abs((_execution_time / n_segs) - (_executed_time % (_execution_time / n_segs)));

	// there is no children segment
	if (_active_segment->Children().size() == 0 || already_reference)
		reference_time = max_execution;
	//Finish job
	if (max_execution <= max_seg_execution && max_execution <= reference_time) {
		_next_action = std::pair<JobAction, int64_t>(JobAction::End, max_execution + delay);
		_next_segment = nullptr;
	}
	//Upper segment
	else if (max_seg_execution <= max_execution && max_seg_execution <= reference_time) {
		_next_action = std::pair<JobAction, int64_t>(JobAction::PreviousSegment, max_seg_execution);
		_next_segment = _active_segment->Parent();
	}
	//Children segment
	else if (!already_reference) {
		_next_action = std::pair<JobAction, int64_t>(JobAction::NextSegment, reference_time);

		int random = rand() % _active_segment->Children().size();
		auto it = _active_segment->Children().begin();
		while (random-- > 0)
			it++;
		_next_segment = const_cast<ProgramSegment*>((&(*it)));
	}
	else
		_next_action = std::pair<JobAction, int64_t>(JobAction::None, 0);
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
	_discount = 0;
	_active_segment = active;
	_active_segment->Executed(0);
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
		int64_t size;
		int64_t nios;
		int64_t arrive_time;
		int64_t priority;
		while ( file >> name >> execution_time >> size >> nios >> arrive_time >> priority) {
			//Stack with segments. Each position contains the segment and number of children
			std::list<std::pair<ProgramSegment&, int64_t>> total_segs;
			//Read first segment
			int64_t segs, n_seg, time, size;
			file >> n_seg >> time >> size >> segs;
			ProgramSegment temp_head_seg(n_seg, size, time);
			auto it = job_list.emplace(name, execution_time, size, nios, priority, temp_head_seg);
			Job* job = const_cast<Job*>(&(*it.first));
			job->n_segs = 1;
			if (segs > 0)
				total_segs.push_back(std::pair<ProgramSegment&, int64_t>(job->SegmentHead(), segs));

			while (!total_segs.empty()) {
				file >> n_seg >> time >> size >> segs;
				ProgramSegment temp_seg(n_seg, size, time);
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
