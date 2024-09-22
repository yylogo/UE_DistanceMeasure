// Copyright Epic Games, Inc. All Rights Reserved.

#include "MeasureDistanceCommands.h"

#define LOCTEXT_NAMESPACE "FMeasureDistanceModule"

void FMeasureDistanceCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "MeasureDistance", "Execute MeasureDistance action", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
