#include "PlayerState_Base.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

void APlayerState_Base::OnRep_JumpCount(int32 OldValue) const
{
	OnJumpCountChanged.Broadcast(JumpCount);
}

void APlayerState_Base::OnRep_DashCount(int32 OldValue) const
{
	OnDashCountChanged.Broadcast(DashCount);
}

void APlayerState_Base::AddJumpCount(int32 Value)
{
	JumpCount += Value;
	OnJumpCountChanged.Broadcast(JumpCount);
}

void APlayerState_Base::AddDashCount(int32 Value)
{
	DashCount += Value;
	OnDashCountChanged.Broadcast(DashCount);
}

void APlayerState_Base::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(APlayerState_Base, JumpCount, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(APlayerState_Base, DashCount, COND_OwnerOnly);
	
}

int APlayerState_Base::ReturnJumpCount()
{
	return JumpCount;
}

int APlayerState_Base::ReturnDashCount()
{
	return DashCount;
}
