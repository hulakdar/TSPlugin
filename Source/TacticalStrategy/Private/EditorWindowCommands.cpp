// Copyright Epic Games, Inc. All Rights Reserved.

#include "EditorWindowCommands.h"

#define LOCTEXT_NAMESPACE "FEditorWindowModule"

void FEditorWindowCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "EditorWindow", "Bring up EditorWindow window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
