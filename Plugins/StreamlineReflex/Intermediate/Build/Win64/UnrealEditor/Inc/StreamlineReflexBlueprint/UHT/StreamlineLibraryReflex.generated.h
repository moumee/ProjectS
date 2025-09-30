// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "StreamlineLibraryReflex.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
enum class EStreamlineFeatureSupport : uint8;
enum class EStreamlineReflexMode : uint8;
#ifdef STREAMLINEREFLEXBLUEPRINT_StreamlineLibraryReflex_generated_h
#error "StreamlineLibraryReflex.generated.h already included, missing '#pragma once' in StreamlineLibraryReflex.h"
#endif
#define STREAMLINEREFLEXBLUEPRINT_StreamlineLibraryReflex_generated_h

#define FID_Users_Yoony_Desktop_Dev_Project_SuraS_Plugins_StreamlineReflex_Source_StreamlineReflexBlueprint_Public_StreamlineLibraryReflex_h_58_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execGetRenderLatencyInMs); \
	DECLARE_FUNCTION(execGetGameLatencyInMs); \
	DECLARE_FUNCTION(execGetGameToRenderLatencyInMs); \
	DECLARE_FUNCTION(execGetDefaultReflexMode); \
	DECLARE_FUNCTION(execGetSupportedReflexModes); \
	DECLARE_FUNCTION(execIsReflexModeSupported); \
	DECLARE_FUNCTION(execGetReflexMode); \
	DECLARE_FUNCTION(execSetReflexMode); \
	DECLARE_FUNCTION(execQueryReflexSupport); \
	DECLARE_FUNCTION(execIsReflexSupported);


#define FID_Users_Yoony_Desktop_Dev_Project_SuraS_Plugins_StreamlineReflex_Source_StreamlineReflexBlueprint_Public_StreamlineLibraryReflex_h_58_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUStreamlineLibraryReflex(); \
	friend struct Z_Construct_UClass_UStreamlineLibraryReflex_Statics; \
public: \
	DECLARE_CLASS(UStreamlineLibraryReflex, UBlueprintFunctionLibrary, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/StreamlineReflexBlueprint"), STREAMLINEREFLEXBLUEPRINT_API) \
	DECLARE_SERIALIZER(UStreamlineLibraryReflex)


#define FID_Users_Yoony_Desktop_Dev_Project_SuraS_Plugins_StreamlineReflex_Source_StreamlineReflexBlueprint_Public_StreamlineLibraryReflex_h_58_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	STREAMLINEREFLEXBLUEPRINT_API UStreamlineLibraryReflex(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UStreamlineLibraryReflex(UStreamlineLibraryReflex&&); \
	UStreamlineLibraryReflex(const UStreamlineLibraryReflex&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(STREAMLINEREFLEXBLUEPRINT_API, UStreamlineLibraryReflex); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UStreamlineLibraryReflex); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UStreamlineLibraryReflex) \
	STREAMLINEREFLEXBLUEPRINT_API virtual ~UStreamlineLibraryReflex();


#define FID_Users_Yoony_Desktop_Dev_Project_SuraS_Plugins_StreamlineReflex_Source_StreamlineReflexBlueprint_Public_StreamlineLibraryReflex_h_53_PROLOG
#define FID_Users_Yoony_Desktop_Dev_Project_SuraS_Plugins_StreamlineReflex_Source_StreamlineReflexBlueprint_Public_StreamlineLibraryReflex_h_58_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_Yoony_Desktop_Dev_Project_SuraS_Plugins_StreamlineReflex_Source_StreamlineReflexBlueprint_Public_StreamlineLibraryReflex_h_58_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Users_Yoony_Desktop_Dev_Project_SuraS_Plugins_StreamlineReflex_Source_StreamlineReflexBlueprint_Public_StreamlineLibraryReflex_h_58_INCLASS_NO_PURE_DECLS \
	FID_Users_Yoony_Desktop_Dev_Project_SuraS_Plugins_StreamlineReflex_Source_StreamlineReflexBlueprint_Public_StreamlineLibraryReflex_h_58_ENHANCED_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> STREAMLINEREFLEXBLUEPRINT_API UClass* StaticClass<class UStreamlineLibraryReflex>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_Yoony_Desktop_Dev_Project_SuraS_Plugins_StreamlineReflex_Source_StreamlineReflexBlueprint_Public_StreamlineLibraryReflex_h


#define FOREACH_ENUM_ESTREAMLINEREFLEXMODE(op) \
	op(EStreamlineReflexMode::Off) \
	op(EStreamlineReflexMode::Enabled) \
	op(EStreamlineReflexMode::Boost) 

enum class EStreamlineReflexMode : uint8;
template<> struct TIsUEnumClass<EStreamlineReflexMode> { enum { Value = true }; };
template<> STREAMLINEREFLEXBLUEPRINT_API UEnum* StaticEnum<EStreamlineReflexMode>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
