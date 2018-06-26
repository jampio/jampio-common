#include <jampio/shared/shared.h>
#include "CGPGroup.h"
#include "generic_parser.h"

CGPGroup::CGPGroup(const char *initName, std::shared_ptr<CGPGroup> initParent)
	: CGPObject(initName)
	, mPairs()
	, mInOrderPairs()
	, mCurrentPair()
	, mSubGroups()
	, mInOrderSubGroups()
	, mCurrentSubGroup()
	, mParent(initParent)
	, mWriteable(false)
{}

void CGPGroup::SortObject(std::shared_ptr<CGPObject> object,
                          std::shared_ptr<CGPObject>& unsortedList,
                          std::shared_ptr<CGPObject>& sortedList,
                          std::shared_ptr<CGPObject>& lastObject) {
	if (!unsortedList) {
		unsortedList = sortedList = object;
	} else {
		lastObject->SetNext(object);

		auto test = sortedList;
		std::shared_ptr<CGPObject> last;
		while (test) {
			if (Q_stricmp(object->GetName(), test->GetName()) < 0) {
				break;
			}

			last = test;
			test = test->GetInOrderNext();
		}

		if (test) {
			test->SetInOrderPrevious(object);
			object->SetInOrderNext(test);
		}
		if (last) {
			last->SetInOrderNext(object);
			object->SetInOrderPrevious(last);
		} else {
			sortedList = object;
		}
	}

	lastObject = object;
}

std::shared_ptr<CGPGroup> CGPGroup::GetParent() {
	return mParent.lock();
}

std::shared_ptr<CGPGroup> CGPGroup::GetNext() {
	return std::dynamic_pointer_cast<CGPGroup>(mNext);
}

int CGPGroup::GetNumSubGroups() const {
	int count = 0;
	auto group = mSubGroups;
	do {
		count++;
		group = group->GetNext();
	} while (group);
	return count;
}

int CGPGroup::GetNumPairs() const {
	int count = 0;
	auto pair = mPairs;
	do
	{
		count++;
		pair = pair->GetNext();
	} while (pair);
	return count;
}

std::shared_ptr<CGPGroup> CGPGroup::Duplicate(CTextPool **textPool, std::shared_ptr<CGPGroup> initParent) {
	const char *name;
	if (textPool) {
		name = (*textPool)->AllocText(mName, true, textPool);
	} else {
		name = mName;
	}

	auto newGroup = std::make_shared<CGPGroup>(name);

	auto subSub = mSubGroups;
	while (subSub) {
		auto newSub = subSub->Duplicate(textPool, newGroup);
		newGroup->AddGroup(newSub);

		subSub = subSub->GetNext();
	}

	auto subPair = mPairs;
	while (subPair) {
		auto newPair = subPair->Duplicate(textPool);
		newGroup->AddPair(newPair);

		subPair = subPair->GetNext();
	}

	return newGroup;
}

void CGPGroup::SetWriteable(bool writeable) {
	mWriteable = writeable;
}

std::shared_ptr<CGPValue>& CGPGroup::GetPairs() {
	return mPairs;
}

std::shared_ptr<CGPValue> CGPGroup::GetInOrderPairs() { 
	return mInOrderPairs.lock();
}

std::shared_ptr<CGPGroup>& CGPGroup::GetSubGroups() {
	return mSubGroups;
}

std::shared_ptr<CGPGroup> CGPGroup::GetInOrderSubGroups() {
	return mInOrderSubGroups.lock();
}

std::shared_ptr<CGPValue> CGPGroup::AddPair(const char *name, const char *value, CTextPool **textPool) {
	if (textPool) {
		name = (*textPool)->AllocText(name, true, textPool);
		if (value) {
			value = (*textPool)->AllocText((char *)value, true, textPool);
		}
	}
	auto newPair = std::make_shared<CGPValue>(name, value);
	AddPair(newPair);
	return newPair;
}

void CGPGroup::AddPair(std::shared_ptr<CGPValue> NewPair) {
	auto _mPairs = std::dynamic_pointer_cast<CGPObject>(mPairs);
	auto _mInOrderPairs = std::dynamic_pointer_cast<CGPObject>(mInOrderPairs.lock());
	auto _mCurrentPair = std::dynamic_pointer_cast<CGPObject>(mCurrentPair.lock());
	SortObject(std::move(NewPair), _mPairs, _mInOrderPairs, _mCurrentPair);
}

std::shared_ptr<CGPGroup> CGPGroup::AddGroup(const char *name, CTextPool **textPool) {
	if (textPool) {
		name = (*textPool)->AllocText(name, true, textPool);
	}
	auto newGroup = std::make_shared<CGPGroup>(name, this);
	AddGroup(newGroup);
	return newGroup;
}
	
void CGPGroup::AddGroup(std::shared_ptr<CGPGroup> NewGroup) {
	auto _mSubGroups = std::dynamic_pointer_cast<CGPObject>(mSubGroups);
	auto _mInOrderSubGroups = std::dynamic_pointer_cast<CGPObject>(mInOrderSubGroups.lock());
	auto _mCurrentSubGroup = std::dynamic_pointer_cast<CGPObject>(mCurrentSubGroup.lock());
	SortObject(std::move(NewGroup), _mSubGroups, _mInOrderSubGroups, _mCurrentSubGroup);
}

std::shared_ptr<CGPGroup> CGPGroup::FindSubGroup(const char *name) {
	auto group = mSubGroups;
	while (group) {
		if (!stricmp(name, group->GetName())) {
			return group;
		}
		group = group->GetNext();
	}
	return {};
}

bool CGPGroup::Parse(char **dataPtr, CTextPool **textPool) {
	char lastToken[MAX_TOKEN_SIZE];
	while (1) {
		auto token = GP_GetToken(dataPtr, true);

		if (!token[0])
		{	// end of data - error!
			if (mParent.lock()) {
				return false;
			} else {
				break;
			}
		}
		else if (Q_stricmp(token, "}") == 0)
		{	// ending brace for this group
			break;
		}

		strcpy(lastToken, token);

		// read ahead to see what we are doing
		token = GP_GetToken(dataPtr, true, true);
		if (Q_stricmp(token, "{") == 0)
		{	// new sub group
			auto newSubGroup = AddGroup(lastToken, textPool);
			newSubGroup->SetWriteable(mWriteable);
			if (!newSubGroup->Parse(dataPtr, textPool))
			{
				return false;
			}
		}
		else if (Q_stricmp(token, "[") == 0)
		{	// new pair list
			auto newPair = AddPair(lastToken, 0, textPool);
			if (!newPair->Parse(dataPtr, textPool))
			{
				return false;
			}
		}
		else
		{	// new pair
			AddPair(lastToken, token, textPool);
		}
	}

	return true;
}

bool CGPGroup::Write(CTextPool **textPool, int depth) {
	if (depth >= 0) {
		for(int i=0;i<depth;i++)
		{
			(*textPool)->AllocText("\t", false, textPool);
		}
		WriteText(textPool, mName);
		(*textPool)->AllocText("\r\n", false, textPool);
		
		for(int i=0;i<depth;i++)
		{
			(*textPool)->AllocText("\t", false, textPool);
		}
		(*textPool)->AllocText("{\r\n", false, textPool);
	}

	auto pair = mPairs;
	while (pair) {
		pair->Write(textPool, depth+1);
		pair = pair->GetNext();
	}

	auto subgroup = mSubGroups;
	while (subgroup) {
		subgroup->Write(textPool, depth+1);
		subgroup = subgroup->GetNext();
	}

	if (depth >= 0) {
		for(int i=0;i<depth;i++)
		{
			(*textPool)->AllocText("\t", false, textPool);
		}
		(*textPool)->AllocText("}\r\n", false, textPool);
	}

	return true;
}

std::shared_ptr<CGPValue> CGPGroup::FindPair(const char *key) const {
	int length;
	const char *next;	
	auto pos = key;
	while (pos[0]) {
		auto separator = strstr(pos, "||");
		if (separator) {
			length = separator - pos;
			next = separator + 2;
		} else {
			length = strlen(pos);
			next = pos + length;
		}
		auto pair = mPairs;
		while (pair) {
			if (strlen(pair->GetName()) == length &&
				Q_stricmpn(pair->GetName(), pos, length) == 0) {
				return pair;
			}
			pair = pair->GetNext();
		}
		pos = next;
	}
	return {};
}

const char *CGPGroup::FindPairValue(const char *key, const char *defaultVal) const {
	auto pair = FindPair(key);
	if (pair) {
		return pair->GetTopValue();
	}
	return defaultVal;
}