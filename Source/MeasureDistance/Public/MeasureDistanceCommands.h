// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "MeasureDistanceStyle.h"

class FMeasureDistanceCommands : public TCommands<FMeasureDistanceCommands>
{
public:

	FMeasureDistanceCommands()
		: TCommands<FMeasureDistanceCommands>(TEXT("MeasureDistance"), NSLOCTEXT("Contexts", "MeasureDistance", "MeasureDistance Plugin"), NAME_None, FMeasureDistanceStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
