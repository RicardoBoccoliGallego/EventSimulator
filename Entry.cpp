//
// Created by Ricardo on 27/04/2015.
//

#include "Entry.h"

#include <sstream>

SO::Entry::Entry(const std::string &line) {
    std::istringstream input(line);
    input >> _processingTime >> _arriveTime >> _neededMemory >> _nios;
}

int SO::Entry::ProcessingTime(void) const {
    return _processingTime;
}

int SO::Entry::ArriveTime(void) const {
    return _arriveTime;
}

int SO::Entry::NumberOfIO(void) const {
    return _nios;
}

long long int SO::Entry::NeededMemory(void) const {
    return _arriveTime;
}
