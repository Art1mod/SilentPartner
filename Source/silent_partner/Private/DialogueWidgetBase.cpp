// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueWidgetBase.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
//#include "Blueprint/UserWidget.h"
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

