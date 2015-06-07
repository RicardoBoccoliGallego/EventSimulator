/*
 * ProgramSegment.cpp
 *
 *  Created on: 06/06/2015
 *      Author: Ricardo
 */

#include "include/ProgramSegment.h"
#include "include/MemorySegment.h"

#include "include/Debug.h"

ProgramSegment::ProgramSegment(int64_t segment_number, int64_t size, int64_t max_execution, ProgramSegment* parent)
: _segment_number(segment_number), _size(size), _max_execution(max_execution), _parent(parent)
{
	_executed = 0;
	_memory = nullptr;

}

ProgramSegment& ProgramSegment::AddChild(const ProgramSegment& child) {
	std::set<ProgramSegment>::iterator it = _children.insert(child).first;
	ProgramSegment& ret = const_cast<ProgramSegment&>(*it);
	ret._parent = this;
	return ret;
}

ProgramSegment* ProgramSegment::Parent() {
	return _parent;
}
const std::set<ProgramSegment>& ProgramSegment::Children() const {
	return _children;
}

int64_t ProgramSegment::Size() const {
	return _size;
}

int64_t ProgramSegment::Number() const {
	return _segment_number;
}

int64_t ProgramSegment::MaxExecution() const {
	return _max_execution;
}

int64_t ProgramSegment::Executed() const {
	return _executed;
}

void ProgramSegment::Executed(int64_t executed) {
	_executed = executed;
}

bool ProgramSegment::operator<(const ProgramSegment& other) const {
	return _segment_number < other._segment_number;
}

void ProgramSegment::Memory(MemorySegment* memory) {
	_memory = memory;
}

MemorySegment* ProgramSegment::Memory() {
	return _memory;
}
