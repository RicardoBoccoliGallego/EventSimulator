/*
 * SystemFile.cpp
 *
 *  Created on: 07/06/2015
 *      Author: Ricardo
 */

#include "include/SystemFile.h"


SystemFile::SystemFile(std::string name, Job* owner,int64_t size, bool privat)
: _name(name), _owner(owner), _size(size), _private(privat) {

}

bool SystemFile::HasAccess(const Job* job) const {
	return !_private || job == _owner;
}


std::string SystemFile::Name() const {
	return _name;
}

int64_t SystemFile::Size() const {
	return _size;
}

const Job* SystemFile::Owner() const {
	return _owner;
}

bool SystemFile::Private() const {
	return _private;
}

bool SystemFile::operator<(const SystemFile& other) const {
	return _name < other._name;
}
