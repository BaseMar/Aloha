#include <iostream>
#include "Channel.h"
#include "Package.h"
#include "Transmitter.h"
#include "TelecomunicationSystem.h"
#include "PackageProcess.h"
#include <ctime>
#include<conio.h>
#include <string>
#include <fstream>
using namespace std;

int main()
{
	srand(time(NULL));
	double intencity_L_;
	cout << "Give intencity L of package generation! (Prefered 0.015)" << endl;
	cin >> intencity_L_;
	intencity_L_ = 1/intencity_L_;
	int customer_simmulation_time_ = 0, time_of_start_gather_stat_=0;
	cout << "Give simmulation time in [s]: " << endl;
	cin >> customer_simmulation_time_;
	cout << "Specify time of start gathering statistics in [s] (Prefered start phase = 10s)" << endl;
	cin >> time_of_start_gather_stat_;
	TelecomunicationSystem *telecomunication_system = new TelecomunicationSystem(0, customer_simmulation_time_*1000);
	telecomunication_system->statistics->SetTimeOfStartGatheringStat(time_of_start_gather_stat_*1000);
	telecomunication_system->InitializeKernels();
	Transmitter *transmitter = new Transmitter();
	Channel *channel = new Channel();
	std::vector<Event *> event_list_;// Lista eventów inaczej agenda.

	bool step_mode_ = false;
	char step_mode_char=' ';

	cout << "Wybierz tryb: " << endl;
	while ((step_mode_char != '1'&& step_mode_char != '2'))
	{
		cout << "[1] Tryb krokowy. \n[2] Tryb ciagly." << endl;
		cin >> step_mode_char;

		if (step_mode_char == '1')
			step_mode_ = true;
		else if(step_mode_char == '2')
			step_mode_ = false;
		else
		cout << "Prosze wybrac na klawiaturze tylko 1 lub 2" << endl;
	} 
	PackageProcess* package_process_station_0 = new PackageProcess(intencity_L_);
	PackageProcess* package_process_station_1 = new PackageProcess(intencity_L_);
	PackageProcess* package_process_station_2 = new PackageProcess(intencity_L_);
	PackageProcess* package_process_station_3 = new PackageProcess(intencity_L_);
	package_process_station_0->Activate(telecomunication_system-> generators_->Exponential(intencity_L_,telecomunication_system->seeds_exp_[0]), 0,-1 ,0, event_list_);
	package_process_station_1->Activate(telecomunication_system->generators_->Exponential(intencity_L_, telecomunication_system->seeds_exp_[1]), 0,-1 ,1, event_list_);
	package_process_station_2->Activate(telecomunication_system->generators_->Exponential(intencity_L_, telecomunication_system->seeds_exp_[2]), 0,-1 ,2, event_list_);
	package_process_station_3->Activate(telecomunication_system->generators_->Exponential(intencity_L_, telecomunication_system->seeds_exp_[3]), 0,-1 ,3, event_list_);
	Process* current;
	int current_phase_ = -1;
	int current_sys_time_before_update_ = 0, current_sys_time_difference_ = 0;
	
	while (telecomunication_system->GetCurrentClock() < telecomunication_system->GetMaxSystemTime())
	{
		current = event_list_[0]->p;
		telecomunication_system->UpdateCurrentClock(event_list_[0]->schedule_time);
		cout << "Current system time: " << telecomunication_system->GetCurrentClock()<<endl;
		cout << "Base station from event list: " << event_list_[0]->station_number_<<endl;
		current->Execute(telecomunication_system->GetCurrentClock(),telecomunication_system, event_list_);
		event_list_.erase(event_list_.begin());
		
		// Check if it's time to start gathering statistics:
		telecomunication_system->statistics->AllowGatheringStatistics(telecomunication_system->GetCurrentClock());

		if (package_process_station_0->terminated == true)
		{
			delete package_process_station_0;
			package_process_station_0 = new PackageProcess(intencity_L_);
			package_process_station_0->Activate(telecomunication_system->GetCurrentClock() + telecomunication_system->generators_->Exponential(intencity_L_, telecomunication_system->seeds_exp_[0]), GENERATEPACKAGE, -1, 0, event_list_);
		}
			
		if (package_process_station_1->terminated == true)
		{
			delete package_process_station_1;
			package_process_station_1 = new PackageProcess(intencity_L_);
			package_process_station_1->Activate(telecomunication_system->GetCurrentClock() + telecomunication_system->generators_->Exponential(intencity_L_, telecomunication_system->seeds_exp_[1]), GENERATEPACKAGE, -1, 1, event_list_);
			
		}
			
		if (package_process_station_2->terminated == true)
		{
			delete package_process_station_2;
			package_process_station_2 = new PackageProcess(intencity_L_);
			package_process_station_2->Activate(telecomunication_system->GetCurrentClock() + telecomunication_system->generators_->Exponential(intencity_L_, telecomunication_system->seeds_exp_[2]), GENERATEPACKAGE, -1, 2, event_list_);
		}
			
		if (package_process_station_3->terminated == true)
		{
			delete package_process_station_3;
			package_process_station_3 = new PackageProcess(intencity_L_);
			package_process_station_3->Activate(telecomunication_system->GetCurrentClock() + telecomunication_system->generators_->Exponential(intencity_L_, telecomunication_system->seeds_exp_[3]), GENERATEPACKAGE, -1, 3, event_list_);
		}
		
		//Debug mode: 
		if (step_mode_ == true)
		{
			cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
			getchar();
		}
	}
	cout << "Number of packages left in BS[0] " << telecomunication_system->GetListOfTransmitter()[0]->GetListOfPackage().size() << endl;
	cout << "Number of packages left in BS[1] " << telecomunication_system->GetListOfTransmitter()[1]->GetListOfPackage().size() << endl;
	cout << "Number of packages left in BS[2] " << telecomunication_system->GetListOfTransmitter()[2]->GetListOfPackage().size() << endl;
	cout << "Number of packages left in BS[3] " << telecomunication_system->GetListOfTransmitter()[3]->GetListOfPackage().size() << endl;
	cout << "Statistical result:" << endl;
	cout << "Avarage Packet Error Ratio: " << telecomunication_system->statistics->CalculateAvaragePackageRatio()*100  << " %" << endl;
	cout << "Maximum Packet Error ratio: " << telecomunication_system->statistics->CalculateMaximumPackageErrorRatio() * 100 << " %" << endl;
	cout << "Avarage Retransmision Attempt: " << telecomunication_system->statistics->CalculateAvarageRtransmitionAttempt() << endl;
	cout << "System flow: " << to_string((double)telecomunication_system->statistics->GetNumberOfSuccesfulyTransmitedPackages()*1000.0 / (double)(telecomunication_system->GetMaxSystemTime() - telecomunication_system->statistics->GetTimeOfStartGatheringStat())) << " [packages/s]" << endl;
	cout << "Avarage Package Delay: " << telecomunication_system->statistics->AravagePackageDelay(0, 0) << " [ms]"<< endl;
	cout << "Avarage Waiting Package Time : " << telecomunication_system->statistics->AvarageSendingPackage(0, 0) << " [ms]" << endl;
	ofstream package_delay_stream_;
	string saturation_to_save_;
	string filename_;
	saturation_to_save_ = telecomunication_system->statistics->GetRetrnsmissionNumberPerBSAsString();
	cout << "Give saturation file name." << endl;
	cin >> filename_;
	package_delay_stream_.open(filename_);
	if (package_delay_stream_.good() == true)
	{
		package_delay_stream_ << saturation_to_save_;
		package_delay_stream_.close();
	}
	delete telecomunication_system;
	delete transmitter;
	delete channel;
	delete package_process_station_0;
	delete package_process_station_1;
	delete package_process_station_2;
	delete package_process_station_3;
	system("Pause");
	return 0;
}