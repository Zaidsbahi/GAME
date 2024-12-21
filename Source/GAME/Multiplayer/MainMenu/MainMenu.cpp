#include "MainMenu.h"
#include "GAME/Multiplayer/EOSGameInstance/EOSGameInstance.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/ScrollBox.h"
#include "GAME/Multiplayer/LobbyEntry/LobbyEntry.h"

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();
	GameInst = GetGameInstance<UEOSGameInstance>();
	
	LoginBtn->OnClicked.AddDynamic(this, &UMainMenu::LoginBtnClicked);
	CreateSessionBtn->OnClicked.AddDynamic(this, &UMainMenu::CreateSessionBtnClicked);
	CreateSessionBtn->SetIsEnabled(false);
	FindSessionBtn->OnClicked.AddDynamic(this, &UMainMenu::FindSessionBtnClicked);
	SessionNameTextBox->OnTextChanged.AddDynamic(this, &UMainMenu::SessionNameChanged);
	JoinLobbyBtn->OnClicked.AddDynamic(this, &UMainMenu::JoinLobbyBtnClicked);
	JoinLobbyBtn->SetIsEnabled(false);
	GameInst->SearchCompleted.AddUObject(this, &UMainMenu::SessionSearchCompleted);
}

void UMainMenu::LoginBtnClicked()
{
	if (GameInst)
	{
		GameInst->Login();
	}
}

void UMainMenu::CreateSessionBtnClicked()
{
	if (GameInst)
	{
		GameInst->CreateSession(FName(SessionNameTextBox->GetText().ToString()));
	}
}

void UMainMenu::FindSessionBtnClicked()
{
	if (GameInst)
	{
		GameInst->FindSession();
	}
}

void UMainMenu::SessionNameChanged(const FText& Text)
{
	CreateSessionBtn->SetIsEnabled(!Text.IsEmpty());
}

void UMainMenu::LobbyEntrySelected(int LobbyEntryIndex)
{
	SelectLobbyEntryIndex = LobbyEntryIndex;
	if (SelectLobbyEntryIndex != -1)
	{
		JoinLobbyBtn->SetIsEnabled(true);
	}
}

void UMainMenu::JoinLobbyBtnClicked()
{
	if (GameInst)
	{
		GameInst->JoinLobbyBySearchResultIndex(SelectLobbyEntryIndex);
	}
}



void UMainMenu::SessionSearchCompleted(const TArray<FOnlineSessionSearchResult>& SearchResults)
{
	LobbyListScrollBox->ClearChildren();
	int Index = 0;
	for (const FOnlineSessionSearchResult& SearchResult : SearchResults)
	{
		FString SessionName = GameInst->GetSessionName(SearchResult);

		ULobbyEntry* LobbyEntry = CreateWidget<ULobbyEntry>(LobbyListScrollBox, LobbyEntryClass);
		LobbyEntry->InitLobbyEntry(FName(SessionName), Index);
		LobbyListScrollBox->AddChild(LobbyEntry);
		LobbyEntry->OnLobbyEntrySelected.AddDynamic(this, &UMainMenu::LobbyEntrySelected);
		++Index;
		
	}
}
