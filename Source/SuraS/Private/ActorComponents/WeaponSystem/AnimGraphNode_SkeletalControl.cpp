// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/WeaponSystem/AnimGraphNode_SkeletalControl.h"
#include "UnrealWidgetFwd.h"
#include "AnimationGraphSchema.h"
#include "Animation/AnimationSettings.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Kismet2/CompilerResultsLog.h"
#include "DetailLayoutBuilder.h"
#include "ScopedTransaction.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "BoneControllers/AnimNode_SkeletalControlBase.h"

#define LOCTEXT_NAMESPACE "A3Nodes"

UAnimGraphNode_SkeletalControl::UAnimGraphNode_SkeletalControl(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
}

FLinearColor UAnimGraphNode_SkeletalControl::GetNodeTitleColor() const
{
	return FLinearColor(1.f, 0.0f, 0.0f);
}

FText UAnimGraphNode_SkeletalControl::GetTooltipText() const
{
	return GetControllerDescription();
}

void UAnimGraphNode_SkeletalControl::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	//const FName PropertyName = (PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None);

	//// Reconstruct node to show updates to PinFriendlyNames.
	//if ((PropertyName == GET_MEMBER_NAME_STRING_CHECKED(FAnimNode_SkeletalControl, AlphaScaleBias))
	//	|| (PropertyName == GET_MEMBER_NAME_STRING_CHECKED(FInputScaleBiasClamp, bMapRange))
	//	|| (PropertyName == GET_MEMBER_NAME_STRING_CHECKED(FInputRange, Min))
	//	|| (PropertyName == GET_MEMBER_NAME_STRING_CHECKED(FInputRange, Max))
	//	|| (PropertyName == GET_MEMBER_NAME_STRING_CHECKED(FInputScaleBiasClamp, Scale))
	//	|| (PropertyName == GET_MEMBER_NAME_STRING_CHECKED(FInputScaleBiasClamp, Bias))
	//	|| (PropertyName == GET_MEMBER_NAME_STRING_CHECKED(FInputScaleBiasClamp, bClampResult))
	//	|| (PropertyName == GET_MEMBER_NAME_STRING_CHECKED(FInputScaleBiasClamp, ClampMin))
	//	|| (PropertyName == GET_MEMBER_NAME_STRING_CHECKED(FInputScaleBiasClamp, ClampMax))
	//	|| (PropertyName == GET_MEMBER_NAME_STRING_CHECKED(FInputScaleBiasClamp, bInterpResult))
	//	|| (PropertyName == GET_MEMBER_NAME_STRING_CHECKED(FInputScaleBiasClamp, InterpSpeedIncreasing))
	//	|| (PropertyName == GET_MEMBER_NAME_STRING_CHECKED(FInputScaleBiasClamp, InterpSpeedDecreasing)))
	//{
	//	ReconstructNode();
	//}

	//if (PropertyName == GET_MEMBER_NAME_STRING_CHECKED(FAnimNode_SkeletalControl, AlphaInputType))
	//{
	//	FScopedTransaction Transaction(LOCTEXT("ChangeAlphaInputType", "Change Alpha Input Type"));
	//	Modify();

	//	const FAnimNode_SkeletalControl* SkelControlNode = GetNode();

	//	// Break links to pins going away
	//	for (int32 PinIndex = 0; PinIndex < Pins.Num(); ++PinIndex)
	//	{
	//		UEdGraphPin* Pin = Pins[PinIndex];
	//		if (Pin->PinName == GET_MEMBER_NAME_STRING_CHECKED(FAnimNode_SkeletalControl, Alpha))
	//		{
	//			if (GetNode()->AlphaInputType != EAnimAlphaInputType::Float)
	//			{
	//				Pin->BreakAllPinLinks();
	//				RemoveBindings(Pin->PinName);
	//			}
	//		}
	//		else if (Pin->PinName == GET_MEMBER_NAME_STRING_CHECKED(FAnimNode_SkeletalControl, bAlphaBoolEnabled))
	//		{
	//			if (GetNode()->AlphaInputType != EAnimAlphaInputType::Bool)
	//			{
	//				Pin->BreakAllPinLinks();
	//				RemoveBindings(Pin->PinName);
	//			}
	//		}
	//		else if (Pin->PinName == GET_MEMBER_NAME_STRING_CHECKED(FAnimNode_SkeletalControl, AlphaCurveName))
	//		{
	//			if (GetNode()->AlphaInputType != EAnimAlphaInputType::Curve)
	//			{
	//				Pin->BreakAllPinLinks();
	//				RemoveBindings(Pin->PinName);
	//			}
	//		}
	//	}

	//	ReconstructNode();

	//	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(GetBlueprint());
	//}

	//Super::PostEditChangeProperty(PropertyChangedEvent);
}

bool UAnimGraphNode_SkeletalControl::ShowVisualWarning() const
{
	return false;
}

FText UAnimGraphNode_SkeletalControl::GetVisualWarningTooltipText() const
{
	return FText();
}

FString UAnimGraphNode_SkeletalControl::GetNodeCategory() const
{
	return FString();
}

void UAnimGraphNode_SkeletalControl::CreateOutputPins()
{
	CreatePin(EGPD_Output, UAnimationGraphSchema::PC_Struct, FComponentSpacePoseLink::StaticStruct(), TEXT("Pose"));
}

void UAnimGraphNode_SkeletalControl::ValidateAnimNodePostCompile(FCompilerResultsLog& MessageLog, UAnimBlueprintGeneratedClass* CompiledClass, int32 CompiledNodeIndex)
{
}

void UAnimGraphNode_SkeletalControl::CustomizePinData(UEdGraphPin* Pin, FName SourcePropertyName, int32 ArrayIndex) const
{
}

void UAnimGraphNode_SkeletalControl::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
}

bool UAnimGraphNode_SkeletalControl::IsPinShown(const FName PinName) const
{
	return false;
}

void UAnimGraphNode_SkeletalControl::SetDefaultValue(const FName InDefaultValueName, const FVector& InValue)
{
}

void UAnimGraphNode_SkeletalControl::GetDefaultValue(const FName UpdateDefaultValueName, FVector& OutVec)
{
}

FText UAnimGraphNode_SkeletalControl::GetControllerDescription() const
{
	return FText();
}


#undef LOCTEXT_NAMESPACE