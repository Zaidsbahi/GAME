// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Multiplayer/EOSGameInstance/EOSGameInstance.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef GAME_EOSGameInstance_generated_h
#error "EOSGameInstance.generated.h already included, missing '#pragma once' in EOSGameInstance.h"
#endif
#define GAME_EOSGameInstance_generated_h

#define FID_UnrealEnginePROJECTS_CollabProjectNetwork_GAME_Source_GAME_Multiplayer_EOSGameInstance_EOSGameInstance_h_13_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execFindSession); \
	DECLARE_FUNCTION(execCreateSession); \
	DECLARE_FUNCTION(execLogin);


#define FID_UnrealEnginePROJECTS_CollabProjectNetwork_GAME_Source_GAME_Multiplayer_EOSGameInstance_EOSGameInstance_h_13_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUEOSGameInstance(); \
	friend struct Z_Construct_UClass_UEOSGameInstance_Statics; \
public: \
	DECLARE_CLASS(UEOSGameInstance, UGameInstance, COMPILED_IN_FLAGS(0 | CLASS_Transient), CASTCLASS_None, TEXT("/Script/GAME"), NO_API) \
	DECLARE_SERIALIZER(UEOSGameInstance)


#define FID_UnrealEnginePROJECTS_CollabProjectNetwork_GAME_Source_GAME_Multiplayer_EOSGameInstance_EOSGameInstance_h_13_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UEOSGameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UEOSGameInstance(UEOSGameInstance&&); \
	UEOSGameInstance(const UEOSGameInstance&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UEOSGameInstance); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UEOSGameInstance); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UEOSGameInstance) \
	NO_API virtual ~UEOSGameInstance();


#define FID_UnrealEnginePROJECTS_CollabProjectNetwork_GAME_Source_GAME_Multiplayer_EOSGameInstance_EOSGameInstance_h_10_PROLOG
#define FID_UnrealEnginePROJECTS_CollabProjectNetwork_GAME_Source_GAME_Multiplayer_EOSGameInstance_EOSGameInstance_h_13_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_UnrealEnginePROJECTS_CollabProjectNetwork_GAME_Source_GAME_Multiplayer_EOSGameInstance_EOSGameInstance_h_13_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_UnrealEnginePROJECTS_CollabProjectNetwork_GAME_Source_GAME_Multiplayer_EOSGameInstance_EOSGameInstance_h_13_INCLASS_NO_PURE_DECLS \
	FID_UnrealEnginePROJECTS_CollabProjectNetwork_GAME_Source_GAME_Multiplayer_EOSGameInstance_EOSGameInstance_h_13_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> GAME_API UClass* StaticClass<class UEOSGameInstance>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UnrealEnginePROJECTS_CollabProjectNetwork_GAME_Source_GAME_Multiplayer_EOSGameInstance_EOSGameInstance_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
