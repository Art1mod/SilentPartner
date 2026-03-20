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

    UFUNCTION(BlueprintCallable, Category = "Mesh")
    void LoadCharacterMesh();

    // must be a UFUNCTION to work with the StreamableDelegate
    UFUNCTION()
    void OnMeshLoaded();

    // Handle the overlap event
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
    TObjectPtr<class USphereComponent> CollisionComponent;


    // The name shown when hovering (e.g., "Old Safe")
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    FText DisplayName;

    // Actor's mesh component
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
    class USkeletalMeshComponent* MeshComponent;

    // Reference the asset data as a soft pointer
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    TSoftObjectPtr<USkeletalMesh> SkeletalMeshAsset;

    // What happens when the player interacts with this?
    UPROPERTY(EditAnywhere, Instanced, Category = "Interaction")
    TArray<TObjectPtr<class UDialogueAction>> InteractionActions;

    // The core function to call from your PlayerController/Character
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void Interact(class UDialogueManagerComponent* Manager);

protected:

    virtual void BeginPlay() override; 

};
