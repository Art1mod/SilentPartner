// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueWidgetBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChoiceSelected, int32, ChoiceID);

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

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* ChoiceTimerBar;

    // A delegate to notify of selected choice
    UPROPERTY(BlueprintCallable, Category = "Dialogue|Events")
    FOnChoiceSelected OnChoiceSelected;

    // A Blueprint event to trigger animations (Fade in/out)
    UFUNCTION(BlueprintImplementableEvent, Category = "UI")
    void PlayTransition(bool bShow);

    // This event will be implemented in WBP_DialogueUI
    UFUNCTION(BlueprintImplementableEvent, Category = "UI")
    void AddChoiceButtonToWidget(int32 ChoiceIndex, const FText& ButtonText);

    UFUNCTION(Category = "UI")
    void UpdateChoices(const TArray<FDialogueChoice>& Choices);

    UFUNCTION()
    void StartVisualTimer(float Duration);

    UFUNCTION()
    void StopVisualTimer();

protected:
    UPROPERTY(EditAnywhere, Category = "Dialogue")
    TSubclassOf<class UDialogueButtonBase> ChoiceButtonClass;

    UPROPERTY(EditAnywhere, Category = "Dialogue|Setup")
    int32 PoolChunkSize = 4;

    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:

    float TotalTimerDuration = 0.0f;
    float ElapsedTimerTime = 0.0f;
    bool bIsTimerActive = false;

    // The actual "Pool" of widgets
    UPROPERTY()
    TArray<TObjectPtr<UDialogueButtonBase>> ButtonPool;

    // Helper to grow the pool in chunks
    void ExpandPool(int32 NewSize);

    // The handler that catches the native button click
    void HandlePooledButtonClicked(int32 ChoiceID);
};
