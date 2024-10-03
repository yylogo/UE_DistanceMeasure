// Copyright (c) 2025 zhangqingyang
// All rights reserved.

#include "MeasureDistance.h"

#include "DistanceString.h"
#include "MeasureDistanceStyle.h"
#include "MeasureDistanceCommands.h"
#include "Selection.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "Elements/Framework/TypedElementQueryBuilder.h"

static const FName MeasureDistanceTabName("MeasureDistance");

#define LOCTEXT_NAMESPACE "FMeasureDistanceModule"

void FMeasureDistanceModule::StartupModule()
{
	FMeasureDistanceStyle::Initialize();
	FMeasureDistanceStyle::ReloadTextures();
	FMeasureDistanceCommands::Register();
	PluginCommands = MakeShareable(new FUICommandList);
	PluginCommands->MapAction(
		FMeasureDistanceCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FMeasureDistanceModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FMeasureDistanceModule::RegisterMenus));
}

void FMeasureDistanceModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);
	FMeasureDistanceStyle::Shutdown();
	FMeasureDistanceCommands::Unregister();
}

void FMeasureDistanceModule::PluginButtonClicked()
{
	USelection *SelectedActors = GEditor->GetSelectedActors();
	if (SelectedActors->Num() < 2)
	{
		FText DialogText = FText::Format(LOCTEXT("MeasureDistanceError", "Please select at least 2 actors to measure distance between them. Currently selected actors: {0}"), FText::AsNumber(SelectedActors->Num()));
		FMessageDialog::Open(EAppMsgType::Ok, DialogText);
		// auto PlayerController = GEngine->GetFirstLocalPlayerController(GEditor->GetPIEWorldContext()->World());
		// auto MainAvatar = PlayerController->GetPawn();
		// auto MainAvatarLocation = MainAvatar->GetActorLocation();
		// DrawDebugString(MainAvatar->GetWorld(), MainAvatarLocation, TEXT("Please select at least 2 actors to measure distance between them."), MainAvatar, FColor::Red, 20.f);
		return;
	}
	UWorld* World = SelectedActors->GetSelectedObject(0)->GetWorld();
	if(!World)
		return;
	if(!CurActor || CurActor->GetWorld() != World)
	{
		if(CurActor)
		{
			CurActor->Destroy();
		}
		CurActor = World->SpawnActor<AActor>();
		UDistanceString *DistanceString = NewObject<UDistanceString>(CurActor);
		DistanceString->RegisterComponent();
	}
	MeasureLines.Empty();
	for(int i = 0; i < SelectedActors->Num(); i++)
	{
		for(int j = i + 1; j < SelectedActors->Num(); j++)
		{
			AActor *StartActor = Cast<AActor>(SelectedActors->GetSelectedObject(i));
			AActor *EndActor = Cast<AActor>(SelectedActors->GetSelectedObject(j));
			if(!StartActor || !EndActor)
			{
				continue;
			}
			FMeasureLine MeasureLine;
			MeasureLine.StartActor = StartActor;
			MeasureLine.EndActor = EndActor;
			MeasureLines.Add(MeasureLine);
		}
	}
	if(MeasureLines.Num() == 0)
	{
		return;
	}

	TArray<FDebugText> DebugLabels;
	for(auto MeasureLine : MeasureLines)
	{
		FVector StartLocation = MeasureLine.StartActor->GetActorLocation();
		FVector EndLocation = MeasureLine.EndActor->GetActorLocation();
		if(MeasureLine.StartActor->GetWorld() != MeasureLine.EndActor->GetWorld())
		{
			continue;
		}
		DrawDebugLine(MeasureLine.StartActor->GetWorld(), StartLocation, EndLocation, FColor::Red, false, ShowTime, 100, 2.2f);
		FString DistanceText = FString::Printf(TEXT("%.1fcm"), (StartLocation - EndLocation).Size());
		FVector MidLocation = (StartLocation + EndLocation) / 2;

		FDebugSceneProxyData ProxyData;
		ProxyData.DebugLabels.Add({
			MidLocation,
			DistanceText
		});
		DebugLabels.Append(ProxyData.DebugLabels);
	}
	UDistanceString *DistanceString = Cast<UDistanceString>(CurActor->GetComponentByClass(UDistanceString::StaticClass()));
	FDebugTextDelegateHelper &DebugDrawDelegateManager = DistanceString->DebugDrawDelegateManager;
	DebugDrawDelegateManager.SetupFromProxy(DebugLabels);
	DistanceString->SetComponentTickEnabled(true);
	DistanceString->CallbackTime = ShowTime;
	DistanceString->TimCallback = [this, &DebugDrawDelegateManager, DistanceString](){
			TArray<FDebugText> TempLabels;
			DebugDrawDelegateManager.SetupFromProxy(TempLabels);
			CurActor->Destroy();
			CurActor = nullptr;
		};
}

void FMeasureDistanceModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FMeasureDistanceCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FMeasureDistanceCommands::Get().PluginAction));
				Entry.Name = "MeasureDistance";
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMeasureDistanceModule, MeasureDistance)
