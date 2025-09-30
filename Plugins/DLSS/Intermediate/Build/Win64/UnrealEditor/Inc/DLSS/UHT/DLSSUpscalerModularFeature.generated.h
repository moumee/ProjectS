// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "DLSSUpscalerModularFeature.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef DLSS_DLSSUpscalerModularFeature_generated_h
#error "DLSSUpscalerModularFeature.generated.h already included, missing '#pragma once' in DLSSUpscalerModularFeature.h"
#endif
#define DLSS_DLSSUpscalerModularFeature_generated_h

#define FID_Users_Yoony_Desktop_Dev_Project_SuraS_Plugins_DLSS_Source_DLSS_Private_DLSSUpscalerModularFeature_h_55_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FDLSSUpscalerModularFeatureSettings_Statics; \
	static class UScriptStruct* StaticStruct();


template<> DLSS_API UScriptStruct* StaticStruct<struct FDLSSUpscalerModularFeatureSettings>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_Yoony_Desktop_Dev_Project_SuraS_Plugins_DLSS_Source_DLSS_Private_DLSSUpscalerModularFeature_h


#define FOREACH_ENUM_EDLSSUPSCALERMODULARFEATUREQUALITY(op) \
	op(EDLSSUpscalerModularFeatureQuality::Auto) \
	op(EDLSSUpscalerModularFeatureQuality::UltraQuality) \
	op(EDLSSUpscalerModularFeatureQuality::Quality) \
	op(EDLSSUpscalerModularFeatureQuality::Balanced) \
	op(EDLSSUpscalerModularFeatureQuality::Performance) \
	op(EDLSSUpscalerModularFeatureQuality::UltraPerformance) \
	op(EDLSSUpscalerModularFeatureQuality::DLAA) \
	op(EDLSSUpscalerModularFeatureQuality::Count) 

enum class EDLSSUpscalerModularFeatureQuality : uint8;
template<> struct TIsUEnumClass<EDLSSUpscalerModularFeatureQuality> { enum { Value = true }; };
template<> DLSS_API UEnum* StaticEnum<EDLSSUpscalerModularFeatureQuality>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
