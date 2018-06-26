#include "CTextPool.h"
#include <cstring>

CTextPool::CTextPool(std::size_t initSize)
	: mNext()
	, mSize(initSize)
	, mUsed(0)
	, mPool(std::make_unique<char[]>(initSize))
{}

const CTextPool *CTextPool::GetNext() const {
	return mNext.get();
}

CTextPool *CTextPool::GetNext() {
	return mNext.get();
}

void CTextPool::SetNext(std::unique_ptr<CTextPool> next) {
	mNext = std::move(next);
}

const char *CTextPool::GetPool() const {
	return mPool.get();
}

std::size_t CTextPool::GetUsed() const {
	return mUsed;
}

const char *CTextPool::AllocText(const char *text, bool addNULL, CTextPool **poolPtr)
{
	const auto length = strlen(text) + (addNULL ? 1 : 0);

	if (mUsed + length + 1 > mSize)
	{	// extra 1 to put a null on the end
		if (poolPtr)
		{
			(*poolPtr)->SetNext(std::make_unique<CTextPool>(mSize));
			*poolPtr = (*poolPtr)->GetNext();

			return (*poolPtr)->AllocText(text, addNULL);
		}
		return nullptr;
	}

	strcpy(mPool.get() + mUsed, text);
	mUsed += length;
	mPool[mUsed] = 0;

	return mPool.get() + mUsed - length;
}