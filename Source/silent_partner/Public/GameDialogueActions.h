#pragma once

#include "silent_partner/Public/DialogueManagerComponent.h"
#include "silent_partner/Public/GameDialogueTypes.h"
#include "GameDialogueActions.generated.h"

// START DIALOGUE
UCLASS()
class UAction_StartDialogue : public UDialogueAction
{
    GENERATED_BODY()

public:

    /** The specific dialogue tree to begin when this action fires */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    TSoftObjectPtr<class UDialogueNode> DialogueToStart;

    virtual void ExecuteAction_Implementation(UDialogueManagerComponent* Manager) override
    {
        if (Manager && !DialogueToStart.IsNull())
        {
            UDialogueNode* Node = DialogueToStart.Get() ? DialogueToStart.Get() : DialogueToStart.LoadSynchronous();
            if (Node) Manager->StartDialogue(Node);
        }
    }
};

// END DIALOGUE
UCLASS()
class UAction_EndDialogue : public UDialogueAction
{
    GENERATED_BODY()

public:

    // If 0, it ends instantly. If > 0, it waits.
    UPROPERTY(EditAnywhere, Category = "Dialogue", meta = (ClampMin = "0.0"))
    float Delay = 0.0f;

    virtual void ExecuteAction_Implementation(UDialogueManagerComponent* Manager) override
    {
        if (!Manager) return;

        if (Delay <= 0.0f)
        {
            Manager->EndDialogue();
        }
        else
        {
            Manager->EndDialogueWithDelay(Delay);
        }
    }
};

// START QTE
UCLASS()
class UAction_StartQTE : public UDialogueAction
{
    GENERATED_BODY()

public:
    
    UPROPERTY(EditAnywhere, Category = "QTE")
    FName RequiredInput;

    UPROPERTY(EditAnywhere, Category = "QTE")
    float TimeLimit = 1.5f;

    virtual void ExecuteAction_Implementation(UDialogueManagerComponent* Manager) override
    {
        if (Manager) Manager->TriggerQTE(RequiredInput, TimeLimit);
    }
};

// UNLOCK DIALOGUE PATH
UCLASS()
class SILENT_PARTNER_API UAction_AddClue : public UDialogueAction
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category = "Intel")
    FGameplayTag ClueToGrant;

    virtual void ExecuteAction_Implementation(UDialogueManagerComponent* Manager) override
    {
        if (Manager)
        {
            if (!Manager->HasClue(ClueToGrant))
            {
                Manager->FoundClues.AddTag(ClueToGrant);
                // TODO: Broadcast a notification to the HUD
                UE_LOG(LogTemp, Log, TEXT("Added Clue: %s"), *ClueToGrant.ToString());
            }
            else 
            {
                UE_LOG(LogTemp, Log, TEXT("Already has a clue: %s"), *ClueToGrant.ToString());
            }
        }
    }
};

