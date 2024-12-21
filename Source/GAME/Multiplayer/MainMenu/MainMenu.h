#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GAME/Multiplayer/EOSGameInstance/EOSGameInstance.h"
#include "OnlineSessionSettings.h"
#include "MainMenu.generated.h"


UCLASS()
class GAME_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	class UEOSGameInstance* GameInst;
private:

	UPROPERTY(meta = (BindWidget))
	class UButton* LoginBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* CreateSessionBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* FindSessionBtn;

	UPROPERTY(meta = (BindWidget))
	class UEditableText* SessionNameTextBox;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* LobbyListScrollBox;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ULobbyEntry> LobbyEntryClass;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinLobbyBtn;

	UFUNCTION()
	void LoginBtnClicked();
	
	UFUNCTION()
	void CreateSessionBtnClicked();

	UFUNCTION()
	void FindSessionBtnClicked();

	UFUNCTION()
	void SessionNameChanged(const FText& Text);

	UFUNCTION()
	void LobbyEntrySelected(int LobbyEntryIndex);

	UFUNCTION()
	void JoinLobbyBtnClicked();
	
	void SessionSearchCompleted(const TArray<FOnlineSessionSearchResult>& SearchResults);

	int SelectLobbyEntryIndex = -1;

};



