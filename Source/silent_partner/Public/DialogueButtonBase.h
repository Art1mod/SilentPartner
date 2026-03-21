// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueButtonBase.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDialogueButtonClicked, int32);
/**
 * 
 */

UCLASS()
class SILENT_PARTNER_API UDialogueButtonBase : public UUserWidget
{
	GENERATED_BODY()

public:

	int32 ChoiceID;

	FOnDialogueButtonClicked OnDialogueButtonClicked;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Dialogue")
	void OnSetupButton(const FText& InLabel);

protected:
	
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void NotifyClicked();

	
	
};
