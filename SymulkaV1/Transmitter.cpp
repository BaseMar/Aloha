#include "Transmitter.h"

Transmitter::Transmitter()
{
}


Transmitter::~Transmitter()
{
}

std::vector<Package*> Transmitter::GetListOfPackage()
{
	return package_bufor_;
}

void Transmitter::SetPackageToBuffor(Package * pc)
{
	package_bufor_.push_back(pc);
}

bool Transmitter::GetReceiveAckFlag()
{
	return receive_ack_;
}
void Transmitter::SetReceiveAckFlag(bool recflag)
{
	receive_ack_ = recflag;
}
void Transmitter::DeletePackageFromBuffer()
{
	package_bufor_.erase(package_bufor_.begin());
}

