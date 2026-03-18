// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class SILENT_PARTNER_API UDialogueWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
    // Should match Blueprints variable name
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* DialogueText;

    // Should match Blueprints variable name
    UPROPERTY(meta = (BindWidget))
    class UVerticalBox* ChoiceContainer;

    // A Blueprint event to trigger animations (Fade in/out)
    UFUNCTION(BlueprintImplementableEvent, Category = "UI")
    void PlayTransition(bool bShow);

    // This event will be implemented in WBP_DialogueUI
    UFUNCTION(BlueprintImplementableEvent, Category = "UI")
    void AddChoiceButtonToWidget(int32 ChoiceIndex, const FText& ButtonText);
	
};
