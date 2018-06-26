#include "CGPObject.h"
#include <cstring>

CGPObject::CGPObject(const char *initName)
	: mName(initName)
	, mNext()
	, mInOrderNext()
	, mInOrderPrevious()
{}

const char *CGPObject::GetName() const {
	return mName;
}

std::shared_ptr<CGPObject>& CGPObject::GetNext() {
	return mNext;
}

void CGPObject::SetNext(std::shared_ptr<CGPObject> next) {
	mNext = std::move(next);
}

std::shared_ptr<CGPObject> CGPObject::GetInOrderNext() {
	return mInOrderNext.lock();
}

void CGPObject::SetInOrderNext(std::shared_ptr<CGPObject> next) {
	mInOrderNext = std::move(next);
}

std::shared_ptr<CGPObject> CGPObject::GetInOrderPrevios() {
	return mInOrderPrevious.lock();
}

void CGPObject::SetInOrderPrevious(std::shared_ptr<CGPObject> previous) {
	mInOrderPrevious = std::move(previous);
}

bool CGPObject::WriteText(CTextPool **textPool, const char *text) const {
	if (strchr(text, ' ') || !text[0]) {
		(*textPool)->AllocText("\"", false, textPool);
		(*textPool)->AllocText(text, false, textPool);
		(*textPool)->AllocText("\"", false, textPool);
	} else {
		(*textPool)->AllocText(text, false, textPool);
	}
	return true;
}