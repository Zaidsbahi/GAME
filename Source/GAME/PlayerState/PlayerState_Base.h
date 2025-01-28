#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlayerState_Base.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnJumpCountChanged, int32, NewJumpCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDashCountChanged, int32, NewDashCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActivateProximityBoostJump, bool, NewActivateProximityJump);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActivateProximityBoostDash, bool, NewActivateProximityDash);

// Declare delegate for readiness changes
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReadyChanged, bool, bIsReady);

UCLASS()
class GAME_API APlayerState_Base : public APlayerState
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, ReplicatedUsing="OnRep_JumpCount", Category = "Stats")
	int JumpCount = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, ReplicatedUsing="OnRep_DashCount", Category = "Stats")
	int DashCount = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, ReplicatedUsing="OnRep_ActivateProximityBoostJump", Category = "Stats")
	bool bIsActivateProximityBoostJump = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, ReplicatedUsing="OnRep_ActivateProximityBoostDash", Category = "Stats")
	bool bIsActivateProximityBoostDash = false;
	
	UFUNCTION()
	void OnRep_JumpCount(int32 OldValue) const;

	UFUNCTION()
	void OnRep_DashCount(int32 OldValue) const;

	UFUNCTION()
	void OnRep_ActivateProximityBoostJump(bool OldValue) const;
	
	UFUNCTION()
	void OnRep_ActivateProximityBoostDash(bool OldValue) const;
	
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnJumpCountChanged OnJumpCountChanged;

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnDashCountChanged OnDashCountChanged;

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnActivateProximityBoostJump OnActivateProximityBoostJump;
	
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnActivateProximityBoostDash OnActivateProximityBoostDash;

	
public:

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void AddJumpCount(int32 Value);

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void AddDashCount(int32 Value);

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetActivateProximityBoostJump();
	
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetActivateProximityBoostDash();
	
	UFUNCTION(BlueprintCallable)
	int ReturnJumpCount();

	UFUNCTION(BlueprintCallable)
	int ReturnDashCount();

	UFUNCTION(BlueprintCallable)
	bool ReturnActiveProxJump();

	UFUNCTION(BlueprintCallable)
	bool ReturnActiveProxDash();

	///////////////////////////
	/// Ready-Up Mechanism ////
	///////////////////////////

	// Ready state, replicated
	UPROPERTY(ReplicatedUsing = OnRep_IsReady, BlueprintReadOnly)
	bool bIsReady = false;

	// Delegate to broadcast when ready state changes
	UPROPERTY(BlueprintAssignable)
	FOnReadyChanged OnReadyChanged;

	// Set ready state on the server
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SetReady(bool bReady);

	// Called when bIsReady changes
	UFUNCTION()
	void OnRep_IsReady();
};


