// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "GAME/Multiplayer/EOSGameInstance/EOSGameInstance.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeEOSGameInstance() {}

// Begin Cross Module References
ENGINE_API UClass* Z_Construct_UClass_UGameInstance();
ENGINE_API UClass* Z_Construct_UClass_UWorld_NoRegister();
GAME_API UClass* Z_Construct_UClass_UEOSGameInstance();
GAME_API UClass* Z_Construct_UClass_UEOSGameInstance_NoRegister();
UPackage* Z_Construct_UPackage__Script_GAME();
// End Cross Module References

// Begin Class UEOSGameInstance Function CreateSession
struct Z_Construct_UFunction_UEOSGameInstance_CreateSession_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Multiplayer/EOSGameInstance/EOSGameInstance.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UEOSGameInstance_CreateSession_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UEOSGameInstance, nullptr, "CreateSession", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UEOSGameInstance_CreateSession_Statics::Function_MetaDataParams), Z_Construct_UFunction_UEOSGameInstance_CreateSession_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UEOSGameInstance_CreateSession()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UEOSGameInstance_CreateSession_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UEOSGameInstance::execCreateSession)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->CreateSession();
	P_NATIVE_END;
}
// End Class UEOSGameInstance Function CreateSession

// Begin Class UEOSGameInstance Function FindSession
struct Z_Construct_UFunction_UEOSGameInstance_FindSession_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Multiplayer/EOSGameInstance/EOSGameInstance.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UEOSGameInstance_FindSession_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UEOSGameInstance, nullptr, "FindSession", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UEOSGameInstance_FindSession_Statics::Function_MetaDataParams), Z_Construct_UFunction_UEOSGameInstance_FindSession_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UEOSGameInstance_FindSession()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UEOSGameInstance_FindSession_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UEOSGameInstance::execFindSession)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->FindSession();
	P_NATIVE_END;
}
// End Class UEOSGameInstance Function FindSession

// Begin Class UEOSGameInstance Function Login
struct Z_Construct_UFunction_UEOSGameInstance_Login_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Multiplayer/EOSGameInstance/EOSGameInstance.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UEOSGameInstance_Login_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UEOSGameInstance, nullptr, "Login", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UEOSGameInstance_Login_Statics::Function_MetaDataParams), Z_Construct_UFunction_UEOSGameInstance_Login_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UEOSGameInstance_Login()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UEOSGameInstance_Login_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UEOSGameInstance::execLogin)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->Login();
	P_NATIVE_END;
}
// End Class UEOSGameInstance Function Login

// Begin Class UEOSGameInstance
void UEOSGameInstance::StaticRegisterNativesUEOSGameInstance()
{
	UClass* Class = UEOSGameInstance::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "CreateSession", &UEOSGameInstance::execCreateSession },
		{ "FindSession", &UEOSGameInstance::execFindSession },
		{ "Login", &UEOSGameInstance::execLogin },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UEOSGameInstance);
UClass* Z_Construct_UClass_UEOSGameInstance_NoRegister()
{
	return UEOSGameInstance::StaticClass();
}
struct Z_Construct_UClass_UEOSGameInstance_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Multiplayer/EOSGameInstance/EOSGameInstance.h" },
		{ "ModuleRelativePath", "Multiplayer/EOSGameInstance/EOSGameInstance.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_GameLevel_MetaData[] = {
		{ "Category", "EOSGameInstance" },
		{ "ModuleRelativePath", "Multiplayer/EOSGameInstance/EOSGameInstance.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FSoftObjectPropertyParams NewProp_GameLevel;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UEOSGameInstance_CreateSession, "CreateSession" }, // 441022921
		{ &Z_Construct_UFunction_UEOSGameInstance_FindSession, "FindSession" }, // 4129393737
		{ &Z_Construct_UFunction_UEOSGameInstance_Login, "Login" }, // 3254372790
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UEOSGameInstance>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FSoftObjectPropertyParams Z_Construct_UClass_UEOSGameInstance_Statics::NewProp_GameLevel = { "GameLevel", nullptr, (EPropertyFlags)0x0044000000010001, UECodeGen_Private::EPropertyGenFlags::SoftObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UEOSGameInstance, GameLevel), Z_Construct_UClass_UWorld_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_GameLevel_MetaData), NewProp_GameLevel_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UEOSGameInstance_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UEOSGameInstance_Statics::NewProp_GameLevel,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UEOSGameInstance_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UEOSGameInstance_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UGameInstance,
	(UObject* (*)())Z_Construct_UPackage__Script_GAME,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UEOSGameInstance_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UEOSGameInstance_Statics::ClassParams = {
	&UEOSGameInstance::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UEOSGameInstance_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UEOSGameInstance_Statics::PropPointers),
	0,
	0x009000A8u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UEOSGameInstance_Statics::Class_MetaDataParams), Z_Construct_UClass_UEOSGameInstance_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UEOSGameInstance()
{
	if (!Z_Registration_Info_UClass_UEOSGameInstance.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UEOSGameInstance.OuterSingleton, Z_Construct_UClass_UEOSGameInstance_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UEOSGameInstance.OuterSingleton;
}
template<> GAME_API UClass* StaticClass<UEOSGameInstance>()
{
	return UEOSGameInstance::StaticClass();
}
UEOSGameInstance::UEOSGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UEOSGameInstance);
UEOSGameInstance::~UEOSGameInstance() {}
// End Class UEOSGameInstance

// Begin Registration
struct Z_CompiledInDeferFile_FID_UnrealEnginePROJECTS_CollabProjectNetwork_GAME_Source_GAME_Multiplayer_EOSGameInstance_EOSGameInstance_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UEOSGameInstance, UEOSGameInstance::StaticClass, TEXT("UEOSGameInstance"), &Z_Registration_Info_UClass_UEOSGameInstance, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UEOSGameInstance), 836257676U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealEnginePROJECTS_CollabProjectNetwork_GAME_Source_GAME_Multiplayer_EOSGameInstance_EOSGameInstance_h_341011066(TEXT("/Script/GAME"),
	Z_CompiledInDeferFile_FID_UnrealEnginePROJECTS_CollabProjectNetwork_GAME_Source_GAME_Multiplayer_EOSGameInstance_EOSGameInstance_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealEnginePROJECTS_CollabProjectNetwork_GAME_Source_GAME_Multiplayer_EOSGameInstance_EOSGameInstance_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
