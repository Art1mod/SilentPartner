// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"

/** * Mapping Editor Collision Channels to Readable C++ Macros
 * Check DefaultEngine.ini to see which 'GameTraceChannel' maps to which name
 */

namespace CollisionProfiles
{
    static const FName Interactable = FName("InteractableActor");
}

