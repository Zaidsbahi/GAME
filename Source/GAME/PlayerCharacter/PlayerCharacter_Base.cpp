#include "PlayerCharacter_Base.h"
#include "GAME/PlayerState/PlayerState_Base.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GAME/ProximityBoost/ProximityBoost_Component.h"
#include "GAME/Multi/GameMode/EOS_GameMode.h"
#include "Net/UnrealNetwork.h"

////////////////////////////////////////
///  Constructor & BeginPlay & TICK  ///
////////////////////////////////////////
APlayerCharacter_Base::APlayerCharacter_Base()
{
    PrimaryActorTick.bCanEverTick = true;

    ////////////////////////////////////////
    //////  Setting Main Variables  ////////
    ////////////////////////////////////////

    //////  Jump Variables  ////////
    MaxJumpCount = 1;  // Default to 1 jump
    CurrentJumpCount = 1;
    bIsGrounded = true;

    //////  Jump Variables  ////////
    AirDashSpeed = 1500.f;
    AirDashDuration = 0.5f;
    bIsAirDashing = false;

    //////  Proximity Boost Component  ////////
    ProximityBoostComponent = CreateDefaultSubobject<UProximityBoost_Component>(TEXT("ProximityBoostComponent"));
    
    ////////////////////////////////////////
    ////////////////////////////////////////
    ////////////////////////////////////////
    
    // Replication
    bReplicates = true;
}
void APlayerCharacter_Base::BeginPlay()
{
    Super::BeginPlay();

    // Adding the MappingContext
    if(APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if(UEnhancedInputLocalPlayerSubsystem* Subsystem =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(MechanicsMappingContext, 0);
        }
    }
    

    // Configure the maximum allowed jumps using JumpMaxCount
    JumpMaxCount = (CurrentJumpCount > 1) ? 2 : 1;

    UE_LOG(LogTemp, Log, TEXT("BeginPlay: JumpMaxCount set to %d"), JumpMaxCount);
}
void APlayerCharacter_Base::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

////////////////////////////////////////
///////////  Jump Logic  ///////////////
////////////////////////////////////////
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
    //UE_LOG(LogTemp, Log, TEXT("Cant Jump:"));
   // if (!CanJump()) return;
    //UE_LOG(LogTemp, Log, TEXT("Can Jump:"));
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
                PS->SetActivateProximityBoostJump();
                JumpMaxCount = JumpMaxCount + 1;
                UE_LOG(LogTemp, Log, TEXT("Decrementing JumpCount. Remaining: %d"), PS->ReturnJumpCount());
            }
        }
        else if (JumpCountFromState == 0 && bIsGrounded)
        {
            Super::Jump();
            bIsGrounded = false;
            JumpMaxCount = 1; // Reset to single jump
        }
    }
    UE_LOG(LogTemp, Log, TEXT("Jump() called. JumpCountFromState: %d"), JumpMaxCount);
}
void APlayerCharacter_Base::ResetJumpCount()
{
    // Reset the current jump count to the maximum
    CurrentJumpCount = MaxJumpCount;
}

////////////////////////////////////////
//////////  AirDash Logic  /////////////
////////////////////////////////////////
void APlayerCharacter_Base::PerformAirDash()    
{
    // Return if AirDashing
    if (bIsAirDashing) return;
    
    if (APlayerState_Base* PS = Cast<APlayerState_Base>(GetPlayerState()))
    {
        int32 DashCountFromState = PS->ReturnDashCount();
    
        if (DashCountFromState > 0)
        {
            
            // Setting is Dashing true
            bIsAirDashing = true;

            PS->AddDashCount(-1);
            PS->SetActivateProximityBoostDash();

            // Calculating The AirDash Direction based on Input
            FVector DashDirection = GetActorForwardVector() * AirDashSpeed;

            // Apply the dash using LaucnhCharacter
            LaunchCharacter(DashDirection, true, true);

            //Notify the Server
            if (HasAuthority())
            {
                PerformAirDash_NetMulticast();
            }
            else
            {
                PerformAirDash_Server();
            }

            // Starting the timer to end the dash
            GetWorldTimerManager().SetTimer(AirDashTimerHandle, this, &APlayerCharacter_Base::EndAirDash, AirDashDuration, false);

        }
    }
}
void APlayerCharacter_Base::EndAirDash()
{
    bIsAirDashing = false;

    //Stop Any Remaining Movement Applied During the Dash
    //GetCharacterMovement()->StopMovementImmediately();
    
}
void APlayerCharacter_Base::PerformAirDash_NetMulticast_Implementation()
{
    if (!HasAuthority())
    {
        PerformAirDash();
    }
}
void APlayerCharacter_Base::PerformAirDash_Server_Implementation()
{
    PerformAirDash();
}

////////////////////////////////////////
//////      Pickup Logic        ////////
////////////////////////////////////////
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

////////////////////////////////////////
//////      Proximity Logic  ///////////
////////////////////////////////////////
void APlayerCharacter_Base::EnableSuperJump(bool bEnable)
{
    if (bEnable)
    {
        UE_LOG(LogTemp, Warning, TEXT("Enable super jump"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Disable super jump"));
    }
}

////////////////////////////////////////
////  PlayerInput And Replication  /////
////////////////////////////////////////
void APlayerCharacter_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(AirDash, ETriggerEvent::Started, this, &APlayerCharacter_Base::PerformAirDash);
        EnhancedInputComponent->BindAction(RestartLevelAction, ETriggerEvent::Triggered, this, &APlayerCharacter_Base::RestartLevel);
    }
}
void APlayerCharacter_Base::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Replicate jump-related variables
    DOREPLIFETIME(APlayerCharacter_Base, MaxJumpCount);
    DOREPLIFETIME(APlayerCharacter_Base, CurrentJumpCount);

    // Replicate AirDash Variables
    DOREPLIFETIME(APlayerCharacter_Base, bIsAirDashing);
    DOREPLIFETIME(APlayerCharacter_Base, AirDashSpeed);
}


void APlayerCharacter_Base::RestartLevel()
{
    if (HasAuthority()) // Only the host can restart
    {
        UE_LOG(LogTemp, Warning, TEXT("Server is restarting the level."));
        if (AEOS_GameMode* GameMode = Cast<AEOS_GameMode>(GetWorld()->GetAuthGameMode()))
        {
            UE_LOG(LogTemp, Warning, TEXT("CastedGameMode"));
            GameMode->RestartCurrentLevel();
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Only the server/host can restart the level."));
    }
}

//////////////////
////  Returns  ///
//////////////////
bool APlayerCharacter_Base::ReturnPlayerJumpActiveBool() const
{
    if (APlayerState_Base* PlayerState_Base = Cast<APlayerState_Base>(GetPlayerState()))
    {
        const bool JumpActiveBool = PlayerState_Base->ReturnActiveProxJump();
        return JumpActiveBool;
    }
    else
    {
    return false; 
    }
}
bool APlayerCharacter_Base::ReturnPlayerDashActiveBool() const
{
    if (APlayerState_Base* PlayerState_Base = Cast<APlayerState_Base>(GetPlayerState()))
    {
        const bool DashActiveBool = PlayerState_Base->ReturnActiveProxDash();
        return DashActiveBool;
    }
    else
    {
        return false; 
    }
}
