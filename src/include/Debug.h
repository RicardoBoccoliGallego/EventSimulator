/*
 * Debug.h
 *
 *  Created on: 04/06/2015
 *      Author: Ricardo
 */

#ifndef INCLUDE_DEBUG_H_
#define INCLUDE_DEBUG_H_
#include <iostream>
#include <sstream>

#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()


#ifndef NDEBUG
#  define DEBUG(x)  std::cout << "[DEBUG] " <<  x << std::endl
#else
#  define DEBUG(x) do {} while (0)
#endif



#endif /* INCLUDE_DEBUG_H_ */
