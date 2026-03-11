#pragma once

#include "Public/DialogueManagerComponent.h"
#include "silent_partner/GameDialogueTypes.h"
#include "GameDialogueActions.generated.h"

// END DIALOGUE
UCLASS()
class UAction_EndDialogue : public UDialogueAction
{
    GENERATED_BODY()

public:
    virtual void ExecuteAction_Implementation(UDialogueManagerComponent* Manager) override
    {
        Manager->EndDialogue();
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

