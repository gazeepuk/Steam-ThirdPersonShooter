// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DisplayTextWidget.h"

#include "Components/TextBlock.h"

void UDisplayTextWidget::SetDisplayText(const FText& Text) const
{
	DisplayTextBlock->SetText(Text);
}

void UDisplayTextWidget::NativeDestruct()
{
	RemoveFromParent();
	Super::NativeDestruct();
}
