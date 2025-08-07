// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD.h"

#include "ActorComponents/WeaponSystem/ACWeapon.h"
#include "ActorComponents/WeaponSystem/WeaponSystemComponent.h"
#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Widgets/Player/PlayerHitWidget.h"

void UPlayerHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	CurrentHitStageText->SetText(FText::AsNumber(CurrentHitStage));

	UpdateWeaponIcons();

	//InitializeHUD();
}

void UPlayerHUD::NativeConstruct()
{
	Super::NativeConstruct();

	SuraPawnPlayer = Cast<ASuraPawnPlayer>(GetOwningPlayerPawn());
	if (SuraPawnPlayer)
	{
		WeaponSystemComponent = SuraPawnPlayer->GetWeaponSystemComponent();
		if (WeaponSystemComponent)
		{
			WeaponSystemComponent->OnWeaponPickedUp.AddDynamic(this, &UPlayerHUD::UpdatePickup);
			WeaponSystemComponent->OnWeaponSwitched.AddDynamic(this, &UPlayerHUD::OnWeaponSwitchAnim);
		}
	}

	// 애니메이션 등록 (키 값은 UMG 애니메이션 변수명과 동일하게 작성)
	AnimationMap.Add(TEXT("Slot0_TopToCenter"), Slot0_TopToCenter);
	AnimationMap.Add(TEXT("Slot0_CenterToBottom"), Slot0_CenterToBottom);
	AnimationMap.Add(TEXT("Slot0_BottomToTop"), Slot0_BottomToTop);

	AnimationMap.Add(TEXT("Slot1_TopToCenter"), Slot1_TopToCenter);
	AnimationMap.Add(TEXT("Slot1_CenterToBottom"), Slot1_CenterToBottom);
	AnimationMap.Add(TEXT("Slot1_BottomToTop"), Slot1_BottomToTop);

	AnimationMap.Add(TEXT("Slot2_TopToCenter"), Slot2_TopToCenter);
	AnimationMap.Add(TEXT("Slot2_CenterToBottom"), Slot2_CenterToBottom);
	AnimationMap.Add(TEXT("Slot2_BottomToTop"), Slot2_BottomToTop);
}

void UPlayerHUD::SetPlayerHUDManager(UACPlayerHUDManager* InManager)
{
	PlayerHUDManager = InManager;
}


void UPlayerHUD::UpdateHpBar()
{
	if (!PlayerHitWidget && SuraPawnPlayer)
	{
		PlayerHitWidget = SuraPawnPlayer->GetPlayerHitWidget();
		if (PlayerHitWidget)
		{
			UE_LOG(LogTemp, Warning, TEXT("UPlayerHUD: PlayerHitWidget assigned on demand in UpdateHpBar."));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UPlayerHUD: PlayerHitWidget is still null after trying to get it in UpdateHpBar."));
		}
	}
	
	// 1. 회복 타이머 초기화 (기존 대기 중이면 제거)
	GetWorld()->GetTimerManager().ClearTimer(RecoveryTimerHandle);

	// 2. 최대 5단계까지만 증가
	if (CurrentHitStage < 5)
	{
		CurrentHitStage++;
	}

	// 3. 체력 이미지 변경
	ApplyHpBarImage(CurrentHitStage);

	// 4. Hit UI 위젯이 있으면 해당 단계로 업데이트
	if (PlayerHitWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdateHitScreen Called"));
		PlayerHitWidget->UpdateHitScreen(CurrentHitStage);
	}
	else if (!PlayerHitWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerHitWidget is null"));
	}

	// 5. 3초 후 회복 처리 타이머 시작
	GetWorld()->GetTimerManager().SetTimer(
		RecoveryTimerHandle,
		this,
		&UPlayerHUD::RecoverHpBarStage,
		3.0f,
		false
	);
}

void UPlayerHUD::RecoverHpBarStage()
{
	// 최소 단계 0
	if (CurrentHitStage > 0)
	{
		CurrentHitStage--;
	}

	// 체력 이미지 갱신
	ApplyHpBarImage(CurrentHitStage);

	// Hit UI 갱신
	if (PlayerHitWidget)
	{
		PlayerHitWidget->UpdateHitScreen(CurrentHitStage);
	}

	// 추가 회복을 위한 타이머 재등록 (남은 단계가 있다면)
	if (CurrentHitStage > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(
			RecoveryTimerHandle,
			this,
			&UPlayerHUD::RecoverHpBarStage,
			3.0f,
			false
		);
	}
}

void UPlayerHUD::ApplyHpBarImage(int32 Stage)
{
	if (HpBar && HPBarTextures.IsValidIndex(Stage))
	{
		HpBar->SetBrushFromTexture(HPBarTextures[Stage]);
	}
}

void UPlayerHUD::UpdateWeaponIcons()
{
	if (!WeaponSystemComponent) return;

	const TArray<AWeapon*>& Inventory = WeaponSystemComponent->GetWeaponInventory();

	// 최대 3개만 처리
	for (int32 i = 0; i < FMath::Min(3, Inventory.Num()); ++i)
	{
		AWeapon* Weapon = Inventory[i];
		if (!Weapon) continue;

		UTexture2D* Icon = Weapon->GetWeaponImage();
		if (!Icon) continue;

		switch (i)
		{
			case 0: Image_0->SetBrushFromTexture(Icon); break;
			case 1: Image_1->SetBrushFromTexture(Icon); break;
			case 2: Image_2->SetBrushFromTexture(Icon); break;
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("WeaponIcon updated!!"));
}

void UPlayerHUD::OnWeaponSwitchAnim(int32 PrevIndex, int32 NewIndex)
{
	HandleWeaponSlotUIUpdate(PrevIndex, NewIndex);
}

void UPlayerHUD::InitializeHUD() const
{
	if (!WeaponSystemComponent) return;
	
	//const TArray<AWeapon*>& Inventory = WeaponSystemComponent->GetWeaponInventory();
	int32 WeaponCount = WeaponSystemComponent->GetWeaponNum();

	WeaponInventoryNum->SetText(FText::AsNumber(WeaponCount));

	
	// 모든 슬롯을 먼저 숨김 처리
	if (WeaponSlot_0) WeaponSlot_0->SetVisibility(ESlateVisibility::Collapsed);
	if (WeaponSlot_1) WeaponSlot_1->SetVisibility(ESlateVisibility::Collapsed);
	if (WeaponSlot_2) WeaponSlot_2->SetVisibility(ESlateVisibility::Collapsed);

	// 인벤토리 수에 따라 슬롯을 보여줌
	if (WeaponCount >= 1 && WeaponSlot_0)
	{
		WeaponSlot_0->SetVisibility(ESlateVisibility::Visible);
	}
	if (WeaponCount >= 2 && WeaponSlot_1)
	{
		WeaponSlot_1->SetVisibility(ESlateVisibility::Visible);
	}
	if (WeaponCount >= 3 && WeaponSlot_2)
	{
		WeaponSlot_2->SetVisibility(ESlateVisibility::Visible);
	}
}

void UPlayerHUD::UpdatePickup(FName WeaponName)
{
	InitializeHUD();
	UpdateWeaponSlotUIByInventoryOrder();
}

int32 UPlayerHUD::GetWeaponCount() const
{
	return WeaponSystemComponent->GetWeaponInventory().Num();
}

void UPlayerHUD::HandleWeaponSlotUIUpdate(int32 PrevIndex, int32 NewIndex)
{
	const int32 NumWeapons = GetWeaponCount(); 

	if (NumWeapons < 2 || PrevIndex == NewIndex)
	{
		return;
	}

	// 위치 문자열 상수 정의
	static const FString Pos_Center = TEXT("Center");
	static const FString Pos_Top    = TEXT("Top");
	static const FString Pos_Bottom = TEXT("Bottom");

	TMap<int32, FString> PrevPosMap;
	TMap<int32, FString> NewPosMap;

	// 공통: Center는 항상 현재 무기 인덱스
	PrevPosMap.Add(PrevIndex, Pos_Center);
	NewPosMap.Add(NewIndex, Pos_Center);

	if (NumWeapons == 2)
	{
		// 나머지 하나는 Top으로 취급
		int32 OtherPrev = (PrevIndex + 1) % 2;
		int32 OtherNew = (NewIndex + 1) % 2;

		PrevPosMap.Add(OtherPrev, Pos_Top);
		NewPosMap.Add(OtherNew, Pos_Top);
	}
	else if (NumWeapons == 3)
	{
		// Top, Bottom 계산
		int32 PrevTop    = (PrevIndex + 1) % 3;
		int32 PrevBottom = (PrevIndex + 2) % 3;

		int32 NewTop     = (NewIndex + 1) % 3;
		int32 NewBottom  = (NewIndex + 2) % 3;

		PrevPosMap.Add(PrevTop, Pos_Top);
		PrevPosMap.Add(PrevBottom, Pos_Bottom);

		NewPosMap.Add(NewTop, Pos_Top);
		NewPosMap.Add(NewBottom, Pos_Bottom);
	}

	// 애니메이션 재생
	for (int32 SlotIndex = 0; SlotIndex < NumWeapons; ++SlotIndex)
	{
		if (!PrevPosMap.Contains(SlotIndex) || !NewPosMap.Contains(SlotIndex))
			continue;

		const FString& FromPos = PrevPosMap[SlotIndex];
		const FString& ToPos   = NewPosMap[SlotIndex];

		if (FromPos != ToPos)
		{
			FString Key = FString::Printf(TEXT("Slot%d_%sTo%s"), SlotIndex, *FromPos, *ToPos);
			if (UWidgetAnimation** FoundAnim = AnimationMap.Find(Key))
			{
				PlayAnimation(*FoundAnim);
			}
			else
			{
				FString ReverseKey = FString::Printf(TEXT("Slot%d_%sTo%s"), SlotIndex, *ToPos, *FromPos);
				if (UWidgetAnimation** ReverseAnim = AnimationMap.Find(ReverseKey))
				{
					PlayAnimation(*ReverseAnim, 0.f, 1, EUMGSequencePlayMode::Reverse);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("No animation found for Slot %d: %s or reverse %s"), SlotIndex, *Key, *ReverseKey);
				}
			}
		}
	}
}

void UPlayerHUD::UpdateWeaponSlotUIByInventoryOrder()
{
	const int32 NumWeapons = WeaponSystemComponent->GetWeaponInventory().Num();
	const int32 CurrentIndex = WeaponSystemComponent->GetCurrentWeaponIndex();

	if (NumWeapons < 2)
		return;

	UE_LOG(LogTemp, Warning, TEXT("=== UpdateWeaponSlotUIByInventoryOrder ==="));
	UE_LOG(LogTemp, Warning, TEXT("CurrentWeaponIndex: %d"), CurrentIndex);

	// 목표 UI 위치 구성: Center 기준으로 순환큐 만들기
	TMap<int32, FString> FinalPosMap;
	for (int32 Offset = 0; Offset < NumWeapons && Offset < 3; ++Offset)
	{
		int32 Index = (CurrentIndex + Offset) % NumWeapons;

		FString Pos;
		switch (Offset)
		{
		case 0: Pos = "Center"; break;
		case 1: Pos = "Top"; break;
		case 2: Pos = "Bottom"; break;
		}
		FinalPosMap.Add(Index, Pos);
	}

	// 디버깅용 출력
	UE_LOG(LogTemp, Warning, TEXT("Final Position Mapping:"));
	for (const auto& Pair : FinalPosMap)
	{
		UE_LOG(LogTemp, Warning, TEXT("  WeaponIndex %d → %s"), Pair.Key, *Pair.Value);
	}

	// 현재 UI 상태 추정 (간단하게 처리)
	TMap<int32, FString> CurrentPosMap;

	const FString AllPos[3] = { TEXT("Center"), TEXT("Top"), TEXT("Bottom") };

	for (int32 SlotIndex = 0; SlotIndex < NumWeapons; ++SlotIndex)
	{
		for (const FString& Pos : AllPos)
		{
			for (const FString& OtherPos : AllPos)
			{
				if (Pos == OtherPos)
					continue;

				FString Key = FString::Printf(TEXT("Slot%d_%sTo%s"), SlotIndex, *Pos, *OtherPos);
				FString ReverseKey = FString::Printf(TEXT("Slot%d_%sTo%s"), SlotIndex, *OtherPos, *Pos);

				if (AnimationMap.Contains(Key) || AnimationMap.Contains(ReverseKey))
				{
					if (!CurrentPosMap.Contains(SlotIndex))
					{
						CurrentPosMap.Add(SlotIndex, Pos);
						break;
					}
				}
			}
		}
	}

	// 변경이 필요한 슬롯이 하나라도 있는지 확인용 플래그
	bool bNeedAnimation = false;

	for (int32 SlotIndex = 0; SlotIndex < NumWeapons; ++SlotIndex)
	{
		if (!FinalPosMap.Contains(SlotIndex))
			continue;

		const FString& DesiredPos = FinalPosMap[SlotIndex];
		const FString* FoundPrevPos = CurrentPosMap.Find(SlotIndex);

		if (!FoundPrevPos || *FoundPrevPos != DesiredPos)
		{
			bNeedAnimation = true;
			break;
		}
	}

	if (!bNeedAnimation)
	{
		UE_LOG(LogTemp, Warning, TEXT("No UI position changes detected. Skipping animation."));
		return; // 이미 정렬 완료 -> 애니메이션 재생 불필요
	}

	// 변경이 필요한 경우에만 애니메이션 재생
	for (int32 SlotIndex = 0; SlotIndex < NumWeapons; ++SlotIndex)
	{
		if (!FinalPosMap.Contains(SlotIndex))
			continue;

		const FString& DesiredPos = FinalPosMap[SlotIndex];
		const FString* FoundPrevPos = CurrentPosMap.Find(SlotIndex);

		if (FoundPrevPos && *FoundPrevPos == DesiredPos)
		{
			UE_LOG(LogTemp, Warning, TEXT("  Slot %d: already at %s, skipping."), SlotIndex, *DesiredPos);
			continue;
		}

		FString Key = FString::Printf(TEXT("Slot%d_%sTo%s"), SlotIndex, FoundPrevPos ? **FoundPrevPos : TEXT("None"), *DesiredPos);
		if (UWidgetAnimation** Anim = AnimationMap.Find(Key))
		{
			PlayAnimation(*Anim);
			UE_LOG(LogTemp, Warning, TEXT("  Slot %d: Play %s"), SlotIndex, *Key);
		}
		else
		{
			FString ReverseKey = FString::Printf(TEXT("Slot%d_%sTo%s"), SlotIndex, *DesiredPos, FoundPrevPos ? **FoundPrevPos : TEXT("None"));
			if (UWidgetAnimation** ReverseAnim = AnimationMap.Find(ReverseKey))
			{
				PlayAnimation(*ReverseAnim, 0.f, 1, EUMGSequencePlayMode::Reverse);
				UE_LOG(LogTemp, Warning, TEXT("  Slot %d: Play %s in Reverse"), SlotIndex, *ReverseKey);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("  Slot %d: No animation found for %s or reverse %s"), SlotIndex, *Key, *ReverseKey);
			}
		}
	}
}













