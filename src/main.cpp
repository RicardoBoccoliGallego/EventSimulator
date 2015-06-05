//============================================================================
// Name        : main.cpp
// Author      : Ricardo Boccoli Gallego
// Version     :
// Copyright   : 
// Description : Main file
//============================================================================

#include <iostream>
#include <set>
#include <sstream>
#include <string>

#include "include/EventQueue.h"
#include "include/Debug.h"
#include "include/Memory.h"
#include "include/Processor.h"
#include "include/DevicePool.h"


using namespace std;
int main(int nargs, char * argv[]) {
	DEBUG("Starting...");
	set<Job> jobs;
	int64_t start = -1, end = -1, current_time;
	std::string jobs_file = "";
	//Check for program inputs
	if (nargs < 4) {
		std::cout << "usage: " << argv[0] << " [options] begin end file" << std::endl
				<< " [-h] for help " << std::endl;
		return 1;
	}
	for (int i = 1; i < nargs; i++) {
		if (std::string(argv[i]) == "-h") {
			std::cout << "This is an Event Simulator and simulates jobs arriving in a Operating System and how they are handled." << std::endl;
			std::cout << "The events_filename should be in the format below:" << std::endl << std::endl;
			std::cout << "initial_time end_time" << std::endl;
			std::cout << "initial_time end_time" << std::endl;
			std::cout << "job_arrive_time processor_time memroy_usage nios" << std::endl << std::endl;

			std::cout << "The last line can be repeated for each job." << std::endl;
			std::cout << "The time is stored as a 64 bits int and is specified in nanoseconds." << std::endl;
			std::cout << "The nios is the number of I/Os the program does and memroy_usage is the size of the program in bytes (it is stored as a 64 bits int as well)."  << std::endl;
			return 0;
		}
		else if (argv[i][0] == '-') {
			std::cout << "Invalid option '" << argv[i] << "'" << std::endl;
			return -1;
		}
		else {
			std::stringstream stream(argv[i]);
			if (start == -1)
				stream >> start;
			else if (end == -1)
				stream >> end;
			else if (jobs_file == "")
				stream >> jobs_file;
			else {
				std::cout << "Invalid arguments size" << std::endl;
				return -1;
			}
		}
	}

	DEBUG("Initializing simulator...");

	//Computer components
	Memory memory((int64_t) 10 * 1024 * 1024); //10MiB memory
	Processor cpu;
	// Three device pools (disk, printer and reader)
	DevicePool devices[] = {
			DevicePool(DeviceType::Disk, 15563*1000, 1),
			DevicePool(DeviceType::Printer, 2*1000*1000*1000, 2),
			DevicePool(DeviceType::Reader, 100*1000*1000, 2)
	};

	EventQueue events(start, end);
	current_time = 0;
	while (!events.LastEvent()) {
		Event& current_event = events.NextEvent();
		Job* current_job = current_event.EventJob();
		//Check if processing last event finished after this event arrival
		if (current_event.Time() > current_time) {
			current_time = current_event.Time();
		}
		DEBUG("Time " << current_time << " event: " << EventDescriptions[(int)current_event.Type()]);
		switch (current_event.Type()) {
			case EventType::BeginSimulation:
				Job::ReadJobsFile(jobs_file, jobs, events);
				current_time = start;
				break;
			case EventType::EndSimulation:
				events.GoToEnd();
				current_time = end;
				break;
			case EventType::BeginJob:
				//Schedule Memory request
				events.InsertEvent(Event(EventType::RequestMemory, current_time, current_job));
				break;
			case EventType::EndJob:
				break;
			case EventType::RequestMemory:
				memory.Request(current_job, events, current_time);
				break;
			case EventType::UseMemory:
				//Schedule CPU
				events.InsertEvent(Event(EventType::RequestCPU, current_time, current_job));
				break;
			case EventType::ReleaseMemory:
				//Job got out
				memory.Release(current_job, events, current_time);
				events.InsertEvent(Event(EventType::EndJob, current_time, current_job));
				break;
			case EventType::RequestCPU:
				//Request CPU
				cpu.Request(current_event.EventJob(), events, current_time);
				break;
			case EventType::UseCPU:
				//Schedule Release CPU
				events.InsertEvent(Event(EventType::ReleaseCPU, current_time + current_job->ReleaseCPUTime(), current_job));
				break;
			case EventType::ReleaseCPU: {
				//Release CPU
				int64_t executed_time = cpu.Release(current_job, events, current_time);
				//Mark execution time
				current_job->AddExecutedTime(executed_time);
				//Schedule end or I/O
				if (current_job->MissingTime() == 0 && current_job->MissingIOs() == 0)
					events.InsertEvent(Event(EventType::ReleaseMemory, current_time, current_job));
				else
					events.InsertEvent(Event(EventType::RequestIO, current_time, current_job));
				break;
			}
			case EventType::RequestIO: {
				//Chooses device and requests
				devices[static_cast<int>(current_job->NextIOType())].Request(current_job, events, current_time);
				break;
			}
			case EventType::UseIO:
				//Schedule I/O end
				events.InsertEvent(Event(EventType::ReleaseIO, current_time + devices[static_cast<int>(current_job->NextIOType())].IOTime(), current_job));
				break;
			case EventType::ReleaseIO:

				devices[static_cast<int>(current_job->NextIOType())].Release(current_job, events, current_time);
				current_job->FinishIO();
				if (current_job->MissingTime() == 0 && current_job->MissingIOs() == 0)
					events.InsertEvent(Event(EventType::ReleaseMemory, current_time, current_job));
				else
					events.InsertEvent(Event(EventType::RequestCPU, current_time, current_job));
				break;
		}

		events.AdvanceQueue();
	}

	events.PrintEventQueue();


	DEBUG("Finished!");
	return 0;
}
