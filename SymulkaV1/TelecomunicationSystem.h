#ifndef TELECOMUNICATIONSYSTEM_H
#define TELECOMUNICATIONSYSTEM_H
#include <vector>
#include "Transmitter.h"
#include "Channel.h"
#include "Generator.h"
#include <iostream>
#include "Statistics.h"
using namespace std;

class TelecomunicationSystem
{
public:
	TelecomunicationSystem(int clock_, int system_time_);
	~TelecomunicationSystem();
	std::vector<Transmitter*> GetListOfTransmitter();
	Channel *GetChannel();
	int GetCurrentClock();
	int GetMaxSystemTime();
	void SetCurrentClock(int tim_);
	void UpdateCurrentClock(int tim_);
	int GetMaxRetransmissionAttempt();
	Statistics* statistics = new Statistics();
	Generator* generators_ = new Generator();
	int seeds_exp_[4] = { 0, 0, 0, 0 };
	int seed_for_ctpk_ = 0;
	int seed_for_R_ = 0;
	int seed_for_TER_ = 0;
	void InitializeKernels();
	void UpdateSeedsCouner(int index);//Indeks jest numerem generatora w talicy liczników seedów.
	vector<vector<int>> OldSeedsTable = { {},{}, {}, {}, {}, {}, {} };
private:
	int current_clock_;
	int max_system_time_;
	Channel *channel_;
	const int kMaximumRetransmitionAttempt = 5; // maksymalna liczba retransmisji
	const int kNumberOfStations = 4; // liczba nadajników 
	std::vector<Transmitter*> transmitters_; // lista nadajników
	//tablica seedów (liczby wed³ug kolumn): 0-3 - generatory wyk³adnicze dla stacji nadawczych, 4- transmisja pakietu, 5-Retransmisja, 6- b³¹d transmisji w kanale
	int seeds_table_[10][7] = { {127175, 497859, 932128, 536412,298563,789145,469631},
								{438225, 396158, 250454, 195602,960408,378425,133201},
								{910724, 170016, 636425, 970352,639362,540250,204617},
								{665931, 113054, 536575, 425151,402965,427504,652043},
								{457007, 774362, 134202, 350691,187175,485820,304181},
								{966608, 849898, 932128, 747030,853830,935888,404533},
								{325312, 878924, 371764, 209894,700312,860756,354532},
								{241329, 948178, 838317, 397282,804416,927168,959712},
								{971605, 683002, 365331, 179099,189638,579725,815952},
								{601497, 639363, 425788, 800399,923631,314759,631117} };
	int seeds_counter[7] = { 0,0,0,0,0,0,0 };
	
};

#endif