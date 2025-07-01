// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "BonePose.h"
#include "Animation/BoneSocketReference.h"
#include "Animation/AnimNodeBase.h"
#include "Animation/InputScaleBias.h"
#include "AnimNode_SkeletalControl.generated.h"

/**
 * 
 */
USTRUCT(BlueprintInternalUseOnly)
struct SURAS_API FAnimNode_SkeletalControl : public FAnimNode_Base
{
	GENERATED_USTRUCT_BODY()

	// Input link
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Links)
	FComponentSpacePoseLink ComponentPose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Performance, meta = (PinHiddenByDefault, DisplayName = "LOD Threshold"))
	int32 LODThreshold;

	UPROPERTY(Transient)
	float ActualAlpha;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Alpha)
	EAnimAlphaInputType AlphaInputType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Alpha, meta = (PinShownByDefault, DisplayName = "bEnabled"))
	bool bAlphaBoolEnabled;

	// Current strength of the skeletal control
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Alpha, meta = (PinShownByDefault))
	float Alpha;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Alpha)
	FInputScaleBias AlphaScaleBias;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Alpha, meta = (DisplayName = "Blend Settings"))
	FInputAlphaBoolBlend AlphaBoolBlend;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Alpha, meta = (PinShownByDefault))
	FName AlphaCurveName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Alpha)
	FInputScaleBiasClamp AlphaScaleBiasClamp;

	//----------------------------------------------
	/** Name of bone to control. This is the main bone chain to modify from. **/
	UPROPERTY(EditAnywhere, Category = Recoil)
	FBoneReference TargetBone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Recoil, meta = (PinShownByDefault))
	FVector MaxRotationValue = FVector(0.f, 0.f, 0.f);

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Recoil, meta = (PinShownByDefault))
	//float MaxRotationDegreesX = 30.f;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Recoil, meta = (PinShownByDefault))
	//float MaxRotationDegreesY = 30.f;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Recoil, meta = (PinShownByDefault))
	//float MaxRotationDegreesZ = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Recoil)
	float RotationTimeSeconds = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Recoil)
	FVector RotationAxis = FVector(1, 1, 1);

	float ElapsedTime = 0.f;


public:
	FAnimNode_SkeletalControl()
		: LODThreshold(INDEX_NONE)
		, ActualAlpha(0.f)
		, AlphaInputType(EAnimAlphaInputType::Float)
		, bAlphaBoolEnabled(true)
		, Alpha(1.0f)
	{
	}

#if WITH_EDITORONLY_DATA
	// forwarded pose data from the wired node which current node's skeletal control is not applied yet
	FCSPose<FCompactHeapPose> ForwardedPose;
#endif //#if WITH_EDITORONLY_DATA

	// FAnimNode_Base interface
	virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;
	virtual void CacheBones_AnyThread(const FAnimationCacheBonesContext& Context)  override;
	virtual void Update_AnyThread(const FAnimationUpdateContext& Context) final;
	virtual void EvaluateComponentSpace_AnyThread(FComponentSpacePoseContext& Output) final;
	virtual int32 GetLODThreshold() const override { return LODThreshold; }
	// End of FAnimNode_Base interface


	// Visual warnings are shown on the node but not logged as an error for build system, use with care
	// The warnigns are cleared at CacheBones_AnyThread and should be added during InitializeBoneReferences
#if WITH_EDITOR
	void AddBoneRefMissingVisualWarning(const FString& BoneName, const FString& SkeletalMeshName);
	void AddValidationVisualWarning(FText ValidationVisualWarning);
	bool HasValidationVisualWarnings() const;
	void ClearValidationVisualWarnings();
	FText GetValidationVisualWarningMessage() const;
#endif

protected:
	// Interface for derived skeletal controls to implement
	// use this function to update for skeletal control base
	virtual void UpdateInternal(const FAnimationUpdateContext& Context);

	// Update incoming component pose.
	virtual void UpdateComponentPose_AnyThread(const FAnimationUpdateContext& Context);

	// Evaluate incoming component pose.
	virtual void EvaluateComponentPose_AnyThread(FComponentSpacePoseContext& Output);

	// use this function to evaluate for skeletal control base
	virtual void EvaluateComponentSpaceInternal(FComponentSpacePoseContext& Context);

	// Evaluate the new component-space transforms for the affected bones.
	virtual void EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms);

	// return true if it is valid to Evaluate
	virtual bool IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones);

	// initialize any bone references you have
	virtual void InitializeBoneReferences(const FBoneContainer& RequiredBones);




private:
	// Resused bone transform array to avoid reallocating in skeletal controls
	TArray<FBoneTransform> BoneTransforms;

#if WITH_EDITORONLY_DATA
	UPROPERTY(transient)
	FText ValidationVisualWarningMessage;
#endif


};
