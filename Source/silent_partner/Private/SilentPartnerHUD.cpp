// Fill out your copyright notice in the Description page of Project Settings.


#include "SilentPartnerHUD.h"
#include "silent_partner/Public/DialogueWidgetBase.h"
#include "silent_partner/Public/DialogueManagerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "silent_partner/Public/SilentPartnerInteracter.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

void ASilentPartnerHUD::BeginPlay()
{
	Super::BeginPlay();

    if (DialogueWidgetClass) 
    {
        DialogueWidgetInstance = CreateWidget<UDialogueWidgetBase>(GetOwningPlayerController(), DialogueWidgetClass);

        if (DialogueWidgetInstance) 
        {
            DialogueWidgetInstance->AddToViewport();
            DialogueWidgetInstance->SetVisibility(ESlateVisibility::Collapsed);

            DialogueWidgetInstance->OnChoiceSelected.AddDynamic(this, &ASilentPartnerHUD::HandleOnChoiceSelected);
        }
    }

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    // Check if the Pawn implements the interface
    if (PlayerPawn && PlayerPawn->GetClass()->ImplementsInterface(USilentPartnerInteracter::StaticClass()))
    {
        // Execute the interface function to get the manager
        UDialogueManagerComponent* Manager = ISilentPartnerInteracter::Execute_GetDialogueManager(PlayerPawn);

        if (Manager)
        {
            Manager->OnDialogueVisibilityChanged.AddDynamic(this, &ASilentPartnerHUD::HandleVisibilityChange);

            // Binds  the data update the text in the text block of the dialogue widget!
            Manager->OnDialogueUpdated.AddDynamic(this, &ASilentPartnerHUD::RefreshDialogueUI);
        }
    }

}

void ASilentPartnerHUD::HandleVisibilityChange(bool bIsVisible)
{
    if (!DialogueWidgetInstance) return;

    APlayerController* PC = GetOwningPlayerController();
    if (!PC) return;

    if (bIsVisible)
    {
        DialogueWidgetInstance->SetVisibility(ESlateVisibility::Visible);
        DialogueWidgetInstance->PlayTransition(true); // TODO: Trigger BP Animation

        FInputModeGameAndUI InputMode;
        InputMode.SetWidgetToFocus(DialogueWidgetInstance->TakeWidget());
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

        PC->SetInputMode(InputMode);
        PC->bShowMouseCursor = true;
    }
    else
    {
        DialogueWidgetInstance->PlayTransition(false); // TODO: Trigger BP Fade Out
        DialogueWidgetInstance->SetVisibility(ESlateVisibility::Collapsed);
        //TODO: a timer to set to Collapsed after the fade animation finishes

        FInputModeGameOnly InputMode;
        PC->SetInputMode(InputMode);
        PC->bShowMouseCursor = false;

        // Give focus back to the game world
        FSlateApplication::Get().SetAllUserFocusToGameViewport();
    }
}

void ASilentPartnerHUD::HandleOnChoiceSelected(int32 ChoiceID)
{
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    // Check if the Pawn implements the interface
    if (PlayerPawn && PlayerPawn->GetClass()->ImplementsInterface(USilentPartnerInteracter::StaticClass()))
    {
        // Execute the interface function to get the manager
        UDialogueManagerComponent* Manager = ISilentPartnerInteracter::Execute_GetDialogueManager(PlayerPawn);

        if (Manager)
        {
            Manager->OnChoiceSelected(ChoiceID);
        }
    }
}

void ASilentPartnerHUD::RefreshDialogueUI(UDialogueNode* CurrentNode, const TArray<FDialogueChoice>& VisibleChoices)
{
    if (!DialogueWidgetInstance || !CurrentNode) return;

    // 1. Set the NPC Text
    if (DialogueWidgetInstance->DialogueText)
    {
        // We use the helper function we wrote in the DialogueNode class earlier
        DialogueWidgetInstance->DialogueText->SetText(CurrentNode->GetNodeText());
    }

    // 2. Update choices using the pool
    DialogueWidgetInstance->UpdateChoices(VisibleChoices);
}
