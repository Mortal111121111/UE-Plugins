#include "FCustomToolBarCommands.h"

#define LOCTEXT_NAMESPACE "CustomToolBarExtend"

void FCustomToolBarCommands::RegisterCommands()
{
    UI_COMMAND( PluginAction, "Custom", "Test Test Test Test Test", EUserInterfaceActionType::Button, FInputChord() );
}

#undef LOCTEXT_NAMESPACE