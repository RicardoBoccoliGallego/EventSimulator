/*
 * Segment.h
 *
 *  Created on: 05/06/2015
 *      Author: Ricardo
 */

#ifndef INCLUDE_MEMORYSEGMENT_H_
#define INCLUDE_MEMORYSEGMENT_H_


#include <cinttypes>

/**
 * Class to handle a segment in the memory
 */
#include "ProgramSegment.h"

class MemorySegment {
public:
	MemorySegment(int64_t position = 0, int64_t size = 0, ProgramSegment * segment = nullptr);

	void Size(int64_t size);
	int64_t Size() const;
	void Position(int64_t position);
	int64_t Position() const;
	ProgramSegment* Segment() const;
	void Segment(ProgramSegment* segment);

	bool operator<(const MemorySegment& other) const;
private:
	int64_t _size;
	int64_t _position;
	ProgramSegment * _segment;
};


#endif /* INCLUDE_MEMORYSEGMENT_H_ */
