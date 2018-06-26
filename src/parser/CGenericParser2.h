#pragma once

#include "CTextPool.h"
#include "CGPGroup.h"

class CGenericParser2 {
private:
	std::shared_ptr<CGPGroup> mTopLevel;
	std::unique_ptr<CTextPool> mTextPool;
	bool mWriteable;
public:
	CGenericParser2();
	~CGenericParser2() = default;
	void SetWriteable(bool writeable);
	std::shared_ptr<CGPGroup> GetBaseParseGroup() const;
	bool Parse(char **dataPtr, bool cleanFirst = true, bool writeable = false);
	bool Parse(char *dataPtr, bool cleanFirst = true, bool writeable = false);
	bool Write(CTextPool *textPool);
};
