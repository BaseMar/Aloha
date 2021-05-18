#ifndef PROCCES_H
#define PROCCES_H
#include <assert.h>
#include "TelecomunicationSystem.h"
#include <vector>

class Process;

class Event {
public:
	int event_phase_;
	int activator_source_;
	int schedule_time;
	int station_number_;
	Process* p;
	Event(Process *ptr) : schedule_time(-1.0), p(ptr), event_phase_(-1), station_number_(-1) {}
};

class Process
{
public:
	void virtual Execute(int clock_, TelecomunicationSystem *telecomunication_system_, std::vector<Event *>& eventlist_) = 0;
	int Phase;
	bool terminated;
	Process();
	~Process();
	int time();
	void Activate(int activation_time_, int phase_, int activator, int source_station_numer_, std::vector<Event *> &eventlist_);
	void UpdateEventListsTimes(int time_to_update_, std::vector<Event *> &eventlist_);
	int clock;

private:
	Event* my_event;
};



#endif