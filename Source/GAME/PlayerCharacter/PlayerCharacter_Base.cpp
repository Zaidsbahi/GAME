#include "PlayerCharacter_Base.h"
#include "EngineUtils.h"
#include "GAME/PlayerState/PlayerState_Base.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "GAME/ProximityBoost/ProximityBoost_Component.h"
#include "GAME/Multi/GameMode/EOS_GameMode.h"
#include "GAME/Multi/GameState/EOS_GameState.h"
#include "GAME/TrailActor/TrailActor.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/World.h"
#include "Components/PostProcessComponent.h"

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
    bAlwaysRelevant = true;
    SetReplicates(true);
    ACharacter::SetReplicateMovement(true);
    
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
    
    // Find the Niagara Component
    TrailEffect = FindComponentByClass<UNiagaraComponent>();
    
    if (TrailEffect)
    {
        TrailEffect->SetAutoActivate(false);  // Ensure it starts deactivated
    }

    // Configure the maximum allowed jumps using JumpMaxCount
    JumpMaxCount = (CurrentJumpCount > 1) ? 2 : 1;

    UE_LOG(LogTemp, Log, TEXT("BeginPlay: JumpMaxCount set to %d"), JumpMaxCount);

    ///////////////////////////////
    ///     Vignette Effect     ///
    ///////////////////////////////
    // Get the player’s post-process volume
    APostProcessVolume* PostProcessVolume = nullptr;
    for (TActorIterator<APostProcessVolume> It(GetWorld()); It; ++It)
    {
        PostProcessVolume = *It;
        break; // Take the first available Post Process Volume
    }

    if (PostProcessVolume && SpeedEffectMaterial)
    {
        // Create a Dynamic Material Instance
        SpeedEffectMaterialInstance = UMaterialInstanceDynamic::Create(SpeedEffectMaterial, this);
        PostProcessVolume->AddOrUpdateBlendable(SpeedEffectMaterialInstance);
    }
  
}

void APlayerCharacter_Base::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    float CurrentSpeed = GetVelocity().Size();
    UpdateSprintEffects(CurrentSpeed);
    
    if (!GetCharacterMovement()->IsMovingOnGround() && bIsGrounded)
    {
        bIsGrounded = false;
        StartCoyoteTime();
    }

    // Use GameState instead of GameMode
    AEOS_GameState* GameState = Cast<AEOS_GameState>(GetWorld()->GetGameState());
    if (!GameState)
    {
        //UE_LOG(LogTemp, Warning, TEXT("[%s] GameState is nullptr"), HasAuthority() ? TEXT("Server") : TEXT("Client"));
        return;
    }

    // Log bShouldJog from GameState
    //UE_LOG(LogTemp, Log, TEXT("[%s] bShouldJog: %s"),
           //HasAuthority() ? TEXT("Server") : TEXT("Client"),
          // GameState->bShouldJog ? TEXT("True") : TEXT("False"));

    // Ensure it's not Track1 and both players are ready
    //FString CurrentLevel = UGameplayStatics::GetCurrentLevelName(GetWorld());
    if (bIsJogging)
    {
        //UE_LOG(LogTemp, Log, TEXT("[%s] Jogging logic executed! Adding movement."), HasAuthority() ? TEXT("Server") : TEXT("Client"));
        AddMovementInput(GetActorForwardVector(), JoggingSpeed * DeltaTime);
    }
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
            if (ProximityBoostComponent)
            {
                if (ProximityBoostComponent->bIsActivatedProximity == true)
                {
                    ProximityBoostComponent->bCanInfiniteDashAndDoubleJump = false;
                    ProximityBoostComponent->ActivateProximityBoost();
                    UE_LOG(LogTemp, Log, TEXT("The Activated Proximity Is True"))
                }
                else
                {
                    UE_LOG(LogTemp, Log, TEXT("The Activated Proximity Is False"))
                }
            }
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("Client Jump Count: %d"), JumpCount);
            if (ProximityBoostComponent)
            {
                if (ProximityBoostComponent->bIsActivatedProximity == true)
                {
                    ProximityBoostComponent->ActivateProximityBoost();
                }
                else
                {
                    UE_LOG(LogTemp, Log, TEXT("The Activated Proximity Is False"))
                }
            }
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

        if (JumpCountFromState > 0 || bCanUseCoyoteTime) // Allow jump if coyote time is active
        {
              
            if (bIsGrounded || bCanUseCoyoteTime)
            {
                Super::Jump();
                bIsGrounded = false;
                JumpMaxCount = 2; // Set max allowed jumps while airborne
                
                // Disable coyote time after jumping
                DisableCoyoteTime();

                if (JumpCountFromState > 0)
                {
                    // Start trail spawning
                    bIsGeneratingTrail = true;
                    GetWorld()->GetTimerManager().SetTimer(TrailSpawnTimerHandle, this, &APlayerCharacter_Base::SpawnTrailRepeatedly, 0.2f, true);
                    // ✅ Activate the Niagara Trail Effect
                    if (TrailEffect)
                    {
                        TrailEffect->Activate();
                    }
                }
            }
            else
            {
                if (JumpCountFromState > 0)
                {
                    bIsGeneratingTrail = true;
                    GetWorld()->GetTimerManager().SetTimer(TrailSpawnTimerHandle, this, &APlayerCharacter_Base::SpawnTrailRepeatedly, 0.2f, true);
                    // ✅ Activate the Niagara Trail Effect
                    if (TrailEffect)
                    {
                        TrailEffect->Activate();
                    }
                }
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
            //FVector DashDirection = GetActorForwardVector() * AirDashSpeed;

            // **Get Dash Direction from Crosshair**
            FVector DashDirection = GetCrosshairDirection();

            // Ensure DashDirection is valid
            if (!DashDirection.IsNearlyZero())
            {
                DashDirection.Normalize(); // Normalize the direction before applying force
                FVector DashVelocity = DashDirection * AirDashSpeed;

                bIsGeneratingTrail = true;
                GetWorld()->GetTimerManager().SetTimer(TrailSpawnTimerHandle, this, &APlayerCharacter_Base::SpawnTrailRepeatedly, 0.2f, true);

                // ✅ Activate the Niagara Trail Effect
                if (TrailEffect)
                {
                    TrailEffect->Activate();
                }

                // Apply the dash using LaucnhCharacter
                LaunchCharacter(DashVelocity, true, true);
               

                // Play Dash Sound
                PlayDashSound();
            
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
//////////  AirDash Logic  /////////////
////////////////////////////////////////
FVector APlayerCharacter_Base::GetCrosshairDirection()
{
    APlayerController* PC = Cast<APlayerController>(GetController());
    if (!PC) return FVector::ZeroVector;

    FVector CameraLocation;
    FRotator CameraRotation;

    PC->GetPlayerViewPoint(CameraLocation, CameraRotation); // Get camera location & rotation

    FVector TraceStart = CameraLocation;
    FVector TraceEnd = TraceStart + (CameraRotation.Vector() * 5000.f); // Long trace

    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this); // Ignore the player

    if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, Params))
    {
        return (HitResult.ImpactPoint - GetActorLocation()).GetSafeNormal();
    }

    return CameraRotation.Vector(); // Default to camera forward if nothing is hit

}

void APlayerCharacter_Base::DeativateNiagraSystem()
{
    // ✅ Deactivate the Niagara Trail Effect
    if (TrailEffect)
    {
        TrailEffect->Deactivate();
    }
}

///////////////////////////////
///      Toggle Jogging     ///
///////////////////////////////
void APlayerCharacter_Base::ToggleJogging()
{
    if (HasAuthority())
    {
        bIsJogging = !bIsJogging; // Toggle jogging
        UE_LOG(LogTemp, Log, TEXT("Jogging Toggled: %s"), bIsJogging ? TEXT("Enabled") : TEXT("Disabled"));
    }
    else
    {
        ToggleJogging_Server(); // Call the server function to sync across clients
    }
}


///////////////////////////////
///     Vignette Effect     ///
///////////////////////////////
void APlayerCharacter_Base::UpdateSprintEffects(float Speed)
{
    if (!SpeedEffectMaterialInstance) return;

    float SpeedThreshold = 400.0f; // Minimum speed for effect
    float MaxEffectSpeed = 1800.0f; // Maximum speed effect intensity
    float MaxIntensity = 1.5f;
    float MinIntensity = 0.0f;

    // Calculate intensity based on speed
    float EffectIntensity = FMath::Clamp((Speed - SpeedThreshold) / (MaxEffectSpeed - SpeedThreshold), MinIntensity, MaxIntensity);

    // Debug Log
    UE_LOG(LogTemp, Warning, TEXT("Current Speed: %f, Effect Intensity: %f"), Speed, EffectIntensity);
    
    // Update material parameter
    SpeedEffectMaterialInstance->SetScalarParameterValue(TEXT("Speed"), EffectIntensity);

}

void APlayerCharacter_Base::ToggleJogging_Server_Implementation()
{
    ToggleJogging(); // Ensure the server updates the variable
}

bool APlayerCharacter_Base::ToggleJogging_Server_Validate()
{
    return true;
}


////////////////////////////////////////
//////      Pickup Logic        ////////
////////////////////////////////////////
void APlayerCharacter_Base::CollectPickup_Implementation()
{
    if (HasAuthority())
    {
        UE_LOG(LogTemp, Log, TEXT("Collect Pickup Entered"))
        if (APlayerState_Base* PS = Cast<APlayerState_Base>(GetPlayerState()))
        {
           // PS->AddJumpCount(1); // Add jump count to PlayerState

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
        EnhancedInputComponent->BindAction(RestartTrackAction, ETriggerEvent::Triggered, this, &APlayerCharacter_Base::RestartTrack);
        EnhancedInputComponent->BindAction(JoggingToggleAction, ETriggerEvent::Triggered, this, &APlayerCharacter_Base::ToggleJogging);
    }
}
void APlayerCharacter_Base::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Replicate jump-related variables
    DOREPLIFETIME(APlayerCharacter_Base, MaxJumpCount);
    DOREPLIFETIME(APlayerCharacter_Base, CurrentJumpCount);
    DOREPLIFETIME(APlayerCharacter_Base, bIsGrounded);

    // Replicate AirDash Variables
    DOREPLIFETIME(APlayerCharacter_Base, bIsAirDashing);
    DOREPLIFETIME(APlayerCharacter_Base, AirDashSpeed);
    DOREPLIFETIME(APlayerCharacter_Base, bIsJogging);
}
void APlayerCharacter_Base::RestartLevel()
{
    if (HasAuthority()) // Only the host can restart
    {
        StopTrailSpawning();
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
void APlayerCharacter_Base::RestartTrack()
{
    if (HasAuthority()) // Only the host can restart
    {
        StopTrailSpawning();
        UE_LOG(LogTemp, Warning, TEXT("Server is restarting the level."));
        if (AEOS_GameMode* GameMode = Cast<AEOS_GameMode>(GetWorld()->GetAuthGameMode()))
        {
            UE_LOG(LogTemp, Warning, TEXT("CastedGameMode"));
            GameMode->RestartCurrentTrack();
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
void APlayerCharacter_Base::ResetJumpHeightAfterLanding()
{
    if (APlayerState_Base* PS = Cast<APlayerState_Base>(GetPlayerState()))
    {
        int32 JumpCountFromState = PS->ReturnJumpCount();

        UE_LOG(LogTemp, Log, TEXT("Resetting Jump Height After Landing "));

        if (JumpCountFromState == 0)
        {
            GetCharacterMovement()->JumpZVelocity = 500;
        }
    }
}


//////////////////////////
///  Trail Actor Logic ///
//////////////////////////
void APlayerCharacter_Base::ActivatePlayersProximityBoost()
{
    if (HasAuthority())
    {
        if (ProximityBoostComponent)
        {
            ProximityBoostComponent->ServerActivateProximityBoost();
        }
    }
}
void APlayerCharacter_Base::DeActivatePlayersProximityBoost()
{
    if (HasAuthority())
    {
        if (ProximityBoostComponent)
        {
            ProximityBoostComponent->bIsStillInRangeOfProximity = false;
            ProximityBoostComponent->ServerDeActivateProximityBoost();
        }
    }
}
void APlayerCharacter_Base::SpawnTrailActors()
{

    if (!bIsGeneratingTrail) return;  // Stop if no longer needed

    
    if (HasAuthority())  // Ensure this runs only on the server for replication purposes
    {
        if (UWorld* World = GetWorld())
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;  // Assign the owner to the player
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

            FVector SpawnLocation = GetActorLocation() + 200 * GetActorForwardVector();
            FRotator SpawnRotation = GetActorRotation();

            ATrailActor* SpawnedTrail = World->SpawnActor<ATrailActor>(ATrailActor::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);

            
            if (SpawnedTrail)
            {
                UE_LOG(LogTemp, Log, TEXT("Successfully spawned TrailActor: %s"), *SpawnedTrail->GetName());
                SpawnedTrail->SetOwner(nullptr); // Ensuring it has no Dependency on the player
                
                // Ignore collisions with the owner
                SpawnedTrail->Sphere->IgnoreActorWhenMoving(this, true);

                
                // Schedule the trail actor to be destroyed after 5 seconds
                World->GetTimerManager().SetTimer(
                    SpawnedTrail->DestructionTimerHandle,  // Use a FTimerHandle within TrailActor class
                    SpawnedTrail,
                    &ATrailActor::DestroySelf,  // Function to destroy itself
                    5.0f,  // Time in seconds before destruction
                    false  // Do not loop
                    );
                
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to spawn TrailActor"));
            }
        }
    }
}

void APlayerCharacter_Base::SpawnTrailRepeatedly()
{
    if (!bIsGeneratingTrail) return;  // Stop if no longer needed

    if (HasAuthority())  // Ensure this runs only on the server for replication purposes
    {
        if (UWorld* World = GetWorld())
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;  
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

            FVector SpawnLocation = GetActorLocation() - GetActorForwardVector() * 50.0f;  // Slight offset behind player
            FRotator SpawnRotation = GetActorRotation();

            ATrailActor* SpawnedTrail = World->SpawnActor<ATrailActor>(ATrailActor::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);

          
            
            if (SpawnedTrail)
            {
                UE_LOG(LogTemp, Log, TEXT("Trail Actor Spawned: %s"), *SpawnedTrail->GetName());
                SpawnedTrail->SetOwner(nullptr);
                SpawnedTrail->Sphere->IgnoreActorWhenMoving(this, true);

                // Schedule destruction after 5 seconds
                World->GetTimerManager().SetTimer(
                    SpawnedTrail->DestructionTimerHandle,
                    SpawnedTrail,
                    &ATrailActor::DestroySelf,
                    5.0f,
                    false
                );
            }
        }
    }
}

void APlayerCharacter_Base::StopTrailSpawning()
{
    bIsGeneratingTrail = false;
    GetWorld()->GetTimerManager().ClearTimer(TrailSpawnTimerHandle);

    UE_LOG(LogTemp, Log, TEXT("Trail spawning stopped."));
}


////////////////
///  Coyote  ///
////////////////
void APlayerCharacter_Base::StartCoyoteTime()
{
    if (bCanUseCoyoteTime) return;  // Already active, no need to restart

    UE_LOG(LogTemp, Log, TEXT("Coyote time started!"));

    bCanUseCoyoteTime = true;

    // Start the timer to disable coyote time after the duration
    GetWorld()->GetTimerManager().SetTimer(
        CoyoteTimeTimerHandle,
        this,
        &APlayerCharacter_Base::DisableCoyoteTime,
        CoyoteTimeDuration,
        false  // Do not loop
    );
}
void APlayerCharacter_Base::DisableCoyoteTime()
{
    bCanUseCoyoteTime = false;
    UE_LOG(LogTemp, Log, TEXT("Coyote time ended."));
}

///////////////////////////
///   Winning Condition ///
///////////////////////////
void APlayerCharacter_Base::AddWinningWidget()
{
    if (!WinningWidgetClass)
    {
        UE_LOG(LogTemp, Error, TEXT("[%s] WinningWidgetClass is not set!"), HasAuthority() ? TEXT("Server") : TEXT("Client"));
        return;
    }

    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    if (!PlayerController)
    {
        UE_LOG(LogTemp, Error, TEXT("[%s] PlayerController is nullptr!"), HasAuthority() ? TEXT("Server") : TEXT("Client"));
        return;
    }

    // Create the widget
    UUserWidget* WinningWidget = CreateWidget<UUserWidget>(PlayerController, WinningWidgetClass);
    if (!WinningWidget)
    {
        UE_LOG(LogTemp, Error, TEXT("[%s] Failed to create Winning Widget!"), HasAuthority() ? TEXT("Server") : TEXT("Client"));
        return;
    }

    // Add to viewport
    WinningWidget->AddToViewport();
    UE_LOG(LogTemp, Warning, TEXT("[%s] Winning Widget Added to Viewport!"), HasAuthority() ? TEXT("Server") : TEXT("Client"));

}

///////////////////////////
///       Sounds        ///
///////////////////////////
void APlayerCharacter_Base::PlayDashSound()
{
    if (DashSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, DashSound, GetActorLocation());
    }
}
