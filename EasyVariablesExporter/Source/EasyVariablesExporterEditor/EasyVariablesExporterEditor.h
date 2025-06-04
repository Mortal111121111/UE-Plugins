#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FEasyVariablesExporterEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

protected:
    FDelegateHandle BlueprintVariableCustomizationHandle;
};
