// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuUserWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/WidgetSwitcher.h"

#include "MenuSystem/MenuInterface.h"

bool UMainMenuUserWidget::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) return false;

	if (HostButton == nullptr) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenuUserWidget::OnHostButton);
	
	if (JoinButton == nullptr) return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenuUserWidget::OnJoinButton);

	if (BackButton == nullptr) return false;
	BackButton->OnClicked.AddDynamic(this, &UMainMenuUserWidget::OnBackButton);
	
	if (GameJoinButton_1 == nullptr) return false;
	GameJoinButton_1->OnClicked.AddDynamic(this, &UMainMenuUserWidget::OnJoinGameButton);

	if (BackButton == nullptr) return false;
	QuitButton->OnClicked.AddDynamic(this, &UMainMenuUserWidget::OnQuitButton);

	return true;
}

void UMainMenuUserWidget::OnHostButton()
{
	UE_LOG(LogTemp, Warning, TEXT("Host Button Clicked"));

	if (menuInterface == nullptr)
	{
		return;
	}

	menuInterface->Host();
}

void UMainMenuUserWidget::OnJoinButton()
{
	if (MenuSwitcher == nullptr) return;

	MenuSwitcher->SetActiveWidgetIndex(1);
}

void UMainMenuUserWidget::OnBackButton()
{
	if (MenuSwitcher == nullptr) return;

	MenuSwitcher->SetActiveWidgetIndex(0);
}

void UMainMenuUserWidget::SetMenuInterface(IMenuInterface* _interface)
{
	menuInterface = _interface;
}

void UMainMenuUserWidget::LoadMenu()
{
	this->AddToViewport();

	UWorld* world = GetWorld();

	if (world == nullptr) return;

	APlayerController* PlayerController = world->GetFirstPlayerController();

	if (PlayerController == nullptr) return;

	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(this->TakeWidget());	//Set widget to focus takes slate widget and we have UUserWidget, so TakeWidget() does the necessary conversion
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = true;
}

void UMainMenuUserWidget::RemoveMenu()
{
	this->RemoveFromViewport();

	UWorld* world = GetWorld();

	if (world == nullptr) return;

	APlayerController* PlayerController = world->GetFirstPlayerController();

	if (PlayerController == nullptr) return;

	FInputModeGameOnly InputMode;

	PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = false;
}

void UMainMenuUserWidget::OnJoinGameButton() 
{
	const FString& ipAddress = IPAddBox->GetText().ToString();

	if (menuInterface == nullptr) return;

	menuInterface->Join(ipAddress);

}

void UMainMenuUserWidget::OnQuitButton()
{
	UWorld* world = GetWorld();

	if (world == nullptr) return;

	APlayerController* PlayerController = world->GetFirstPlayerController();

	if (PlayerController == nullptr) return;

	PlayerController->ConsoleCommand(TEXT("quit"));
}