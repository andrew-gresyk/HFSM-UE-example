// Created by Andrew Gresyk, see https://hfsm.dev.

#include "FightHUDWidget.h"

#include "Components/TextBlock.h"

void UFightHUDWidget::SetText(uint8 InIndex, FText InText)
{
	if (InIndex < UE_ARRAY_COUNT(Lines))
	{
		Lines[InIndex]->SetText(InText);
	}
}

void UFightHUDWidget::SetColorAndOpacity(uint8 InIndex, FSlateColor InColorAndOpacity)
{
	if (InIndex < UE_ARRAY_COUNT(Lines))
	{
		Lines[InIndex]->SetColorAndOpacity(InColorAndOpacity);
	}
}

void UFightHUDWidget::NativeConstruct()
{
	Lines[ 0] = Line_00;
	Lines[ 1] = Line_01;
	Lines[ 2] = Line_02;
	Lines[ 3] = Line_03;
	Lines[ 4] = Line_04;
	Lines[ 5] = Line_05;
	Lines[ 6] = Line_06;
	Lines[ 7] = Line_07;
	Lines[ 8] = Line_08;
	Lines[ 9] = Line_09;
	Lines[10] = Line_10;
	Lines[11] = Line_11;
	Lines[12] = Line_12;
	Lines[13] = Line_13;
	Lines[14] = Line_14;
	Lines[15] = Line_15;
	Lines[16] = Line_16;
	Lines[17] = Line_17;
	Lines[18] = Line_18;
	Lines[19] = Line_19;
}
