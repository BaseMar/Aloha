#ifndef PACKAGEPROCESS_H
#define PACKAGEPROCESS_H
#include"Process.h"
#include"Package.h"
#include <iostream>
#define GENERATEPACKAGE 0
#define SENDINGPACKAGE 1
#define SENDINGACK 2
#define RECEIVEACK 3
#define RETRANSMITIONPACKAGE 4

using namespace std;

class PackageProcess: public Process
{
public:
	PackageProcess(int intencity_of_package_gen_);
	~PackageProcess();
	void SetSourceStationNumber(int s_stat_num_);
	int GetSourceStationNumber();
	void Execute(int clock_, TelecomunicationSystem *telecomunication_system_, std::vector<Event *>& eventlist_)override;
private:
	int intencity_L_of_package_generation_;
	static int package_id_number;
	int source_station_number;
	int processId;
	Package *package_;
	int FindPackageIndexInChannelBySourceStation(TelecomunicationSystem *telecomunication_system_, int source_station_);
};
#endif