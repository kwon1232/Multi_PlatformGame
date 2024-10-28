// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformTrigger.h"
#include "PuzzlePlatformGameInstance.h"

#include "Blueprint/UserWidget.h"

#include "PlatformTrigger.h"


UPuzzlePlatformGameInstance::UPuzzlePlatformGameInstance(const FObjectInitializer& objectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MainBPClass(TEXT("/Game/_Main/MenuSystem/WBP_MainMenu"));
	if (!ensure(MainBPClass.Class != nullptr)) return;
	
	menuClass = MainBPClass.Class;
}

void UPuzzlePlatformGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *menuClass->GetName());
}

void UPuzzlePlatformGameInstance::LoadMenu()
{
	if (!ensure(menuClass != nullptr)) return;

	UUserWidget* menu = CreateWidget<UUserWidget>(this, menuClass);

	if (!ensure(menu != nullptr)) return;

	menu->AddToViewport();
}

void UPuzzlePlatformGameInstance::Host()
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));
	
	UWorld* World = GetWorld();
	if (ensure(World != nullptr)) return;

	World->ServerTravel("/Game/ThirdPerson/Maps/ThirdPersonMap");
}

void UPuzzlePlatformGameInstance::Join(const FString& Address)
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

