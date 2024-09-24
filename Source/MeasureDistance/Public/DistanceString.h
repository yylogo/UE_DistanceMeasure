// Copyright (c) 2025 zhangqingyang
// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Debug/DebugDrawComponent.h"
#include "DistanceString.generated.h"


struct FDebugText
{
	FVector Location;
	FString Text;

	FDebugText() {}
	FDebugText(const FVector& InLocation, const FString& InText) : Location(InLocation), Text(InText){}
};


struct FDebugSceneProxyData
{
	TArray<FDebugText> DebugLabels;
};

class FDebugSceneProxy : public FDebugRenderSceneProxy
{
public:
	FDebugSceneProxy(const UPrimitiveComponent* InComponent, FDebugSceneProxyData* ProxyData);

	FDebugSceneProxyData ProxyData;
};

class FDebugTextDelegateHelper : public FDebugDrawDelegateHelper
{
public:
	virtual void DrawDebugLabels(UCanvas* Canvas, APlayerController*) override;
	void SetupFromProxy(const TArray<FDebugText> &Labels);
	TArray<FDebugText> DebugLabels;
	FDebugRenderSceneProxy* DebugSceneProxy;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MEASUREDISTANCE_API UDistanceString : public UDebugDrawComponent
{
	GENERATED_BODY()
public:
	UDistanceString(const FObjectInitializer& ObjectInitializer);
	virtual FDebugRenderSceneProxy* CreateDebugSceneProxy() override;
	virtual FDebugDrawDelegateHelper& GetDebugDrawDelegateHelper() override { return DebugDrawDelegateManager; }
	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float CallbackTime = 2.0f;
	TFunction<void()> TimCallback;
	FDebugTextDelegateHelper DebugDrawDelegateManager;
};
