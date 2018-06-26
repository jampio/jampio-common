#pragma once

#include "CTextPool.h"

class CGPObject {
protected:
	const char *mName; // non-owning into CTextPool
	std::shared_ptr<CGPObject> mNext;
	std::weak_ptr<CGPObject> mInOrderNext;
	std::weak_ptr<CGPObject> mInOrderPrevious;
public:
	CGPObject(const char *initName);
	virtual ~CGPObject() = default;
	const char *GetName() const;
	std::shared_ptr<CGPObject>& GetNext();
	std::shared_ptr<CGPObject> GetInOrderNext();
	std::shared_ptr<CGPObject> GetInOrderPrevios();
	void SetNext(std::shared_ptr<CGPObject> next);
	void SetInOrderNext(std::shared_ptr<CGPObject> next);
	void SetInOrderPrevious(std::shared_ptr<CGPObject> previous);
	bool WriteText(CTextPool **textPool, const char *text) const;
};