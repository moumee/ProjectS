// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "DLSS/Private/DLSSUpscalerModularFeature.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeDLSSUpscalerModularFeature() {}

// Begin Cross Module References
DLSS_API UEnum* Z_Construct_UEnum_DLSS_EDLSSUpscalerModularFeatureQuality();
DLSS_API UScriptStruct* Z_Construct_UScriptStruct_FDLSSUpscalerModularFeatureSettings();
UPackage* Z_Construct_UPackage__Script_DLSS();
// End Cross Module References

// Begin Enum EDLSSUpscalerModularFeatureQuality
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EDLSSUpscalerModularFeatureQuality;
static UEnum* EDLSSUpscalerModularFeatureQuality_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EDLSSUpscalerModularFeatureQuality.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EDLSSUpscalerModularFeatureQuality.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_DLSS_EDLSSUpscalerModularFeatureQuality, (UObject*)Z_Construct_UPackage__Script_DLSS(), TEXT("EDLSSUpscalerModularFeatureQuality"));
	}
	return Z_Registration_Info_UEnum_EDLSSUpscalerModularFeatureQuality.OuterSingleton;
}
template<> DLSS_API UEnum* StaticEnum<EDLSSUpscalerModularFeatureQuality>()
{
	return EDLSSUpscalerModularFeatureQuality_StaticEnum();
}
struct Z_Construct_UEnum_DLSS_EDLSSUpscalerModularFeatureQuality_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "Auto.DisplayName", "Auto" },
		{ "Auto.Name", "EDLSSUpscalerModularFeatureQuality::Auto" },
		{ "Auto.ToolTip", "Use Auto to select best quality setting for a given resolution" },
		{ "Balanced.DisplayName", "Balanced" },
		{ "Balanced.Name", "EDLSSUpscalerModularFeatureQuality::Balanced" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** DLSS Quality modes. */" },
#endif
		{ "Count.Hidden", "" },
		{ "Count.Name", "EDLSSUpscalerModularFeatureQuality::Count" },
		{ "DLAA.DisplayName", "DLAA" },
		{ "DLAA.Name", "EDLSSUpscalerModularFeatureQuality::DLAA" },
		{ "ModuleRelativePath", "Private/DLSSUpscalerModularFeature.h" },
		{ "Performance.DisplayName", "Performance" },
		{ "Performance.Name", "EDLSSUpscalerModularFeatureQuality::Performance" },
		{ "Quality.DisplayName", "Quality" },
		{ "Quality.Name", "EDLSSUpscalerModularFeatureQuality::Quality" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "DLSS Quality modes." },
#endif
		{ "UltraPerformance.DisplayName", "Ultra Performance" },
		{ "UltraPerformance.Name", "EDLSSUpscalerModularFeatureQuality::UltraPerformance" },
		{ "UltraQuality.DisplayName", "Ultra Quality" },
		{ "UltraQuality.Name", "EDLSSUpscalerModularFeatureQuality::UltraQuality" },
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EDLSSUpscalerModularFeatureQuality::Auto", (int64)EDLSSUpscalerModularFeatureQuality::Auto },
		{ "EDLSSUpscalerModularFeatureQuality::UltraQuality", (int64)EDLSSUpscalerModularFeatureQuality::UltraQuality },
		{ "EDLSSUpscalerModularFeatureQuality::Quality", (int64)EDLSSUpscalerModularFeatureQuality::Quality },
		{ "EDLSSUpscalerModularFeatureQuality::Balanced", (int64)EDLSSUpscalerModularFeatureQuality::Balanced },
		{ "EDLSSUpscalerModularFeatureQuality::Performance", (int64)EDLSSUpscalerModularFeatureQuality::Performance },
		{ "EDLSSUpscalerModularFeatureQuality::UltraPerformance", (int64)EDLSSUpscalerModularFeatureQuality::UltraPerformance },
		{ "EDLSSUpscalerModularFeatureQuality::DLAA", (int64)EDLSSUpscalerModularFeatureQuality::DLAA },
		{ "EDLSSUpscalerModularFeatureQuality::Count", (int64)EDLSSUpscalerModularFeatureQuality::Count },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_DLSS_EDLSSUpscalerModularFeatureQuality_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_DLSS,
	nullptr,
	"EDLSSUpscalerModularFeatureQuality",
	"EDLSSUpscalerModularFeatureQuality",
	Z_Construct_UEnum_DLSS_EDLSSUpscalerModularFeatureQuality_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_DLSS_EDLSSUpscalerModularFeatureQuality_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_DLSS_EDLSSUpscalerModularFeatureQuality_Statics::Enum_MetaDataParams), Z_Construct_UEnum_DLSS_EDLSSUpscalerModularFeatureQuality_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_DLSS_EDLSSUpscalerModularFeatureQuality()
{
	if (!Z_Registration_Info_UEnum_EDLSSUpscalerModularFeatureQuality.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EDLSSUpscalerModularFeatureQuality.InnerSingleton, Z_Construct_UEnum_DLSS_EDLSSUpscalerModularFeatureQuality_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EDLSSUpscalerModularFeatureQuality.InnerSingleton;
}
// End Enum EDLSSUpscalerModularFeatureQuality

// Begin ScriptStruct FDLSSUpscalerModularFeatureSettings
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_DLSSUpscalerModularFeatureSettings;
class UScriptStruct* FDLSSUpscalerModularFeatureSettings::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_DLSSUpscalerModularFeatureSettings.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_DLSSUpscalerModularFeatureSettings.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FDLSSUpscalerModularFeatureSettings, (UObject*)Z_Construct_UPackage__Script_DLSS(), TEXT("DLSSUpscalerModularFeatureSettings"));
	}
	return Z_Registration_Info_UScriptStruct_DLSSUpscalerModularFeatureSettings.OuterSingleton;
}
template<> DLSS_API UScriptStruct* StaticStruct<FDLSSUpscalerModularFeatureSettings>()
{
	return FDLSSUpscalerModularFeatureSettings::StaticStruct();
}
struct Z_Construct_UScriptStruct_FDLSSUpscalerModularFeatureSettings_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n* DLSS settings used by the Modular Feature.\n*/" },
#endif
		{ "ModuleRelativePath", "Private/DLSSUpscalerModularFeature.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "DLSS settings used by the Modular Feature." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Quality_MetaData[] = {
		{ "Category", "DLSS" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** DLSS quality. */" },
#endif
		{ "DisplayName", "Quality" },
		{ "ModuleRelativePath", "Private/DLSSUpscalerModularFeature.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "DLSS quality." },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_Quality_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_Quality;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FDLSSUpscalerModularFeatureSettings>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UScriptStruct_FDLSSUpscalerModularFeatureSettings_Statics::NewProp_Quality_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UScriptStruct_FDLSSUpscalerModularFeatureSettings_Statics::NewProp_Quality = { "Quality", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FDLSSUpscalerModularFeatureSettings, Quality), Z_Construct_UEnum_DLSS_EDLSSUpscalerModularFeatureQuality, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Quality_MetaData), NewProp_Quality_MetaData) }; // 2317621751
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FDLSSUpscalerModularFeatureSettings_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FDLSSUpscalerModularFeatureSettings_Statics::NewProp_Quality_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FDLSSUpscalerModularFeatureSettings_Statics::NewProp_Quality,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FDLSSUpscalerModularFeatureSettings_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FDLSSUpscalerModularFeatureSettings_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_DLSS,
	nullptr,
	&NewStructOps,
	"DLSSUpscalerModularFeatureSettings",
	Z_Construct_UScriptStruct_FDLSSUpscalerModularFeatureSettings_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FDLSSUpscalerModularFeatureSettings_Statics::PropPointers),
	sizeof(FDLSSUpscalerModularFeatureSettings),
	alignof(FDLSSUpscalerModularFeatureSettings),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FDLSSUpscalerModularFeatureSettings_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FDLSSUpscalerModularFeatureSettings_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FDLSSUpscalerModularFeatureSettings()
{
	if (!Z_Registration_Info_UScriptStruct_DLSSUpscalerModularFeatureSettings.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_DLSSUpscalerModularFeatureSettings.InnerSingleton, Z_Construct_UScriptStruct_FDLSSUpscalerModularFeatureSettings_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_DLSSUpscalerModularFeatureSettings.InnerSingleton;
}
// End ScriptStruct FDLSSUpscalerModularFeatureSettings

// Begin Registration
struct Z_CompiledInDeferFile_FID_Github_SuraS_Plugins_DLSS_Source_DLSS_Private_DLSSUpscalerModularFeature_h_Statics
{
	static constexpr FEnumRegisterCompiledInInfo EnumInfo[] = {
		{ EDLSSUpscalerModularFeatureQuality_StaticEnum, TEXT("EDLSSUpscalerModularFeatureQuality"), &Z_Registration_Info_UEnum_EDLSSUpscalerModularFeatureQuality, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 2317621751U) },
	};
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FDLSSUpscalerModularFeatureSettings::StaticStruct, Z_Construct_UScriptStruct_FDLSSUpscalerModularFeatureSettings_Statics::NewStructOps, TEXT("DLSSUpscalerModularFeatureSettings"), &Z_Registration_Info_UScriptStruct_DLSSUpscalerModularFeatureSettings, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FDLSSUpscalerModularFeatureSettings), 2192163905U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Github_SuraS_Plugins_DLSS_Source_DLSS_Private_DLSSUpscalerModularFeature_h_3124613378(TEXT("/Script/DLSS"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Github_SuraS_Plugins_DLSS_Source_DLSS_Private_DLSSUpscalerModularFeature_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Github_SuraS_Plugins_DLSS_Source_DLSS_Private_DLSSUpscalerModularFeature_h_Statics::ScriptStructInfo),
	Z_CompiledInDeferFile_FID_Github_SuraS_Plugins_DLSS_Source_DLSS_Private_DLSSUpscalerModularFeature_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Github_SuraS_Plugins_DLSS_Source_DLSS_Private_DLSSUpscalerModularFeature_h_Statics::EnumInfo));
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
