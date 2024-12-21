// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyEntry.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void ULobbyEntry::InitLobbyEntry(const FName& LobbyName, int EntryLobbyIndex)
{
	LobbyNameText->SetText(FText::FromName(LobbyName));
	LobbyIndex = EntryLobbyIndex;
}

void ULobbyEntry::LobbyBtnClicked()
{
	OnLobbyEntrySelected.Broadcast(LobbyIndex);
}

void ULobbyEntry::NativeConstruct()
{
	Super::NativeConstruct();

	LobbyBtn->OnClicked.AddDynamic(this, &ULobbyEntry::LobbyBtnClicked);
}
