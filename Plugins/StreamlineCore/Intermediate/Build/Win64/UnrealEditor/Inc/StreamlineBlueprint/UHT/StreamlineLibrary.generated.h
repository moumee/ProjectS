// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "StreamlineLibrary.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
enum class EStreamlineFeature : uint8;
enum class EStreamlineFeatureRequirementsFlags : uint8;
enum class EStreamlineFeatureSupport : uint8;
struct FStreamlineFeatureRequirements;
#ifdef STREAMLINEBLUEPRINT_StreamlineLibrary_generated_h
#error "StreamlineLibrary.generated.h already included, missing '#pragma once' in StreamlineLibrary.h"
#endif
#define STREAMLINEBLUEPRINT_StreamlineLibrary_generated_h

#define FID_Github_SuraS_Plugins_StreamlineCore_Source_StreamlineBlueprint_Public_StreamlineLibrary_h_76_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FStreamlineVersion_Statics; \
	STREAMLINEBLUEPRINT_API static class UScriptStruct* StaticStruct();


template<> STREAMLINEBLUEPRINT_API UScriptStruct* StaticStruct<struct FStreamlineVersion>();

#define FID_Github_SuraS_Plugins_StreamlineCore_Source_StreamlineBlueprint_Public_StreamlineLibrary_h_93_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FStreamlineFeatureRequirements_Statics; \
	STREAMLINEBLUEPRINT_API static class UScriptStruct* StaticStruct();


template<> STREAMLINEBLUEPRINT_API UScriptStruct* StaticStruct<struct FStreamlineFeatureRequirements>();

#define FID_Github_SuraS_Plugins_StreamlineCore_Source_StreamlineBlueprint_Public_StreamlineLibrary_h_118_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execQueryStreamlineFeatureSupport); \
	DECLARE_FUNCTION(execIsStreamlineFeatureSupported); \
	DECLARE_FUNCTION(execBreakStreamlineFeatureRequirements); \
	DECLARE_FUNCTION(execGetStreamlineFeatureInformation);


#define FID_Github_SuraS_Plugins_StreamlineCore_Source_StreamlineBlueprint_Public_StreamlineLibrary_h_118_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUStreamlineLibrary(); \
	friend struct Z_Construct_UClass_UStreamlineLibrary_Statics; \
public: \
	DECLARE_CLASS(UStreamlineLibrary, UBlueprintFunctionLibrary, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/StreamlineBlueprint"), STREAMLINEBLUEPRINT_API) \
	DECLARE_SERIALIZER(UStreamlineLibrary)


#define FID_Github_SuraS_Plugins_StreamlineCore_Source_StreamlineBlueprint_Public_StreamlineLibrary_h_118_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	STREAMLINEBLUEPRINT_API UStreamlineLibrary(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UStreamlineLibrary(UStreamlineLibrary&&); \
	UStreamlineLibrary(const UStreamlineLibrary&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(STREAMLINEBLUEPRINT_API, UStreamlineLibrary); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UStreamlineLibrary); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UStreamlineLibrary) \
	STREAMLINEBLUEPRINT_API virtual ~UStreamlineLibrary();


#define FID_Github_SuraS_Plugins_StreamlineCore_Source_StreamlineBlueprint_Public_StreamlineLibrary_h_114_PROLOG
#define FID_Github_SuraS_Plugins_StreamlineCore_Source_StreamlineBlueprint_Public_StreamlineLibrary_h_118_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Github_SuraS_Plugins_StreamlineCore_Source_StreamlineBlueprint_Public_StreamlineLibrary_h_118_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Github_SuraS_Plugins_StreamlineCore_Source_StreamlineBlueprint_Public_StreamlineLibrary_h_118_INCLASS_NO_PURE_DECLS \
	FID_Github_SuraS_Plugins_StreamlineCore_Source_StreamlineBlueprint_Public_StreamlineLibrary_h_118_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> STREAMLINEBLUEPRINT_API UClass* StaticClass<class UStreamlineLibrary>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Github_SuraS_Plugins_StreamlineCore_Source_StreamlineBlueprint_Public_StreamlineLibrary_h


#define FOREACH_ENUM_ESTREAMLINEFEATURE(op) \
	op(EStreamlineFeature::DLSSG) \
	op(EStreamlineFeature::Latewarp) \
	op(EStreamlineFeature::Reflex) \
	op(EStreamlineFeature::DeepDVC) \
	op(EStreamlineFeature::Count) 

enum class EStreamlineFeature : uint8;
template<> struct TIsUEnumClass<EStreamlineFeature> { enum { Value = true }; };
template<> STREAMLINEBLUEPRINT_API UEnum* StaticEnum<EStreamlineFeature>();

#define FOREACH_ENUM_ESTREAMLINEFEATURESUPPORT(op) \
	op(EStreamlineFeatureSupport::Supported) \
	op(EStreamlineFeatureSupport::NotSupported) \
	op(EStreamlineFeatureSupport::NotSupportedIncompatibleHardware) \
	op(EStreamlineFeatureSupport::NotSupportedDriverOutOfDate) \
	op(EStreamlineFeatureSupport::NotSupportedOperatingSystemOutOfDate) \
	op(EStreamlineFeatureSupport::NotSupportedHardewareSchedulingDisabled) \
	op(EStreamlineFeatureSupport::NotSupportedByRHI) \
	op(EStreamlineFeatureSupport::NotSupportedByPlatformAtBuildTime) \
	op(EStreamlineFeatureSupport::NotSupportedIncompatibleAPICaptureToolActive) 

enum class EStreamlineFeatureSupport : uint8;
template<> struct TIsUEnumClass<EStreamlineFeatureSupport> { enum { Value = true }; };
template<> STREAMLINEBLUEPRINT_API UEnum* StaticEnum<EStreamlineFeatureSupport>();

#define FOREACH_ENUM_ESTREAMLINEFEATUREREQUIREMENTSFLAGS(op) \
	op(EStreamlineFeatureRequirementsFlags::None) \
	op(EStreamlineFeatureRequirementsFlags::D3D11Supported) \
	op(EStreamlineFeatureRequirementsFlags::D3D12Supported) \
	op(EStreamlineFeatureRequirementsFlags::VulkanSupported) \
	op(EStreamlineFeatureRequirementsFlags::VSyncOffRequired) \
	op(EStreamlineFeatureRequirementsFlags::HardwareSchedulingRequired) 

enum class EStreamlineFeatureRequirementsFlags : uint8;
template<> struct TIsUEnumClass<EStreamlineFeatureRequirementsFlags> { enum { Value = true }; };
template<> STREAMLINEBLUEPRINT_API UEnum* StaticEnum<EStreamlineFeatureRequirementsFlags>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
