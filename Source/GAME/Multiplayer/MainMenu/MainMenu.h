#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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

	UFUNCTION()
	void LoginBtnClicked();
	
	UFUNCTION()
	void CreateSessionBtnClicked();

	UFUNCTION()
	void FindSessionBtnClicked();

	UFUNCTION()
	void SessionNameChanged(const FText& Text);

};
