//
// Created by Ricardo on 27/04/2015.
//

#ifndef EVENTSIMULATOR_ENTRY_H
#define EVENTSIMULATOR_ENTRY_H


#include <string>

namespace SO {
    class Entry {
    public:
        Entry(const std::string& line);
        int ArriveTime(void) const;
        int ProcessingTime(void) const;
        long long int NeededMemory(void) const;
        int NumberOfIO(void) const;

    private:
        int _arriveTime; /*Tempo de chegada do job */
        int _processingTime; /*Tempo necess�rio de processamento */
        long long int _neededMemory; /* mem�ria necess�ria em bytes */
        int _nios; /*N�mero de entradas/sa�das */

    };
}


#endif //EVENTSIMULATOR_ENTRY_H
