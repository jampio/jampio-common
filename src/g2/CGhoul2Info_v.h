#pragma once

#include <cassert>
#include <vector>
#include "CGhoul2Info.h"
#include "IGhoul2InfoArray.h"

IGhoul2InfoArray &TheGhoul2InfoArray();

class CGhoul2Info_v
{
	IGhoul2InfoArray &InfoArray() const
	{
		return TheGhoul2InfoArray();
	}

	void Alloc()
	{
		assert(!mItem); //already alloced
		mItem=InfoArray().New();
		assert(!Array().size());
	}
	void Free()
	{
		if (mItem)
		{
			assert(InfoArray().IsValid(mItem));
			InfoArray().Delete(mItem);
			mItem=0;
		}
	}
	std::vector<CGhoul2Info> &Array()
	{
		assert(InfoArray().IsValid(mItem));
		return InfoArray().Get(mItem);
	}
	const std::vector<CGhoul2Info> &Array() const
	{
		assert(InfoArray().IsValid(mItem));
		return InfoArray().Get(mItem);
	}
public:
	int mItem;	//dont' be bad and muck with this
	CGhoul2Info_v()
	{
		mItem=0;
	}
	CGhoul2Info_v(const int item)
	{	//be VERY carefull with what you pass in here
		mItem=item;
	}
	~CGhoul2Info_v()
	{
		Free(); //this had better be taken care of via the clean ghoul2 models call
	}
	void operator=(const CGhoul2Info_v &other)
	{
		mItem=other.mItem;
	}
	void operator=(const int otherItem)	//assigning one from the VM side item number
	{
		mItem=otherItem;
	}
	void DeepCopy(const CGhoul2Info_v &other)
	{
		Free();
		if (other.mItem)
		{
			Alloc();
			Array()=other.Array();
			int i;
			for (i=0;i<size();i++)
			{
				Array()[i].mBoneCache=0;
				Array()[i].mTransformedVertsArray=0;
				Array()[i].mSkelFrameNum=0;
				Array()[i].mMeshFrameNum=0;
			}
		}
	}
	CGhoul2Info &operator[](int idx)
	{
		assert(mItem);
		assert(idx>=0&&idx<size());
		return Array()[idx];
	}
	const CGhoul2Info &operator[](int idx) const
	{
		assert(mItem);
		assert(idx>=0&&idx<size());
		return Array()[idx];
	}
	void resize(int num)
	{
		assert(num>=0);
		if (num)
		{
			if (!mItem)
			{
				Alloc();
			}
		}
		if (mItem||num)
		{
			Array().resize(num);
		}
	}
	void clear()
	{
		Free();
	}
	void push_back(const CGhoul2Info &model)
	{
		if (!mItem)
		{
			Alloc();
		}
		Array().push_back(model);
	}
	int size() const
	{
		if (!IsValid())
		{
			return 0;
		}
		return Array().size();
	}
	bool IsValid() const
	{
		return InfoArray().IsValid(mItem);
	}
	void kill()
	{
		// this scary method zeros the infovector handle without actually freeing it
		// it is used for some places where a copy is made, but we don't want to go through the trouble
		// of making a deep copy
		mItem=0;
	}
};
