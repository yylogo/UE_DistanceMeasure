// Copyright (c) 2025 zhangqingyang
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "DistanceString.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

struct FMeasureLine
{
	AActor* StartActor;
	AActor* EndActor;
};

class FMeasureDistanceModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command. */
	void PluginButtonClicked();

	void RegisterMenus();

private:
	TSharedPtr<class FUICommandList> PluginCommands;
	TArray<FMeasureLine> MeasureLines;
	float ShowTime = 4.f;
	FTimerHandle TimerHandle;
	UPROPERTY()
	AActor* CurActor = nullptr;
};
