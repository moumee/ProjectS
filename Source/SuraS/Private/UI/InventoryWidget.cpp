#include "UI/InventoryWidget.h"

#include "ActorComponents/WeaponSystem/ACWeapon.h"
#include "ActorComponents/WeaponSystem/SuraCharacterPlayerWeapon.h"
#include "ActorComponents/WeaponSystem/WeaponData.h"
#include "ActorComponents/WeaponSystem/WeaponSystemComponent.h"
#include "Characters/Player/SuraCharacterPlayer.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"

void UInventoryWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 키보드 입력을 받기 위해 IsFocusable를 true로 설정
    this->SetIsFocusable(true);

    // 기본값 설정
    CurrentTab = EInventoryTab::Weapon;
    SetActiveTab(CurrentTab);

    

#pragma region Weapon
    
    // Player의 UWeaponSystemComponent 가져오기
    ASuraCharacterPlayerWeapon* Player = Cast<ASuraCharacterPlayerWeapon>(GetOwningPlayerPawn());
    if (Player)
    {
        UWeaponSystemComponent* WeaponSystem = Player->FindComponentByClass<UWeaponSystemComponent>();
        if (WeaponSystem)
        {
            // 델리게이트 바인딩
            if (!WeaponSystem->OnWeaponPickedUp.IsAlreadyBound(this, &UInventoryWidget::OnWeaponPickedUp))
            {
                WeaponSystem->OnWeaponPickedUp.AddDynamic(this, &UInventoryWidget::OnWeaponPickedUp);
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("WeaponSystem is nullptr!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Player is nullptr!"));
    }

    /** UpdateCurrentWeaponWindow 호출 **/
    // InventoryManager 가져오는 코드
    AActor* OwnerActor = GetOwningPlayerPawn();
    if (!OwnerActor) return;

    UACInventoryManager* InventoryManager = OwnerActor->FindComponentByClass<UACInventoryManager>();
    if (!InventoryManager) return;

    InventoryManager->UpdateCurrentWeaponWindow(); // ✅ 여기서 호출!
    
    
#pragma endregion Weapon
    
    // 최초 한 번만 초기화되도록 체크
    if (bIsInitialized)
    {
        return;
    }

    /** 이 주석 아래 코드들은 최초 초기화 시에만 호출됨 **/
    UE_LOG(LogTemp, Warning, TEXT("WBP_Inventory 생성됨"));

    InitializeInventory(); // UI 초기화

    if (BtnRifle)
        BtnRifle->OnClicked.AddDynamic(this, &UInventoryWidget::OnWeaponButtonClicked_Rifle);

    if (BtnShotGun)
        BtnShotGun->OnClicked.AddDynamic(this, &UInventoryWidget::OnWeaponButtonClicked_ShotGun);

    if (BtnMissileLauncher)
        BtnMissileLauncher->OnClicked.AddDynamic(this, &UInventoryWidget::OnWeaponButtonClicked_MissileLauncher);

    // 초기화 완료 후 bIsInitialized를 true로 설정
    bIsInitialized = true;

    
    
}

FReply UInventoryWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    // 키 입력 처리: Q 또는 E 키에 대한 처리를 추가
    if (InKeyEvent.GetKey() == EKeys::Q)
    {
        SwitchToWeaponTab();  // Weapon 탭으로 돌아가기
        return FReply::Handled();  // 이벤트 처리 완료
    }
    if (InKeyEvent.GetKey() == EKeys::E)
    {
        SwitchToChipTab();  // Chip 탭으로 전환
        return FReply::Handled();  // 이벤트 처리 완료
    }

    if (InKeyEvent.GetKey() == EKeys::A)
    {
        SwitchPage(0); // A 키를 누르면 1페이지로 이동
        return FReply::Handled();
    }
    if (InKeyEvent.GetKey() == EKeys::D)
    {
        SwitchPage(1); // D 키를 누르면 2페이지로 이동
        return FReply::Handled();
    }

    // 키 입력 처리: I와 Tab 키를 눌러서 인벤토리 닫기
    if (InKeyEvent.GetKey() == EKeys::I || InKeyEvent.GetKey() == EKeys::Tab)
    {
        UInventoryWidget::CloseUI();
        return FReply::Handled();
    }

    if (InKeyEvent.GetKey() == EKeys::Delete)
    {
        AllWeaponDiscard();
    }

    if (InKeyEvent.GetKey() == EKeys::F)
    {
       // GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("F 키 감지됨"));

        ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwningPlayerPawn());
        if (!OwnerCharacter) return FReply::Unhandled();

        UACInventoryManager* Manager = OwnerCharacter->FindComponentByClass<UACInventoryManager>();
        if (!Manager)
        {
            //GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("InventoryManager 못 찾음"));
            return FReply::Unhandled();
        }

        Manager->OnConfirmWeaponEquip();
        //GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("OnConfrimWeaponEquip함수 호출됨"));
        return FReply::Handled();
    }

    return FReply::Handled();  // Tab 키 기본 동작 방지
}


void UInventoryWidget::SetActiveTab(EInventoryTab NewTab)
{
    CurrentTab = NewTab;

    // 탭 강조 (불투명도 설정)
    if (TabWeapon && TabChip)
    {
        TabWeapon->SetOpacity(CurrentTab == EInventoryTab::Weapon ? 1.0f : 0.5f);
        TabChip->SetOpacity(CurrentTab == EInventoryTab::Chip ? 1.0f : 0.5f);
    }

    // 활성화된 탭에 해당하는 콘텐츠를 표시
    ShowTabContent();
    
}

void UInventoryWidget::SwitchToWeaponTab()
{
    if (CurrentTab == EInventoryTab::Chip)
    {
        SetActiveTab(EInventoryTab::Weapon);
    }
}

void UInventoryWidget::SwitchToChipTab()
{
    if (CurrentTab == EInventoryTab::Weapon)
    {
        SetActiveTab(EInventoryTab::Chip);
    }
}

void UInventoryWidget::ShowTabContent() const
{
    // 현재 탭에 따라 Widget Switcher의 Index를 전환
    if (ContentSwitcher)
    {
        int32 Index = (CurrentTab == EInventoryTab::Weapon) ? 0 : 1;
        ContentSwitcher->SetActiveWidgetIndex(Index);
        
        // 콘텐츠가 바뀔 때 효과음 추가
        if (USoundBase* SwitchSound = TabSwitchSound)
        {
            UGameplayStatics::PlaySound2D(this, SwitchSound);
        }
    }
   
}

void UInventoryWidget::OpenUI()
{
    Super::OpenUI();
    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, "OpenInventory");
}

void UInventoryWidget::CloseUI()
{
    Super::CloseUI();
    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, "CloseInventory");

    // invnetory를 닫을 때 gameinstance의 saveweapondata메서드를 호출
    // if (UCustomGameInstance* GameInstance = Cast<UCustomGameInstance>(GetWorld()->GetGameInstance()))
    // {
    //     GameInstance->SaveWeaponData();
    // }
}

void UInventoryWidget::InitializeInventory()
{
    if (!DTWeapon)
    {
        UE_LOG(LogTemp, Warning, TEXT("DT_Weapon이 설정되지 않았습니다!"));
        return;
    }

    static const FString ContextString(TEXT("Weapon Data Context"));
    TArray<FName> RowNames = DTWeapon->GetRowNames();

    // 이미지와 텍스트를 같이 저장
    WeaponUIElements.Add(TEXT("Rifle"), FWeaponUI(Rifle, RifleName));
    WeaponUIElements.Add(TEXT("ShotGun"), FWeaponUI(ShotGun, ShotGunName));
    WeaponUIElements.Add(TEXT("MissileLauncher"), FWeaponUI(MissileLauncher, MissileLauncherName));
    //WeaponUIElements.Add(TEXT("RailGun"), FWeaponUI(RailGun, RailGunName));

    for (FName RowName : RowNames)
    {
        // RowName을 기반으로 데이터 테이블에서 해당 행 찾기
        FWeaponData* WeaponData = DTWeapon->FindRow<FWeaponData>(RowName, ContextString);
        if (WeaponData)
        {
            FString WeaponNameStr = RowName.ToString(); // RowName을 문자열로 변환

            if (WeaponUIElements.Contains(WeaponNameStr))
            {
                UpdateWeaponUI(WeaponNameStr);
            }
        }
    }
}

void UInventoryWidget::SwitchPage(int32 PageIndex)
{
    if (WeaponWidgetSwitcher)
    {
        WeaponWidgetSwitcher->SetActiveWidgetIndex(PageIndex);
    }
}

void UInventoryWidget::UpdateWeaponUI(FString WeaponNameStr)
{
    if (!DTWeapon) return;

    // 수정된 WeaponNameStr을 사용하여 FindRow 호출
    static const FString ContextString(TEXT("Weapon Unlock Context"));
    FWeaponData* WeaponData = DTWeapon->FindRow<FWeaponData>(*WeaponNameStr, ContextString);

    if (WeaponData)
    {
        // 수정된 WeaponNameStr을 사용하여 FindRow 호출
        if (WeaponUIElements.Contains(WeaponNameStr))
        {
            FWeaponUI& WeaponUI = WeaponUIElements[WeaponNameStr];

            // 이미지 설정
            if (WeaponUI.WeaponImage)
            {
                WeaponUI.WeaponImage->SetBrushFromTexture(WeaponData->WeaponImage);

                // 무기 소유 여부에 따른 색상 조정
                if (WeaponData->bIsWeaponOwned)
                {
                    WeaponUI.WeaponImage->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f));
                }
                else
                {
                    WeaponUI.WeaponImage->SetColorAndOpacity(FLinearColor(0.0f, 0.0f, 0.0f));
                }
            }
            // 텍스트 설정
            if (WeaponUI.WeaponText)
            {
                if (WeaponData->bIsWeaponOwned)
                {
                    WeaponUI.WeaponText->SetText(FText::FromString(WeaponNameStr));
                    WeaponUI.WeaponText->SetOpacity(1.0f);
                }
            }
        }
    }
}

void UInventoryWidget::UnlockWeapon(FName WeaponName)
{
    if (!DTWeapon) return;

    // WeaponName_ 접두어를 제거하고, 실제 이름만 추출
    FString WeaponNameStr = WeaponName.ToString().RightChop(24);  // "EWeaponName::WeaponName_"을 제거

    // 수정된 WeaponNameStr을 사용하여 FindRow 호출
    static const FString ContextString(TEXT("Weapon Unlock Context"));
    FWeaponData* WeaponData = DTWeapon->FindRow<FWeaponData>(*WeaponNameStr, ContextString);

    if (WeaponData && !WeaponData->bIsWeaponOwned)
    {
        WeaponData->bIsWeaponOwned = true;
        UpdateWeaponUI(WeaponNameStr);
    }
}

void UInventoryWidget::OnWeaponPickedUp(FName WeaponName)
{
    UnlockWeapon(WeaponName);
}

void UInventoryWidget::AllWeaponDiscard()
{
    if (!DTWeapon)
    {
        UE_LOG(LogTemp, Warning, TEXT("DTWeapon is not set in GameInstance!"));
        return;
    }
    
    for (const auto& Row : DTWeapon->GetRowMap())
    {
        FWeaponData* WeaponData = (FWeaponData*)Row.Value;
        if (WeaponData)
        {
            WeaponData->bIsWeaponOwned = false;

            UpdateWeaponUI(Row.Key.ToString());
        }
    }


    
    UE_LOG(LogTemp, Warning, TEXT("All weapons have been discarded."));
}

void UInventoryWidget::OnWeaponButtonClicked_Rifle()
{
    //GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, TEXT("Rifle 버튼 클릭됨"));
    RequestWeaponChangeByName("Rifle");
}

void UInventoryWidget::OnWeaponButtonClicked_ShotGun()
{
    RequestWeaponChangeByName("ShotGun");
}

void UInventoryWidget::OnWeaponButtonClicked_MissileLauncher()
{
    RequestWeaponChangeByName("MissileLauncher");
}

void UInventoryWidget::RequestWeaponChangeByName(const FString& WeaponNameStr)
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan,
        FString::Printf(TEXT("요청된 무기 이름: %s"), *WeaponNameStr));

    APawn* Pawn = GetOwningPlayerPawn();
    if (!Pawn)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Pawn 가져오기 실패"));
        return;
    }

    UWeaponSystemComponent* WeaponSystem = Pawn->FindComponentByClass<UWeaponSystemComponent>();
    if (!WeaponSystem)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("WeaponSystemComponent 찾기 실패"));
        return;
    }

    UACInventoryManager* Manager = Pawn->FindComponentByClass<UACInventoryManager>();
    if (!Manager)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("InventoryManager 찾기 실패"));
        return;
    }

    const TArray<AWeapon*>& Inventory = WeaponSystem->GetWeaponInventory();
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
        FString::Printf(TEXT("WeaponInventory에 있는 무기 수: %d"), Inventory.Num()));

    // 무기 인벤토리 탐색
    for (int32 i = 0; i < Inventory.Num(); ++i)
    {
        AWeapon* Weapon = Inventory[i];
        if (!Weapon || !Weapon->WeaponData)
        {
            GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow,
                FString::Printf(TEXT("[%d] 무기 또는 WeaponData가 nullptr"), i));
            continue;
        }

        FString WeaponEnumStr = UEnum::GetDisplayValueAsText(Weapon->WeaponData->WeaponName).ToString();

        // GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::White,
        //     FString::Printf(TEXT("[%d] 무기 이름: %s"), i, *WeaponEnumStr));

        if (WeaponEnumStr == WeaponNameStr)
        {
            GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
                FString::Printf(TEXT("일치하는 무기 인덱스: %d"), i));

            GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("무기를 교체하려면 F키를 눌러주세요")));

            Manager->SetPendingWeaponIndex(i);
            return;
        }
    }

    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red,
        FString::Printf(TEXT("'%s' 무기를 WeaponInventory에서 찾을 수 없음"), *WeaponNameStr));
}
