// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/WeaponSystem/AnimGraphNode_BoneRotation.h"
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

UAnimGraphNode_BoneRotation::UAnimGraphNode_BoneRotation(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FLinearColor UAnimGraphNode_BoneRotation::GetNodeTitleColor() const
{
	return FLinearColor(1.f, 0.0f, 0.0f);
}

FText UAnimGraphNode_BoneRotation::GetTooltipText() const
{
	return GetControllerDescription();
}

void UAnimGraphNode_BoneRotation::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
}

bool UAnimGraphNode_BoneRotation::ShowVisualWarning() const
{
	return false;
}

FText UAnimGraphNode_BoneRotation::GetVisualWarningTooltipText() const
{
	return FText();
}

FString UAnimGraphNode_BoneRotation::GetNodeCategory() const
{
	return FString();
}

void UAnimGraphNode_BoneRotation::CreateOutputPins()
{
	CreatePin(EGPD_Output, UAnimationGraphSchema::PC_Struct, FComponentSpacePoseLink::StaticStruct(), TEXT("Pose"));
}

void UAnimGraphNode_BoneRotation::ValidateAnimNodePostCompile(FCompilerResultsLog& MessageLog, UAnimBlueprintGeneratedClass* CompiledClass, int32 CompiledNodeIndex)
{
}

void UAnimGraphNode_BoneRotation::CustomizePinData(UEdGraphPin* Pin, FName SourcePropertyName, int32 ArrayIndex) const
{
}

void UAnimGraphNode_BoneRotation::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
}

bool UAnimGraphNode_BoneRotation::IsPinShown(const FName PinName) const
{
	return false;
}

void UAnimGraphNode_BoneRotation::SetDefaultValue(const FName InDefaultValueName, const FVector& InValue)
{
}

void UAnimGraphNode_BoneRotation::GetDefaultValue(const FName UpdateDefaultValueName, FVector& OutVec)
{
}

FText UAnimGraphNode_BoneRotation::GetControllerDescription() const
{
	return FText();
}