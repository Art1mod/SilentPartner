#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "DialogueDataRow.generated.h"

USTRUCT()
struct FDialogueDataRow : public FTableRowBase
{
	GENERATED_BODY()
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText DialogueText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGameplayTag SpeakerTag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<USoundBase> VoiceLine;
};