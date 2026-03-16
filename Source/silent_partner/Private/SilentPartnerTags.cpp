#include "SilentPartnerTags.h"

// Allocate the static instance
FSPTags FSPTags::Tags;

void FSPTags::InitializeNativeTags()
{
    UGameplayTagsManager& Manager = UGameplayTagsManager::Get();

    Tags.Intel_Pictures = Manager.AddNativeGameplayTag(TEXT("Intel.Pictures"), TEXT("Intel gathered from office photos"));
    Tags.Intel_Sabotage = Manager.AddNativeGameplayTag(TEXT("Intel.Sabotage"), TEXT("Overheard employee sabotage plot"));
    Tags.Evidence_Romance = Manager.AddNativeGameplayTag(TEXT("Evidence.Romance"), TEXT("Evidence of the boss's affair"));
    Tags.Evidence_MoneyLaundering = Manager.AddNativeGameplayTag(TEXT("Evidence.MoneyLaundering"), TEXT("Financial records of money laundering"));

    Tags.Item_Gun = Manager.AddNativeGameplayTag(TEXT("Item.Gun"), TEXT("Player has a firearm"));
    Tags.Item_Camera = Manager.AddNativeGameplayTag(TEXT("Item.Camera"), TEXT("Player has a hidden camera"));
}
