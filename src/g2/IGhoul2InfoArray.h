#pragma once

#include <vector>
#include "CGhoul2Info.h"

class IGhoul2InfoArray
{
public:
	virtual int New()=0;
	virtual void Delete(int handle)=0;
	virtual bool IsValid(int handle) const=0;
	virtual std::vector<CGhoul2Info> &Get(int handle)=0;
	virtual const std::vector<CGhoul2Info> &Get(int handle) const=0;
};