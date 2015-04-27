//
// Created by Ricardo on 27/04/2015.
//

#ifndef EVENTSIMULATOR_EVENT_H
#define EVENTSIMULATOR_EVENT_H

namespace SO {

    enum class EventType : int {
        BEGIN_SIMULATION,
        END_SIMULATION,
        BEGIN_JOB,
        END_JOB

    };
    class Event {
    public:
        Event(int time, EventType type) : _time(time), _type(type) {}
    private:
        int _time;
        EventType _type;
    };

}


#endif //EVENTSIMULATOR_EVENT_H
