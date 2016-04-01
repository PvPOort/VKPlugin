// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "VKProjectSettings.generated.h"

UCLASS(Config=Engine, DefaultConfig)
class UVKProjectSettings : public UObject
{
    GENERATED_UCLASS_BODY()
    
public:

	UPROPERTY(Config, EditAnywhere, Category = "Initialization")
	TArray<FString> Scopes;
    
    UPROPERTY(Config, EditAnywhere, Category="Login")
	TArray<FString> LoginPermissions;
};