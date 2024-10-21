// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class MULTI_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	AMovingPlatform();

	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	float speed = 200.f;

	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true))
	FVector targetLocation;

	UFUNCTION(BlueprintCallable)
	void AddActiveTrigger();
	UFUNCTION(CallInEditor, Category = "Editor Tools")
	void RemoveActiveTrigger();

private:
	FVector globalStartLocation;
	FVector globalTargetLocation;

	UPROPERTY(EditAnywhere)
	int activeTriggers = 1;
};
