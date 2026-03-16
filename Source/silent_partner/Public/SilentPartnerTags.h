#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"

/**
 * Singleton struct containing native Gameplay Tags
 */
struct FSPTags
{
public:
    static const FSPTags& Get() { return Tags; }

    //Intel Tags
    FGameplayTag Intel_Pictures;
    FGameplayTag Intel_Sabotage;
    FGameplayTag Evidence_Romance;
    FGameplayTag Evidence_MoneyLaundering;

    //Items
    FGameplayTag Item_Gun;
    FGameplayTag Item_Camera;

    // Initialize tags with the Manager
    static void InitializeNativeTags();

private:
    // Only the 'Tags' instance itself is private to ensure 
    // nobody can "overwrite" the whole list by accident.
    static FSPTags Tags;
};
