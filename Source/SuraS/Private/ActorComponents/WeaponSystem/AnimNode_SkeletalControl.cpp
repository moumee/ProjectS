// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/WeaponSystem/AnimNode_SkeletalControl.h"
#include "Engine/SkeletalMesh.h"
#include "Animation/AnimInstanceProxy.h"

// Macro to disable inlining of function calls within the statement that follows
// e.g: UE_DONT_INLINE_CALL ThisFunctionCallWontBeInlined(Foo->ThisIsntInlinedEither());
#if defined(__clang__) && defined(__has_cpp_attribute) && __has_cpp_attribute(clang::noinline)
#define UE_DONT_INLINE_CALL [[clang::noinline]]
#else
#define UE_DONT_INLINE_CALL
#endif

/////////////////////////////////////////////////////
// FAnimNode_SkeletalControl

void FAnimNode_SkeletalControl::Initialize_AnyThread(const FAnimationInitializeContext& Context)
{
	//Unreal enigne의 profiling macro -> 이 함수가 실행되는 동안 성능을 측정하기 위해 사용함
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(Initialize_AnyThread)

	FAnimNode_Base::Initialize_AnyThread(Context);

	ComponentPose.Initialize(Context);

	AlphaBoolBlend.Reinitialize();
	AlphaScaleBiasClamp.Reinitialize();

	//-----------------------------------

}

void FAnimNode_SkeletalControl::CacheBones_AnyThread(const FAnimationCacheBonesContext& Context)
{
#if WITH_EDITOR
	ClearValidationVisualWarnings();
#endif

	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(CacheBones_AnyThread)
	FAnimNode_Base::CacheBones_AnyThread(Context);
	InitializeBoneReferences(Context.AnimInstanceProxy->GetRequiredBones());
	ComponentPose.CacheBones(Context);

	//-----------------------------------
}

void FAnimNode_SkeletalControl::Update_AnyThread(const FAnimationUpdateContext& Context)
{
	//////////////////////////////////////////////////////////////////////////
	// PERFORMANCE CRITICAL NOTE
	// 
	// This function is called recursively as we traverse nodes, as such, it is critical to keep the
	// amount of stack space used to a minimum as many nodes can be traversed. Using too much stack
	// here can quickly lead to stack overflows.
	// 
	// We explicitly disable inlineing for virtual calls. Normally, virtual calls are never inlined
	// but when PGO and LTO are enabled, the compiler can speculatively de-virtualize the calls. It does
	// this by comparing the v-table pointer and inlineing the call directly in here with a static jump.
	// As a result, it can significantly increase the amount of stack space used.
	//////////////////////////////////////////////////////////////////////////

	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(Update_AnyThread)
	UE_DONT_INLINE_CALL UpdateComponentPose_AnyThread(Context);

	ActualAlpha = 0.f;
	if (IsLODEnabled(Context.AnimInstanceProxy))
	{
		GetEvaluateGraphExposedInputs().Execute(Context);

		// Apply the skeletal control if it's valid
		switch (AlphaInputType)
		{
		case EAnimAlphaInputType::Float:
			ActualAlpha = AlphaScaleBias.ApplyTo(AlphaScaleBiasClamp.ApplyTo(Alpha, Context.GetDeltaTime()));
			break;
		case EAnimAlphaInputType::Bool:
			ActualAlpha = AlphaBoolBlend.ApplyTo(bAlphaBoolEnabled, Context.GetDeltaTime());
			break;
		case EAnimAlphaInputType::Curve:
			if (UAnimInstance* AnimInstance = Cast<UAnimInstance>(Context.AnimInstanceProxy->GetAnimInstanceObject()))
			{
				ActualAlpha = AlphaScaleBiasClamp.ApplyTo(AnimInstance->GetCurveValue(AlphaCurveName), Context.GetDeltaTime());
			}
			break;
		};

		// Make sure Alpha is clamped between 0 and 1.
		ActualAlpha = FMath::Clamp<float>(ActualAlpha, 0.f, 1.f);

		if (FAnimWeight::IsRelevant(ActualAlpha))
		{
			const USkeleton* Skeleton = Context.AnimInstanceProxy->GetSkeleton();
			const FBoneContainer& RequiredBones = Context.AnimInstanceProxy->GetRequiredBones();

			bool bIsValidToEvaluate;
			UE_DONT_INLINE_CALL bIsValidToEvaluate = IsValidToEvaluate(Skeleton, RequiredBones);
			if (bIsValidToEvaluate)
			{
				UE_DONT_INLINE_CALL UpdateInternal(Context);
			}
		}
	}

	TRACE_ANIM_NODE_VALUE(Context, TEXT("Alpha"), ActualAlpha);

}

void FAnimNode_SkeletalControl::EvaluateComponentSpace_AnyThread(FComponentSpacePoseContext& Output)
{
	//////////////////////////////////////////////////////////////////////////
	// PERFORMANCE CRITICAL NOTE
	// 
	// This function is called recursively as we traverse nodes, as such, it is critical to keep the
	// amount of stack space used to a minimum as many nodes can be traversed. Using too much stack
	// here can quickly lead to stack overflows.
	// 
	// We explicitly disable inlineing for virtual calls. Normally, virtual calls are never inlined
	// but when PGO and LTO are enabled, the compiler can speculatively de-virtualize the calls. It does
	// this by comparing the v-table pointer and inlineing the call directly in here with a static jump.
	// As a result, it can significantly increase the amount of stack space used.
	//////////////////////////////////////////////////////////////////////////

	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(EvaluateComponentSpace_AnyThread)
	ANIM_MT_SCOPE_CYCLE_COUNTER_VERBOSE(SkeletalControlBase, !IsInGameThread());

	// Cache the incoming node IDs in a base context
	FAnimationBaseContext CachedContext(Output);

	UE_DONT_INLINE_CALL EvaluateComponentPose_AnyThread(Output);

#if WITH_EDITORONLY_DATA
	// save current pose before applying skeletal control to compute the exact gizmo location in AnimGraphNode
	ForwardedPose.CopyPose(Output.Pose);
#endif // #if WITH_EDITORONLY_DATA

#if DO_CHECK
	// this is to ensure Source data does not contain NaN
	ensure(Output.ContainsNaN() == false);
#endif

	// Apply the skeletal control if it's valid
	if (FAnimWeight::IsRelevant(ActualAlpha))
	{
		const USkeleton* Skeleton = Output.AnimInstanceProxy->GetSkeleton();
		const FBoneContainer& RequiredBones = Output.AnimInstanceProxy->GetRequiredBones();

		bool bIsValidToEvaluate;
		UE_DONT_INLINE_CALL bIsValidToEvaluate = IsValidToEvaluate(Skeleton, RequiredBones);
		if (bIsValidToEvaluate)
		{
			Output.SetNodeIds(CachedContext);

			UE_DONT_INLINE_CALL EvaluateComponentSpaceInternal(Output);

			BoneTransforms.Reset(BoneTransforms.Num());
			UE_DONT_INLINE_CALL EvaluateSkeletalControl_AnyThread(Output, BoneTransforms);

			if (BoneTransforms.Num() > 0)
			{
				const float BlendWeight = FMath::Clamp<float>(ActualAlpha, 0.f, 1.f);
				Output.Pose.LocalBlendCSBoneTransforms(BoneTransforms, BlendWeight);
			}

			// we check NaN when you get out of this function in void FComponentSpacePoseLink::EvaluateComponentSpace(FComponentSpacePoseContext& Output)
		}
	}
}

void FAnimNode_SkeletalControl::ClearValidationVisualWarnings()
{
	ValidationVisualWarningMessage = FText::GetEmpty();
}

FText FAnimNode_SkeletalControl::GetValidationVisualWarningMessage() const
{
	return FText();
}

void FAnimNode_SkeletalControl::UpdateInternal(const FAnimationUpdateContext& Context)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(UpdateInternal)

	ElapsedTime += Context.GetDeltaTime();
	ElapsedTime = FMath::Min(ElapsedTime, RotationTimeSeconds / 2);
}

void FAnimNode_SkeletalControl::UpdateComponentPose_AnyThread(const FAnimationUpdateContext& Context)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(UpdateComponentPose_AnyThread)
	ComponentPose.Update(Context);
}

void FAnimNode_SkeletalControl::EvaluateComponentPose_AnyThread(FComponentSpacePoseContext& Output)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(EvaluateComponentPose_AnyThread)
	// Evaluate the input
	ComponentPose.EvaluateComponentSpace(Output);
}

void FAnimNode_SkeletalControl::EvaluateComponentSpaceInternal(FComponentSpacePoseContext& Context)
{
}

void FAnimNode_SkeletalControl::EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(EvaluateSkeletalControl_AnyThread)

	if (!TargetBone.IsValidToEvaluate(Output.Pose.GetPose().GetBoneContainer()))
	{
		return;
	}

	float NormalizedTime = ElapsedTime / RotationTimeSeconds;
	float EaseAlpha = FMath::Sin(PI * NormalizedTime);


	FVector CurrentRotationValue = MaxRotationValue * EaseAlpha;

	//float CurrentRotationDegreesX = MaxRotationDegreesX * EaseAlpha;
	//float CurrentRotationDegreesY = MaxRotationDegreesY * EaseAlpha;
	//float CurrentRotationDegreesZ = MaxRotationDegreesZ * EaseAlpha;

	FCompactPoseBoneIndex BoneIndex = TargetBone.GetCompactPoseIndex(Output.Pose.GetPose().GetBoneContainer());
	FTransform BoneTransform = Output.Pose.GetComponentSpaceTransform(BoneIndex);

	FRotator DeltaRotator(CurrentRotationValue.X, CurrentRotationValue.Y, CurrentRotationValue.Z);
	FQuat DeltaRotation = DeltaRotator.Quaternion();

	BoneTransform.ConcatenateRotation(DeltaRotation);
	BoneTransform.NormalizeRotation();

	OutBoneTransforms.Add(FBoneTransform(BoneIndex, BoneTransform));
}

bool FAnimNode_SkeletalControl::IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones)
{
	if (!TargetBone.IsValidToEvaluate(RequiredBones))
	{
		return false;
	}

	return true;
}

void FAnimNode_SkeletalControl::InitializeBoneReferences(const FBoneContainer& RequiredBones)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(InitializeBoneReferences)
	TargetBone.Initialize(RequiredBones);
}
