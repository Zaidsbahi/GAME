#include "PlayerState_Base.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

//////////////////////////////
/// ReplicatedUsing OnRep ////
//////////////////////////////
void APlayerState_Base::OnRep_JumpCount(int32 OldValue) const
{
	OnJumpCountChanged.Broadcast(JumpCount);
}
void APlayerState_Base::OnRep_DashCount(int32 OldValue) const
{
	OnDashCountChanged.Broadcast(DashCount);
}

void APlayerState_Base::OnRep_ActivateProximityBoostJump(bool OldValue) const
{
	OnActivateProximityBoostJump.Broadcast(bIsActivateProximityBoostJump);
}

void APlayerState_Base::OnRep_ActivateProximityBoostDash(bool OldValue) const
{
	OnActivateProximityBoostDash.Broadcast(bIsActivateProximityBoostDash);
}


//////////////////////
/// Setting Values ////
//////////////////////
void APlayerState_Base::AddJumpCount(int32 Value)
{
	JumpCount += Value;
	OnJumpCountChanged.Broadcast(JumpCount);
	SetActivateProximityBoostJump();
}
void APlayerState_Base::AddDashCount(int32 Value)
{
	DashCount += Value;
	OnDashCountChanged.Broadcast(DashCount);
	SetActivateProximityBoostDash();
}

void APlayerState_Base::SetActivateProximityBoostJump()
{
	if (JumpCount > 0)
	{
		bIsActivateProximityBoostJump = true;
	}
	else
	{
		bIsActivateProximityBoostJump = false;
	}
	
	OnActivateProximityBoostJump.Broadcast(bIsActivateProximityBoostJump);
}



void APlayerState_Base::SetActivateProximityBoostDash()
{
	if (DashCount > 0)
	{
		bIsActivateProximityBoostDash = true;
	}
	else
	{
		bIsActivateProximityBoostDash = false;
	}

	OnActivateProximityBoostDash.Broadcast(bIsActivateProximityBoostDash);
}

///////////////
/// Returns ///
///////////////
int APlayerState_Base::ReturnJumpCount()
{
	return JumpCount;
}
int APlayerState_Base::ReturnDashCount()
{
	return DashCount;
}

bool APlayerState_Base::ReturnActiveProxJump()
{
	return bIsActivateProximityBoostJump;
}

bool APlayerState_Base::ReturnActiveProxDash()
{
	return bIsActivateProximityBoostDash;
}

/////////////////////
/// Replications ////
/////////////////////
void APlayerState_Base::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(APlayerState_Base, JumpCount, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(APlayerState_Base, DashCount, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(APlayerState_Base, bIsActivateProximityBoostJump, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(APlayerState_Base, bIsActivateProximityBoostDash, COND_OwnerOnly);
}
