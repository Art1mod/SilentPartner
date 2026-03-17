// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActor.h"
#include "DialogueManagerComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "silent_partner/silent_partner.h"
#include "silent_partner/Public/SilentPartnerInteracter.h"

AInteractableActor::AInteractableActor()
{
	PrimaryActorTick.bCanEverTick = false;

    // Setup Collision first as the Root
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    RootComponent = CollisionComponent;

    // Set the Modern Collision Profile
    // "InteractableActor" is a preset defined in Project Settings > Collision > Presets
    CollisionComponent->SetCollisionProfileName(CollisionProfiles::Interactable);
    CollisionComponent->SetGenerateOverlapEvents(true);
    CollisionComponent->SetComponentTickEnabled(false); // Collisions don't need Tick to detect overlaps

    // Setup the Mesh and attach it to the Collision Root
    MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
    MeshComponent->SetupAttachment(RootComponent);

    // Disable Tick on the mese; don't have any procedural animations
    MeshComponent->SetComponentTickEnabled(false);


}

void AInteractableActor::BeginPlay()
{
    Super::BeginPlay();

    // Bind the collision component to OnOverlapBegin
    if (CollisionComponent)
    {
        CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AInteractableActor::OnOverlapBegin);
    }
}

void AInteractableActor::LoadCharacterMesh()
{
    if (SkeletalMeshAsset.IsNull()) return;

    // Async load the mesh using the Asset Manager / Streamable Manager
    TSharedPtr<FStreamableHandle> Handle = UAssetManager::GetStreamableManager().RequestAsyncLoad(
        SkeletalMeshAsset.ToSoftObjectPath(),
        FStreamableDelegate::CreateUObject(this, &AInteractableActor::OnMeshLoaded)
    );
}

void AInteractableActor::OnMeshLoaded()
{
    // Get() works now because the asset is fully loaded in RAM
    if (USkeletalMesh* LoadedMesh = SkeletalMeshAsset.Get())
    {
        MeshComponent->SetSkeletalMesh(LoadedMesh);
        UE_LOG(LogTemp, Log, TEXT("OnMeshLoaded: %s"), *DisplayName.ToString());
    }
}

void AInteractableActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->Implements<USilentPartnerInteracter>())
    {
        if (UDialogueManagerComponent* Manager = ISilentPartnerInteracter::Execute_GetDialogueManager(OtherActor))
        {
            UE_LOG(LogTemp, Log, TEXT("Overlap Success: %s found Manager on %s"), *GetName(), *OtherActor->GetName());
            Interact(Manager);
        }
    }
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
        else
        {
            UE_LOG(LogTemp, Error, TEXT("%s has a NULL action in its InteractionActions array!"), *GetName());
        }
    }
}





