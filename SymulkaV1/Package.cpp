#include "Package.h"


Package::Package(int package_id_, int Source_Station_, double arrival_time_, int ctpk_V_)
{
	package_genetator = new Generator();
	id_ = package_id_;
	source_station_ = Source_Station_;
	time_of_apear_ = arrival_time_;
	r_help_variable_ = 0;
	ctpk_ = ctpk_V_;
	r_ = 0;
	leaving_time_ = 0.0;
}


Package::~Package()
{
	delete package_genetator;
}

int Package::GetPackageID()
{
	return id_;
}

// funkcja zliczaj¹ca czas CRP
int Package::GenerateCrpTime(int seed_)
{
	++r_;
	r_help_variable_ = package_genetator->Uniform(seed_,0, pow(2.0, static_cast<double>(r_)))*ctpk_;
	return r_help_variable_;
}

int Package::GetRetransmissionVariable()
{
	return r_;
}

bool Package::GetSendingPackage()
{
	return sending_package_;
}

void Package::SetSendingPackage(bool sentpackage)
{
	sending_package_ = sentpackage;
}

bool Package::GetPackageSentCorectly()
{
	return package_sent_correctly_;
}
void Package::SetPackageSentCorectly(bool corpackage)
{
	package_sent_correctly_ = corpackage;
}
bool Package::SetTER(int& seed_)
{
	if (package_genetator->ZeroOne(1.0,seed_)==1.0)
	{
		ter_ =false;
	}
	else
	{
		ter_ = true;
	}
	return ter_;
}
int Package::GetCTPK()
{
	return ctpk_;
}

double Package::GetLeavingTime()
{
	return leaving_time_;
}

void Package::SetLeavingTime(double time)
{
	leaving_time_ = time;
}

double Package::GetArrivalTime()
{
	return arrival_time_;
}

void Package::SetArrivalTime(int arrivetime)
{
	arrival_time_ = arrivetime;
}

double Package::GetSendingPackageTime()
{
	return sending_package_time_;
}

void Package::SetSendingPackageTime(int sendingtime)
{
	sending_package_time_ = sendingtime;

}

