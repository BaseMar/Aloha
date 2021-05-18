#include "Statistics.h"



Statistics::Statistics()
{
	gathering_statistics_permission_ = false;
	time_of_start_gathering_statistics_ = 2147483647;
	NumberOfSuccesfulyTransmitedPackages = 0;
	retransmission_modulo_divider = 0;
}


Statistics::~Statistics()
{
}

void Statistics::AllowGatheringStatistics(int time_)
{
	if (time_ > time_of_start_gathering_statistics_)
	{
		gathering_statistics_permission_ = true;
	}
	else
	{
		gathering_statistics_permission_ = false;
	}
}

void Statistics::SetTimeOfStartGatheringStat(int time_)
{
	time_of_start_gathering_statistics_ = time_;
}

int Statistics::GetTimeOfStartGatheringStat()
{
	return time_of_start_gathering_statistics_;
}

void Statistics::IncrementNumberOfSuccesfulyTransmitedPackages()
{
	if (gathering_statistics_permission_ == true)
	{
		NumberOfSuccesfulyTransmitedPackages++;
	}
}

int Statistics::GetNumberOfSuccesfulyTransmitedPackages()
{
	return NumberOfSuccesfulyTransmitedPackages;
}
void Statistics::IncrementNegativePackageNumber(int nrstation)
{
	if (gathering_statistics_permission_==true)
		negative_package_number_[nrstation]++;
}
void Statistics::IncrementPositivePackageNumber(int nrstation)
{
	if (gathering_statistics_permission_ == true)
		positive_package_number_[nrstation]++;
}

double Statistics::CalculateAvaragePackageRatio()
{
	double sum = 0.0;
	for (int i = 0; i < 4; i++)
	{
		sum += (double)negative_package_number_[i] / (double)positive_package_number_[i];
	}
	return sum/4;
}

double Statistics::CalculateMaximumPackageErrorRatio()
{
	double value = 0.0;
	if (gathering_statistics_permission_ == true)
	{
		for (int i = 0; i < 4; i++)
		{
			if ((double)negative_package_number_[i] / (double)positive_package_number_[i] > value)
				value = (double)negative_package_number_[i] / (double)positive_package_number_[i];
		}

	}
	return value;
}

double Statistics::CalculateAvarageRtransmitionAttempt()
{
	double suma = 0.0;
	for (int i = 0; i < retransmission_per_package_per_BS[0].size(); i++)
	{
		for (int q = 0; q < 4; q++)
		{
			suma += retransmission_per_package_per_BS[q][i];
		}
	}
	return suma / 4;
}

void Statistics::IncreaseRetransmissionAttemptPerTransmitter(vector <Transmitter*> transmitter_,double clock)
{
	if (gathering_statistics_permission_ == true)
	{
		for (int i = 0; i < 4; i++)
		{
			if (transmitter_[i]->GetListOfPackage().size() != 0)
			{
				retransmission_per_package_per_BS[i].push_back(transmitter_[i]->GetListOfPackage()[0]->GetRetransmissionVariable());
			}
			else
			{
				retransmission_per_package_per_BS[i].push_back(0.0);
			}
		}
		retransmition_clock_per_bs.push_back(clock);
	}
}

double Statistics::AravagePackageDelay(int arrivaltime, int leavingtime)
{
	double avaragetime=0.0;
	if (gathering_statistics_permission_ == true)
	{
		packagedelaytime.push_back(leavingtime - arrivaltime);
		for (int i = 0; i < packagedelaytime.size(); i++)
		{
			avaragetime += packagedelaytime[i];
		}
	}
	return (avaragetime / packagedelaytime.size());
}

double Statistics::AvarageSendingPackage(int arrivaltime, int sendingtime)
{
	double avaragetime = 0.0;
	if (gathering_statistics_permission_ == true)
	{
		packagesendingtime.push_back(sendingtime - arrivaltime);
		for (int i = 0; i < packagesendingtime.size(); i++)
		{
			avaragetime += packagesendingtime[i];
		}
	}
	return (avaragetime / packagesendingtime.size());
}

string Statistics::GetPackageDelayTimeAsString()
{
	for (int i = 0; i < packagedelaytime.size(); i++)
	{
		packagedelaytimestring_ += to_string(packagedelaytime[i]) + "\n";
	}
	return packagedelaytimestring_;
}

string Statistics::GetRetrnsmissionNumberPerBSAsString()//Ta funkcja musi byæ wywo³ana po skoñczeniu symulacji!!!
{
	double suma = 0.0;
	string txt = "";
	for (int i = 0; i < retransmission_per_package_per_BS[0].size(); i++)
	{
		if (retransmission_modulo_divider % 10 == 0)
		{
			for (int q = 0; q < 4; q++)
			{
				suma += retransmission_per_package_per_BS[q][i];
				txt += to_string(retransmission_per_package_per_BS[q][i]) + "\t";
			}
			txt += to_string(suma / 4) + "\t";
			suma = 0.0;
			txt += to_string(retransmition_clock_per_bs[i])+"\n";
		}
		retransmission_modulo_divider++;
	}
	return txt;
}

string Statistics::GetNumberOfPackagesInTheChannelAsString()
{
	string txt_ = "";
	for (int i = 0; i < number_of_packages_in_channel_.size(); i++)
	{
		txt_ += to_string(number_of_packages_in_channel_[i]) + "\n";
	}
	return txt_;
}

void Statistics::UpdateNumberOfPackagesInTheChannel(int number)
{
	number_of_packages_in_channel_.push_back(number);
}
