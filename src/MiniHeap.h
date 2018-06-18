#pragma once

#include <cstdlib>

class CMiniHeap
{
private:
	char	*mHeap;
	char	*mCurrentHeap;
	int		mSize;
public:

// reset the heap back to the start
void ResetHeap()
{
	mCurrentHeap = mHeap;
}

// initialise the heap
CMiniHeap(int size)
{
	mHeap = (char *)malloc(size);
	mSize = size;
	if (mHeap)
	{
		ResetHeap();
	}
}

// free up the heap
~CMiniHeap()
{
	if (mHeap)
	{
		free(mHeap);
	}
}

// give me some space from the heap please
char *MiniHeapAlloc(int size)
{
	if (size < (mSize - (mCurrentHeap - mHeap)))
	{
		char *tempAddress =  mCurrentHeap;
		mCurrentHeap += size;
		return tempAddress;
	}
	return NULL;
}

};
