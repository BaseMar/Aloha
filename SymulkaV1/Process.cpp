#include "Process.h"



Process::Process() : Phase(0), terminated(false)
{
	my_event = new Event(this);
	clock = 0;
}


Process::~Process() {
	clock = NULL;
	delete my_event;
}
int Process::time()
{
	return my_event->schedule_time;
}

void Process::Activate(int activation_time_, int phase_,int activator ,int source_station_numer_, std::vector<Event *> &eventlist_)
{
	my_event = new Event(this);
	my_event->event_phase_ = phase_;
	my_event->activator_source_ = activator;
	my_event->station_number_ = source_station_numer_;
	my_event->schedule_time = clock + activation_time_;
	if (eventlist_.size() > 0)
	{
		std::vector<Event *>::iterator pos = eventlist_.end() - 1;
		while ((pos != eventlist_.begin()) && (my_event->schedule_time < (*pos)->schedule_time))
			pos--;
		if (my_event->schedule_time < (*pos)->schedule_time)
			eventlist_.insert(pos, static_cast<Event *> (my_event));
		else
			eventlist_.insert(pos + 1, static_cast<Event *> (my_event));
	}
	else
	{
		eventlist_.insert(eventlist_.begin(), static_cast<Event *> (my_event));
	}
}

void Process::UpdateEventListsTimes(int time_to_update_, std::vector<Event *> &eventlist_)
{
	for (int i = 0; i < eventlist_.size(); i++)
	{
		eventlist_[i]->schedule_time -= time_to_update_;
		assert(eventlist_[i]->schedule_time >= 0);
	}
}
