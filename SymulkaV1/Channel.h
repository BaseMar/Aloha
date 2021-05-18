#ifndef CHANNEL_H
#define CHANNEL_H
#include <queue>
#include "Package.h"
class Channel
{
public:
	Channel();
	~Channel();
	std::vector<Package *> GetQueuePackage();
	void SetChannelStatus(bool freechannel);
	void CopyPackageToChannelBuffer(Package * package_to_be_copied_);
	void DeletePackageAtIndex(int index);
private:
	
	bool free_channel_ = true; // wolnoœæ kana³u
	std::vector<Package *> queue_package;
};

#endif