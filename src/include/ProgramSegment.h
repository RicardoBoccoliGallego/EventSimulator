/*
 * ProgramSegment.h
 *
 *  Created on: 05/06/2015
 *      Author: Ricardo
 */

#ifndef INCLUDE_PROGRAMSEGMENT_H_
#define INCLUDE_PROGRAMSEGMENT_H_

#include <set>
#include <cinttypes>

class MemorySegment;
class Job;
class ProgramSegment {
public:
	ProgramSegment(int64_t segment_number, int64_t size, ProgramSegment* parent = nullptr);
	//Adds child and returns poninter
	ProgramSegment& AddChild(const ProgramSegment& child);
	ProgramSegment* Parent();
	const std::set<ProgramSegment>& Children() const;
	int64_t Size() const;
	int64_t Number() const;
	void Memory(MemorySegment* memory);
	MemorySegment* Memory();
	const Job* SegmentJob() const;
	void SegmentJob(Job* job);

	void Executed(int64_t executed);

	bool operator<(const ProgramSegment& other) const;

private:
	MemorySegment* _memory;
	const int64_t _segment_number;
	const int64_t _size;

	ProgramSegment* _parent;
	std::set<ProgramSegment> _children;
	Job* _job;
};


#endif /* INCLUDE_PROGRAMSEGMENT_H_ */
