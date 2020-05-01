#pragma once
#include <string>

class ISerialize
{
public:
	virtual ~ISerialize() {};
	virtual void Pack(std::ostringstream& o_stream) const = 0;
	virtual void Unpack(std::istringstream& i_stream) = 0;
};