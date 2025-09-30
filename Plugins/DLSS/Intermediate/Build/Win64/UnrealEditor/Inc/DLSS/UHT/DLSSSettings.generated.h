// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "DLSSSettings.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef DLSS_DLSSSettings_generated_h
#error "DLSSSettings.generated.h already included, missing '#pragma once' in DLSSSettings.h"
#endif
#define DLSS_DLSSSettings_generated_h

#define FID_Users_Yoony_Desktop_Dev_Project_SuraS_Plugins_DLSS_Source_DLSS_Public_DLSSSettings_h_81_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUDLSSOverrideSettings(); \
	friend struct Z_Construct_UClass_UDLSSOverrideSettings_Statics; \
public: \
	DECLARE_CLASS(UDLSSOverrideSettings, UObject, COMPILED_IN_FLAGS(0 | CLASS_ProjectUserConfig | CLASS_Config), CASTCLASS_None, TEXT("/Script/DLSS"), NO_API) \
	DECLARE_SERIALIZER(UDLSSOverrideSettings) \
	static const TCHAR* StaticConfigName() {return TEXT("Engine");} \



#define FID_Users_Yoony_Desktop_Dev_Project_SuraS_Plugins_DLSS_Source_DLSS_Public_DLSSSettings_h_81_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UDLSSOverrideSettings(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UDLSSOverrideSettings(UDLSSOverrideSettings&&); \
	UDLSSOverrideSettings(const UDLSSOverrideSettings&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UDLSSOverrideSettings); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UDLSSOverrideSettings); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UDLSSOverrideSettings) \
	NO_API virtual ~UDLSSOverrideSettings();


#define FID_Users_Yoony_Desktop_Dev_Project_SuraS_Plugins_DLSS_Source_DLSS_Public_DLSSSettings_h_76_PROLOG
#define FID_Users_Yoony_Desktop_Dev_Project_SuraS_Plugins_DLSS_Source_DLSS_Public_DLSSSettings_h_81_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_Yoony_Desktop_Dev_Project_SuraS_Plugins_DLSS_Source_DLSS_Public_DLSSSettings_h_81_INCLASS_NO_PURE_DECLS \
	FID_Users_Yoony_Desktop_Dev_Project_SuraS_Plugins_DLSS_Source_DLSS_Public_DLSSSettings_h_81_ENHANCED_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> DLSS_API UClass* StaticClass<class UDLSSOverrideSettings>();

#define FID_Users_Yoony_Desktop_Dev_Project_SuraS_Plugins_DLSS_Source_DLSS_Public_DLSSSettings_h_103_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUDLSSSettings(); \
	friend struct Z_Construct_UClass_UDLSSSettings_Statics; \
public: \
	DECLARE_CLASS(UDLSSSettings, UObject, COMPILED_IN_FLAGS(0 | CLASS_DefaultConfig | CLASS_Config), CASTCLASS_None, TEXT("/Script/DLSS"), NO_API) \
	DECLARE_SERIALIZER(UDLSSSettings) \
	static const TCHAR* StaticConfigName() {return TEXT("Engine");} \



#define FID_Users_Yoony_Desktop_Dev_Project_SuraS_Plugins_DLSS_Source_DLSS_Public_DLSSSettings_h_103_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UDLSSSettings(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UDLSSSettings(UDLSSSettings&&); \
	UDLSSSettings(const UDLSSSettings&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UDLSSSettings); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UDLSSSettings); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UDLSSSettings) \
	NO_API virtual ~UDLSSSettings();


#define FID_Users_Yoony_Desktop_Dev_Project_SuraS_Plugins_DLSS_Source_DLSS_Public_DLSSSettings_h_100_PROLOG
#define FID_Users_Yoony_Desktop_Dev_Project_SuraS_Plugins_DLSS_Source_DLSS_Public_DLSSSettings_h_103_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_Yoony_Desktop_Dev_Project_SuraS_Plugins_DLSS_Source_DLSS_Public_DLSSSettings_h_103_INCLASS_NO_PURE_DECLS \
	FID_Users_Yoony_Desktop_Dev_Project_SuraS_Plugins_DLSS_Source_DLSS_Public_DLSSSettings_h_103_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> DLSS_API UClass* StaticClass<class UDLSSSettings>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_Yoony_Desktop_Dev_Project_SuraS_Plugins_DLSS_Source_DLSS_Public_DLSSSettings_h


#define FOREACH_ENUM_EDLSSSETTINGOVERRIDE(op) \
	op(EDLSSSettingOverride::Enabled) \
	op(EDLSSSettingOverride::Disabled) \
	op(EDLSSSettingOverride::UseProjectSettings) 

enum class EDLSSSettingOverride : uint8;
template<> struct TIsUEnumClass<EDLSSSettingOverride> { enum { Value = true }; };
template<> DLSS_API UEnum* StaticEnum<EDLSSSettingOverride>();

#define FOREACH_ENUM_EDLSSPRESET(op) \
	op(EDLSSPreset::Default) \
	op(EDLSSPreset::A) \
	op(EDLSSPreset::B) \
	op(EDLSSPreset::C) \
	op(EDLSSPreset::D) \
	op(EDLSSPreset::E) \
	op(EDLSSPreset::F) \
	op(EDLSSPreset::G) \
	op(EDLSSPreset::H) \
	op(EDLSSPreset::I) \
	op(EDLSSPreset::J) \
	op(EDLSSPreset::K) \
	op(EDLSSPreset::L) \
	op(EDLSSPreset::M) \
	op(EDLSSPreset::N) \
	op(EDLSSPreset::O) 

enum class EDLSSPreset : uint8;
template<> struct TIsUEnumClass<EDLSSPreset> { enum { Value = true }; };
template<> DLSS_API UEnum* StaticEnum<EDLSSPreset>();

#define FOREACH_ENUM_EDLSSRRPRESET(op) \
	op(EDLSSRRPreset::Default) \
	op(EDLSSRRPreset::A) \
	op(EDLSSRRPreset::B) \
	op(EDLSSRRPreset::C) \
	op(EDLSSRRPreset::D) \
	op(EDLSSRRPreset::E) \
	op(EDLSSRRPreset::F) \
	op(EDLSSRRPreset::G) \
	op(EDLSSRRPreset::H) \
	op(EDLSSRRPreset::I) \
	op(EDLSSRRPreset::J) \
	op(EDLSSRRPreset::K) \
	op(EDLSSRRPreset::L) \
	op(EDLSSRRPreset::M) \
	op(EDLSSRRPreset::N) \
	op(EDLSSRRPreset::O) 

enum class EDLSSRRPreset : uint8;
template<> struct TIsUEnumClass<EDLSSRRPreset> { enum { Value = true }; };
template<> DLSS_API UEnum* StaticEnum<EDLSSRRPreset>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
