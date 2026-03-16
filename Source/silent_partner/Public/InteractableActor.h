// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableActor.generated.h"

UCLASS()
class SILENT_PARTNER_API AInteractableActor : public AActor
{
	GENERATED_BODY()
	
public:
    AInteractableActor();

    // The name shown when hovering (e.g., "Old Safe")
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    FText DisplayName;

    // What happens when the player interacts with this?
    UPROPERTY(EditAnywhere, Instanced, Category = "Interaction")
    TArray<TObjectPtr<class UDialogueAction>> InteractionActions;

    // The core function to call from your PlayerController/Character
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void Interact(class UDialogueManagerComponent* Manager);

};
