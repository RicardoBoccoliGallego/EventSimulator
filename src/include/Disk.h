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

class Job;

enum class DiskOperation {
	Read, Write, ReadWrite
};

class Disk {
public:
	Disk(int64_t record_read_time, int64_t record_write_time, int64_t size);
	void Request(Job* job, const std::string& filename, DiskOperation oper);
	void Release(Job* job);
	/* Try to add file */
	bool AddFile(const SystemFile& file);
	bool AddFile(const SystemFile& file);

private:
	const int64_t _read_time;
	const int64_t _write_time;
	std::map<int64_t, SystemFile> files;
	std::list<Job*> _queue_list;
};




#endif /* INCLUDE_DISK_H_ */
