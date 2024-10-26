// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DisplayTextWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class STEAMTPS_API UDisplayTextWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetDisplayText(const FText& Text) const;
	
protected:
	virtual void NativeDestruct() override;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> DisplayTextBlock;
};
