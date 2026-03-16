#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "silent_partner/Public/DialogueDataRow.h"
#include "GameDialogueTypes.generated.h"

//Base class for Dialogue Actions 
UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class SILENT_PARTNER_API UDialogueAction: public UObject
{
    GENERATED_BODY()

public:
    // Logic to execute (QTE, End Dialogue, Give Item, etc)
    UFUNCTION(BlueprintNativeEvent, Category = "Dialogue")
    void ExecuteAction(class UDialogueManagerComponent* Manager);
    virtual void ExecuteAction_Implementation(class UDialogueManagerComponent* Manager) {}

};


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

    
    //The writer can add "End Dialogue", "Start QTE", etc. here
    UPROPERTY(EditAnywhere, Instanced, Category = "Actions")
    TArray<TObjectPtr<UDialogueAction>> OnSelectedActions;

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
    
    // This makes the writing "Code-Change-Proof."
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Writing")
    FDataTableRowHandle DialogueRow;

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

    UPROPERTY(EditAnywhere, Instanced, Category = "Actions")
    TArray<TObjectPtr<UDialogueAction>> OnTimeoutActions;

public: 
    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    FText GetNodeText() const;
};



