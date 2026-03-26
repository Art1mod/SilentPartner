// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SilentPartnerHUD.generated.h"

/**
 * 
 */
UCLASS()
class SILENT_PARTNER_API ASilentPartnerHUD : public AHUD
{
	GENERATED_BODY()

public:
    // Assign Blueprint Widget in the Editor
    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<class UDialogueWidgetBase> DialogueWidgetClass;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    TObjectPtr<class UDialogueWidgetBase> DialogueWidgetInstance;

    // The function that reacts to the delegate and toggles visibility of the dialogue widget
    UFUNCTION()
    void HandleVisibilityChange(bool bIsVisible);

    // The function that reacts to a player chosing a dialogue option
    UFUNCTION()
    void HandleOnChoiceSelected(int32 ChoiceID);
    
    UFUNCTION()
    void RefreshDialogueUI(UDialogueNode* CurrentNode, const TArray<FDialogueChoice>& VisibleChoices);

    // The function that reacts to a player chosing a dialogue option
    UFUNCTION()
    void HandleTimerStarted(float Duration);

    UFUNCTION()
    void HandleTimerFinished();
};
