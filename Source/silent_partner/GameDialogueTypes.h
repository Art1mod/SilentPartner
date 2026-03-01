#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "GameDialogueTypes.generated.h"

//This is a helper structure inside your Data Asset. It represents a single "button" the player can press.
USTRUCT(BlueprintType)
struct FDialogueChoice
{
	GENERATED_BODY()

    // The text shown on the holographic button (e.g., "Reason with Ben")
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ChoiceLabel;

    // The next node to jump to if this is selected
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<class UDialogueNode> NextNode;

    // The "Unlock" requirement (Leave empty for always available)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag UnlockRequirement; 

    // Tactical impact: How much this choice affects Ben's stress
    //UPROPERTY(EditAnywhere, BlueprintReadWrite)
    //float StressModifier = 0.0f;
	
};

//Think of each Node as a single "page" in a script.
UCLASS(BlueprintType)
class UDialogueNode : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    
    // What the NPC says (The subtitle text)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content")
    FText NPCLine;

    // What the NPC says (a voice line)
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content")
    //USoundBase* VoiceLine;

    // A list of choices available to the player at this moment
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Logic")
    TArray<FDialogueChoice> Choices;

    // How long the player has to choose (Detroit-style timer)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Logic")
    float TimerDuration = 10.0f;

    // The node to trigger if the timer runs out 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Logic")
    TSoftObjectPtr<UDialogueNode> DefaultTimeoutNode;
};



