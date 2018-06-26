#pragma once

#include "CGPObject.h"

class CGPValue : public CGPObject {
private:
	std::shared_ptr<CGPObject> mList;
public:
	CGPValue(const char *initName, const char *initValue = 0);
	~CGPValue() = default;
	std::shared_ptr<CGPValue> GetNext();
	std::shared_ptr<CGPValue> Duplicate(CTextPool **textPool = 0);
	bool IsList() const;
	const char *GetTopValue() const;
	std::shared_ptr<CGPObject>& GetList();
	void AddValue(const char *newValue, CTextPool **textPool = 0);
	bool Parse(char **dataPtr, CTextPool **textPool);
	bool Write(CTextPool **textPool, int depth);
};