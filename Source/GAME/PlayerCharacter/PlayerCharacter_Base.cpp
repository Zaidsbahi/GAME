#include "PlayerCharacter_Base.h"
#include "GAME/PlayerState/PlayerState_Base.h"
#include "Net/UnrealNetwork.h"

APlayerCharacter_Base::APlayerCharacter_Base()
{
    PrimaryActorTick.bCanEverTick = true;

    MaxJumpCount = 1;  // Default to 1 jump
    CurrentJumpCount = 1;
    bIsGrounded = true;
    bReplicates = true;
}

void APlayerCharacter_Base::BeginPlay()
{
    Super::BeginPlay();
    

    // Configure the maximum allowed jumps using JumpMaxCount
    JumpMaxCount = (CurrentJumpCount > 1) ? 2 : 1;

    UE_LOG(LogTemp, Log, TEXT("BeginPlay: JumpMaxCount set to %d"), JumpMaxCount);
}

void APlayerCharacter_Base::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void APlayerCharacter_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlayerCharacter_Base::PrintJumpCount()
{
    if (APlayerState_Base* PS = Cast<APlayerState_Base>(GetPlayerState()))
    {
        int JumpCount = PS->ReturnJumpCount();
        if (HasAuthority())
        {
            UE_LOG(LogTemp, Log, TEXT("Server Jump Count: %d"), JumpCount);
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("Client Jump Count: %d"), JumpCount);
        }
    }
}

void APlayerCharacter_Base::Jump()
{
    if (!CanJump())
    {
        return;
    }
    
    if (APlayerState_Base* PS = Cast<APlayerState_Base>(GetPlayerState()))
    {
        int32 JumpCountFromState = PS->ReturnJumpCount();

        UE_LOG(LogTemp, Log, TEXT("Jump() called. JumpCountFromState: %d"), JumpCountFromState);

        if (JumpCountFromState > 0)
        {
              
            if (bIsGrounded)
            {
                Super::Jump();
                bIsGrounded = false;
                JumpMaxCount = 2; // Set max allowed jumps while airborne
            }
            else
            {
                Super::Jump();
                PS->AddJumpCount(-1); // Decrement jump count
                JumpMaxCount = JumpMaxCount + 1;
                UE_LOG(LogTemp, Log, TEXT("Decrementing JumpCount. Remaining: %d"), PS->ReturnJumpCount());
            }
        }
        else if (JumpCountFromState == 0 && bIsGrounded)
        {
            Super::Jump();
            JumpMaxCount = 1; // Reset to single jump
        }
    }
}


void APlayerCharacter_Base::CollectPickup_Implementation()
{
    if (HasAuthority())
    {
        if (APlayerState_Base* PS = Cast<APlayerState_Base>(GetPlayerState()))
        {
            PS->AddJumpCount(1); // Add jump count to PlayerState

            // Update JumpMaxCount dynamically
            //JumpMaxCount = (PS->ReturnJumpCount() > 1) ? 2 : 1;

            UE_LOG(LogTemp, Log, TEXT("Pickup collected. JumpCount: %d, JumpMaxCount: %d"),
                PS->ReturnJumpCount(),
                JumpMaxCount);
        }
    }
}

bool APlayerCharacter_Base::CollectPickup_Validate()
{
    return true;  // Validation always succeeds
}

void APlayerCharacter_Base::ResetJumpCount()
{
    // Reset the current jump count to the maximum
    CurrentJumpCount = MaxJumpCount;
}

void APlayerCharacter_Base::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Replicate jump-related variables
    DOREPLIFETIME(APlayerCharacter_Base, MaxJumpCount);
    DOREPLIFETIME(APlayerCharacter_Base, CurrentJumpCount);
}
