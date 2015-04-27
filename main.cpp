/**
 * Arquivo principal: main.cpp
 * Autor: Ricardo Boccoli Gallego
 * Nusp:  8041992
 */
#include <iostream>
#include <fstream>

#include "Exception.h"
#include "Entry.h"
#include "Event.h"
#include <list>
#include <map>

using namespace std;

void ReadEntries(string fileName, std::map<int, SO::Entry>& entries, int& begin, int& end) {
    ifstream inputFile(fileName);
    if (!inputFile.is_open())
        throw SO::Exception("Arquivo " + fileName + " nao encontrado");
    std::string line;
    inputFile >> begin;
    inputFile >> end;
    while (std::getline(inputFile, line)) {

        if (line.size() > 0) {
            SO::Entry entry(line);
            entries.insert(std::pair<int, SO::Entry>(entry.ArriveTime(), entry));
        }


    }
}


int main (int nargs, char * argv[])
{
    std::map<int, SO::Entry> entries; /* Árvore: facilitar ordenação por ordem de chegada */
    std::list<SO::Event> events; /*Eventos de execução */
    int begin, end;
    try {
        ReadEntries("C:\\Users\\Ricardo\\Documents\\repos\\EventSimulator\\entradas.txt", entries, begin, end);
        std::cout << "Inicio: " << begin << ". Fim: " << end << std::endl;
        std::cout << entries.size() << " entrada(s)" << std::endl;

        events.push_back(SO::Event(begin, SO::EventType::BEGIN_SIMULATION));

        for (const auto& entry : entries) {
            if (entry.second.ArriveTime() >= begin && entry.second.ArriveTime() <= end) {
                
            }
        }
        //TODO: Ler entries e criar eventos

        events.push_back(SO::Event(begin, SO::EventType::BEGIN_SIMULATION));
    }
    catch(SO::Exception e) {
        std::cout << e.Message() << std::endl;
    }

    // Terminou simulação, imprime resultados

    return 0;
}