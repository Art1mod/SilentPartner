// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "silent_partner/GameDialogueTypes.h"
#include "DialogueManagerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogueUpdated, UDialogueNode*, CurrentNode, const TArray<FDialogueChoice>&, VisibleChoices);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueFinished);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SILENT_PARTNER_API UDialogueManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDialogueManagerComponent();

	// Call this to start a conversation (e.g., when the player enters a room)
	UFUNCTION(BlueprintCallable)
	void StartDialogue(UDialogueNode* NewNode);

	// Call this when the player clicks a button in your UI
	UFUNCTION(BlueprintCallable)
	void OnChoiceSelected(int32 ChoiceIndex);

	// Call this to end a conversation (e.g.,)
	UFUNCTION(BlueprintCallable)
	void EndDialogue();

	// This stores all tags/clues the player has found so far
	UPROPERTY(BlueprintReadWrite, Category = "State")
	FGameplayTagContainer FoundClues;

	// Bind your UI to this to refresh the buttons
	UPROPERTY(BlueprintAssignable, Category = "Dialogue|Events")
	FOnDialogueUpdated OnDialogueUpdated;

	// Bind your UI to notify that the dialogue is finished 
	UPROPERTY(BlueprintAssignable, Category = "Dialogue|Events")
	FOnDialogueFinished OnDialogueFinished;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	UPROPERTY()
	UDialogueNode* CurrentNode;

	UPROPERTY()
	TArray<FDialogueChoice> AvailableChoices;

	FTimerHandle TimerHandle_Dialogue;

	void OnTimerExpired();
		
};
