// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "silent_partner/Public/SilentPartnerInteracter.h"
#include "SilentPartnerCharacter.generated.h"

UCLASS()
class SILENT_PARTNER_API ASilentPartnerCharacter : public ACharacter, public ISilentPartnerInteracter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASilentPartnerCharacter();

	// This is the C++ side of the BlueprintNativeEvent
	virtual UDialogueManagerComponent* GetDialogueManager_Implementation() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	

	// --- COMPONENTS ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogue", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDialogueManagerComponent> DialogueManager;



};
