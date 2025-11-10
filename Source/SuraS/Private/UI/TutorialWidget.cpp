#include "UI/TutorialWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Input/Events.h"
#include "UI/TutorialTriggerBox.h"

FReply UTutorialWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::Escape)
	{
		CloseTutorialWidget();
		// PlayerController로 입력이 전파되는 것을 차단
		return FReply::Handled();
	}

	// 다른 키가 눌렸다면 "Unhandled"를 반환하여 다른 곳에서 처리하도록 함
	return FReply::Unhandled();
}

void UTutorialWidget::CloseTutorialWidget()
{
	APlayerController* PlayerController = GetOwningPlayer();
	
	UGameplayStatics::SetGamePaused(GetWorld(), false);

	if (PlayerController)
	{
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->bShowMouseCursor = false;
	}
	
	this->RemoveFromParent();
	
	if (OwningTriggerBox)
	{
		OwningTriggerBox->CloseTutorialWidget();
	}
}
