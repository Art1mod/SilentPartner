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

    UPROPERTY(EditAnywhere)
    FName QTE_ID; // The ID for QTE system to look up


    virtual void ExecuteAction_Implementation(UDialogueManagerComponent* Manager) override
    {
        // Logic to trigger via manager or global system
    }
};

