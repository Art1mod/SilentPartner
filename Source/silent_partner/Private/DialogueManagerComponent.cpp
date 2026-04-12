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
	for (const FDialogueChoice& AssetChoice : CurrentNode->Choices)
	{

		// Check if the player meets the requirement
		const bool bHasRequirement = !AssetChoice.UnlockRequirement.IsValid()
			|| FoundClues.HasTag(AssetChoice.UnlockRequirement);

		if (bHasRequirement)
		{
			// We create a local copy of the choice to modify it safely
			FDialogueChoice ChoiceInstance = AssetChoice;

			if (AssetChoice.UnlockRequirement.IsValid())
			{
				// This only affects local 'AvailableChoices' array, 
				// leaving the original Data Asset clean for the next playthrough.
				ChoiceInstance.bPlayUnlockAnim = true;
			}

			AvailableChoices.Add(ChoiceInstance);
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

	// 4. Tell the UI to display the new text, buttons, and the timer
	OnDialogueTimerStarted.Broadcast(CurrentNode->TimerDuration);
	OnDialogueVisibilityChanged.Broadcast(true);
	OnDialogueUpdated.Broadcast(CurrentNode, AvailableChoices);
}

void UDialogueManagerComponent::OnChoiceSelected(int32 ChoiceIndex)
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Dialogue);
	
	// Tell the UI to reset the timer
	OnDialogueTimerFinished.Broadcast();

	if (!AvailableChoices.IsValidIndex(ChoiceIndex)) return;

	const FDialogueChoice& SelectedChoice = AvailableChoices[ChoiceIndex];

	for (UDialogueAction* Action : SelectedChoice.OnSelectedActions) 
	{
		if (Action) 
		{
			Action->ExecuteAction(this);
		}
	}
	
	// Transition
	if (!SelectedChoice.NextNode.IsNull())
	{
		StartDialogue(SelectedChoice.NextNode.LoadSynchronous());
	}
	else
	{
		// If there's no next node, the dialogue is over
		EndDialogue();
	}
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

void UDialogueManagerComponent::EndDialogueWithDelay(float Delay)
{
	// Start the timer
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle_EndDialogueWithDelay,
		this,
		&UDialogueManagerComponent::OnDelayedEndTriggered,
		Delay,
		false
	);
}

void UDialogueManagerComponent::OnDelayedEndTriggered()
{
	EndDialogue();
}

bool UDialogueManagerComponent::HasClue(FGameplayTag ClueTag) const
{
	// Safety check: if the tag isn't valid, we definitely don't have it
	if (!ClueTag.IsValid())
	{
		return false;
	}

	return FoundClues.HasTag(ClueTag);
}

void UDialogueManagerComponent::OnTimerExpired()
{
	if (!CurrentNode) return;

	// 1. Tell the HUD/Widget to hide the timer bar immediately
	OnDialogueTimerFinished.Broadcast();

	// 2. Execute Timeout Actions (e.g., "Ben thinks you're hiding something")
	for (UDialogueAction* Action : CurrentNode->OnTimeoutActions)
	{
		if (Action)
		{
			Action->ExecuteAction(this);
		}
	}

	// 3. Transition to the "Failure/Silence" Node
	if (!CurrentNode->DefaultTimeoutNode.IsNull())
	{
		UDialogueNode* TimeoutNode = CurrentNode->DefaultTimeoutNode.LoadSynchronous();
		if (TimeoutNode)
		{
			StartDialogue(TimeoutNode);
		}
	}
	else
	{
		// If no timeout node is specified, we treat silence as "End of Conversation"
		EndDialogue();
	}
}

void UDialogueManagerComponent::TriggerQTE(FName Input, float Duration)
{
	OnQTETriggered.Broadcast(Input, Duration);
}



