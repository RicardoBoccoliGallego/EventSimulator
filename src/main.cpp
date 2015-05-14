//============================================================================
// Name        : main.cpp
// Author      : Ricardo Boccoli Gallego
// Version     :
// Copyright   : 
// Description : Main file
//============================================================================

#include <iostream>

int main(int nargs, char * argv[]) {
	//Check for program inputs
	if (nargs != 2 && nargs != 3) {
		std::cout << "usage: " << argv[0] << " [-h] events_file " << std::endl;
		return 1;
	}
	std::string arg(argv[1]);
	if (argv[1][0] == '-') {

		if (arg != "-h") {
			std::cout << "Invalid option " << arg << std::endl;
			return 2;
		}
		std::cout << "This is an Event Simulator and simulates jobs arriving in a Operating System and how they are handled." << std::endl;
		std::cout << "The events_filename should be in the format below:" << std::endl << std::endl;
		std::cout << "initial_time end_time" << std::endl;
		std::cout << "initial_time end_time" << std::endl;
		std::cout << "job_arrive_time processor_time memroy_usage nios" << std::endl << std::endl;

		std::cout << "The last line can be repeated for each job." << std::endl;
		std::cout << "The time is stored as a 64 bits int and is specified in nanoseconds." << std::endl;
		std::cout << "The nios is the number of I/Os the program does and memroy_usage is the size of the program in bytes (it is stored as a 64 bits int as well)."  << std::endl;
		if (nargs == 2)
			return 0;
		arg = std::string(argv[2]);
	}
	std::cout << "Initializing simulator..." << std::endl;


	std::cout << "END" << std::endl;
	return 0;
}
