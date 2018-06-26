#pragma once

#include "CGPValue.h"

class CGPGroup : public CGPObject {
private:
	std::shared_ptr<CGPValue> mPairs;
	std::weak_ptr<CGPValue> mInOrderPairs;
	std::weak_ptr<CGPValue> mCurrentPair;
	std::shared_ptr<CGPGroup> mSubGroups;
	std::weak_ptr<CGPGroup> mInOrderSubGroups;
	std::weak_ptr<CGPGroup> mCurrentSubGroup;
	std::weak_ptr<CGPGroup> mParent;
	bool mWriteable;
	void SortObject(std::shared_ptr<CGPObject> object,
                    std::shared_ptr<CGPObject>& unsortedList,
                    std::shared_ptr<CGPObject>& sortedList,
                    std::shared_ptr<CGPObject>& lastObject);
public:
	CGPGroup(const char *initName = "Top Level", std::shared_ptr<CGPGroup> initParent = nullptr);
	~CGPGroup() = default;
	std::shared_ptr<CGPGroup> GetParent();
	std::shared_ptr<CGPGroup> GetNext();
	int GetNumSubGroups() const;
	int	GetNumPairs() const;
	std::shared_ptr<CGPGroup> Duplicate(CTextPool **textPool = 0, std::shared_ptr<CGPGroup> initParent = nullptr);
	void SetWriteable(bool writeable);
	std::shared_ptr<CGPValue>& GetPairs();
	std::shared_ptr<CGPValue> GetInOrderPairs();
	std::shared_ptr<CGPGroup>& GetSubGroups();
	std::shared_ptr<CGPGroup> GetInOrderSubGroups();
	std::shared_ptr<CGPValue> AddPair(const char *name, const char *value, CTextPool **textPool = 0);
	void AddPair(std::shared_ptr<CGPValue> NewPair);
	std::shared_ptr<CGPGroup> AddGroup(const char *name, CTextPool **textPool = 0);
	void AddGroup(std::shared_ptr<CGPGroup> NewGroup);
	std::shared_ptr<CGPGroup> FindSubGroup(const char *name);
	bool Parse(char **dataPtr, CTextPool **textPool);
	bool Write(CTextPool **textPool, int depth);
	std::shared_ptr<CGPValue> FindPair(const char *key) const;
	const char *FindPairValue(const char *key, const char *defaultVal = 0) const;
};