// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter_Base.generated.h"

UCLASS()
class GAME_API APlayerCharacter_Base : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Track the maximum jump count based on pickups
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Jump")
	int32 MaxJumpCount;

	// Current jumps remaining during a jump sequence
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Jump")
	int32 CurrentJumpCount;

	// Flag to track if the character is on the ground
	bool bIsGrounded;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void PrintJumpCount();

	// Function to handle jumps
	virtual void Jump() override;

	// Function to collect pickups and increment jump count
	UFUNCTION(Server, Reliable, WithValidation)
	void CollectPickup();

	UFUNCTION(BlueprintCallable, Category = "Jump")
	void ResetJumpCount();

	// Replication setup
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void Landed(const FHitResult& Hit) override;
};

inline void APlayerCharacter_Base::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	bIsGrounded = true;
}

