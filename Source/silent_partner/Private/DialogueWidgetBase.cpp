// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueWidgetBase.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/ProgressBar.h"
#include "silent_partner/Public/DialogueButtonBase.h"
#include "silent_partner/Public/GameDialogueTypes.h"

void UDialogueWidgetBase::UpdateChoices(const TArray<FDialogueChoice>& Choices)
{
    if (!ChoiceContainer || !ChoiceButtonClass) return;

    int32 NeededCount = Choices.Num();

    // 1. If we don't have enough buttons, expand in chunks of 4
    if (NeededCount > ButtonPool.Num())
    {
        // Logic: Round up to the next multiple of 4
        int32 NewCapacity = ((NeededCount + PoolChunkSize - 1) / PoolChunkSize) * PoolChunkSize;
        ExpandPool(NewCapacity);
    }

    // 2. Iterate through the entire pool
    for (int32 i = 0; i < ButtonPool.Num(); ++i)
    {
        UDialogueButtonBase* Button = ButtonPool[i];
        if (!Button) continue;

        if (i < NeededCount)
        {
            Button->SetVisibility(ESlateVisibility::Visible);
            Button->ChoiceID = i;
            Button->OnSetupButton(Choices[i].ChoiceLabel);
        }
        else
        {
            Button->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
}

void UDialogueWidgetBase::StartVisualTimer(float Duration)
{
    if (!ChoiceTimerBar) return;

    TotalTimerDuration = Duration;
    ElapsedTimerTime = 0.0f;
    bIsTimerActive = (Duration > 0.0f);

    ChoiceTimerBar->SetVisibility(bIsTimerActive ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UDialogueWidgetBase::StopVisualTimer()
{
    bIsTimerActive = false;
    ElapsedTimerTime = 0.0f;

    if (ChoiceTimerBar)
    {
        //Hide the bar 
        ChoiceTimerBar->SetVisibility(ESlateVisibility::Collapsed);

        // Reset colors/opacity so it's fresh for the next timer
        ChoiceTimerBar->SetFillColorAndOpacity(FLinearColor::White);
        ChoiceTimerBar->SetRenderOpacity(1.0f);
    }
}

void UDialogueWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    if (bIsTimerActive && ChoiceTimerBar)
    {
        ElapsedTimerTime += InDeltaTime;
        float Percent = FMath::Clamp(1.0f - (ElapsedTimerTime / TotalTimerDuration), 0.0f, 1.0f);

        ChoiceTimerBar->SetPercent(Percent);

        // If less than 25% time remains, turn the bar Red and maybe make it pulse
        if (Percent < 0.25f)
        {
            ChoiceTimerBar->SetFillColorAndOpacity(FLinearColor::Red);

            // Subtle pulse effect using a Sine wave
            float Pulse = FMath::Sin(GetWorld()->GetTimeSeconds() * 15.0f) * 0.2f + 0.8f;
            ChoiceTimerBar->SetRenderOpacity(Pulse);
        }
        else
        {
            ChoiceTimerBar->SetFillColorAndOpacity(FLinearColor::White);
            ChoiceTimerBar->SetRenderOpacity(1.0f);
        }

        if (Percent <= 0.0f) bIsTimerActive = false;
    }
}

void UDialogueWidgetBase::ExpandPool(int32 NewSize)
{
	int32 AmountToSpawn = NewSize - ButtonPool.Num();

	for (int32 i=0; i< AmountToSpawn; ++i)
	{
        UDialogueButtonBase* NewButton = CreateWidget<UDialogueButtonBase>(GetOwningPlayer(), ChoiceButtonClass);
		if (NewButton) 
		{
			ButtonPool.Add(NewButton);
			ChoiceContainer->AddChild(NewButton);
            NewButton->OnDialogueButtonClicked.AddUObject(this, &UDialogueWidgetBase::HandlePooledButtonClicked);
			NewButton->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Dialogue Pool expanded to %d buttons."), ButtonPool.Num());
}

void UDialogueWidgetBase::HandlePooledButtonClicked(int32 ChoiceID)
{
    OnChoiceSelected.Broadcast(ChoiceID);
}

