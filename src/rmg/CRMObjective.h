#pragma once

#include <string>
#include "../GenericParser2.h"

class CRMObjective {
private:
	int mPriority;		// sequence in which objectives need to be completed
	std::string mMessage; // message outputed when objective is completed
	std::string mDescription; // description of objective
	std::string mInfo; // more info for objective
	std::string mTrigger; // trigger associated with objective
	std::string mName; // name of objective
	bool mCompleted; // Is objective completed?
	int mOrderIndex; // objective index in ui
	bool mActive; // set to false if the objective requires another objective to be met first
	int mCompleteSoundID; // sound for when objective is finished
public:
	CRMObjective(CGPGroup& group);
	~CRMObjective() = default;
	bool Link();
	bool IsCompleted() const;
	bool IsActive() const;
	void Activate();
	void Complete(bool comp);
	int GetPriority() const;
	int GetOrderIndex() const;
	const char *GetMessage() const;
	const char *GetDescription() const;
	const char *GetInfo() const;
	const char *GetName() const;
	const char *GetTrigger() const;
	int CompleteSoundID() const;
	void SetPriority(int priority);
	void SetOrderIndex(int order);
	void SetMessage(const char *msg);
	void SetDescription(const char *desc);
	void SetInfo(const char *info);
	void SetName(const char *name);
	void SetTrigger(const char *name);
};
