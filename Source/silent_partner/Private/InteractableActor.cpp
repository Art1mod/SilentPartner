// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActor.h"
#include "DialogueManagerComponent.h"
//#include "silent_partner/Public/GameDialogueTypes.h"

AInteractableActor::AInteractableActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AInteractableActor::Interact(UDialogueManagerComponent* Manager)
{
    if (!Manager) return;

    // Run every action assigned to this object
    for (UDialogueAction* Action : InteractionActions)
    {
        if (Action)
        {
            Action->ExecuteAction(Manager);
        }
    }
}



