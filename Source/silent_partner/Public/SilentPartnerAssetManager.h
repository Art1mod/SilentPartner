// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "SilentPartnerAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class SILENT_PARTNER_API USilentPartnerAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	virtual void StartInitialLoading() override;
	
};
