// Created by Andrew Gresyk, see https://hfsm.dev.

#include "FightHUD.h"

#include "FightHUDWidget.h"

void AFightHUD::SetText(uint8 InIndex, FText InText)
{
	if (HUDWidget)
	{
		HUDWidget->SetText(InIndex, InText);
	}
}

void AFightHUD::SetColorAndOpacity(uint8 InIndex, FSlateColor InColorAndOpacity)
{
	if (HUDWidget)
	{
		HUDWidget->SetColorAndOpacity(InIndex, InColorAndOpacity);
	}
}

void AFightHUD::BeginPlay()
{
	Super::BeginPlay();

	if (HUDWidgetClass)
	{
		if (UFightHUDWidget* Widget = CreateWidget<UFightHUDWidget>(GetWorld(), HUDWidgetClass))
		{
			HUDWidget = Widget;
			HUDWidget->AddToViewport();
		}
	}
}

#if WITH_EDITOR

EDataValidationResult AFightHUD::IsDataValid(TArray<FText>& ValidationErrors)
{
	EDataValidationResult SuperResult = Super::IsDataValid(ValidationErrors);
	EDataValidationResult Result = EDataValidationResult::Valid;

	if (!IsValid(HUDWidgetClass))
	{
		Result = EDataValidationResult::Invalid;
		ValidationErrors.Add(FText::FromString(TEXT("HUDWidgetClass unset.")));
	}

	return CombineDataValidationResults(SuperResult, Result);
}

#endif
