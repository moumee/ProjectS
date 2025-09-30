// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "StreamlineLibraryDLSSG.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
enum class EStreamlineDLSSGMode : uint8;
enum class EStreamlineFeatureSupport : uint8;
#ifdef STREAMLINEDLSSGBLUEPRINT_StreamlineLibraryDLSSG_generated_h
#error "StreamlineLibraryDLSSG.generated.h already included, missing '#pragma once' in StreamlineLibraryDLSSG.h"
#endif
#define STREAMLINEDLSSGBLUEPRINT_StreamlineLibraryDLSSG_generated_h

#define FID_HostProject_Plugins_StreamlineDLSSG_Source_StreamlineDLSSGBlueprint_Public_StreamlineLibraryDLSSG_h_48_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execGetDLSSGFrameTiming); \
	DECLARE_FUNCTION(execGetDefaultDLSSGMode); \
	DECLARE_FUNCTION(execGetDLSSGMode); \
	DECLARE_FUNCTION(execSetDLSSGMode); \
	DECLARE_FUNCTION(execGetSupportedDLSSGModes); \
	DECLARE_FUNCTION(execIsDLSSGModeSupported); \
	DECLARE_FUNCTION(execQueryDLSSGSupport); \
	DECLARE_FUNCTION(execIsDLSSGSupported);


#define FID_HostProject_Plugins_StreamlineDLSSG_Source_StreamlineDLSSGBlueprint_Public_StreamlineLibraryDLSSG_h_48_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUStreamlineLibraryDLSSG(); \
	friend struct Z_Construct_UClass_UStreamlineLibraryDLSSG_Statics; \
public: \
	DECLARE_CLASS(UStreamlineLibraryDLSSG, UBlueprintFunctionLibrary, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/StreamlineDLSSGBlueprint"), STREAMLINEDLSSGBLUEPRINT_API) \
	DECLARE_SERIALIZER(UStreamlineLibraryDLSSG)


#define FID_HostProject_Plugins_StreamlineDLSSG_Source_StreamlineDLSSGBlueprint_Public_StreamlineLibraryDLSSG_h_48_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	STREAMLINEDLSSGBLUEPRINT_API UStreamlineLibraryDLSSG(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UStreamlineLibraryDLSSG(UStreamlineLibraryDLSSG&&); \
	UStreamlineLibraryDLSSG(const UStreamlineLibraryDLSSG&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(STREAMLINEDLSSGBLUEPRINT_API, UStreamlineLibraryDLSSG); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UStreamlineLibraryDLSSG); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UStreamlineLibraryDLSSG) \
	STREAMLINEDLSSGBLUEPRINT_API virtual ~UStreamlineLibraryDLSSG();


#define FID_HostProject_Plugins_StreamlineDLSSG_Source_StreamlineDLSSGBlueprint_Public_StreamlineLibraryDLSSG_h_44_PROLOG
#define FID_HostProject_Plugins_StreamlineDLSSG_Source_StreamlineDLSSGBlueprint_Public_StreamlineLibraryDLSSG_h_48_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_HostProject_Plugins_StreamlineDLSSG_Source_StreamlineDLSSGBlueprint_Public_StreamlineLibraryDLSSG_h_48_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_HostProject_Plugins_StreamlineDLSSG_Source_StreamlineDLSSGBlueprint_Public_StreamlineLibraryDLSSG_h_48_INCLASS_NO_PURE_DECLS \
	FID_HostProject_Plugins_StreamlineDLSSG_Source_StreamlineDLSSGBlueprint_Public_StreamlineLibraryDLSSG_h_48_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> STREAMLINEDLSSGBLUEPRINT_API UClass* StaticClass<class UStreamlineLibraryDLSSG>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_HostProject_Plugins_StreamlineDLSSG_Source_StreamlineDLSSGBlueprint_Public_StreamlineLibraryDLSSG_h


#define FOREACH_ENUM_ESTREAMLINEDLSSGMODE(op) \
	op(EStreamlineDLSSGMode::Off) \
	op(EStreamlineDLSSGMode::Auto) \
	op(EStreamlineDLSSGMode::On2X) \
	op(EStreamlineDLSSGMode::On3X) \
	op(EStreamlineDLSSGMode::On4X) 

enum class EStreamlineDLSSGMode : uint8;
template<> struct TIsUEnumClass<EStreamlineDLSSGMode> { enum { Value = true }; };
template<> STREAMLINEDLSSGBLUEPRINT_API UEnum* StaticEnum<EStreamlineDLSSGMode>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
