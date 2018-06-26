#include <jampio/shared/shared.h>
#include "CGPValue.h"
#include "generic_parser.h"

CGPValue::CGPValue(const char *initName, const char *initValue)
	: CGPObject(initName)
	, mList() {
	if (initValue) {
		AddValue(initValue);
	}
}

std::shared_ptr<CGPValue> CGPValue::GetNext() {
	return std::dynamic_pointer_cast<CGPValue>(mNext);
}

std::shared_ptr<CGPValue> CGPValue::Duplicate(CTextPool **textPool) {
	const char *name;
	if (textPool) {
		name = (*textPool)->AllocText(mName, true, textPool);
	} else {
		name = mName;
	}
	auto newValue = std::make_shared<CGPValue>(name);
	auto iterator = mList;
	while (iterator) {
		if (textPool) {
			name = (*textPool)->AllocText(iterator->GetName(), true, textPool);
		} else {
			name = iterator->GetName();
		}
		newValue->AddValue(name);
		iterator = iterator->GetNext();
	}
	return newValue;
}

bool CGPValue::IsList() const {
	if (!mList || !mList->GetNext()) {
		return false;
	}
	return true;
}

const char *CGPValue::GetTopValue() const { 
	if (mList) return mList->GetName();
	return 0;
}

std::shared_ptr<CGPObject>& CGPValue::GetList() {
	return mList;
}

void CGPValue::AddValue(const char *newValue, CTextPool **textPool) {
	if (textPool) {
		newValue = (*textPool)->AllocText(newValue, true, textPool);
	}
	if (!mList) {
		mList = std::make_shared<CGPObject>(newValue);
		mList->SetInOrderNext(mList);
	} else {
		mList->GetInOrderNext()->SetNext(std::make_shared<CGPObject>(newValue));
		mList->SetInOrderNext(mList->GetInOrderNext()->GetNext());
	}
}

bool CGPValue::Parse(char **dataPtr, CTextPool **textPool) {
	while (1) {
		auto token = GP_GetToken(dataPtr, true, true);
		if (!token[0])
		{	// end of data - error!
			return false;
		}
		else if (Q_stricmp(token, "]") == 0)
		{	// ending brace for this list
			break;
		}
		auto value = (*textPool)->AllocText(token, true, textPool);
		AddValue(value);
	}
	return true;
}

bool CGPValue::Write(CTextPool **textPool, int depth) {
	if (!mList) return true;

	for (int i = 0;i < depth; i++) {
		(*textPool)->AllocText("\t", false, textPool);
	}

	WriteText(textPool, mName);

	if (!mList->GetNext()) {

		(*textPool)->AllocText("\t\t", false, textPool);
		mList->WriteText(textPool, mList->GetName());
		(*textPool)->AllocText("\r\n", false, textPool);

	} else {
		(*textPool)->AllocText("\r\n", false, textPool);

		for (int i=0;i<depth;i++) {
			(*textPool)->AllocText("\t", false, textPool);
		}

		(*textPool)->AllocText("[\r\n", false, textPool);

		auto next = mList;
		while (next) {
			for (int i = 0;i < depth+1; i++) {
				(*textPool)->AllocText("\t", false, textPool);
			}

			mList->WriteText(textPool, next->GetName());
			(*textPool)->AllocText("\r\n", false, textPool);

			next = next->GetNext();
		}

		for (int i=0;i<depth;i++) {
			(*textPool)->AllocText("\t", false, textPool);
		}
		(*textPool)->AllocText("]\r\n", false, textPool);
	}
	return true;
}