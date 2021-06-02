// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformGameInstance.h"

#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuUserWidget.h"
#include "MainMenuUserWidget.h"

UPuzzlePlatformGameInstance :: UPuzzlePlatformGameInstance()
{
	UE_LOG(LogTemp, Warning, TEXT("Cnstructor called!"));

	ConstructorHelpers::FClassFinder<UUserWidget> WBPMainMenu(TEXT("/Game/UI/WBP_MainMenu"));

	ConstructorHelpers::FClassFinder<UUserWidget> WBPPauseMenu(TEXT("/Game/UI/WBP_PauseMenu"));

	if (WBPMainMenu.Class != nullptr) 
	{
		WBPMainMenuClass = WBPMainMenu.Class;
	}

	if (WBPPauseMenu.Class != nullptr)
	{
		WBPPauseMenuClass = WBPPauseMenu.Class;
	}

	UE_LOG(LogTemp, Warning, TEXT("Found Class %s"), *WBPMainMenuClass->GetName());
}

void UPuzzlePlatformGameInstance::Init()
{
	Super::Init();

	UE_LOG(LogTemp, Warning, TEXT("Init called!"));

}

void UPuzzlePlatformGameInstance::Host()
{
	UEngine* Engine = GetEngine();
	
	if (Engine == nullptr)
		return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Emerald, TEXT("Hosting!"));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	if (WBPMainMenuClass != nullptr)
	{
		Menu = CreateWidget<UMainMenuUserWidget>(this, WBPMainMenuClass);
		if (Menu != nullptr)
		{
			Menu->RemoveMenu();
		}
	}

	World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
}

void UPuzzlePlatformGameInstance::Join(const FString& ip)
{
	if (Menu != nullptr)
	{
		Menu->RemoveMenu();
	}

	UEngine* Engine = GetEngine();

	if (Engine == nullptr)
		return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Emerald, FString::Printf(TEXT("Joining! %s"),*ip));

	APlayerController* PlayerController = GetFirstLocalPlayerController();

	if (PlayerController == nullptr) return;

	PlayerController->ClientTravel(*ip, ETravelType::TRAVEL_Absolute);

}

void UPuzzlePlatformGameInstance::Leave()
{
	if (PMenu != nullptr)
	{
		PMenu->RemoveMenu();
	}
	ReturnToMainMenu();
	/** Instead of the above method, ClientTravel can also be used with string containing path to main menu level */
}

void UPuzzlePlatformGameInstance::LoadMainMenu()
{
	if (WBPMainMenuClass != nullptr)
	{
		Menu = CreateWidget<UMainMenuUserWidget>(this, WBPMainMenuClass);
		if (Menu != nullptr)
		{
			Menu->SetMenuInterface(this);

			Menu->LoadMenu();
		}
	}

}

void UPuzzlePlatformGameInstance::LoadPauseMenu()
{
	if (WBPPauseMenuClass != nullptr)
	{
		PMenu = CreateWidget<UPauseMenuUserWidget>(this, WBPPauseMenuClass);

		if (PMenu != nullptr) 
		{
			PMenu->SetMenuInterface(this);
			
			PMenu->LoadMenu();
		}
	}
}