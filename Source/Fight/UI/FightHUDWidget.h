// Created by Andrew Gresyk, see https://hfsm.dev.

#pragma once

#include "Blueprint/UserWidget.h"

#include "FightHUDWidget.generated.h"

class UTextBlock;

UCLASS()
class FIGHT_API UFightHUDWidget
	: public UUserWidget
{
	GENERATED_BODY()

public:
	void SetText(uint8 InIndex, FText InText);
	void SetColorAndOpacity(uint8 InIndex, FSlateColor InColorAndOpacity);

protected:
	virtual void NativeConstruct() override final;

private:
	UPROPERTY(EditAnywhere, Category="Fight|UI", meta=(BindWidget)) UTextBlock* Line_00;
	UPROPERTY(EditAnywhere, Category="Fight|UI", meta=(BindWidget)) UTextBlock* Line_01;
	UPROPERTY(EditAnywhere, Category="Fight|UI", meta=(BindWidget)) UTextBlock* Line_02;
	UPROPERTY(EditAnywhere, Category="Fight|UI", meta=(BindWidget)) UTextBlock* Line_03;
	UPROPERTY(EditAnywhere, Category="Fight|UI", meta=(BindWidget)) UTextBlock* Line_04;
	UPROPERTY(EditAnywhere, Category="Fight|UI", meta=(BindWidget)) UTextBlock* Line_05;
	UPROPERTY(EditAnywhere, Category="Fight|UI", meta=(BindWidget)) UTextBlock* Line_06;
	UPROPERTY(EditAnywhere, Category="Fight|UI", meta=(BindWidget)) UTextBlock* Line_07;
	UPROPERTY(EditAnywhere, Category="Fight|UI", meta=(BindWidget)) UTextBlock* Line_08;
	UPROPERTY(EditAnywhere, Category="Fight|UI", meta=(BindWidget)) UTextBlock* Line_09;
	UPROPERTY(EditAnywhere, Category="Fight|UI", meta=(BindWidget)) UTextBlock* Line_10;
	UPROPERTY(EditAnywhere, Category="Fight|UI", meta=(BindWidget)) UTextBlock* Line_11;
	UPROPERTY(EditAnywhere, Category="Fight|UI", meta=(BindWidget)) UTextBlock* Line_12;
	UPROPERTY(EditAnywhere, Category="Fight|UI", meta=(BindWidget)) UTextBlock* Line_13;
	UPROPERTY(EditAnywhere, Category="Fight|UI", meta=(BindWidget)) UTextBlock* Line_14;
	UPROPERTY(EditAnywhere, Category="Fight|UI", meta=(BindWidget)) UTextBlock* Line_15;
	UPROPERTY(EditAnywhere, Category="Fight|UI", meta=(BindWidget)) UTextBlock* Line_16;
	UPROPERTY(EditAnywhere, Category="Fight|UI", meta=(BindWidget)) UTextBlock* Line_17;
	UPROPERTY(EditAnywhere, Category="Fight|UI", meta=(BindWidget)) UTextBlock* Line_18;
	UPROPERTY(EditAnywhere, Category="Fight|UI", meta=(BindWidget)) UTextBlock* Line_19;

	UTextBlock* Lines[20];
};
