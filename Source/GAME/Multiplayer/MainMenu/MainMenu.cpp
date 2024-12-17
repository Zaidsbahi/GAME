#include "MainMenu.h"
#include "GAME/Multiplayer/EOSGameInstance/EOSGameInstance.h"
#include "Components/Button.h"

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();
	GameInst = GetGameInstance<UEOSGameInstance>();
	
	LoginBtn->OnClicked.AddDynamic(this, &UMainMenu::LoginBtnClicked);
	CreateSessionBtn->OnClicked.AddDynamic(this, &UMainMenu::CreateSessionBtnClicked);
	FindSessionBtn->OnClicked.AddDynamic(this, &UMainMenu::FindSessionBtnClicked);
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
		GameInst->CreateSession();
	}
}

void UMainMenu::FindSessionBtnClicked()
{
	if (GameInst)
	{
		GameInst->FindSession();
	}
}
