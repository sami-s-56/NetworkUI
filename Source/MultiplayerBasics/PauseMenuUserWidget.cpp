// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenuUserWidget.h"
#include "Components/Button.h"

bool UPauseMenuUserWidget::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) return false;

	if (ResumeButton == nullptr) return false;
	ResumeButton->OnClicked.AddDynamic(this, &UPauseMenuUserWidget::OnResumeButton);

	if (LeaveButton == nullptr) return false;

	LeaveButton->OnClicked.AddDynamic(this, &UPauseMenuUserWidget::OnLeaveButton);

	return true;
}

void UPauseMenuUserWidget::LoadMenu()
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

void UPauseMenuUserWidget::RemoveMenu()
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

void UPauseMenuUserWidget::OnResumeButton()
{
	if (this->IsInViewport())
	{
		RemoveMenu();
	}
}

void UPauseMenuUserWidget::OnLeaveButton()
{
	menuInterface->Leave();
}
