/*
 * MemorySegment.cpp
 *
 *  Created on: 05/06/2015
 *      Author: Ricardo
 */

#include "include/MemorySegment.h"


MemorySegment::MemorySegment(int64_t position, int64_t size, ProgramSegment * segment)
	: _size(size), _position(position), _segment(segment) {

}

void MemorySegment::Position(int64_t position) {
	_position = position;
}

int64_t MemorySegment::Position() const {
	return _position;
}
void MemorySegment::Size(int64_t size) {
	_size = size;
}

int64_t MemorySegment::Size() const {
	return _size;
}

ProgramSegment* MemorySegment::Segment() const {
	return _segment;
}

void MemorySegment::Segment(ProgramSegment* segment) {
	_segment = segment;
}

bool MemorySegment::operator<(const MemorySegment& other) const {
	return _position < other._position;
}
