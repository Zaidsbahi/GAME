#include "MainMenu.h"
#include "GAME/Multiplayer/EOSGameInstance/EOSGameInstance.h"
#include "Components/Button.h"
#include "Components/EditableText.h"

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();
	GameInst = GetGameInstance<UEOSGameInstance>();
	
	LoginBtn->OnClicked.AddDynamic(this, &UMainMenu::LoginBtnClicked);
	CreateSessionBtn->OnClicked.AddDynamic(this, &UMainMenu::CreateSessionBtnClicked);
	CreateSessionBtn->SetIsEnabled(false);
	FindSessionBtn->OnClicked.AddDynamic(this, &UMainMenu::FindSessionBtnClicked);
	SessionNameTextBox->OnTextChanged.AddDynamic(this, &UMainMenu::SessionNameChanged);
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
