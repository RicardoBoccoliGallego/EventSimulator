/*
 * EventQueue.cpp
 *
 *  Created on: May 14, 2015
 *      Author: ricardo
 */

#include "include/EventQueue.h"

EventQueue::EventQueue() {
	// TODO Auto-generated constructor stub

}

Event& EventQueue::NextEvent() {
	return *_current_position;
}

void EventQueue::InsertEvent(Event e) {

}
void EventQueue::PrintEventQueue() const {

}

