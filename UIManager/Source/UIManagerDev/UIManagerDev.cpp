#include "UIManagerDev.h"

#include "EdGraphUtilities.h"
#include "K2Node/CreateUIGraphPanelPinFactory.h"

#define LOCTEXT_NAMESPACE "FUIManagerDevModule"

void FUIManagerDevModule::StartupModule()
{
    const TSharedPtr<FCreateUIGraphPanelPinFactory> BlueprintGraphPanelPinFactory = MakeShareable( new FCreateUIGraphPanelPinFactory() );
    FEdGraphUtilities::RegisterVisualPinFactory( BlueprintGraphPanelPinFactory );
}

void FUIManagerDevModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE( FUIManagerDevModule, UIManagerDev )