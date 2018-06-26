#include "CGenericParser2.h"

CGenericParser2::CGenericParser2()
	: mTopLevel(std::make_shared<CGPGroup>())
	, mTextPool()
	, mWriteable(false)
{}

void CGenericParser2::SetWriteable(bool writeable) {
	mWriteable = writeable;
}

std::shared_ptr<CGPGroup> CGenericParser2::GetBaseParseGroup() const {
	return mTopLevel;
}

bool CGenericParser2::Parse(char **dataPtr, bool cleanFirst, bool writeable) {
	if (cleanFirst) {
		mTopLevel = std::make_shared<CGPGroup>();
		mTextPool = {};
	}

	if (!mTextPool) {
		mTextPool = std::make_unique<CTextPool>();
	}

	SetWriteable(writeable);
	mTopLevel->SetWriteable(writeable);
	auto topPool = mTextPool.get();
	bool ret = mTopLevel->Parse(dataPtr, &topPool);

	return ret;
}

bool CGenericParser2::Parse(char *dataPtr, bool cleanFirst = true, bool writeable = false) {
	return Parse(&dataPtr, cleanFirst, writeable);
}

bool CGenericParser2::Write(CTextPool *textPool) {
	return mTopLevel->Write(&textPool, -1);
}
