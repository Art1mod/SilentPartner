// Fill out your copyright notice in the Description page of Project Settings.

#include "SilentPartnerAssetManager.h"
#include "SilentPartnerTags.h"

void USilentPartnerAssetManager::StartInitialLoading()
{
    Super::StartInitialLoading();

    // Initialize our native tags here
    FSPTags::InitializeNativeTags();

}