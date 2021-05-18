#ifndef PACKAGE_H
#define PACKAGE_H
#include <math.h>
#include <stdlib.h>
#include "Generator.h"

class Package
{
public:
	Package(int package_id_, int Source_Station_,double arrival_time_, int ctpk_V_);
	~Package();
	int GetPackageID();
	int GenerateCrpTime(int seed_);
	int GetRetransmissionVariable();
	bool GetSendingPackage();
	void SetSendingPackage(bool sentpackage);
	bool GetPackageSentCorectly();
	void SetPackageSentCorectly(bool corpackage);
	bool SetTER(int& seed_);
	int GetCTPK();
	double GetLeavingTime();
	void SetLeavingTime(double time);
	double GetArrivalTime();
	void SetArrivalTime(int arrivetime);
	double GetSendingPackageTime();
	void SetSendingPackageTime(int sendingtime);

private:
	Generator* package_genetator;
	int source_station_; // numer Ÿród³a pakietu
	int id_; // id pakietu
	int r_help_variable_; // zmienna pomocnicza do wyznaczenia r_ (2^r_ -1)
	int r_; // aktualna liczba retransmisji 
	double time_of_apear_; // czas pojawienia sie pakietu po wygenerowaniu czasu
	bool sending_package_ = false; // flaga mówi¹ca czy wysy³a pakiet, flaga wspomagaj¹ca czy pakiet jest najstarszy
	bool package_sent_correctly_=true;
	bool ter_ = false; // wyst¹pienie b³êdu w kanale
	int ctpk_;
	double arrival_time_; //czas w którym pojawi³ siê pakiet w systemie
	double leaving_time_; // czas poprawnego odebrania
	double sending_package_time_; // czas wys³ania z bufora
};

#endif