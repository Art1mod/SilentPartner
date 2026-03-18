// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueManagerComponent.h"
#include "silent_partner/Public/DialogueDataRow.h"

#include "TimerManager.h"


UDialogueManagerComponent::UDialogueManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDialogueManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UDialogueManagerComponent::StartDialogue(UDialogueNode* NewNode)
{
	if (!NewNode) return;

	CurrentNode = NewNode;
	
	// 1. Fetch the Text from the Data Table Row Handle
	FText DisplayText;
	if (const FDialogueDataRow* Row = CurrentNode->DialogueRow.GetRow<FDialogueDataRow>(TEXT("DialogueContext")))
	{
		DisplayText = Row->DialogueText;
	}
	else
	{
		DisplayText = FText::FromString("Error: Row Not Found");
	}



	AvailableChoices.Empty();

	// 2. Filter choices: Only show what the player "knows"
	for (const FDialogueChoice& Choice: CurrentNode->Choices) 
	{
		if (!Choice.UnlockRequirement.IsValid() || FoundClues.HasTag(Choice.UnlockRequirement))
		{
			AvailableChoices.Add(Choice);
		}
	}

	// 3. Start the timer
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle_Dialogue,
		this,
		&UDialogueManagerComponent::OnTimerExpired,
		CurrentNode->TimerDuration,
		false
	);

	// 4. Tell the UI to display the new text and buttons
	OnDialogueVisibilityChanged.Broadcast(true);
	OnDialogueUpdated.Broadcast(CurrentNode, AvailableChoices);
}

void UDialogueManagerComponent::OnChoiceSelected(int32 ChoiceIndex)
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Dialogue);

	if (!AvailableChoices.IsValidIndex(ChoiceIndex)) return;

	const FDialogueChoice& SelectedChoice = AvailableChoices[ChoiceIndex];

	for (UDialogueAction* Action : SelectedChoice.OnSelectedActions) 
	{
		if (Action) 
		{
			Action->ExecuteAction(this);
		}
	}
	
	if (!SelectedChoice.NextNode.IsNull()) 
	{
		// Load and move to the next part of the story
		StartDialogue(SelectedChoice.NextNode.LoadSynchronous());
	}
	
	// If NextNode is null and an "End" action was fired, the dialogue simply stops
}

void UDialogueManagerComponent::EndDialogue()
{
	CurrentNode = nullptr;
	AvailableChoices.Empty();
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Dialogue);
	
	// Broadcast to UI to hide itself 
	OnDialogueVisibilityChanged.Broadcast(false);
	OnDialogueFinished.Broadcast();

}

void UDialogueManagerComponent::OnTimerExpired()
{
	if (CurrentNode && CurrentNode->DefaultTimeoutNode.IsValid()) 
	{
		// Force the "Negative/Silent" outcome
		UDialogueNode* DefaultTimeoutNode = CurrentNode->DefaultTimeoutNode.LoadSynchronous();
		StartDialogue(DefaultTimeoutNode);
	}
}

void UDialogueManagerComponent::TriggerQTE(FName Input, float Duration)
{
	OnQTETriggered.Broadcast(Input, Duration);
}



