#ifndef STATISTICS_H
#define STATISTICS_H
#include <vector>
#include <string>
#include "Transmitter.h"
using namespace std;

class Statistics
{
public:
	Statistics();
	~Statistics();
	void AllowGatheringStatistics(int time_);
	void SetTimeOfStartGatheringStat(int time_);
	int GetTimeOfStartGatheringStat();
	void IncrementNumberOfSuccesfulyTransmitedPackages();
	int GetNumberOfSuccesfulyTransmitedPackages();
	void IncrementNegativePackageNumber(int nrstation);
	void IncrementPositivePackageNumber(int nrstation);
	double CalculateAvaragePackageRatio();
	double CalculateMaximumPackageErrorRatio();
	double CalculateAvarageRtransmitionAttempt();
	void IncreaseRetransmissionAttemptPerTransmitter(vector <Transmitter*> transmitter_,double clock);
	double AravagePackageDelay(int arrivaltime, int leavingtime);
	double AvarageSendingPackage(int arrivaltime, int sendingtime);
	string GetPackageDelayTimeAsString();
	string GetRetrnsmissionNumberPerBSAsString();
	string GetNumberOfPackagesInTheChannelAsString();
	void UpdateNumberOfPackagesInTheChannel(int number);
private:
	bool gathering_statistics_permission_;
	int time_of_start_gathering_statistics_;
	int NumberOfSuccesfulyTransmitedPackages;
	int negative_package_number_[4] = {0, 0, 0, 0 }; // liczba odrzuconych pakietów
	int positive_package_number_[4] = { 0, 0, 0, 0 }; // liczba odrzuconych pakietów
	int number_of_retransmition_per_base_station[4]= { 0, 0, 0, 0 };
	vector<vector<int>> retransmission_per_package_per_BS { {}, {}, {}, {} };
	vector <double> packagedelaytime;
	vector <double> packagesendingtime;
	vector<int> number_of_packages_in_channel_;
	string packagedelaytimestring_;
	int retransmission_modulo_divider;
	vector <double> retransmition_clock_per_bs;
};

#endif