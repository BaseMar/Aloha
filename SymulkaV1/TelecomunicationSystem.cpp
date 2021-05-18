#include "TelecomunicationSystem.h"
#include <iostream>

TelecomunicationSystem::TelecomunicationSystem(int clock_, int system_time_)
{
	current_clock_ = clock_;
	max_system_time_ = system_time_;
	channel_ = new Channel();
	for (int i = 0; i < 4; i++)
		transmitters_.push_back(new Transmitter());
}


TelecomunicationSystem::~TelecomunicationSystem()
{
	delete generators_;
}

std::vector<Transmitter*> TelecomunicationSystem::GetListOfTransmitter()
{
	return transmitters_;
}

Channel* TelecomunicationSystem::GetChannel()
{
	return channel_;
}

int TelecomunicationSystem::GetCurrentClock()
{
	return current_clock_;
}

int TelecomunicationSystem::GetMaxSystemTime()
{
	return max_system_time_;
}

void TelecomunicationSystem::SetCurrentClock(int tim_)
{
	current_clock_ = tim_;
}

void TelecomunicationSystem::UpdateCurrentClock(int tim_)
{
	current_clock_ = tim_;
}

int TelecomunicationSystem::GetMaxRetransmissionAttempt()
{
	return kMaximumRetransmitionAttempt;
}

void TelecomunicationSystem::InitializeKernels()
{
	int line_ = 0;
	do
	{
		cout << "Choose seed line from arry to initialize seeds variables [0-9] \n";	
		cin >> line_;
	} while (line_ < 0 || line_ > 9);
	assert(line_ < 10 && line_ >= 0);
	for (int i = 0; i < 7; i++)
	{
		if (i < 4)
		{
			seeds_exp_[i] = seeds_table_[line_][i];
		}
		if (i == 4)
		{
			seed_for_ctpk_= seeds_table_[line_][i];
		}
		if (i == 5)
		{
			seed_for_R_ = seeds_table_[line_][i];
		}
		if (i == 6)
		{
			seed_for_TER_ = seeds_table_[line_][i];
		}
	}
}

void TelecomunicationSystem::UpdateSeedsCouner(int index)
{
	assert(index < 0 && index > 7);
	seeds_counter[index]++;
	if (seeds_counter[index]>=100000)
	{
		switch (index)
		{
		case 0:
		case 1:
		case 2:
		case 3:
			OldSeedsTable[index].push_back(seeds_exp_[index]);
			break;
		case 4:
			OldSeedsTable[index].push_back(seed_for_ctpk_);
			break;
		case 6:
			OldSeedsTable[index].push_back(seed_for_R_);
			break;
		case 7:
			OldSeedsTable[index].push_back(seed_for_TER_);
			break;
		}
	}
	
}
