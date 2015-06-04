/*
 * EventQueue.cpp
 *
 *  Created on: May 14, 2015
 *      Author: ricardo
 */

#include "include/EventQueue.h"
#include <iomanip>

EventQueue::EventQueue(int64_t simulation_start, int64_t simulation_end) {
	_events.push_back(Event(EventType::BeginSimulation, simulation_start));
	_events.push_back(Event(EventType::EndSimulation, simulation_end));
	_current_position = _events.begin();
}

const Event& EventQueue::NextEvent() {
	return *_current_position;
}

void EventQueue::AdvanceQueue() {
	_current_position++;
}

void EventQueue::InsertEvent(Event e) {
	std::list<Event>::iterator walker = _current_position;
	//Shouldn't happen
	if (e.Time() < walker->Time())
		return;
	while (e.Time() >= walker->Time() && walker != _events.end())
		walker++;
	_events.insert(--walker, e);
}

void EventQueue::PrintEventQueue() const {
	std::cout << std::setw(10) << "Time" << std::setw(20) << "Event Type"
			<< std::setw(30) << "Program" << std::setw(30) << "Action"
			<< std::setw(20) << "Result";
	for (const std::list<Event>::iterator it : _events) {
		std::cout  << std::setw(10) << it->Time()  << std::setw(20)
				<< EventDescriptions[it->Type()] << std::setw(30);
		if (it->EventJob() != nullptr)
			std::cout << it->EventJob()->Name();
		else
			std::cout << " ";
		std::cout << std::setw(30) << EventRoutines[it->Type()]
				<< std::setw(20) << "Action!" << std::endl;
	}
}

