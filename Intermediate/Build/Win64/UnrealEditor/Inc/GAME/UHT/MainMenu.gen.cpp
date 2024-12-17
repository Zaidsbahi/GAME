// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "GAME/Multiplayer/MainMenu/MainMenu.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeMainMenu() {}

// Begin Cross Module References
GAME_API UClass* Z_Construct_UClass_UMainMenu();
GAME_API UClass* Z_Construct_UClass_UMainMenu_NoRegister();
UMG_API UClass* Z_Construct_UClass_UButton_NoRegister();
UMG_API UClass* Z_Construct_UClass_UUserWidget();
UPackage* Z_Construct_UPackage__Script_GAME();
// End Cross Module References

// Begin Class UMainMenu Function CreateSessionBtnClicked
struct Z_Construct_UFunction_UMainMenu_CreateSessionBtnClicked_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Multiplayer/MainMenu/MainMenu.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UMainMenu_CreateSessionBtnClicked_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UMainMenu, nullptr, "CreateSessionBtnClicked", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00040401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UMainMenu_CreateSessionBtnClicked_Statics::Function_MetaDataParams), Z_Construct_UFunction_UMainMenu_CreateSessionBtnClicked_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UMainMenu_CreateSessionBtnClicked()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UMainMenu_CreateSessionBtnClicked_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UMainMenu::execCreateSessionBtnClicked)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->CreateSessionBtnClicked();
	P_NATIVE_END;
}
// End Class UMainMenu Function CreateSessionBtnClicked

// Begin Class UMainMenu Function FindSessionBtnClicked
struct Z_Construct_UFunction_UMainMenu_FindSessionBtnClicked_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Multiplayer/MainMenu/MainMenu.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UMainMenu_FindSessionBtnClicked_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UMainMenu, nullptr, "FindSessionBtnClicked", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00040401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UMainMenu_FindSessionBtnClicked_Statics::Function_MetaDataParams), Z_Construct_UFunction_UMainMenu_FindSessionBtnClicked_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UMainMenu_FindSessionBtnClicked()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UMainMenu_FindSessionBtnClicked_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UMainMenu::execFindSessionBtnClicked)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->FindSessionBtnClicked();
	P_NATIVE_END;
}
// End Class UMainMenu Function FindSessionBtnClicked

// Begin Class UMainMenu Function LoginBtnClicked
struct Z_Construct_UFunction_UMainMenu_LoginBtnClicked_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Multiplayer/MainMenu/MainMenu.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UMainMenu_LoginBtnClicked_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UMainMenu, nullptr, "LoginBtnClicked", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00040401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UMainMenu_LoginBtnClicked_Statics::Function_MetaDataParams), Z_Construct_UFunction_UMainMenu_LoginBtnClicked_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UMainMenu_LoginBtnClicked()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UMainMenu_LoginBtnClicked_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UMainMenu::execLoginBtnClicked)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->LoginBtnClicked();
	P_NATIVE_END;
}
// End Class UMainMenu Function LoginBtnClicked

// Begin Class UMainMenu
void UMainMenu::StaticRegisterNativesUMainMenu()
{
	UClass* Class = UMainMenu::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "CreateSessionBtnClicked", &UMainMenu::execCreateSessionBtnClicked },
		{ "FindSessionBtnClicked", &UMainMenu::execFindSessionBtnClicked },
		{ "LoginBtnClicked", &UMainMenu::execLoginBtnClicked },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UMainMenu);
UClass* Z_Construct_UClass_UMainMenu_NoRegister()
{
	return UMainMenu::StaticClass();
}
struct Z_Construct_UClass_UMainMenu_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Multiplayer/MainMenu/MainMenu.h" },
		{ "ModuleRelativePath", "Multiplayer/MainMenu/MainMenu.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LoginBtn_MetaData[] = {
		{ "BindWidget", "" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Multiplayer/MainMenu/MainMenu.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CreateSessionBtn_MetaData[] = {
		{ "BindWidget", "" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Multiplayer/MainMenu/MainMenu.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_FindSessionBtn_MetaData[] = {
		{ "BindWidget", "" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Multiplayer/MainMenu/MainMenu.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_LoginBtn;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_CreateSessionBtn;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_FindSessionBtn;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UMainMenu_CreateSessionBtnClicked, "CreateSessionBtnClicked" }, // 294159093
		{ &Z_Construct_UFunction_UMainMenu_FindSessionBtnClicked, "FindSessionBtnClicked" }, // 1440383979
		{ &Z_Construct_UFunction_UMainMenu_LoginBtnClicked, "LoginBtnClicked" }, // 3553081499
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UMainMenu>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UMainMenu_Statics::NewProp_LoginBtn = { "LoginBtn", nullptr, (EPropertyFlags)0x0040000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UMainMenu, LoginBtn), Z_Construct_UClass_UButton_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LoginBtn_MetaData), NewProp_LoginBtn_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UMainMenu_Statics::NewProp_CreateSessionBtn = { "CreateSessionBtn", nullptr, (EPropertyFlags)0x0040000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UMainMenu, CreateSessionBtn), Z_Construct_UClass_UButton_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CreateSessionBtn_MetaData), NewProp_CreateSessionBtn_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UMainMenu_Statics::NewProp_FindSessionBtn = { "FindSessionBtn", nullptr, (EPropertyFlags)0x0040000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UMainMenu, FindSessionBtn), Z_Construct_UClass_UButton_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_FindSessionBtn_MetaData), NewProp_FindSessionBtn_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UMainMenu_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UMainMenu_Statics::NewProp_LoginBtn,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UMainMenu_Statics::NewProp_CreateSessionBtn,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UMainMenu_Statics::NewProp_FindSessionBtn,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UMainMenu_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UMainMenu_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UUserWidget,
	(UObject* (*)())Z_Construct_UPackage__Script_GAME,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UMainMenu_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UMainMenu_Statics::ClassParams = {
	&UMainMenu::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UMainMenu_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UMainMenu_Statics::PropPointers),
	0,
	0x00B010A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UMainMenu_Statics::Class_MetaDataParams), Z_Construct_UClass_UMainMenu_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UMainMenu()
{
	if (!Z_Registration_Info_UClass_UMainMenu.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UMainMenu.OuterSingleton, Z_Construct_UClass_UMainMenu_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UMainMenu.OuterSingleton;
}
template<> GAME_API UClass* StaticClass<UMainMenu>()
{
	return UMainMenu::StaticClass();
}
UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UMainMenu);
UMainMenu::~UMainMenu() {}
// End Class UMainMenu

// Begin Registration
struct Z_CompiledInDeferFile_FID_UnrealEnginePROJECTS_CollabProjectNetwork_GAME_Source_GAME_Multiplayer_MainMenu_MainMenu_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UMainMenu, UMainMenu::StaticClass, TEXT("UMainMenu"), &Z_Registration_Info_UClass_UMainMenu, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UMainMenu), 642486511U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealEnginePROJECTS_CollabProjectNetwork_GAME_Source_GAME_Multiplayer_MainMenu_MainMenu_h_3918723857(TEXT("/Script/GAME"),
	Z_CompiledInDeferFile_FID_UnrealEnginePROJECTS_CollabProjectNetwork_GAME_Source_GAME_Multiplayer_MainMenu_MainMenu_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealEnginePROJECTS_CollabProjectNetwork_GAME_Source_GAME_Multiplayer_MainMenu_MainMenu_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
