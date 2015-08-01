/*
 * Disk.h
 *
 *  Created on: 07/06/2015
 *      Author: Ricardo
 */

#ifndef INCLUDE_DISK_H_
#define INCLUDE_DISK_H_

#include <map>
#include <list>

#include "SystemFile.h"
#include "EventQueue.h"

class Job;



class Disk {
public:
	Disk(int64_t access_time, int64_t data_transfer_time, int64_t size);
	void Request(Job* job, const std::string& filename, DiskOperation oper, int n_opers, EventQueue& events, int64_t& curr_time);
	void Release(Job* job, EventQueue& events, int64_t& curr_time);
	/* Try to add file */
	bool AddFile(const SystemFile& file);

	int64_t UseTime(int n_access) const;

private:
	int64_t _total_size;
	const int64_t _access_time;
	const int64_t _data_transfer_time;

	std::list<SystemFile> _files;
	int64_t _remaining_size;

	Job* _using_job = nullptr;
	std::list<Job*> _queue_list;
	std::list<DiskOperation> _operation_list;
	std::list<int64_t> _n_opers;
};




#endif /* INCLUDE_DISK_H_ */
