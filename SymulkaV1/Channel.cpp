#include "Channel.h"



Channel::Channel()
{

}


Channel::~Channel()
{
}

std::vector<Package *>Channel::GetQueuePackage()
{
	return queue_package;
}
void Channel::SetChannelStatus(bool freechannel)
{
	free_channel_ = freechannel;
}
void Channel::CopyPackageToChannelBuffer(Package* package_to_be_copied_)
{
	queue_package.push_back(package_to_be_copied_);
}
void Channel::DeletePackageAtIndex(int index)
{
	queue_package.erase(queue_package.begin() + index);
}
