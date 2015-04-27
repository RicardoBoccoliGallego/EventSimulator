//
// Created by Ricardo on 27/04/2015.
//

#ifndef EVENTSIMULATOR_EXCEPTION_H
#define EVENTSIMULATOR_EXCEPTION_H

#include <string>

namespace SO {

    class Exception {
    public:
        Exception(const std::string& message) : _message(message) {}
        const std::string& Message(void) const;
    private:
        std::string _message;
    };
}

#endif //EVENTSIMULATOR_EXCEPTION_H
