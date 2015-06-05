/*
 * Device.h
 *
 *  Created on: 04/06/2015
 *      Author: Ricardo
 */

#ifndef INCLUDE_DEVICEPOOL_H_
#define INCLUDE_DEVICEPOOL_H_

#include <vector>

#include "EventQueue.h"

enum class DeviceType : int {
	Disk = 0,
	Printer,
	Reader
};

const std::string DeviceNames[] = {
		"Disk", "Printer", "Reader"
};

class DevicePool {
public:
	DevicePool(DeviceType type, int64_t io_time, unsigned int n_devices);

	void Request(Job* job, EventQueue& events, int64_t& curr_time);

	void Release(Job* job, EventQueue& events, int64_t& curr_time);

	DeviceType Type() const;
	int64_t IOTime() const;
private:
	const static int OVERHEAD = 10;
	const DeviceType _type;
	const int64_t _io_time;
	const unsigned int _n_devices;
	int _used_devices;
	std::list<Job*> _queue_list;
	std::set<Job*> _jobs_using_devices;

};



#endif /* INCLUDE_DEVICEPOOL_H_ */
