#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlayerState_Base.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnJumpCountChanged, int32, NewJumpCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDashCountChanged, int32, NewDashCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActivateProximityBoost, bool, NewActivateProximity);

UCLASS()
class GAME_API APlayerState_Base : public APlayerState
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, ReplicatedUsing="OnRep_JumpCount", Category = "Stats")
	int JumpCount = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, ReplicatedUsing="OnRep_DashCount", Category = "Stats")
	int DashCount = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, ReplicatedUsing="OnRep_ActivateProximityBoost", Category = "Stats")
	bool bIsActivateProximityBoost = false;

	UFUNCTION()
	void OnRep_JumpCount(int32 OldValue) const;

	UFUNCTION()
	void OnRep_DashCount(int32 OldValue) const;

	UFUNCTION()
	void OnRep_ActivateProximityBoost(bool OldValue) const;
	
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnJumpCountChanged OnJumpCountChanged;

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnDashCountChanged OnDashCountChanged;

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnActivateProximityBoost OnActivateProximityBoost;
	
public:

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void AddJumpCount(int32 Value);

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void AddDashCount(int32 Value);

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetActivateProximityBoost(bool Value);
	
	UFUNCTION(BlueprintCallable)
	int ReturnJumpCount();

	UFUNCTION(BlueprintCallable)
	int ReturnDashCount();

	UFUNCTION()
	bool ReturnActivateProximityBoostBool();
};
