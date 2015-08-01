/*
 * Disk.cpp
 *
 *  Created on: 26/06/2015
 *      Author: Ricardo
 */

#include "include/Disk.h"

using namespace std;

Disk::Disk(int64_t access_time, int64_t data_transfer_time, int64_t size)
: _total_size(size), _access_time(access_time), _data_transfer_time(data_transfer_time)
{
	_remaining_size = _total_size;
}

namespace {
	SystemFile* FindFile(list<SystemFile>& files, const std::string& filename) {
		for (SystemFile& file : files) {
			if (file.Name() == filename)
				return &file;
		}
		return nullptr;
	}
}

void Disk::Request(Job* job, const std::string& filename, DiskOperation oper, int n_opers, EventQueue& events, int64_t& curr_time)
{
	//Find file
	if (_using_job == nullptr) {
		_using_job = job;
		if (oper == DiskOperation::Write)
			_total_size -= 512*n_opers;
		events.InsertEvent(Event(EventType::UseFile, curr_time, job));
	}
	else {
		_queue_list.push_back(job);
		_operation_list.push_back(oper);
		_n_opers.push_back(n_opers);
	}
}

void Disk::Release(Job* job, EventQueue& events, int64_t& curr_time)
{
	_using_job = nullptr;
	if (!_queue_list.empty()) {
		_using_job = _queue_list.front();
		if (_operation_list.front() == DiskOperation::Write)
			_total_size -= 512 * (_n_opers.front());
		events.InsertEvent(Event(EventType::UseFile, curr_time, _using_job));
		_queue_list.pop_front();
		_operation_list.pop_front();
		_n_opers.pop_front();
	}
}

bool Disk::AddFile(const SystemFile& file)
{
	if (FindFile(_files, file.Name()) == nullptr) {
		_files.push_back(file);
	}
	return true;
}

int64_t Disk::UseTime(int n_access) const {
	return _access_time + n_access*_data_transfer_time;
}
