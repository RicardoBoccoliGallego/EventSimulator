/*
 * SystemFile.h
 *
 *  Created on: 07/06/2015
 *      Author: Ricardo
 */

#ifndef INCLUDE_SYSTEMFILE_H_
#define INCLUDE_SYSTEMFILE_H_


#include <string>
#include <cinttypes>
class Job;

class SystemFile {
public:
	SystemFile() { }
	SystemFile(std::string name, Job* owner, int64_t size, bool privat = true);
	bool HasAccess(const Job* job) const;
	std::string Name() const;
	int64_t Size() const;
	const Job* Owner() const;
	bool Private() const;

	bool operator<(const SystemFile& other) const;

private:
	std::string _name;
	Job* _owner;
	int64_t _size;
	bool _private;

};


#endif /* INCLUDE_SYSTEMFILE_H_ */
