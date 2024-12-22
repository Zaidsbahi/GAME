#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlayerState_Base.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnJumpCountChanged, int32, NewXp);


UCLASS()
class GAME_API APlayerState_Base : public APlayerState
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, ReplicatedUsing="OnRep_JumpCount", Category = "Stats")
	int JumpCount = 0;

	UFUNCTION()
	void OnRep_JumpCount(int32 OldValue) const;

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnJumpCountChanged OnJumpCountChanged;

public:

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void AddJumpCount(int32 Value);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
