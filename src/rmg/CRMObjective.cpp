#include "CRMObjective.h"

CRMObjective::CRMObjective(/* const */CGPGroup &group)
	: mPriority(atoi(group.FindPairValue("priority", "0")))
	, mMessage(group.FindPairValue("message", va("Objective %i Completed", mPriority)))
	, mDescription(group.FindPairValue("description", va("Objective %i", mPriority)))
	, mInfo(group.FindPairValue("info", va("Info %i", mPriority)))
	, mTrigger(group.FindPairValue("trigger", ""))
	, mName(group.GetName())
	, mCompleted(false)
	, mOrderIndex(-1)
	, mActive(!mPriority) // If no priority was specified for this objective then its active by default.
	, mCompleteSoundID(0)
{}	

/************************************************************************************************
 * CRMObjective::FindRandomTrigger
 *	Searches the entitySystem form a random arioche trigger that matches the objective name
 *
 * inputs:
 *  none
 *
 * return:
 *	trigger: a random trigger or NULL if one couldnt be found
 *
 ************************************************************************************************/
/*CTriggerAriocheObjective* CRMObjective::FindRandomTrigger ( )
{	
	CEntity*	search;
	CEntity*	triggers[20];
	int			numTriggers;

	// Start off the first trigger
	numTriggers = 0;
	search      = entitySystem->GetEntityFromClassname ( NULL, "trigger_arioche_objective" );

	// Make a list of triggers
	while ( numTriggers < 20 && search )
	{
		CTriggerAriocheObjective* trigger = (CTriggerAriocheObjective*) search;

		// Move on to the next trigger
		search = entitySystem->GetEntityFromClassname ( search, "trigger_arioche_objective" );

		// See if this trigger is already in use
		if ( trigger->GetObjective ( ) )
		{
			continue;
		}

		// If the objective names dont match then ignore this trigger
		if ( stricmp ( trigger->GetObjectiveName ( ), GetTrigger() ) )
		{
			continue;
		}
	
		// Add the trigger to the list
		triggers[numTriggers++] = trigger;
	}

	// If no matching triggers then just return NULL
	if ( 0 == numTriggers )
	{
		return NULL;
	}

	// Return a random choice from the trigger list
	return (CTriggerAriocheObjective*)triggers[TheRandomMissionManager->GetLandScape()->irand(0,numTriggers-1)];
}
*/
/************************************************************************************************
 * CRMObjective::Link
 *	Links the objective into the world using the current state of the world to determine
 *  where it should link
 *
 * inputs:
 *  none
 *
 * return:
 *	true: objective successfully linked
 *  false: objective failed to link
 *
 ************************************************************************************************/
bool CRMObjective::Link() {
/*	CTriggerAriocheObjective* trigger;

	// Look for a random trigger to associate this objective to.
	trigger = FindRandomTrigger ( );
	if ( NULL != trigger )
	{
		trigger->SetObjective ( this );
	}
*/	
	return true;
}

bool CRMObjective::IsCompleted() const {
	return mCompleted;
}

bool CRMObjective::IsActive() const {
	return mActive;
}

void CRMObjective::Activate() {
	mActive = true;
}

void CRMObjective::Complete(bool comp) { 
	mCompleted = comp;
}

int CRMObjective::GetPriority() const {
	return mPriority;
}

int CRMObjective::GetOrderIndex() const {
	return mOrderIndex;
}

const char *CRMObjective::GetMessage() const {
	return mMessage.c_str();
}

const char *CRMObjective::GetDescription() const {
	return mDescription.c_str();
}

const char *CRMObjective::GetInfo() const {
	return mInfo.c_str();
}

const char *CRMObjective::GetName() const {
	return mName.c_str();
}

const char *CRMObjective::GetTrigger() const {
	return mTrigger.c_str();
}

int CRMObjective::CompleteSoundID() const {
	return mCompleteSoundID;
}

void CRMObjective::SetPriority(int priority) {
	mPriority = priority;
}

void CRMObjective::SetOrderIndex(int order) { 
	mOrderIndex = order;
}

void CRMObjective::SetMessage(const char *msg) {
	mMessage = msg;
}

void CRMObjective::SetDescription(const char *desc) {
	mDescription = desc;
}

void CRMObjective::SetInfo(const char *info) {
	mInfo = info;
}

void CRMObjective::SetName(const char *name) {
	mName = name;
}

void CRMObjective::SetTrigger(const char *name) {
	mTrigger = name;
}
