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
#include "include/Disk.h"

int64_t Job::n_jobs = 0;
Job::Job(const std::string& name, int64_t execution_time, std::list<IO> ios, int64_t priority, const ProgramSegment& head) :
		_id(++n_jobs), _name(name), _execution_time(execution_time), _ios(ios), _priority(priority), _segment_head(head) {

	_executed_time = 0;
	_next_io = _ios.begin();
	//No segment running
	_active_segment = nullptr;
	_done_ios = 0;
	srand(time(NULL));

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
	return _ios.size();
}

int64_t Job::MissingIOs() const {
	return _ios.size() - _done_ios;
}

int64_t Job::Priority() const {
	return _priority;
}

int64_t Job::MissingTime() const {
	return _execution_time - _executed_time;
}

IO* Job::NextIO() const {
	if (_next_io == _ios.end())
		return nullptr;
	return const_cast<IO*>(&(*_next_io));
}

void Job::AddExecutedTime(int64_t time) {
	DEBUG("[Job::AddExecutedTime] " << Name() << " Executed time " << (_executed_time + time) << "/" << _execution_time);
	_executed_time += time;
	_next_action.second -= time;
	for (IO& io : _ios)
		io.time -= time;
}

std::pair<JobAction, int64_t> Job::NextAction() const {
	//If the next action is I/O, do it
	if (_next_io != _ios.end() && (_next_io->time < _next_action.second || _next_action.first == JobAction::None || _next_action.second <= 0)) {
		return std::pair<JobAction, int64_t>(JobAction::IO, _next_io->time);
	}
	return _next_action;
}

void Job::AdvanceIO() {
	_done_ios++;
	_next_io++;
}

void Job::AdvanceAction() {
	//There is 33% of chance of memory reference
	int reference = rand() % 3;
	static std::mt19937 generator;
	static std::normal_distribution<> d(Processor::TIMESLICE / 2, Processor::TIMESLICE/5);
	int64_t reference_time = std::abs(d(generator));

	//Memory reference
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

void PrintIOs(const std::string& name, const std::list<IO>& ios) {
	DEBUG(name << " IOs list: ");
	for (const IO& io : ios) {
		if (io.type == DeviceType::Printer)
			DEBUG(io.time <<  ": Printer");
		else if (io.type == DeviceType::Reader)
			DEBUG(io.time <<  ": Reader");
		else
			DEBUG(io.time << ": Disk - File " << io.file.Name() << " (" << io.n_tracks << " sectors)");
	}
}

}
void Job::PrintSegmentTree() {
	DEBUG(Name() << " Segment Tree: ");
	PrintSegment(&_segment_head, "");
}

void Job::ReadJobsFile(std::string filename, std::set<Job>& job_list, EventQueue& events, Disk& disk) {
	std::ifstream file(filename);
	DEBUG("Opening File " << filename << "...");

	if (file.is_open()) {
		DEBUG("Opened File " << filename);
		std::string name;
		int64_t execution_time;
		int64_t nios;
		int64_t arrive_time;
		int64_t priority;
		std::list<IO> ios;
		while ( file >> name >> execution_time >> nios >> arrive_time >> priority) {
			//Stack with segments. Each position contains the segment and number of children
			std::list<std::pair<ProgramSegment&, int64_t>> total_segs;
			//Read first segment
			int64_t segs, n_seg, size;
			file >> n_seg >> size >> segs;
			ProgramSegment temp_head_seg(n_seg, size);
			auto it = job_list.emplace(name, execution_time, ios, priority, temp_head_seg);
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

			//Get job I/Os

			int64_t inter_request = execution_time / (nios + 1);
			int cont = 0;
			while (cont < nios) {
				IO io;
				std::string device;

				//Read the device
				file >> device;
				if (device == "printer")
					io.type = DeviceType::Printer;
				else if (device == "reader")
					io.type = DeviceType::Reader;
				//Disk I/O
				else {
					io.type = DeviceType::Disk;
					std::string filename;
					std::string read_write;
					int64_t n_opers, size, is_private;
					file >> filename >> read_write >> n_opers >> size >> is_private;
					SystemFile sys_file(filename, job, size, is_private);
					io.file = sys_file;
					io.n_tracks = n_opers;
					io.oper = read_write == "r" ? DiskOperation::Read : DiskOperation::Write;
					disk.AddFile(sys_file);
				}

				io.time = inter_request * (++cont);
				ios.push_back(io);
			}
			job->_ios = ios;
			job->_next_io = job->_ios.begin();
			PrintIOs(job->Name(), job->_ios);
			//Insert event
			events.InsertEvent(Event(EventType::BeginJob, arrive_time, const_cast<Job*>(&(*it.first))));

		}

	}
	else
		DEBUG("Couldn't open " << filename << "!");
}
