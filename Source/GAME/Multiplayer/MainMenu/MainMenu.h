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

	UFUNCTION()
	void LoginBtnClicked();
};
