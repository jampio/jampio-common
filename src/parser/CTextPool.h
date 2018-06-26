#pragma once

#include <memory>

class CTextPool {
private:
	std::unique_ptr<char[]> mPool;
	std::unique_ptr<CTextPool> mNext;
	std::size_t mSize, mUsed;
public:
	CTextPool(std::size_t initSize = 10240);
	~CTextPool() = default;
	const CTextPool *GetNext() const;
	CTextPool *GetNext();
	void SetNext(std::unique_ptr<CTextPool> next);
	const char *GetPool() const;
	std::size_t GetUsed() const;
	const char *AllocText(const char *text, bool addNULL = true, CTextPool **poolPtr = nullptr);
};