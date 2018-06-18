#pragma once

#include <string>
#include <list>
#include "../GenericParser2.h"

class CRMObjective
{
protected:

	bool		mCompleted;		// Is objective completed?
	bool		mActive;		// set to false if the objective requires another objective to be met first
	int			mPriority;		// sequence in which objectives need to be completed
	int			mOrderIndex;	// objective index in ui
	int			mCompleteSoundID; // sound for when objective is finished
	std::string		mMessage;		// message outputed when objective is completed
	std::string		mDescription;	// description of objective
	std::string		mInfo;			// more info for objective
	std::string		mName;			// name of objective
	std::string		mTrigger;		// trigger associated with objective

public:
	
	CRMObjective(CGPGroup *group);
	~CRMObjective(void) {}

	bool			Link			(void);

	bool			IsCompleted		(void) const { return mCompleted; }
	bool			IsActive		(void) const { return mActive; }

	void			Activate		(void)		 { mActive = true; }
	void			Complete		(bool comp)  { mCompleted = comp;}

	// Get methods
	int				GetPriority(void){return mPriority;}
	int				GetOrderIndex(void) { return mOrderIndex; }
	const char*		GetMessage(void) { return mMessage.c_str(); }
	const char*		GetDescription(void) { return mDescription.c_str(); }
	const char*		GetInfo(void) { return mInfo.c_str(); }
	const char*		GetName(void) { return mName.c_str(); }
	const char*		GetTrigger(void) { return mTrigger.c_str(); }
	int				CompleteSoundID() { return mCompleteSoundID; };

	// Set methods
	void			SetPriority(int priority){mPriority = priority;}
	void			SetOrderIndex(int order) { mOrderIndex = order; }
	void			SetMessage(const char* msg) { mMessage = msg; }
	void			SetDescription(const char* desc) { mDescription = desc; }
	void			SetInfo(const char* info) { mInfo = info; }
	void			SetName(const char* name) { mName = name; }
	void			SetTrigger(const char* name) { mTrigger = name; }

private:

//	CTriggerAriocheObjective*		FindRandomTrigger		( );
};

typedef std::list<CRMObjective *>::iterator	rmObjectiveIter_t;
typedef std::list<CRMObjective *>			rmObjectiveList_t;
