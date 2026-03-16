// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SilentPartnerInteracter.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USilentPartnerInteracter : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SILENT_PARTNER_API ISilentPartnerInteracter
{
	GENERATED_BODY()

public:

	// This function allows the object to "Ask" the player for the manager
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	class UDialogueManagerComponent* GetDialogueManager();
};
