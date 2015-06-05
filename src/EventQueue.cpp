/*
 * EventQueue.cpp
 *
 *  Created on: May 14, 2015
 *      Author: ricardo
 */

#include <iomanip>
#include <iostream>
#include "include/EventQueue.h"
#include "include/Debug.h"

EventQueue::EventQueue(int64_t simulation_start, int64_t simulation_end) {
	_events.push_back(Event(EventType::BeginSimulation, simulation_start, nullptr));
	_events.push_back(Event(EventType::EndSimulation, simulation_end, nullptr));
	_current_position = _events.begin();
}

Event& EventQueue::NextEvent() {
	return *_current_position;
}

void EventQueue::AdvanceQueue() {
	if (_current_position != _events.end())
		_current_position++;
}

void EventQueue::InsertEvent(Event e) {
	std::list<Event>::iterator walker = _current_position;
	//Shouldn't happen
	if (e.Time() < walker->Time()) {
		DEBUG("Unexpected Situation" << __LINE__);
		return;
	}
	while (e.Time() >= walker->Time() && walker != _events.end())
		walker++;
	_events.insert(walker, e);
}

void EventQueue::PrintEventQueue() const {

	// tell cout to use our new locale.
	std::cout << std::setw(20) << "Time" << std::setw(20) << "Event Type"
			<< std::setw(20) << "Program" << std::setw(30) << "Action"
			<< std::setw(50) << "Result" << std::endl;;


	for (auto it : _events) {
		std::string time = std::string(SSTR(it.Time()));
		if (time.length() > 3)
			time.insert(time.length() - 3, " ");
		if (time.length() > 7)
					time.insert(time.length() - 7, " ");
		if (time.length() > 11)
							time.insert(time.length() - 11, " ");
		std::cout << std::setw(17) << std::fixed << time  << "ns" << std::setw(20)
				<< EventDescriptions[static_cast<int>(it.Type())] << std::setw(20);
		if (it.EventJob() != nullptr)
			std::cout << it.EventJob()->Name();
		else
			std::cout << " ";
		std::cout << std::setw(30) << EventRoutines[static_cast<int>(it.Type())]
				<< std::setw(50) << it.Action() << std::endl;
	}

}

void EventQueue::GoToEnd() {
	_current_position = _events.end();
}

bool EventQueue::LastEvent() const {
	return _current_position == _events.end();
}

