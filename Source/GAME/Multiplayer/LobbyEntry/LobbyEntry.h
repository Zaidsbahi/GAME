#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyEntry.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyEntrySelected, int, EntryLobbyIndex);

UCLASS()
class GAME_API ULobbyEntry : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitLobbyEntry(const FName& LobbyName, int EntryLobbyIndex);
	FOnLobbyEntrySelected OnLobbyEntrySelected;
	
private:

	UPROPERTY(meta=(BindWidget))
	class UButton* LobbyBtn;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* LobbyNameText;

	UFUNCTION()
	void LobbyBtnClicked();

	int LobbyIndex = -1;

protected:
	virtual void NativeConstruct() override;
	
};
