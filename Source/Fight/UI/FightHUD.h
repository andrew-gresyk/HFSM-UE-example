// Created by Andrew Gresyk, see https://hfsm.dev.

#pragma once

#include "GameFramework/HUD.h"

#include "FightHUD.generated.h"

class UTextBlock;

class UFightHUDWidget;

UCLASS()
class FIGHT_API AFightHUD
	: public AHUD
{
	GENERATED_BODY()

public:
	void SetText(uint8 InIndex, FText InText);
	void SetColorAndOpacity(uint8 InIndex, FSlateColor InColorAndOpacity);

protected:
	virtual void BeginPlay() override final;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override final;
#endif

private:
	UPROPERTY(EditDefaultsOnly, Category="Fight|UI")
	TSubclassOf<UFightHUDWidget> HUDWidgetClass;

	UPROPERTY()
	UFightHUDWidget* HUDWidget;
};
