/*
 * Memory.cpp
 *
 *  Created on: May 26, 2015
 *      Author: ricardo
 */

#include "include/Memory.h"
#include "include/Debug.h"

#include <algorithm>

Memory::Memory(int64_t size)
{
	_segments_table[0] = MemorySegment(0, size, nullptr);
	_free_segments.insert(&_segments_table[0]);
}

void Memory::Request(Job* job, ProgramSegment* segment, EventQueue& events, int64_t& curr_time) {
	//Try to find free segment
	for (auto it  = _free_segments.begin(); it != _free_segments.end(); it++) {
		//Get segment
		MemorySegment& free = *(*it);
		if (free.Size() >= segment->Size()) {
			int64_t free_size = free.Size() - segment->Size();
			//Insert element (size and segment loaded)
			free.Size(segment->Size());
			free.Segment(segment);
			//Remove this element
			_free_segments.erase(it);
			//Insert free
			if (free_size > 0) {
				//Insert new free entry
				_segments_table[free.Position() + segment->Size()] = MemorySegment(free.Position() + segment->Size(), free_size, nullptr);
				_free_segments.insert(&_segments_table[free.Position() + segment->Size()]);
			}
			//curr_time += OVERHEAD;
			//Set active segment and is loaded

			job->NextSegmentReference(segment);

			segment->Memory(&free);
			//Loaded segment
			events.InsertEvent(Event(EventType::SegmentLoaded, curr_time + OVERHEAD, job));
			return;
		}
	}
	//Didn't find free segment, go to the queue
	_queue_list.push_back(std::pair<Job*, ProgramSegment*>(job, segment));

}

void Memory::FreeSegment(MemorySegment* seg) {
	seg->Segment(nullptr);


	auto it = _segments_table.find(seg->Position());
	//Go behind until it is free
	while (it != _segments_table.begin() && it->second.Segment() == nullptr)
		it--;
	//Begining of merge
	auto beginning = it;
	//Go forward
	while (it != _segments_table.end() && it->second.Segment() == nullptr) {
		//Increase size
		beginning->second.Size(beginning->second.Size() + it->second.Size());
		//Remove existing free entry
		_free_segments.erase(&it->second);
		//Remove entry
		if (it != beginning)
			_segments_table.erase(it++);
		else
			it++;
	}

	//Add free entry
	_free_segments.insert(&beginning->second);
}


void Memory::UnloadSegmentTree(ProgramSegment* seg) {
	if (seg != nullptr) {
		if (seg->Memory() != nullptr) {
			FreeSegment(seg->Memory());
		}
		for (auto& it : seg->Children()) {
			ProgramSegment* curr = &const_cast<ProgramSegment&>(it);
			if (curr->Parent() != seg)
				DEBUG("ERROR - CHILDEN PARENT DOESN'T MATCH");
			UnloadSegmentTree(curr);
		}
	}
}


void Memory::Release(Job* job, EventQueue& events, int64_t& curr_time) {
	//Release all jobs segments
	UnloadSegmentTree(&job->SegmentHead());
}


