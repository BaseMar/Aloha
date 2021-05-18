#ifndef TRANSMITTER_H
#define TRANSMITTER_H
#include "Package.h"
#include <vector>
#include <assert.h>
class Transmitter
{
public:
	Transmitter();
	~Transmitter();
	std::vector<Package*> GetListOfPackage();
	void SetPackageToBuffor(Package* pc);
	bool GetReceiveAckFlag();
	void SetReceiveAckFlag(bool recflag);
	void DeletePackageFromBuffer();
	

private:
	std::vector<Package*> package_bufor_; // lista pakietów wygenerowan¹ przez nadajnik
	const int  kCTIZ = 1; // czas wys³ania ACK
	bool receive_ack_ = false; // flaga mówi¹ca czy nadajnik otrzyma³ ACK
};

#endif