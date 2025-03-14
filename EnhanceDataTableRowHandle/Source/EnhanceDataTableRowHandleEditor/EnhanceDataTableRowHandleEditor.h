// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DTRowOptionNamePropertyCustomization.h"
#include "Modules/ModuleManager.h"

class FEnhanceDataTableRowHandleEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

protected:
	TSharedPtr<FDTRowOptionNamePropertyTypeIdentifier> Identifier;
	
private:
	TArray<FName> UnregisterStructNames;
	FDelegateHandle DTRowOptionNameDelegateHandle;
};
