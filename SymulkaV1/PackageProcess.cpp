#include "PackageProcess.h"

PackageProcess::PackageProcess(int intencity_of_package_gen_)
{
	processId = 1;
	intencity_L_of_package_generation_ = intencity_of_package_gen_;
}

int PackageProcess::package_id_number = 0;

PackageProcess::~PackageProcess()
{
	delete package_;
}

void PackageProcess::SetSourceStationNumber(int s_stat_num_)
{
	source_station_number = s_stat_num_;
}

int PackageProcess::GetSourceStationNumber()
{
	return source_station_number;
}

void PackageProcess::Execute(int clock_, TelecomunicationSystem *telecomunication_system_, std::vector<Event *>& eventlist_)
{
	bool active = true;
	int process_time_=0;
	int index_of_package_in_channel_ = -1;
	source_station_number = eventlist_[0]->station_number_;
	Phase = eventlist_[0]->event_phase_;
	while (active)
	{
		switch (Phase)
		{
		case GENERATEPACKAGE:
			//cout << "Phase - Generate Package. Source station: " << source_station_number <<". Next package will apear at system time: "<< (process_time_ + clock_) << endl;
			package_ = new Package(++package_id_number, source_station_number,telecomunication_system_->GetCurrentClock() ,telecomunication_system_->generators_->Uniform(telecomunication_system_->seed_for_ctpk_,1.0,10.0));
			telecomunication_system_->statistics->IncrementPositivePackageNumber(source_station_number);
			telecomunication_system_->GetListOfTransmitter()[source_station_number]->SetPackageToBuffor(package_);
			telecomunication_system_->GetListOfTransmitter()[source_station_number]->GetListOfPackage()[0]->SetArrivalTime(telecomunication_system_->GetCurrentClock());
			if (telecomunication_system_->GetListOfTransmitter()[source_station_number]->GetListOfPackage().size() != 0)
			{
				//cout << "Package ID: " << telecomunication_system_->GetListOfTransmitter()[source_station_number]->GetListOfPackage()[0]->GetPackageID()<<endl;
			}
			else
			{
				//cout << "No package in the buffer!" << endl;
			}
			//cout << "\n";
			process_time_ = telecomunication_system_->GetCurrentClock();
			active = false;
			Activate(process_time_, SENDINGPACKAGE, GENERATEPACKAGE, source_station_number, eventlist_);
			break;
		case SENDINGPACKAGE:
			//cout << "Phase - Sending Package. Source station: " << source_station_number << endl;
			if (telecomunication_system_->GetListOfTransmitter()[source_station_number]->GetListOfPackage().size() != 0)
			{
				//cout << "Package ID: " << telecomunication_system_->GetListOfTransmitter()[source_station_number]->GetListOfPackage()[0]->GetPackageID() << endl;
			}
			else
			{
				//cout << "No package in the buffer!" << endl;
			}
			// sprawdzenie czy wygenerowany pakiet jest najstarszy
			assert(telecomunication_system_->GetListOfTransmitter()[source_station_number]->GetListOfPackage().size() > 0);
			if(telecomunication_system_->GetListOfTransmitter()[source_station_number]->GetListOfPackage().size() > 0)
			{ 
				if (telecomunication_system_->GetListOfTransmitter()[source_station_number]->GetListOfPackage()[0]->GetSendingPackage() == false)
				{
					//cout << "Sending package... " << endl;
					telecomunication_system_->GetListOfTransmitter()[source_station_number]->GetListOfPackage()[0]->SetSendingPackage(true);
					telecomunication_system_->GetChannel()->CopyPackageToChannelBuffer(telecomunication_system_->GetListOfTransmitter()[source_station_number]->GetListOfPackage()[0]);
					telecomunication_system_->GetListOfTransmitter()[source_station_number]->SetReceiveAckFlag(false);
					//cout << "Set status channel: busy. " << endl;
					telecomunication_system_->GetChannel()->SetChannelStatus(false);
					
					telecomunication_system_->GetListOfTransmitter()[source_station_number]->GetListOfPackage()[0]->SetSendingPackageTime(telecomunication_system_->GetCurrentClock());
					telecomunication_system_->statistics->AvarageSendingPackage(telecomunication_system_->GetListOfTransmitter()[source_station_number]->GetListOfPackage()[0]->GetArrivalTime(), telecomunication_system_->GetListOfTransmitter()[source_station_number]->GetListOfPackage()[0]->GetSendingPackageTime());

					//sprawdzenie kolizji
					if (telecomunication_system_->GetChannel()->GetQueuePackage().size() > 1)
					{
						//cout << "Collision in channel !" << endl;
						for (int i = 0; i < telecomunication_system_->GetChannel()->GetQueuePackage().size(); i++)
						{
							telecomunication_system_->GetChannel()->GetQueuePackage()[i]->SetPackageSentCorectly(false);
						}
						Activate(telecomunication_system_->GetCurrentClock(), RETRANSMITIONPACKAGE, SENDINGPACKAGE, source_station_number, eventlist_);
						//cout << "Error transmition. Package is attempting retransmition. " << endl;
						active = false;
						break;
					}
					// sprawdzenie b³êdu transmisji
					else if (telecomunication_system_->GetChannel()->GetQueuePackage()[FindPackageIndexInChannelBySourceStation(telecomunication_system_, source_station_number)]->SetTER(telecomunication_system_->seed_for_TER_) == true)
					{
						process_time_ = telecomunication_system_->GetChannel()->GetQueuePackage()[FindPackageIndexInChannelBySourceStation(telecomunication_system_, source_station_number)]->GetCTPK() + 1;
						telecomunication_system_->GetChannel()->GetQueuePackage()[FindPackageIndexInChannelBySourceStation(telecomunication_system_, source_station_number)]->SetPackageSentCorectly(false);
						//cout << "There is a transmition error (ter)" << endl;
						Activate(telecomunication_system_->GetCurrentClock() + process_time_, RETRANSMITIONPACKAGE, SENDINGPACKAGE, source_station_number, eventlist_);
						active = false;
						break;
					}

					process_time_ = telecomunication_system_->GetChannel()->GetQueuePackage()[FindPackageIndexInChannelBySourceStation(telecomunication_system_, source_station_number)]->GetCTPK(); // czas CTPk
					Activate(telecomunication_system_->GetCurrentClock()+process_time_, SENDINGACK, SENDINGPACKAGE,source_station_number, eventlist_);
					//cout << "Receiver will get package at system time: " << (process_time_ + clock_) << endl;
				}
				else
				{
					//cout << "In the bufor there is a package already sending for transmitter nr. " << source_station_number << endl;
				}
			}
			telecomunication_system_->statistics->UpdateNumberOfPackagesInTheChannel(telecomunication_system_->GetChannel()->GetQueuePackage().size());
			//cout << "\n";
			active = false;
			break;
		
		case SENDINGACK:
			//std::cout << "Phase - Sending ACK. " << "Source Station: " << source_station_number << endl;
			if (telecomunication_system_->GetListOfTransmitter()[source_station_number]->GetListOfPackage().size() != 0)
			{
				//cout << "Package ID: " << telecomunication_system_->GetListOfTransmitter()[source_station_number]->GetListOfPackage()[0]->GetPackageID() << endl;
			}
			else
			{
				//cout << "No package in the buffer!" << endl;
			}
			process_time_ = 1; //czas CTIZ
			

			//sprawdzenie kolizji
			if (telecomunication_system_->GetChannel()->GetQueuePackage().size() > 1)
			{
				//cout << "Collision in channel !" << endl;
				for (int i = 0; i < telecomunication_system_->GetChannel()->GetQueuePackage().size(); i++)
				{
					telecomunication_system_->GetChannel()->GetQueuePackage()[i]->SetPackageSentCorectly(false);
				}
				Activate(telecomunication_system_->GetCurrentClock()+1, RETRANSMITIONPACKAGE, SENDINGACK,source_station_number, eventlist_);
				//cout << "Error transmition. Package is attempting retransmition. " << endl;
			}
			else
			{
				//cout << "There was no collision. " << endl;
				telecomunication_system_->GetListOfTransmitter()[source_station_number]->SetReceiveAckFlag(true);
				Activate(telecomunication_system_->GetCurrentClock()+process_time_, RECEIVEACK, SENDINGACK,source_station_number, eventlist_);
			}
			//cout << "\n";
			active = false;
			break;
		
		case RECEIVEACK:
			//cout << "Phase - Receive ACK. " << "Source Station: " << source_station_number << endl;
			if (telecomunication_system_->GetListOfTransmitter()[source_station_number]->GetListOfPackage().size() != 0)
			{
				//cout << "Package ID: " << telecomunication_system_->GetListOfTransmitter()[source_station_number]->GetListOfPackage()[0]->GetPackageID() << endl;
			}
			else
			{
				//cout << "No package in the buffer!" << endl;
			}
			
			//sprawdzenie kolizji
			if (telecomunication_system_->GetChannel()->GetQueuePackage().size() > 1)
			{
				//cout << "Collision in channel !" << endl;
				for (int i = 0; i < telecomunication_system_->GetChannel()->GetQueuePackage().size(); i++)
				{
					telecomunication_system_->GetChannel()->GetQueuePackage()[i]->SetPackageSentCorectly(false);
				}
				Activate(telecomunication_system_->GetCurrentClock(), RETRANSMITIONPACKAGE, RECEIVEACK, source_station_number, eventlist_);
				//cout << "Error transmition. Package is attempting retransmition. " << endl;
				active = false;
				break;
			}

			// usuwanie pakietu
			if ( telecomunication_system_->GetListOfTransmitter()[source_station_number]->GetListOfPackage()[0]->GetPackageSentCorectly() == true)
			{
				telecomunication_system_->statistics->IncrementNumberOfSuccesfulyTransmitedPackages();// Zwiêksz liczbê (w statystykach) prawid³owo wygenerowanych pakietów
				//cout << "Deleting Package from source station : " << source_station_number << endl;
				//cout << "***********************************************************************************" << endl;
				telecomunication_system_->GetListOfTransmitter()[source_station_number]->GetListOfPackage()[0]->SetLeavingTime( telecomunication_system_->GetCurrentClock());
				telecomunication_system_->statistics->AravagePackageDelay(telecomunication_system_->GetListOfTransmitter()[source_station_number]->GetListOfPackage()[0]->GetArrivalTime(), telecomunication_system_->GetListOfTransmitter()[source_station_number]->GetListOfPackage()[0]->GetLeavingTime());
				telecomunication_system_->statistics->IncreaseRetransmissionAttemptPerTransmitter(telecomunication_system_->GetListOfTransmitter(),telecomunication_system_->GetCurrentClock());
				telecomunication_system_->GetChannel()->DeletePackageAtIndex(FindPackageIndexInChannelBySourceStation(telecomunication_system_, source_station_number));
				telecomunication_system_->GetListOfTransmitter()[source_station_number]->DeletePackageFromBuffer();
				terminated = true;	
			}
			else
			{
				Activate(telecomunication_system_->GetCurrentClock(), RETRANSMITIONPACKAGE, RECEIVEACK,source_station_number, eventlist_);
				//cout << "Package with ID: "+to_string(telecomunication_system_->GetListOfTransmitter()[source_station_number]->GetListOfPackage()[0]->GetPackageID())+" must be retransmited." << endl;
			}
			//sprawdzenie czy jest wolny kana³ po odebraniu ACK
			if (telecomunication_system_->GetChannel()->GetQueuePackage().size() == 0)
			{
				//cout << "Set channel status to free." << endl;
				telecomunication_system_->GetChannel()->SetChannelStatus(true);
			}
			cout << "\n";
			active = false;
			break;
		case RETRANSMITIONPACKAGE:
			//cout << "Phase - Retransmition Attempt. " << "Source Station: " << source_station_number << endl;
			if (telecomunication_system_->GetListOfTransmitter()[source_station_number]->GetListOfPackage().size() != 0)
			{
				assert(telecomunication_system_->GetListOfTransmitter()[source_station_number]->GetListOfPackage().size() != 0);
				//cout << "Package ID: " << telecomunication_system_->GetListOfTransmitter()[source_station_number]->GetListOfPackage()[0]->GetPackageID() << endl;
			}
			else
			{
				//cout << "No package in the buffer!" << endl;
			}
			process_time_ = telecomunication_system_->GetListOfTransmitter()[source_station_number]->GetListOfPackage()[0]->GenerateCrpTime(telecomunication_system_->seed_for_R_);
			//cout << "Number of retransmission of current package: " << telecomunication_system_->GetListOfTransmitter()[source_station_number]->GetListOfPackage()[0]->GetRetransmissionVariable() << endl;
			telecomunication_system_->GetListOfTransmitter()[source_station_number]->GetListOfPackage()[0]->SetPackageSentCorectly(true);

			// sprawdzenie czy nie przekroczy³ liczby prób retransmisji
			if (telecomunication_system_->GetListOfTransmitter()[source_station_number]->GetListOfPackage()[0]->GetRetransmissionVariable() > telecomunication_system_->GetMaxRetransmissionAttempt())
			{
				//std::cout << "Error. Exceeded attempts of retransmition. Deleting package..." << endl;
				//cout << "###########################################################################################" << endl;
				telecomunication_system_->GetChannel()->DeletePackageAtIndex(FindPackageIndexInChannelBySourceStation(telecomunication_system_, source_station_number));
				telecomunication_system_->GetListOfTransmitter()[source_station_number]->DeletePackageFromBuffer();
				telecomunication_system_->statistics->IncrementNegativePackageNumber(source_station_number);
				terminated = true;
				if (telecomunication_system_->GetChannel()->GetQueuePackage().size() == 0)
				{
					//cout << "Set channel status to free." << endl;
					telecomunication_system_->GetChannel()->SetChannelStatus(true);
				}
			}
			else
			{
				//std::cout << "Retransmition attempt of time  : " << (process_time_ + clock_) << endl;
				telecomunication_system_->GetChannel()->DeletePackageAtIndex(FindPackageIndexInChannelBySourceStation(telecomunication_system_, source_station_number));
				telecomunication_system_->GetListOfTransmitter()[source_station_number]->GetListOfPackage()[0]->SetSendingPackage(false);
				Activate(telecomunication_system_->GetCurrentClock()+process_time_, SENDINGPACKAGE, RETRANSMITIONPACKAGE,source_station_number, eventlist_);
			}
			//cout << "\n";
			active = false;
			break;
		}
	}
}

int PackageProcess::FindPackageIndexInChannelBySourceStation(TelecomunicationSystem * telecomunication_system_, int source_station_)
{
	if (telecomunication_system_->GetChannel()->GetQueuePackage().size() > 0&& telecomunication_system_->GetListOfTransmitter()[source_station_]->GetListOfPackage().size() > 0)
	{
		for (int i = 0; i < telecomunication_system_->GetChannel()->GetQueuePackage().size(); i++)
		{
			if (telecomunication_system_->GetChannel()->GetQueuePackage()[i]->GetPackageID() == telecomunication_system_->GetListOfTransmitter()[source_station_]->GetListOfPackage()[0]->GetPackageID())
			{
				return i;
			}
		}
		return 0;
	}
	else
	{
		assert(telecomunication_system_->GetChannel()->GetQueuePackage().size() > 0);
		return -1;
	}
}
