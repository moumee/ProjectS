// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "DLSS/Public/DLSSSettings.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeDLSSSettings() {}

// Begin Cross Module References
COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
DLSS_API UClass* Z_Construct_UClass_UDLSSOverrideSettings();
DLSS_API UClass* Z_Construct_UClass_UDLSSOverrideSettings_NoRegister();
DLSS_API UClass* Z_Construct_UClass_UDLSSSettings();
DLSS_API UClass* Z_Construct_UClass_UDLSSSettings_NoRegister();
DLSS_API UEnum* Z_Construct_UEnum_DLSS_EDLSSPreset();
DLSS_API UEnum* Z_Construct_UEnum_DLSS_EDLSSRRPreset();
DLSS_API UEnum* Z_Construct_UEnum_DLSS_EDLSSSettingOverride();
UPackage* Z_Construct_UPackage__Script_DLSS();
// End Cross Module References

// Begin Enum EDLSSSettingOverride
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EDLSSSettingOverride;
static UEnum* EDLSSSettingOverride_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EDLSSSettingOverride.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EDLSSSettingOverride.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_DLSS_EDLSSSettingOverride, (UObject*)Z_Construct_UPackage__Script_DLSS(), TEXT("EDLSSSettingOverride"));
	}
	return Z_Registration_Info_UEnum_EDLSSSettingOverride.OuterSingleton;
}
template<> DLSS_API UEnum* StaticEnum<EDLSSSettingOverride>()
{
	return EDLSSSettingOverride_StaticEnum();
}
struct Z_Construct_UEnum_DLSS_EDLSSSettingOverride_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "Disabled.DisplayName", "False" },
		{ "Disabled.Name", "EDLSSSettingOverride::Disabled" },
		{ "Enabled.DisplayName", "True" },
		{ "Enabled.Name", "EDLSSSettingOverride::Enabled" },
		{ "ModuleRelativePath", "Public/DLSSSettings.h" },
		{ "UseProjectSettings.DisplayName", "Use project settings" },
		{ "UseProjectSettings.Name", "EDLSSSettingOverride::UseProjectSettings" },
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EDLSSSettingOverride::Enabled", (int64)EDLSSSettingOverride::Enabled },
		{ "EDLSSSettingOverride::Disabled", (int64)EDLSSSettingOverride::Disabled },
		{ "EDLSSSettingOverride::UseProjectSettings", (int64)EDLSSSettingOverride::UseProjectSettings },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_DLSS_EDLSSSettingOverride_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_DLSS,
	nullptr,
	"EDLSSSettingOverride",
	"EDLSSSettingOverride",
	Z_Construct_UEnum_DLSS_EDLSSSettingOverride_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_DLSS_EDLSSSettingOverride_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_DLSS_EDLSSSettingOverride_Statics::Enum_MetaDataParams), Z_Construct_UEnum_DLSS_EDLSSSettingOverride_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_DLSS_EDLSSSettingOverride()
{
	if (!Z_Registration_Info_UEnum_EDLSSSettingOverride.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EDLSSSettingOverride.InnerSingleton, Z_Construct_UEnum_DLSS_EDLSSSettingOverride_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EDLSSSettingOverride.InnerSingleton;
}
// End Enum EDLSSSettingOverride

// Begin Enum EDLSSPreset
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EDLSSPreset;
static UEnum* EDLSSPreset_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EDLSSPreset.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EDLSSPreset.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_DLSS_EDLSSPreset, (UObject*)Z_Construct_UPackage__Script_DLSS(), TEXT("EDLSSPreset"));
	}
	return Z_Registration_Info_UEnum_EDLSSPreset.OuterSingleton;
}
template<> DLSS_API UEnum* StaticEnum<EDLSSPreset>()
{
	return EDLSSPreset_StaticEnum();
}
struct Z_Construct_UEnum_DLSS_EDLSSPreset_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "A.Hidden", "" },
		{ "A.Name", "EDLSSPreset::A" },
		{ "A.ToolTip", "Deprecated, use preset J or K" },
		{ "B.Hidden", "" },
		{ "B.Name", "EDLSSPreset::B" },
		{ "B.ToolTip", "Deprecated, use preset J or K" },
		{ "BlueprintType", "true" },
		{ "C.Hidden", "" },
		{ "C.Name", "EDLSSPreset::C" },
		{ "C.ToolTip", "Deprecated, use preset J or K" },
		{ "D.Hidden", "" },
		{ "D.Name", "EDLSSPreset::D" },
		{ "D.ToolTip", "Deprecated, use preset J or K" },
		{ "Default.Name", "EDLSSPreset::Default" },
		{ "Default.ToolTip", "default behavior, preset specified per DLSS SDK release" },
		{ "E.Hidden", "" },
		{ "E.Name", "EDLSSPreset::E" },
		{ "E.ToolTip", "Deprecated, use preset J or K" },
		{ "F.DisplayName", "Preset F" },
		{ "F.Name", "EDLSSPreset::F" },
		{ "F.ToolTip", "Force preset F, For Ultra Perf/DLAA modes, The default preset for Ultra Perf and DLAA modes." },
		{ "G.Hidden", "" },
		{ "G.Name", "EDLSSPreset::G" },
		{ "G.ToolTip", "Force preset G, Do not use \xe2\x80\x93 reverts to default behavior" },
		{ "H.Hidden", "" },
		{ "H.Name", "EDLSSPreset::H" },
		{ "H.ToolTip", "Force preset H, Do not use \xe2\x80\x93 reverts to default behavior" },
		{ "I.Hidden", "" },
		{ "I.Name", "EDLSSPreset::I" },
		{ "I.ToolTip", "Force preset I, Do not use \xe2\x80\x93 reverts to default behavior" },
		{ "J.DisplayName", "Preset J" },
		{ "J.Name", "EDLSSPreset::J" },
		{ "J.ToolTip", "Force preset J, Similar to preset K. Preset J might exhibit slightly less ghosting at the cost of extra flickering. Preset K is generally recommended over preset J" },
		{ "K.DisplayName", "Preset K" },
		{ "K.Name", "EDLSSPreset::K" },
		{ "K.ToolTip", "Force preset K, Default preset for DLAA/Perf/Balanced/Quality modes that is transformer based. Best image quality preset at a higher performance cost." },
		{ "L.Hidden", "" },
		{ "L.Name", "EDLSSPreset::L" },
		{ "L.ToolTip", "Force preset L, Do not use - Reverts to default behavior" },
		{ "M.Hidden", "" },
		{ "M.Name", "EDLSSPreset::M" },
		{ "M.ToolTip", "Force preset M, Do not use - Reverts to default behavior" },
		{ "MAX.Comment", "//Please add new presets above this line.\n" },
		{ "MAX.Hidden", "" },
		{ "MAX.Name", "EDLSSPreset::MAX" },
		{ "MAX.ToolTip", "Please add new presets above this line." },
		{ "ModuleRelativePath", "Public/DLSSSettings.h" },
		{ "N.Hidden", "" },
		{ "N.Name", "EDLSSPreset::N" },
		{ "N.ToolTip", "Force preset N, Do not use - Reverts to default behavior" },
		{ "O.Hidden", "" },
		{ "O.Name", "EDLSSPreset::O" },
		{ "O.ToolTip", "Force preset O, Do not use - Reverts to default behavior" },
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EDLSSPreset::Default", (int64)EDLSSPreset::Default },
		{ "EDLSSPreset::A", (int64)EDLSSPreset::A },
		{ "EDLSSPreset::B", (int64)EDLSSPreset::B },
		{ "EDLSSPreset::C", (int64)EDLSSPreset::C },
		{ "EDLSSPreset::D", (int64)EDLSSPreset::D },
		{ "EDLSSPreset::E", (int64)EDLSSPreset::E },
		{ "EDLSSPreset::F", (int64)EDLSSPreset::F },
		{ "EDLSSPreset::G", (int64)EDLSSPreset::G },
		{ "EDLSSPreset::H", (int64)EDLSSPreset::H },
		{ "EDLSSPreset::I", (int64)EDLSSPreset::I },
		{ "EDLSSPreset::J", (int64)EDLSSPreset::J },
		{ "EDLSSPreset::K", (int64)EDLSSPreset::K },
		{ "EDLSSPreset::L", (int64)EDLSSPreset::L },
		{ "EDLSSPreset::M", (int64)EDLSSPreset::M },
		{ "EDLSSPreset::N", (int64)EDLSSPreset::N },
		{ "EDLSSPreset::O", (int64)EDLSSPreset::O },
		{ "EDLSSPreset::MAX", (int64)EDLSSPreset::MAX },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_DLSS_EDLSSPreset_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_DLSS,
	nullptr,
	"EDLSSPreset",
	"EDLSSPreset",
	Z_Construct_UEnum_DLSS_EDLSSPreset_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_DLSS_EDLSSPreset_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_DLSS_EDLSSPreset_Statics::Enum_MetaDataParams), Z_Construct_UEnum_DLSS_EDLSSPreset_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_DLSS_EDLSSPreset()
{
	if (!Z_Registration_Info_UEnum_EDLSSPreset.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EDLSSPreset.InnerSingleton, Z_Construct_UEnum_DLSS_EDLSSPreset_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EDLSSPreset.InnerSingleton;
}
// End Enum EDLSSPreset

// Begin Enum EDLSSRRPreset
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EDLSSRRPreset;
static UEnum* EDLSSRRPreset_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EDLSSRRPreset.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EDLSSRRPreset.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_DLSS_EDLSSRRPreset, (UObject*)Z_Construct_UPackage__Script_DLSS(), TEXT("EDLSSRRPreset"));
	}
	return Z_Registration_Info_UEnum_EDLSSRRPreset.OuterSingleton;
}
template<> DLSS_API UEnum* StaticEnum<EDLSSRRPreset>()
{
	return EDLSSRRPreset_StaticEnum();
}
struct Z_Construct_UEnum_DLSS_EDLSSRRPreset_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "A.Hidden", "" },
		{ "A.Name", "EDLSSRRPreset::A" },
		{ "A.ToolTip", "Force preset A" },
		{ "B.Hidden", "" },
		{ "B.Name", "EDLSSRRPreset::B" },
		{ "B.ToolTip", "Force preset B" },
		{ "BlueprintType", "true" },
		{ "C.Hidden", "" },
		{ "C.Name", "EDLSSRRPreset::C" },
		{ "C.ToolTip", "Force preset C" },
		{ "D.DisplayName", "Preset D" },
		{ "D.Name", "EDLSSRRPreset::D" },
		{ "D.ToolTip", "Force preset D, Default model (transformer)" },
		{ "Default.Name", "EDLSSRRPreset::Default" },
		{ "Default.ToolTip", "Default behavior, may or may not change after OTA" },
		{ "E.DisplayName", "Preset E" },
		{ "E.Name", "EDLSSRRPreset::E" },
		{ "E.ToolTip", "Force preset E, Latest transformer model (must use if DoF guide is needed)" },
		{ "F.Hidden", "" },
		{ "F.Name", "EDLSSRRPreset::F" },
		{ "F.ToolTip", "Force preset F, Do not use \xe2\x80\x93 reverts to default behavior" },
		{ "G.Hidden", "" },
		{ "G.Name", "EDLSSRRPreset::G" },
		{ "G.ToolTip", "Force preset G, Do not use \xe2\x80\x93 reverts to default behavior" },
		{ "H.Hidden", "" },
		{ "H.Name", "EDLSSRRPreset::H" },
		{ "H.ToolTip", "Force preset H, Do not use \xe2\x80\x93 reverts to default behavior" },
		{ "I.Hidden", "" },
		{ "I.Name", "EDLSSRRPreset::I" },
		{ "I.ToolTip", "Force preset I, Do not use \xe2\x80\x93 reverts to default behavior" },
		{ "J.Hidden", "" },
		{ "J.Name", "EDLSSRRPreset::J" },
		{ "J.ToolTip", "Force preset J, Do not use \xe2\x80\x93 reverts to default behavior" },
		{ "K.Hidden", "" },
		{ "K.Name", "EDLSSRRPreset::K" },
		{ "K.ToolTip", "Force preset K, Do not use \xe2\x80\x93 reverts to default behavior" },
		{ "L.Hidden", "" },
		{ "L.Name", "EDLSSRRPreset::L" },
		{ "L.ToolTip", "Force preset L, Do not use \xe2\x80\x93 reverts to default behavior" },
		{ "M.Hidden", "" },
		{ "M.Name", "EDLSSRRPreset::M" },
		{ "M.ToolTip", "Force preset M, Do not use \xe2\x80\x93 reverts to default behavior" },
		{ "MAX.Comment", "//Please add new presets above this line.\n" },
		{ "MAX.Hidden", "" },
		{ "MAX.Name", "EDLSSRRPreset::MAX" },
		{ "MAX.ToolTip", "Please add new presets above this line." },
		{ "ModuleRelativePath", "Public/DLSSSettings.h" },
		{ "N.Hidden", "" },
		{ "N.Name", "EDLSSRRPreset::N" },
		{ "N.ToolTip", "Force preset N, Do not use \xe2\x80\x93 reverts to default behavior" },
		{ "O.Hidden", "" },
		{ "O.Name", "EDLSSRRPreset::O" },
		{ "O.ToolTip", "Force preset O, Do not use \xe2\x80\x93 reverts to default behavior" },
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EDLSSRRPreset::Default", (int64)EDLSSRRPreset::Default },
		{ "EDLSSRRPreset::A", (int64)EDLSSRRPreset::A },
		{ "EDLSSRRPreset::B", (int64)EDLSSRRPreset::B },
		{ "EDLSSRRPreset::C", (int64)EDLSSRRPreset::C },
		{ "EDLSSRRPreset::D", (int64)EDLSSRRPreset::D },
		{ "EDLSSRRPreset::E", (int64)EDLSSRRPreset::E },
		{ "EDLSSRRPreset::F", (int64)EDLSSRRPreset::F },
		{ "EDLSSRRPreset::G", (int64)EDLSSRRPreset::G },
		{ "EDLSSRRPreset::H", (int64)EDLSSRRPreset::H },
		{ "EDLSSRRPreset::I", (int64)EDLSSRRPreset::I },
		{ "EDLSSRRPreset::J", (int64)EDLSSRRPreset::J },
		{ "EDLSSRRPreset::K", (int64)EDLSSRRPreset::K },
		{ "EDLSSRRPreset::L", (int64)EDLSSRRPreset::L },
		{ "EDLSSRRPreset::M", (int64)EDLSSRRPreset::M },
		{ "EDLSSRRPreset::N", (int64)EDLSSRRPreset::N },
		{ "EDLSSRRPreset::O", (int64)EDLSSRRPreset::O },
		{ "EDLSSRRPreset::MAX", (int64)EDLSSRRPreset::MAX },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_DLSS_EDLSSRRPreset_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_DLSS,
	nullptr,
	"EDLSSRRPreset",
	"EDLSSRRPreset",
	Z_Construct_UEnum_DLSS_EDLSSRRPreset_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_DLSS_EDLSSRRPreset_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_DLSS_EDLSSRRPreset_Statics::Enum_MetaDataParams), Z_Construct_UEnum_DLSS_EDLSSRRPreset_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_DLSS_EDLSSRRPreset()
{
	if (!Z_Registration_Info_UEnum_EDLSSRRPreset.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EDLSSRRPreset.InnerSingleton, Z_Construct_UEnum_DLSS_EDLSSRRPreset_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EDLSSRRPreset.InnerSingleton;
}
// End Enum EDLSSRRPreset

// Begin Class UDLSSOverrideSettings
void UDLSSOverrideSettings::StaticRegisterNativesUDLSSOverrideSettings()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UDLSSOverrideSettings);
UClass* Z_Construct_UClass_UDLSSOverrideSettings_NoRegister()
{
	return UDLSSOverrideSettings::StaticClass();
}
struct Z_Construct_UClass_UDLSSOverrideSettings_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "DLSSSettings.h" },
		{ "ModuleRelativePath", "Public/DLSSSettings.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bShowDLSSIncompatiblePluginsToolsWarnings_MetaData[] = {
		{ "Category", "Editor (Local)" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** This enables warnings about plugins & tools that are incompatible with DLSS/DLAA in the editor. This setting and the project setting both must be set to get warnings */" },
#endif
		{ "DisplayName", "Warn about incompatible plugins and tools" },
		{ "ModuleRelativePath", "Public/DLSSSettings.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "This enables warnings about plugins & tools that are incompatible with DLSS/DLAA in the editor. This setting and the project setting both must be set to get warnings" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ShowDLSSSDebugOnScreenMessages_MetaData[] = {
		{ "Category", "Editor (Local)" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** This enables on screen warnings and errors about DLSS/DLAA  */" },
#endif
		{ "DisplayName", "Show various DLSS/DLAA on screen debug messages" },
		{ "ModuleRelativePath", "Public/DLSSSettings.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "This enables on screen warnings and errors about DLSS/DLAA" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_EnableDLSSInEditorViewportsOverride_MetaData[] = {
		{ "Category", "Editor (Local)" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** This enables DLSS/DLAA in editor viewports. Saved to local user config only.*/" },
#endif
		{ "DisplayName", "Enable DLSS/DLAA to be turned on in Editor viewports" },
		{ "ModuleRelativePath", "Public/DLSSSettings.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "This enables DLSS/DLAA in editor viewports. Saved to local user config only." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_EnableDLSSInPlayInEditorViewportsOverride_MetaData[] = {
		{ "Category", "Editor (Local)" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** This enables DLSS/DLAA in play in editor viewports. Saved to local user config only. */" },
#endif
		{ "DisplayName", "Enable DLSS/DLAA in Play In Editor viewports" },
		{ "ModuleRelativePath", "Public/DLSSSettings.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "This enables DLSS/DLAA in play in editor viewports. Saved to local user config only." },
#endif
	};
#endif // WITH_METADATA
	static void NewProp_bShowDLSSIncompatiblePluginsToolsWarnings_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bShowDLSSIncompatiblePluginsToolsWarnings;
	static const UECodeGen_Private::FBytePropertyParams NewProp_ShowDLSSSDebugOnScreenMessages_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_ShowDLSSSDebugOnScreenMessages;
	static const UECodeGen_Private::FBytePropertyParams NewProp_EnableDLSSInEditorViewportsOverride_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_EnableDLSSInEditorViewportsOverride;
	static const UECodeGen_Private::FBytePropertyParams NewProp_EnableDLSSInPlayInEditorViewportsOverride_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_EnableDLSSInPlayInEditorViewportsOverride;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UDLSSOverrideSettings>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
void Z_Construct_UClass_UDLSSOverrideSettings_Statics::NewProp_bShowDLSSIncompatiblePluginsToolsWarnings_SetBit(void* Obj)
{
	((UDLSSOverrideSettings*)Obj)->bShowDLSSIncompatiblePluginsToolsWarnings = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UDLSSOverrideSettings_Statics::NewProp_bShowDLSSIncompatiblePluginsToolsWarnings = { "bShowDLSSIncompatiblePluginsToolsWarnings", nullptr, (EPropertyFlags)0x0010000000004001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UDLSSOverrideSettings), &Z_Construct_UClass_UDLSSOverrideSettings_Statics::NewProp_bShowDLSSIncompatiblePluginsToolsWarnings_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bShowDLSSIncompatiblePluginsToolsWarnings_MetaData), NewProp_bShowDLSSIncompatiblePluginsToolsWarnings_MetaData) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_UDLSSOverrideSettings_Statics::NewProp_ShowDLSSSDebugOnScreenMessages_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UClass_UDLSSOverrideSettings_Statics::NewProp_ShowDLSSSDebugOnScreenMessages = { "ShowDLSSSDebugOnScreenMessages", nullptr, (EPropertyFlags)0x0010000000004001, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UDLSSOverrideSettings, ShowDLSSSDebugOnScreenMessages), Z_Construct_UEnum_DLSS_EDLSSSettingOverride, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ShowDLSSSDebugOnScreenMessages_MetaData), NewProp_ShowDLSSSDebugOnScreenMessages_MetaData) }; // 2279089454
const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_UDLSSOverrideSettings_Statics::NewProp_EnableDLSSInEditorViewportsOverride_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UClass_UDLSSOverrideSettings_Statics::NewProp_EnableDLSSInEditorViewportsOverride = { "EnableDLSSInEditorViewportsOverride", nullptr, (EPropertyFlags)0x0010000000004001, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UDLSSOverrideSettings, EnableDLSSInEditorViewportsOverride), Z_Construct_UEnum_DLSS_EDLSSSettingOverride, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_EnableDLSSInEditorViewportsOverride_MetaData), NewProp_EnableDLSSInEditorViewportsOverride_MetaData) }; // 2279089454
const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_UDLSSOverrideSettings_Statics::NewProp_EnableDLSSInPlayInEditorViewportsOverride_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UClass_UDLSSOverrideSettings_Statics::NewProp_EnableDLSSInPlayInEditorViewportsOverride = { "EnableDLSSInPlayInEditorViewportsOverride", nullptr, (EPropertyFlags)0x0010000000004001, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UDLSSOverrideSettings, EnableDLSSInPlayInEditorViewportsOverride), Z_Construct_UEnum_DLSS_EDLSSSettingOverride, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_EnableDLSSInPlayInEditorViewportsOverride_MetaData), NewProp_EnableDLSSInPlayInEditorViewportsOverride_MetaData) }; // 2279089454
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UDLSSOverrideSettings_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSOverrideSettings_Statics::NewProp_bShowDLSSIncompatiblePluginsToolsWarnings,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSOverrideSettings_Statics::NewProp_ShowDLSSSDebugOnScreenMessages_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSOverrideSettings_Statics::NewProp_ShowDLSSSDebugOnScreenMessages,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSOverrideSettings_Statics::NewProp_EnableDLSSInEditorViewportsOverride_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSOverrideSettings_Statics::NewProp_EnableDLSSInEditorViewportsOverride,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSOverrideSettings_Statics::NewProp_EnableDLSSInPlayInEditorViewportsOverride_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSOverrideSettings_Statics::NewProp_EnableDLSSInPlayInEditorViewportsOverride,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UDLSSOverrideSettings_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UDLSSOverrideSettings_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UObject,
	(UObject* (*)())Z_Construct_UPackage__Script_DLSS,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UDLSSOverrideSettings_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UDLSSOverrideSettings_Statics::ClassParams = {
	&UDLSSOverrideSettings::StaticClass,
	"Engine",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UDLSSOverrideSettings_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UDLSSOverrideSettings_Statics::PropPointers),
	0,
	0x001000E4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UDLSSOverrideSettings_Statics::Class_MetaDataParams), Z_Construct_UClass_UDLSSOverrideSettings_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UDLSSOverrideSettings()
{
	if (!Z_Registration_Info_UClass_UDLSSOverrideSettings.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UDLSSOverrideSettings.OuterSingleton, Z_Construct_UClass_UDLSSOverrideSettings_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UDLSSOverrideSettings.OuterSingleton;
}
template<> DLSS_API UClass* StaticClass<UDLSSOverrideSettings>()
{
	return UDLSSOverrideSettings::StaticClass();
}
UDLSSOverrideSettings::UDLSSOverrideSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UDLSSOverrideSettings);
UDLSSOverrideSettings::~UDLSSOverrideSettings() {}
// End Class UDLSSOverrideSettings

// Begin Class UDLSSSettings
void UDLSSSettings::StaticRegisterNativesUDLSSSettings()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UDLSSSettings);
UClass* Z_Construct_UClass_UDLSSSettings_NoRegister()
{
	return UDLSSSettings::StaticClass();
}
struct Z_Construct_UClass_UDLSSSettings_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "DisplayName", "NVIDIA DLSS" },
		{ "IncludePath", "DLSSSettings.h" },
		{ "ModuleRelativePath", "Public/DLSSSettings.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bAllowOTAUpdate_MetaData[] = {
		{ "Category", "General Settings" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Allow OTA updates of DLSS models */" },
#endif
		{ "ConfigRestartRequired", "TRUE" },
		{ "DisplayName", "Allow OTA update" },
		{ "ModuleRelativePath", "Public/DLSSSettings.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Allow OTA updates of DLSS models" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_NVIDIANGXApplicationId_MetaData[] = {
		{ "Category", "General Settings" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** By default the DLSS plugin uses the UE Project ID to initialize DLSS. In some cases NVIDIA might provide a separate NVIDIA Application ID, which should be put here. Please refer to https://developer.nvidia.com/dlss for details*/" },
#endif
		{ "ConfigRestartRequired", "TRUE" },
		{ "DisplayName", "NVIDIA NGX Application ID" },
		{ "ModuleRelativePath", "Public/DLSSSettings.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "By default the DLSS plugin uses the UE Project ID to initialize DLSS. In some cases NVIDIA might provide a separate NVIDIA Application ID, which should be put here. Please refer to https://developer.nvidia.com/dlss for details" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_BiasCurrentColorStencilValue_MetaData[] = {
		{ "Category", "General Settings" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** The value that would be considered as Bias Color in the custom depth stencil buffer. Must not be set to 0**/" },
#endif
		{ "DisplayName", "Bias Current Color Custom Stencil Value" },
		{ "ModuleRelativePath", "Public/DLSSSettings.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "The value that would be considered as Bias Color in the custom depth stencil buffer. Must not be set to 0*" },
#endif
		{ "UIMin", "1" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bEnableDLSSD3D12_MetaData[] = {
		{ "Category", "General Settings" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Enable DLSS/DLAA for D3D12, if the driver supports it at runtime */" },
#endif
		{ "ConfigRestartRequired", "TRUE" },
		{ "DisplayName", "Enable DLSS/DLAA for the D3D12RHI" },
		{ "ModuleRelativePath", "Public/DLSSSettings.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Enable DLSS/DLAA for D3D12, if the driver supports it at runtime" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bEnableDLSSD3D11_MetaData[] = {
		{ "Category", "General Settings" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Enable DLSS/DLAA for D3D11, if the driver supports it at runtime */" },
#endif
		{ "ConfigRestartRequired", "TRUE" },
		{ "DisplayName", "Enable DLSS/DLAA for the D3D11RHI" },
		{ "ModuleRelativePath", "Public/DLSSSettings.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Enable DLSS/DLAA for D3D11, if the driver supports it at runtime" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bEnableDLSSVulkan_MetaData[] = {
		{ "Category", "General Settings" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Enable DLSS/DLAA for Vulkan, if the driver supports it at runtime */" },
#endif
		{ "ConfigRestartRequired", "TRUE" },
		{ "DisplayName", "Enable DLSS/DLAA for the VulkanRHI" },
		{ "ModuleRelativePath", "Public/DLSSSettings.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Enable DLSS/DLAA for Vulkan, if the driver supports it at runtime" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bShowDLSSIncompatiblePluginsToolsWarnings_MetaData[] = {
		{ "Category", "Editor" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** This enables warnings about plugins & tools that are incompatible with DLSS/DLAA in the editor. This setting and the local setting both must be set to get warnings */" },
#endif
		{ "DisplayName", "Warn about incompatible plugins and tools" },
		{ "ModuleRelativePath", "Public/DLSSSettings.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "This enables warnings about plugins & tools that are incompatible with DLSS/DLAA in the editor. This setting and the local setting both must be set to get warnings" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bEnableDLSSInEditorViewports_MetaData[] = {
		{ "Category", "Editor" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** This enables DLSS/DLAA in editor viewports. This project wide setting can be locally overridden in the NVIDIA DLSS (Local) settings.*/" },
#endif
		{ "DisplayName", "Enable DLSS/DLAA to be turned on in Editor viewports" },
		{ "ModuleRelativePath", "Public/DLSSSettings.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "This enables DLSS/DLAA in editor viewports. This project wide setting can be locally overridden in the NVIDIA DLSS (Local) settings." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bEnableDLSSInPlayInEditorViewports_MetaData[] = {
		{ "Category", "Editor" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** This enables DLSS/DLAA in play in editor viewports. This project wide setting can be locally overridden in in the NVIDIA DLSS (Local) settings.*/" },
#endif
		{ "DisplayName", "Enable DLSS/DLAA in Play In Editor viewports" },
		{ "ModuleRelativePath", "Public/DLSSSettings.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "This enables DLSS/DLAA in play in editor viewports. This project wide setting can be locally overridden in in the NVIDIA DLSS (Local) settings." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bShowDLSSSDebugOnScreenMessages_MetaData[] = {
		{ "Category", "Editor" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** This enables on screen warnings and errors about DLSS/DLAA. This project wide setting can be locally overridden in the NVIDIA DLSS (Local) settings. */" },
#endif
		{ "DisplayName", "Show various DLSS/DLAA on screen debug messages" },
		{ "ModuleRelativePath", "Public/DLSSSettings.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "This enables on screen warnings and errors about DLSS/DLAA. This project wide setting can be locally overridden in the NVIDIA DLSS (Local) settings." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_GenericDLSSSRBinaryPath_MetaData[] = {
		{ "Category", "DLSS-SR Settings" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** This is part of the DLSS plugin and used by most projects*/" },
#endif
		{ "DisplayName", "Generic DLSS-SR Binary Path" },
		{ "ModuleRelativePath", "Public/DLSSSettings.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "This is part of the DLSS plugin and used by most projects" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bGenericDLSSSRBinaryExists_MetaData[] = {
		{ "Category", "DLSS-SR Settings" },
		{ "DisplayName", "Exists" },
		{ "ModuleRelativePath", "Public/DLSSSettings.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CustomDLSSSRBinaryPath_MetaData[] = {
		{ "Category", "DLSS-SR Settings" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** In some cases NVIDIA might provide a project specific DLSS binary for your project. Please refer to https://developer.nvidia.com/dlss for details*/" },
#endif
		{ "DisplayName", "Custom DLSS-SR Binary Path" },
		{ "ModuleRelativePath", "Public/DLSSSettings.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "In some cases NVIDIA might provide a project specific DLSS binary for your project. Please refer to https://developer.nvidia.com/dlss for details" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bCustomDLSSSRBinaryExists_MetaData[] = {
		{ "Category", "DLSS-SR Settings" },
		{ "DisplayName", "Exists" },
		{ "ModuleRelativePath", "Public/DLSSSettings.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DLAAPreset_MetaData[] = {
		{ "Category", "DLSS-SR Settings" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** DLAA preset setting. Allows selecting a different DL model than the default */" },
#endif
		{ "DisplayName", "DLAA Preset" },
		{ "ModuleRelativePath", "Public/DLSSSettings.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "DLAA preset setting. Allows selecting a different DL model than the default" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DLSSQualityPreset_MetaData[] = {
		{ "Category", "DLSS-SR Settings" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** DLSS quality mode preset setting. Allows selecting a different DL model than the default */" },
#endif
		{ "DisplayName", "DLSS Quality Preset" },
		{ "ModuleRelativePath", "Public/DLSSSettings.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "DLSS quality mode preset setting. Allows selecting a different DL model than the default" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DLSSBalancedPreset_MetaData[] = {
		{ "Category", "DLSS-SR Settings" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** DLSS balanced mode preset setting. Allows selecting a different DL model than the default */" },
#endif
		{ "DisplayName", "DLSS Balanced Preset" },
		{ "ModuleRelativePath", "Public/DLSSSettings.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "DLSS balanced mode preset setting. Allows selecting a different DL model than the default" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DLSSPerformancePreset_MetaData[] = {
		{ "Category", "DLSS-SR Settings" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** DLSS performance mode preset setting. Allows selecting a different DL model than the default */" },
#endif
		{ "DisplayName", "DLSS Performance Preset" },
		{ "ModuleRelativePath", "Public/DLSSSettings.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "DLSS performance mode preset setting. Allows selecting a different DL model than the default" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DLSSUltraPerformancePreset_MetaData[] = {
		{ "Category", "DLSS-SR Settings" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** DLSS ultra performance mode preset setting. Allows selecting a different DL model than the default */" },
#endif
		{ "DisplayName", "DLSS Ultra Performance Preset" },
		{ "ModuleRelativePath", "Public/DLSSSettings.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "DLSS ultra performance mode preset setting. Allows selecting a different DL model than the default" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_GenericDLSSRRBinaryPath_MetaData[] = {
		{ "Category", "DLSS-RR Settings" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** This is part of the DLSS plugin and used by most projects*/" },
#endif
		{ "DisplayName", "Generic DLSS-RR Binary Path" },
		{ "ModuleRelativePath", "Public/DLSSSettings.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "This is part of the DLSS plugin and used by most projects" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bGenericDLSSRRBinaryExists_MetaData[] = {
		{ "Category", "DLSS-RR Settings" },
		{ "DisplayName", "Exists" },
		{ "ModuleRelativePath", "Public/DLSSSettings.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CustomDLSSRRBinaryPath_MetaData[] = {
		{ "Category", "DLSS-RR Settings" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** In some cases NVIDIA might provide a project specific DLSS binary for your project. Please refer to https://developer.nvidia.com/dlss for details*/" },
#endif
		{ "DisplayName", "Custom DLSS-RR Binary Path" },
		{ "ModuleRelativePath", "Public/DLSSSettings.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "In some cases NVIDIA might provide a project specific DLSS binary for your project. Please refer to https://developer.nvidia.com/dlss for details" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bCustomDLSSRRBinaryExists_MetaData[] = {
		{ "Category", "DLSS-RR Settings" },
		{ "DisplayName", "Exists" },
		{ "ModuleRelativePath", "Public/DLSSSettings.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DLAARRPreset_MetaData[] = {
		{ "Category", "DLSS-RR Settings" },
		{ "DisplayName", "DLAA-RR Preset" },
		{ "ModuleRelativePath", "Public/DLSSSettings.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DLSSRRQualityPreset_MetaData[] = {
		{ "Category", "DLSS-RR Settings" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** DLSS-RR quality mode preset setting. Allows selecting a different DL model than the default */" },
#endif
		{ "DisplayName", "DLSS-RR Quality Preset" },
		{ "ModuleRelativePath", "Public/DLSSSettings.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "DLSS-RR quality mode preset setting. Allows selecting a different DL model than the default" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DLSSRRBalancedPreset_MetaData[] = {
		{ "Category", "DLSS-RR Settings" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** DLSS-RR balanced mode preset setting. Allows selecting a different DL model than the default */" },
#endif
		{ "DisplayName", "DLSS-RR Balanced Preset" },
		{ "ModuleRelativePath", "Public/DLSSSettings.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "DLSS-RR balanced mode preset setting. Allows selecting a different DL model than the default" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DLSSRRPerformancePreset_MetaData[] = {
		{ "Category", "DLSS-RR Settings" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** DLSS-RR performance mode preset setting. Allows selecting a different DL model than the default */" },
#endif
		{ "DisplayName", "DLSS-RR Performance Preset" },
		{ "ModuleRelativePath", "Public/DLSSSettings.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "DLSS-RR performance mode preset setting. Allows selecting a different DL model than the default" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DLSSRRUltraPerformancePreset_MetaData[] = {
		{ "Category", "DLSS-RR Settings" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** DLSS-RR ultra performance mode preset setting. Allows selecting a different DL model than the default */" },
#endif
		{ "DisplayName", "DLSS-RR Ultra Performance Preset" },
		{ "ModuleRelativePath", "Public/DLSSSettings.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "DLSS-RR ultra performance mode preset setting. Allows selecting a different DL model than the default" },
#endif
	};
#endif // WITH_METADATA
	static void NewProp_bAllowOTAUpdate_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bAllowOTAUpdate;
	static const UECodeGen_Private::FUInt32PropertyParams NewProp_NVIDIANGXApplicationId;
	static const UECodeGen_Private::FBytePropertyParams NewProp_BiasCurrentColorStencilValue;
	static void NewProp_bEnableDLSSD3D12_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bEnableDLSSD3D12;
	static void NewProp_bEnableDLSSD3D11_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bEnableDLSSD3D11;
	static void NewProp_bEnableDLSSVulkan_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bEnableDLSSVulkan;
	static void NewProp_bShowDLSSIncompatiblePluginsToolsWarnings_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bShowDLSSIncompatiblePluginsToolsWarnings;
	static void NewProp_bEnableDLSSInEditorViewports_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bEnableDLSSInEditorViewports;
	static void NewProp_bEnableDLSSInPlayInEditorViewports_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bEnableDLSSInPlayInEditorViewports;
	static void NewProp_bShowDLSSSDebugOnScreenMessages_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bShowDLSSSDebugOnScreenMessages;
	static const UECodeGen_Private::FStrPropertyParams NewProp_GenericDLSSSRBinaryPath;
	static void NewProp_bGenericDLSSSRBinaryExists_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bGenericDLSSSRBinaryExists;
	static const UECodeGen_Private::FStrPropertyParams NewProp_CustomDLSSSRBinaryPath;
	static void NewProp_bCustomDLSSSRBinaryExists_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bCustomDLSSSRBinaryExists;
	static const UECodeGen_Private::FBytePropertyParams NewProp_DLAAPreset_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_DLAAPreset;
	static const UECodeGen_Private::FBytePropertyParams NewProp_DLSSQualityPreset_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_DLSSQualityPreset;
	static const UECodeGen_Private::FBytePropertyParams NewProp_DLSSBalancedPreset_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_DLSSBalancedPreset;
	static const UECodeGen_Private::FBytePropertyParams NewProp_DLSSPerformancePreset_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_DLSSPerformancePreset;
	static const UECodeGen_Private::FBytePropertyParams NewProp_DLSSUltraPerformancePreset_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_DLSSUltraPerformancePreset;
	static const UECodeGen_Private::FStrPropertyParams NewProp_GenericDLSSRRBinaryPath;
	static void NewProp_bGenericDLSSRRBinaryExists_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bGenericDLSSRRBinaryExists;
	static const UECodeGen_Private::FStrPropertyParams NewProp_CustomDLSSRRBinaryPath;
	static void NewProp_bCustomDLSSRRBinaryExists_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bCustomDLSSRRBinaryExists;
	static const UECodeGen_Private::FBytePropertyParams NewProp_DLAARRPreset_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_DLAARRPreset;
	static const UECodeGen_Private::FBytePropertyParams NewProp_DLSSRRQualityPreset_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_DLSSRRQualityPreset;
	static const UECodeGen_Private::FBytePropertyParams NewProp_DLSSRRBalancedPreset_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_DLSSRRBalancedPreset;
	static const UECodeGen_Private::FBytePropertyParams NewProp_DLSSRRPerformancePreset_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_DLSSRRPerformancePreset;
	static const UECodeGen_Private::FBytePropertyParams NewProp_DLSSRRUltraPerformancePreset_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_DLSSRRUltraPerformancePreset;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UDLSSSettings>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
void Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bAllowOTAUpdate_SetBit(void* Obj)
{
	((UDLSSSettings*)Obj)->bAllowOTAUpdate = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bAllowOTAUpdate = { "bAllowOTAUpdate", nullptr, (EPropertyFlags)0x0010000000004001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UDLSSSettings), &Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bAllowOTAUpdate_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bAllowOTAUpdate_MetaData), NewProp_bAllowOTAUpdate_MetaData) };
const UECodeGen_Private::FUInt32PropertyParams Z_Construct_UClass_UDLSSSettings_Statics::NewProp_NVIDIANGXApplicationId = { "NVIDIANGXApplicationId", nullptr, (EPropertyFlags)0x0010040000004001, UECodeGen_Private::EPropertyGenFlags::UInt32, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UDLSSSettings, NVIDIANGXApplicationId), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_NVIDIANGXApplicationId_MetaData), NewProp_NVIDIANGXApplicationId_MetaData) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_UDLSSSettings_Statics::NewProp_BiasCurrentColorStencilValue = { "BiasCurrentColorStencilValue", nullptr, (EPropertyFlags)0x0010000000004001, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UDLSSSettings, BiasCurrentColorStencilValue), nullptr, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_BiasCurrentColorStencilValue_MetaData), NewProp_BiasCurrentColorStencilValue_MetaData) };
void Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bEnableDLSSD3D12_SetBit(void* Obj)
{
	((UDLSSSettings*)Obj)->bEnableDLSSD3D12 = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bEnableDLSSD3D12 = { "bEnableDLSSD3D12", nullptr, (EPropertyFlags)0x0010000000004001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UDLSSSettings), &Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bEnableDLSSD3D12_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bEnableDLSSD3D12_MetaData), NewProp_bEnableDLSSD3D12_MetaData) };
void Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bEnableDLSSD3D11_SetBit(void* Obj)
{
	((UDLSSSettings*)Obj)->bEnableDLSSD3D11 = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bEnableDLSSD3D11 = { "bEnableDLSSD3D11", nullptr, (EPropertyFlags)0x0010000000004001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UDLSSSettings), &Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bEnableDLSSD3D11_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bEnableDLSSD3D11_MetaData), NewProp_bEnableDLSSD3D11_MetaData) };
void Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bEnableDLSSVulkan_SetBit(void* Obj)
{
	((UDLSSSettings*)Obj)->bEnableDLSSVulkan = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bEnableDLSSVulkan = { "bEnableDLSSVulkan", nullptr, (EPropertyFlags)0x0010000000004001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UDLSSSettings), &Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bEnableDLSSVulkan_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bEnableDLSSVulkan_MetaData), NewProp_bEnableDLSSVulkan_MetaData) };
void Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bShowDLSSIncompatiblePluginsToolsWarnings_SetBit(void* Obj)
{
	((UDLSSSettings*)Obj)->bShowDLSSIncompatiblePluginsToolsWarnings = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bShowDLSSIncompatiblePluginsToolsWarnings = { "bShowDLSSIncompatiblePluginsToolsWarnings", nullptr, (EPropertyFlags)0x0010000000004001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UDLSSSettings), &Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bShowDLSSIncompatiblePluginsToolsWarnings_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bShowDLSSIncompatiblePluginsToolsWarnings_MetaData), NewProp_bShowDLSSIncompatiblePluginsToolsWarnings_MetaData) };
void Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bEnableDLSSInEditorViewports_SetBit(void* Obj)
{
	((UDLSSSettings*)Obj)->bEnableDLSSInEditorViewports = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bEnableDLSSInEditorViewports = { "bEnableDLSSInEditorViewports", nullptr, (EPropertyFlags)0x0010000000004001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UDLSSSettings), &Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bEnableDLSSInEditorViewports_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bEnableDLSSInEditorViewports_MetaData), NewProp_bEnableDLSSInEditorViewports_MetaData) };
void Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bEnableDLSSInPlayInEditorViewports_SetBit(void* Obj)
{
	((UDLSSSettings*)Obj)->bEnableDLSSInPlayInEditorViewports = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bEnableDLSSInPlayInEditorViewports = { "bEnableDLSSInPlayInEditorViewports", nullptr, (EPropertyFlags)0x0010000000004001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UDLSSSettings), &Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bEnableDLSSInPlayInEditorViewports_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bEnableDLSSInPlayInEditorViewports_MetaData), NewProp_bEnableDLSSInPlayInEditorViewports_MetaData) };
void Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bShowDLSSSDebugOnScreenMessages_SetBit(void* Obj)
{
	((UDLSSSettings*)Obj)->bShowDLSSSDebugOnScreenMessages = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bShowDLSSSDebugOnScreenMessages = { "bShowDLSSSDebugOnScreenMessages", nullptr, (EPropertyFlags)0x0010000000004001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UDLSSSettings), &Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bShowDLSSSDebugOnScreenMessages_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bShowDLSSSDebugOnScreenMessages_MetaData), NewProp_bShowDLSSSDebugOnScreenMessages_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UDLSSSettings_Statics::NewProp_GenericDLSSSRBinaryPath = { "GenericDLSSSRBinaryPath", nullptr, (EPropertyFlags)0x0010000000024001, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UDLSSSettings, GenericDLSSSRBinaryPath), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_GenericDLSSSRBinaryPath_MetaData), NewProp_GenericDLSSSRBinaryPath_MetaData) };
void Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bGenericDLSSSRBinaryExists_SetBit(void* Obj)
{
	((UDLSSSettings*)Obj)->bGenericDLSSSRBinaryExists = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bGenericDLSSSRBinaryExists = { "bGenericDLSSSRBinaryExists", nullptr, (EPropertyFlags)0x0010000000024001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UDLSSSettings), &Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bGenericDLSSSRBinaryExists_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bGenericDLSSSRBinaryExists_MetaData), NewProp_bGenericDLSSSRBinaryExists_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UDLSSSettings_Statics::NewProp_CustomDLSSSRBinaryPath = { "CustomDLSSSRBinaryPath", nullptr, (EPropertyFlags)0x0010040000024001, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UDLSSSettings, CustomDLSSSRBinaryPath), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CustomDLSSSRBinaryPath_MetaData), NewProp_CustomDLSSSRBinaryPath_MetaData) };
void Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bCustomDLSSSRBinaryExists_SetBit(void* Obj)
{
	((UDLSSSettings*)Obj)->bCustomDLSSSRBinaryExists = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bCustomDLSSSRBinaryExists = { "bCustomDLSSSRBinaryExists", nullptr, (EPropertyFlags)0x0010040000024001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UDLSSSettings), &Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bCustomDLSSSRBinaryExists_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bCustomDLSSSRBinaryExists_MetaData), NewProp_bCustomDLSSSRBinaryExists_MetaData) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_UDLSSSettings_Statics::NewProp_DLAAPreset_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UClass_UDLSSSettings_Statics::NewProp_DLAAPreset = { "DLAAPreset", nullptr, (EPropertyFlags)0x0010040000004001, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UDLSSSettings, DLAAPreset), Z_Construct_UEnum_DLSS_EDLSSPreset, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DLAAPreset_MetaData), NewProp_DLAAPreset_MetaData) }; // 2840954565
const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_UDLSSSettings_Statics::NewProp_DLSSQualityPreset_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UClass_UDLSSSettings_Statics::NewProp_DLSSQualityPreset = { "DLSSQualityPreset", nullptr, (EPropertyFlags)0x0010040000004001, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UDLSSSettings, DLSSQualityPreset), Z_Construct_UEnum_DLSS_EDLSSPreset, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DLSSQualityPreset_MetaData), NewProp_DLSSQualityPreset_MetaData) }; // 2840954565
const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_UDLSSSettings_Statics::NewProp_DLSSBalancedPreset_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UClass_UDLSSSettings_Statics::NewProp_DLSSBalancedPreset = { "DLSSBalancedPreset", nullptr, (EPropertyFlags)0x0010040000004001, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UDLSSSettings, DLSSBalancedPreset), Z_Construct_UEnum_DLSS_EDLSSPreset, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DLSSBalancedPreset_MetaData), NewProp_DLSSBalancedPreset_MetaData) }; // 2840954565
const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_UDLSSSettings_Statics::NewProp_DLSSPerformancePreset_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UClass_UDLSSSettings_Statics::NewProp_DLSSPerformancePreset = { "DLSSPerformancePreset", nullptr, (EPropertyFlags)0x0010040000004001, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UDLSSSettings, DLSSPerformancePreset), Z_Construct_UEnum_DLSS_EDLSSPreset, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DLSSPerformancePreset_MetaData), NewProp_DLSSPerformancePreset_MetaData) }; // 2840954565
const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_UDLSSSettings_Statics::NewProp_DLSSUltraPerformancePreset_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UClass_UDLSSSettings_Statics::NewProp_DLSSUltraPerformancePreset = { "DLSSUltraPerformancePreset", nullptr, (EPropertyFlags)0x0010040000004001, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UDLSSSettings, DLSSUltraPerformancePreset), Z_Construct_UEnum_DLSS_EDLSSPreset, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DLSSUltraPerformancePreset_MetaData), NewProp_DLSSUltraPerformancePreset_MetaData) }; // 2840954565
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UDLSSSettings_Statics::NewProp_GenericDLSSRRBinaryPath = { "GenericDLSSRRBinaryPath", nullptr, (EPropertyFlags)0x0010000000024001, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UDLSSSettings, GenericDLSSRRBinaryPath), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_GenericDLSSRRBinaryPath_MetaData), NewProp_GenericDLSSRRBinaryPath_MetaData) };
void Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bGenericDLSSRRBinaryExists_SetBit(void* Obj)
{
	((UDLSSSettings*)Obj)->bGenericDLSSRRBinaryExists = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bGenericDLSSRRBinaryExists = { "bGenericDLSSRRBinaryExists", nullptr, (EPropertyFlags)0x0010000000024001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UDLSSSettings), &Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bGenericDLSSRRBinaryExists_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bGenericDLSSRRBinaryExists_MetaData), NewProp_bGenericDLSSRRBinaryExists_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UDLSSSettings_Statics::NewProp_CustomDLSSRRBinaryPath = { "CustomDLSSRRBinaryPath", nullptr, (EPropertyFlags)0x0010040000024001, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UDLSSSettings, CustomDLSSRRBinaryPath), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CustomDLSSRRBinaryPath_MetaData), NewProp_CustomDLSSRRBinaryPath_MetaData) };
void Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bCustomDLSSRRBinaryExists_SetBit(void* Obj)
{
	((UDLSSSettings*)Obj)->bCustomDLSSRRBinaryExists = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bCustomDLSSRRBinaryExists = { "bCustomDLSSRRBinaryExists", nullptr, (EPropertyFlags)0x0010040000024001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UDLSSSettings), &Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bCustomDLSSRRBinaryExists_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bCustomDLSSRRBinaryExists_MetaData), NewProp_bCustomDLSSRRBinaryExists_MetaData) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_UDLSSSettings_Statics::NewProp_DLAARRPreset_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UClass_UDLSSSettings_Statics::NewProp_DLAARRPreset = { "DLAARRPreset", nullptr, (EPropertyFlags)0x0010040000004001, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UDLSSSettings, DLAARRPreset), Z_Construct_UEnum_DLSS_EDLSSRRPreset, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DLAARRPreset_MetaData), NewProp_DLAARRPreset_MetaData) }; // 4293121250
const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_UDLSSSettings_Statics::NewProp_DLSSRRQualityPreset_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UClass_UDLSSSettings_Statics::NewProp_DLSSRRQualityPreset = { "DLSSRRQualityPreset", nullptr, (EPropertyFlags)0x0010040000004001, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UDLSSSettings, DLSSRRQualityPreset), Z_Construct_UEnum_DLSS_EDLSSRRPreset, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DLSSRRQualityPreset_MetaData), NewProp_DLSSRRQualityPreset_MetaData) }; // 4293121250
const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_UDLSSSettings_Statics::NewProp_DLSSRRBalancedPreset_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UClass_UDLSSSettings_Statics::NewProp_DLSSRRBalancedPreset = { "DLSSRRBalancedPreset", nullptr, (EPropertyFlags)0x0010040000004001, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UDLSSSettings, DLSSRRBalancedPreset), Z_Construct_UEnum_DLSS_EDLSSRRPreset, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DLSSRRBalancedPreset_MetaData), NewProp_DLSSRRBalancedPreset_MetaData) }; // 4293121250
const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_UDLSSSettings_Statics::NewProp_DLSSRRPerformancePreset_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UClass_UDLSSSettings_Statics::NewProp_DLSSRRPerformancePreset = { "DLSSRRPerformancePreset", nullptr, (EPropertyFlags)0x0010040000004001, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UDLSSSettings, DLSSRRPerformancePreset), Z_Construct_UEnum_DLSS_EDLSSRRPreset, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DLSSRRPerformancePreset_MetaData), NewProp_DLSSRRPerformancePreset_MetaData) }; // 4293121250
const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_UDLSSSettings_Statics::NewProp_DLSSRRUltraPerformancePreset_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UClass_UDLSSSettings_Statics::NewProp_DLSSRRUltraPerformancePreset = { "DLSSRRUltraPerformancePreset", nullptr, (EPropertyFlags)0x0010040000004001, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UDLSSSettings, DLSSRRUltraPerformancePreset), Z_Construct_UEnum_DLSS_EDLSSRRPreset, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DLSSRRUltraPerformancePreset_MetaData), NewProp_DLSSRRUltraPerformancePreset_MetaData) }; // 4293121250
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UDLSSSettings_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bAllowOTAUpdate,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSSettings_Statics::NewProp_NVIDIANGXApplicationId,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSSettings_Statics::NewProp_BiasCurrentColorStencilValue,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bEnableDLSSD3D12,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bEnableDLSSD3D11,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bEnableDLSSVulkan,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bShowDLSSIncompatiblePluginsToolsWarnings,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bEnableDLSSInEditorViewports,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bEnableDLSSInPlayInEditorViewports,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bShowDLSSSDebugOnScreenMessages,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSSettings_Statics::NewProp_GenericDLSSSRBinaryPath,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bGenericDLSSSRBinaryExists,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSSettings_Statics::NewProp_CustomDLSSSRBinaryPath,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bCustomDLSSSRBinaryExists,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSSettings_Statics::NewProp_DLAAPreset_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSSettings_Statics::NewProp_DLAAPreset,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSSettings_Statics::NewProp_DLSSQualityPreset_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSSettings_Statics::NewProp_DLSSQualityPreset,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSSettings_Statics::NewProp_DLSSBalancedPreset_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSSettings_Statics::NewProp_DLSSBalancedPreset,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSSettings_Statics::NewProp_DLSSPerformancePreset_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSSettings_Statics::NewProp_DLSSPerformancePreset,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSSettings_Statics::NewProp_DLSSUltraPerformancePreset_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSSettings_Statics::NewProp_DLSSUltraPerformancePreset,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSSettings_Statics::NewProp_GenericDLSSRRBinaryPath,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bGenericDLSSRRBinaryExists,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSSettings_Statics::NewProp_CustomDLSSRRBinaryPath,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSSettings_Statics::NewProp_bCustomDLSSRRBinaryExists,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSSettings_Statics::NewProp_DLAARRPreset_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSSettings_Statics::NewProp_DLAARRPreset,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSSettings_Statics::NewProp_DLSSRRQualityPreset_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSSettings_Statics::NewProp_DLSSRRQualityPreset,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSSettings_Statics::NewProp_DLSSRRBalancedPreset_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSSettings_Statics::NewProp_DLSSRRBalancedPreset,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSSettings_Statics::NewProp_DLSSRRPerformancePreset_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSSettings_Statics::NewProp_DLSSRRPerformancePreset,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSSettings_Statics::NewProp_DLSSRRUltraPerformancePreset_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDLSSSettings_Statics::NewProp_DLSSRRUltraPerformancePreset,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UDLSSSettings_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UDLSSSettings_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UObject,
	(UObject* (*)())Z_Construct_UPackage__Script_DLSS,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UDLSSSettings_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UDLSSSettings_Statics::ClassParams = {
	&UDLSSSettings::StaticClass,
	"Engine",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UDLSSSettings_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UDLSSSettings_Statics::PropPointers),
	0,
	0x001000A6u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UDLSSSettings_Statics::Class_MetaDataParams), Z_Construct_UClass_UDLSSSettings_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UDLSSSettings()
{
	if (!Z_Registration_Info_UClass_UDLSSSettings.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UDLSSSettings.OuterSingleton, Z_Construct_UClass_UDLSSSettings_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UDLSSSettings.OuterSingleton;
}
template<> DLSS_API UClass* StaticClass<UDLSSSettings>()
{
	return UDLSSSettings::StaticClass();
}
UDLSSSettings::UDLSSSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UDLSSSettings);
UDLSSSettings::~UDLSSSettings() {}
// End Class UDLSSSettings

// Begin Registration
struct Z_CompiledInDeferFile_FID_Github_SuraS_Plugins_DLSS_Source_DLSS_Public_DLSSSettings_h_Statics
{
	static constexpr FEnumRegisterCompiledInInfo EnumInfo[] = {
		{ EDLSSSettingOverride_StaticEnum, TEXT("EDLSSSettingOverride"), &Z_Registration_Info_UEnum_EDLSSSettingOverride, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 2279089454U) },
		{ EDLSSPreset_StaticEnum, TEXT("EDLSSPreset"), &Z_Registration_Info_UEnum_EDLSSPreset, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 2840954565U) },
		{ EDLSSRRPreset_StaticEnum, TEXT("EDLSSRRPreset"), &Z_Registration_Info_UEnum_EDLSSRRPreset, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 4293121250U) },
	};
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UDLSSOverrideSettings, UDLSSOverrideSettings::StaticClass, TEXT("UDLSSOverrideSettings"), &Z_Registration_Info_UClass_UDLSSOverrideSettings, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UDLSSOverrideSettings), 3904533167U) },
		{ Z_Construct_UClass_UDLSSSettings, UDLSSSettings::StaticClass, TEXT("UDLSSSettings"), &Z_Registration_Info_UClass_UDLSSSettings, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UDLSSSettings), 3309733862U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Github_SuraS_Plugins_DLSS_Source_DLSS_Public_DLSSSettings_h_1025544899(TEXT("/Script/DLSS"),
	Z_CompiledInDeferFile_FID_Github_SuraS_Plugins_DLSS_Source_DLSS_Public_DLSSSettings_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Github_SuraS_Plugins_DLSS_Source_DLSS_Public_DLSSSettings_h_Statics::ClassInfo),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Github_SuraS_Plugins_DLSS_Source_DLSS_Public_DLSSSettings_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Github_SuraS_Plugins_DLSS_Source_DLSS_Public_DLSSSettings_h_Statics::EnumInfo));
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
