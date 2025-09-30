// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "StreamlineDeepDVCBlueprint/Public/StreamlineLibraryDeepDVC.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeStreamlineLibraryDeepDVC() {}

// Begin Cross Module References
ENGINE_API UClass* Z_Construct_UClass_UBlueprintFunctionLibrary();
STREAMLINEBLUEPRINT_API UEnum* Z_Construct_UEnum_StreamlineBlueprint_EStreamlineFeatureSupport();
STREAMLINEDEEPDVCBLUEPRINT_API UClass* Z_Construct_UClass_UStreamlineLibraryDeepDVC();
STREAMLINEDEEPDVCBLUEPRINT_API UClass* Z_Construct_UClass_UStreamlineLibraryDeepDVC_NoRegister();
STREAMLINEDEEPDVCBLUEPRINT_API UEnum* Z_Construct_UEnum_StreamlineDeepDVCBlueprint_EStreamlineDeepDVCMode();
UPackage* Z_Construct_UPackage__Script_StreamlineDeepDVCBlueprint();
// End Cross Module References

// Begin Enum EStreamlineDeepDVCMode
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EStreamlineDeepDVCMode;
static UEnum* EStreamlineDeepDVCMode_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EStreamlineDeepDVCMode.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EStreamlineDeepDVCMode.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_StreamlineDeepDVCBlueprint_EStreamlineDeepDVCMode, (UObject*)Z_Construct_UPackage__Script_StreamlineDeepDVCBlueprint(), TEXT("EStreamlineDeepDVCMode"));
	}
	return Z_Registration_Info_UEnum_EStreamlineDeepDVCMode.OuterSingleton;
}
template<> STREAMLINEDEEPDVCBLUEPRINT_API UEnum* StaticEnum<EStreamlineDeepDVCMode>()
{
	return EStreamlineDeepDVCMode_StaticEnum();
}
struct Z_Construct_UEnum_StreamlineDeepDVCBlueprint_EStreamlineDeepDVCMode_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryDeepDVC.h" },
		{ "Off.DisplayName", "Off" },
		{ "Off.Name", "EStreamlineDeepDVCMode::Off" },
		{ "On.DisplayName", "On" },
		{ "On.Name", "EStreamlineDeepDVCMode::On" },
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EStreamlineDeepDVCMode::Off", (int64)EStreamlineDeepDVCMode::Off },
		{ "EStreamlineDeepDVCMode::On", (int64)EStreamlineDeepDVCMode::On },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_StreamlineDeepDVCBlueprint_EStreamlineDeepDVCMode_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_StreamlineDeepDVCBlueprint,
	nullptr,
	"EStreamlineDeepDVCMode",
	"EStreamlineDeepDVCMode",
	Z_Construct_UEnum_StreamlineDeepDVCBlueprint_EStreamlineDeepDVCMode_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_StreamlineDeepDVCBlueprint_EStreamlineDeepDVCMode_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_StreamlineDeepDVCBlueprint_EStreamlineDeepDVCMode_Statics::Enum_MetaDataParams), Z_Construct_UEnum_StreamlineDeepDVCBlueprint_EStreamlineDeepDVCMode_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_StreamlineDeepDVCBlueprint_EStreamlineDeepDVCMode()
{
	if (!Z_Registration_Info_UEnum_EStreamlineDeepDVCMode.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EStreamlineDeepDVCMode.InnerSingleton, Z_Construct_UEnum_StreamlineDeepDVCBlueprint_EStreamlineDeepDVCMode_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EStreamlineDeepDVCMode.InnerSingleton;
}
// End Enum EStreamlineDeepDVCMode

// Begin Class UStreamlineLibraryDeepDVC Function GetDeepDVCIntensity
struct Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCIntensity_Statics
{
	struct StreamlineLibraryDeepDVC_eventGetDeepDVCIntensity_Parms
	{
		float ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Streamline|DeepDVC" },
		{ "Comment", "/* Read the console variables to infer the current DeepDVC intensity (\"r.Streamline.DeepDVC.Intensity) */" },
		{ "DisplayName", "Get DeepDVC Intensity" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryDeepDVC.h" },
		{ "ToolTip", "Read the console variables to infer the current DeepDVC intensity (\"r.Streamline.DeepDVC.Intensity)" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCIntensity_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(StreamlineLibraryDeepDVC_eventGetDeepDVCIntensity_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCIntensity_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCIntensity_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCIntensity_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCIntensity_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UStreamlineLibraryDeepDVC, nullptr, "GetDeepDVCIntensity", nullptr, nullptr, Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCIntensity_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCIntensity_Statics::PropPointers), sizeof(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCIntensity_Statics::StreamlineLibraryDeepDVC_eventGetDeepDVCIntensity_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCIntensity_Statics::Function_MetaDataParams), Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCIntensity_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCIntensity_Statics::StreamlineLibraryDeepDVC_eventGetDeepDVCIntensity_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCIntensity()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCIntensity_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UStreamlineLibraryDeepDVC::execGetDeepDVCIntensity)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(float*)Z_Param__Result=UStreamlineLibraryDeepDVC::GetDeepDVCIntensity();
	P_NATIVE_END;
}
// End Class UStreamlineLibraryDeepDVC Function GetDeepDVCIntensity

// Begin Class UStreamlineLibraryDeepDVC Function GetDeepDVCMode
struct Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCMode_Statics
{
	struct StreamlineLibraryDeepDVC_eventGetDeepDVCMode_Parms
	{
		EStreamlineDeepDVCMode ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Streamline|DeepDVC" },
		{ "Comment", "/* Reads the console variables to infer the current DeepDVC mode*/" },
		{ "DisplayName", "Get DeepDVC Mode" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryDeepDVC.h" },
		{ "ToolTip", "Reads the console variables to infer the current DeepDVC mode" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_ReturnValue_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCMode_Statics::NewProp_ReturnValue_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCMode_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(StreamlineLibraryDeepDVC_eventGetDeepDVCMode_Parms, ReturnValue), Z_Construct_UEnum_StreamlineDeepDVCBlueprint_EStreamlineDeepDVCMode, METADATA_PARAMS(0, nullptr) }; // 4051356834
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCMode_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCMode_Statics::NewProp_ReturnValue_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCMode_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCMode_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCMode_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UStreamlineLibraryDeepDVC, nullptr, "GetDeepDVCMode", nullptr, nullptr, Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCMode_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCMode_Statics::PropPointers), sizeof(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCMode_Statics::StreamlineLibraryDeepDVC_eventGetDeepDVCMode_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCMode_Statics::Function_MetaDataParams), Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCMode_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCMode_Statics::StreamlineLibraryDeepDVC_eventGetDeepDVCMode_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCMode()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCMode_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UStreamlineLibraryDeepDVC::execGetDeepDVCMode)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(EStreamlineDeepDVCMode*)Z_Param__Result=UStreamlineLibraryDeepDVC::GetDeepDVCMode();
	P_NATIVE_END;
}
// End Class UStreamlineLibraryDeepDVC Function GetDeepDVCMode

// Begin Class UStreamlineLibraryDeepDVC Function GetDeepDVCSaturationBoost
struct Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCSaturationBoost_Statics
{
	struct StreamlineLibraryDeepDVC_eventGetDeepDVCSaturationBoost_Parms
	{
		float ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Streamline|DeepDVC" },
		{ "Comment", "/* Read the console variables to infer the current DeepDVC saturation boost (\"r.Streamline.DeepDVC.SaturationBoost) */" },
		{ "DisplayName", "Get DeepDVC Saturation Boost" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryDeepDVC.h" },
		{ "ToolTip", "Read the console variables to infer the current DeepDVC saturation boost (\"r.Streamline.DeepDVC.SaturationBoost)" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCSaturationBoost_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(StreamlineLibraryDeepDVC_eventGetDeepDVCSaturationBoost_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCSaturationBoost_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCSaturationBoost_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCSaturationBoost_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCSaturationBoost_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UStreamlineLibraryDeepDVC, nullptr, "GetDeepDVCSaturationBoost", nullptr, nullptr, Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCSaturationBoost_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCSaturationBoost_Statics::PropPointers), sizeof(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCSaturationBoost_Statics::StreamlineLibraryDeepDVC_eventGetDeepDVCSaturationBoost_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCSaturationBoost_Statics::Function_MetaDataParams), Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCSaturationBoost_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCSaturationBoost_Statics::StreamlineLibraryDeepDVC_eventGetDeepDVCSaturationBoost_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCSaturationBoost()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCSaturationBoost_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UStreamlineLibraryDeepDVC::execGetDeepDVCSaturationBoost)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(float*)Z_Param__Result=UStreamlineLibraryDeepDVC::GetDeepDVCSaturationBoost();
	P_NATIVE_END;
}
// End Class UStreamlineLibraryDeepDVC Function GetDeepDVCSaturationBoost

// Begin Class UStreamlineLibraryDeepDVC Function GetDefaultDeepDVCMode
struct Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDefaultDeepDVCMode_Statics
{
	struct StreamlineLibraryDeepDVC_eventGetDefaultDeepDVCMode_Parms
	{
		EStreamlineDeepDVCMode ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Streamline|DeepDVC" },
		{ "Comment", "/* Find a reasonable default DeepDVC mode based on current hardware */" },
		{ "DisplayName", "Get Default DeepDVC Mode" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryDeepDVC.h" },
		{ "ToolTip", "Find a reasonable default DeepDVC mode based on current hardware" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_ReturnValue_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDefaultDeepDVCMode_Statics::NewProp_ReturnValue_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDefaultDeepDVCMode_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(StreamlineLibraryDeepDVC_eventGetDefaultDeepDVCMode_Parms, ReturnValue), Z_Construct_UEnum_StreamlineDeepDVCBlueprint_EStreamlineDeepDVCMode, METADATA_PARAMS(0, nullptr) }; // 4051356834
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDefaultDeepDVCMode_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDefaultDeepDVCMode_Statics::NewProp_ReturnValue_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDefaultDeepDVCMode_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDefaultDeepDVCMode_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDefaultDeepDVCMode_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UStreamlineLibraryDeepDVC, nullptr, "GetDefaultDeepDVCMode", nullptr, nullptr, Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDefaultDeepDVCMode_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDefaultDeepDVCMode_Statics::PropPointers), sizeof(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDefaultDeepDVCMode_Statics::StreamlineLibraryDeepDVC_eventGetDefaultDeepDVCMode_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDefaultDeepDVCMode_Statics::Function_MetaDataParams), Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDefaultDeepDVCMode_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDefaultDeepDVCMode_Statics::StreamlineLibraryDeepDVC_eventGetDefaultDeepDVCMode_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDefaultDeepDVCMode()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDefaultDeepDVCMode_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UStreamlineLibraryDeepDVC::execGetDefaultDeepDVCMode)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(EStreamlineDeepDVCMode*)Z_Param__Result=UStreamlineLibraryDeepDVC::GetDefaultDeepDVCMode();
	P_NATIVE_END;
}
// End Class UStreamlineLibraryDeepDVC Function GetDefaultDeepDVCMode

// Begin Class UStreamlineLibraryDeepDVC Function GetSupportedDeepDVCModes
struct Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetSupportedDeepDVCModes_Statics
{
	struct StreamlineLibraryDeepDVC_eventGetSupportedDeepDVCModes_Parms
	{
		TArray<EStreamlineDeepDVCMode> ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Streamline|DeepDVC" },
		{ "Comment", "/** Retrieves all supported DeepDVC modes. Can be used to populate UI */" },
		{ "DisplayName", "Get Supported DeepDVC Modes" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryDeepDVC.h" },
		{ "ToolTip", "Retrieves all supported DeepDVC modes. Can be used to populate UI" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_ReturnValue_Inner_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_ReturnValue_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetSupportedDeepDVCModes_Statics::NewProp_ReturnValue_Inner_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetSupportedDeepDVCModes_Statics::NewProp_ReturnValue_Inner = { "ReturnValue", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UEnum_StreamlineDeepDVCBlueprint_EStreamlineDeepDVCMode, METADATA_PARAMS(0, nullptr) }; // 4051356834
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetSupportedDeepDVCModes_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(StreamlineLibraryDeepDVC_eventGetSupportedDeepDVCModes_Parms, ReturnValue), EArrayPropertyFlags::None, METADATA_PARAMS(0, nullptr) }; // 4051356834
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetSupportedDeepDVCModes_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetSupportedDeepDVCModes_Statics::NewProp_ReturnValue_Inner_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetSupportedDeepDVCModes_Statics::NewProp_ReturnValue_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetSupportedDeepDVCModes_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetSupportedDeepDVCModes_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetSupportedDeepDVCModes_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UStreamlineLibraryDeepDVC, nullptr, "GetSupportedDeepDVCModes", nullptr, nullptr, Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetSupportedDeepDVCModes_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetSupportedDeepDVCModes_Statics::PropPointers), sizeof(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetSupportedDeepDVCModes_Statics::StreamlineLibraryDeepDVC_eventGetSupportedDeepDVCModes_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetSupportedDeepDVCModes_Statics::Function_MetaDataParams), Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetSupportedDeepDVCModes_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetSupportedDeepDVCModes_Statics::StreamlineLibraryDeepDVC_eventGetSupportedDeepDVCModes_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetSupportedDeepDVCModes()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetSupportedDeepDVCModes_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UStreamlineLibraryDeepDVC::execGetSupportedDeepDVCModes)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(TArray<EStreamlineDeepDVCMode>*)Z_Param__Result=UStreamlineLibraryDeepDVC::GetSupportedDeepDVCModes();
	P_NATIVE_END;
}
// End Class UStreamlineLibraryDeepDVC Function GetSupportedDeepDVCModes

// Begin Class UStreamlineLibraryDeepDVC Function IsDeepDVCModeSupported
struct Z_Construct_UFunction_UStreamlineLibraryDeepDVC_IsDeepDVCModeSupported_Statics
{
	struct StreamlineLibraryDeepDVC_eventIsDeepDVCModeSupported_Parms
	{
		EStreamlineDeepDVCMode DeepDVCMode;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Streamline|DeepDVC" },
		{ "Comment", "/** Checks whether a DeepDVC mode is supported */" },
		{ "DisplayName", "Is DeepDVC Mode Supported" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryDeepDVC.h" },
		{ "ToolTip", "Checks whether a DeepDVC mode is supported" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_DeepDVCMode_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_DeepDVCMode;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UStreamlineLibraryDeepDVC_IsDeepDVCModeSupported_Statics::NewProp_DeepDVCMode_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UStreamlineLibraryDeepDVC_IsDeepDVCModeSupported_Statics::NewProp_DeepDVCMode = { "DeepDVCMode", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(StreamlineLibraryDeepDVC_eventIsDeepDVCModeSupported_Parms, DeepDVCMode), Z_Construct_UEnum_StreamlineDeepDVCBlueprint_EStreamlineDeepDVCMode, METADATA_PARAMS(0, nullptr) }; // 4051356834
void Z_Construct_UFunction_UStreamlineLibraryDeepDVC_IsDeepDVCModeSupported_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((StreamlineLibraryDeepDVC_eventIsDeepDVCModeSupported_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UStreamlineLibraryDeepDVC_IsDeepDVCModeSupported_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(StreamlineLibraryDeepDVC_eventIsDeepDVCModeSupported_Parms), &Z_Construct_UFunction_UStreamlineLibraryDeepDVC_IsDeepDVCModeSupported_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UStreamlineLibraryDeepDVC_IsDeepDVCModeSupported_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryDeepDVC_IsDeepDVCModeSupported_Statics::NewProp_DeepDVCMode_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryDeepDVC_IsDeepDVCModeSupported_Statics::NewProp_DeepDVCMode,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryDeepDVC_IsDeepDVCModeSupported_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_IsDeepDVCModeSupported_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UStreamlineLibraryDeepDVC_IsDeepDVCModeSupported_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UStreamlineLibraryDeepDVC, nullptr, "IsDeepDVCModeSupported", nullptr, nullptr, Z_Construct_UFunction_UStreamlineLibraryDeepDVC_IsDeepDVCModeSupported_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_IsDeepDVCModeSupported_Statics::PropPointers), sizeof(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_IsDeepDVCModeSupported_Statics::StreamlineLibraryDeepDVC_eventIsDeepDVCModeSupported_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_IsDeepDVCModeSupported_Statics::Function_MetaDataParams), Z_Construct_UFunction_UStreamlineLibraryDeepDVC_IsDeepDVCModeSupported_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_IsDeepDVCModeSupported_Statics::StreamlineLibraryDeepDVC_eventIsDeepDVCModeSupported_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UStreamlineLibraryDeepDVC_IsDeepDVCModeSupported()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UStreamlineLibraryDeepDVC_IsDeepDVCModeSupported_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UStreamlineLibraryDeepDVC::execIsDeepDVCModeSupported)
{
	P_GET_ENUM(EStreamlineDeepDVCMode,Z_Param_DeepDVCMode);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=UStreamlineLibraryDeepDVC::IsDeepDVCModeSupported(EStreamlineDeepDVCMode(Z_Param_DeepDVCMode));
	P_NATIVE_END;
}
// End Class UStreamlineLibraryDeepDVC Function IsDeepDVCModeSupported

// Begin Class UStreamlineLibraryDeepDVC Function IsDeepDVCSupported
struct Z_Construct_UFunction_UStreamlineLibraryDeepDVC_IsDeepDVCSupported_Statics
{
	struct StreamlineLibraryDeepDVC_eventIsDeepDVCSupported_Parms
	{
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Streamline|DeepDVC" },
		{ "Comment", "/** Checks whether DeepDVC is supported by the current GPU. Further details can be retrieved via QueryDeepDVCSupport*/" },
		{ "DisplayName", "Is NVIDIA DeepDVC Supported" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryDeepDVC.h" },
		{ "ToolTip", "Checks whether DeepDVC is supported by the current GPU. Further details can be retrieved via QueryDeepDVCSupport" },
	};
#endif // WITH_METADATA
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UStreamlineLibraryDeepDVC_IsDeepDVCSupported_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((StreamlineLibraryDeepDVC_eventIsDeepDVCSupported_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UStreamlineLibraryDeepDVC_IsDeepDVCSupported_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(StreamlineLibraryDeepDVC_eventIsDeepDVCSupported_Parms), &Z_Construct_UFunction_UStreamlineLibraryDeepDVC_IsDeepDVCSupported_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UStreamlineLibraryDeepDVC_IsDeepDVCSupported_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryDeepDVC_IsDeepDVCSupported_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_IsDeepDVCSupported_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UStreamlineLibraryDeepDVC_IsDeepDVCSupported_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UStreamlineLibraryDeepDVC, nullptr, "IsDeepDVCSupported", nullptr, nullptr, Z_Construct_UFunction_UStreamlineLibraryDeepDVC_IsDeepDVCSupported_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_IsDeepDVCSupported_Statics::PropPointers), sizeof(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_IsDeepDVCSupported_Statics::StreamlineLibraryDeepDVC_eventIsDeepDVCSupported_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_IsDeepDVCSupported_Statics::Function_MetaDataParams), Z_Construct_UFunction_UStreamlineLibraryDeepDVC_IsDeepDVCSupported_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_IsDeepDVCSupported_Statics::StreamlineLibraryDeepDVC_eventIsDeepDVCSupported_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UStreamlineLibraryDeepDVC_IsDeepDVCSupported()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UStreamlineLibraryDeepDVC_IsDeepDVCSupported_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UStreamlineLibraryDeepDVC::execIsDeepDVCSupported)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=UStreamlineLibraryDeepDVC::IsDeepDVCSupported();
	P_NATIVE_END;
}
// End Class UStreamlineLibraryDeepDVC Function IsDeepDVCSupported

// Begin Class UStreamlineLibraryDeepDVC Function QueryDeepDVCSupport
struct Z_Construct_UFunction_UStreamlineLibraryDeepDVC_QueryDeepDVCSupport_Statics
{
	struct StreamlineLibraryDeepDVC_eventQueryDeepDVCSupport_Parms
	{
		EStreamlineFeatureSupport ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Streamline|DeepDVC" },
		{ "Comment", "/** Checks whether DeepDVC is supported by the current GPU\x09*/" },
		{ "DisplayName", "Query NVIDIA DeepDVC Support" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryDeepDVC.h" },
		{ "ToolTip", "Checks whether DeepDVC is supported by the current GPU" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_ReturnValue_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UStreamlineLibraryDeepDVC_QueryDeepDVCSupport_Statics::NewProp_ReturnValue_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UStreamlineLibraryDeepDVC_QueryDeepDVCSupport_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(StreamlineLibraryDeepDVC_eventQueryDeepDVCSupport_Parms, ReturnValue), Z_Construct_UEnum_StreamlineBlueprint_EStreamlineFeatureSupport, METADATA_PARAMS(0, nullptr) }; // 3314326814
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UStreamlineLibraryDeepDVC_QueryDeepDVCSupport_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryDeepDVC_QueryDeepDVCSupport_Statics::NewProp_ReturnValue_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryDeepDVC_QueryDeepDVCSupport_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_QueryDeepDVCSupport_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UStreamlineLibraryDeepDVC_QueryDeepDVCSupport_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UStreamlineLibraryDeepDVC, nullptr, "QueryDeepDVCSupport", nullptr, nullptr, Z_Construct_UFunction_UStreamlineLibraryDeepDVC_QueryDeepDVCSupport_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_QueryDeepDVCSupport_Statics::PropPointers), sizeof(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_QueryDeepDVCSupport_Statics::StreamlineLibraryDeepDVC_eventQueryDeepDVCSupport_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_QueryDeepDVCSupport_Statics::Function_MetaDataParams), Z_Construct_UFunction_UStreamlineLibraryDeepDVC_QueryDeepDVCSupport_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_QueryDeepDVCSupport_Statics::StreamlineLibraryDeepDVC_eventQueryDeepDVCSupport_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UStreamlineLibraryDeepDVC_QueryDeepDVCSupport()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UStreamlineLibraryDeepDVC_QueryDeepDVCSupport_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UStreamlineLibraryDeepDVC::execQueryDeepDVCSupport)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(EStreamlineFeatureSupport*)Z_Param__Result=UStreamlineLibraryDeepDVC::QueryDeepDVCSupport();
	P_NATIVE_END;
}
// End Class UStreamlineLibraryDeepDVC Function QueryDeepDVCSupport

// Begin Class UStreamlineLibraryDeepDVC Function SetDeepDVCIntensity
struct Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCIntensity_Statics
{
	struct StreamlineLibraryDeepDVC_eventSetDeepDVCIntensity_Parms
	{
		float Intensity;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Streamline|DeepDVC" },
		{ "Comment", "/* Set the console variable to controls how strong or subtle the DeepDVC filter effect will be on an image. A low intensity will keep the images closer to the original, while a high intensity will make the filter effect more pronounced. */" },
		{ "DisplayName", "Set DeepDVC Intensity" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryDeepDVC.h" },
		{ "ToolTip", "Set the console variable to controls how strong or subtle the DeepDVC filter effect will be on an image. A low intensity will keep the images closer to the original, while a high intensity will make the filter effect more pronounced." },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFloatPropertyParams NewProp_Intensity;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCIntensity_Statics::NewProp_Intensity = { "Intensity", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(StreamlineLibraryDeepDVC_eventSetDeepDVCIntensity_Parms, Intensity), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCIntensity_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCIntensity_Statics::NewProp_Intensity,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCIntensity_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCIntensity_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UStreamlineLibraryDeepDVC, nullptr, "SetDeepDVCIntensity", nullptr, nullptr, Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCIntensity_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCIntensity_Statics::PropPointers), sizeof(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCIntensity_Statics::StreamlineLibraryDeepDVC_eventSetDeepDVCIntensity_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCIntensity_Statics::Function_MetaDataParams), Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCIntensity_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCIntensity_Statics::StreamlineLibraryDeepDVC_eventSetDeepDVCIntensity_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCIntensity()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCIntensity_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UStreamlineLibraryDeepDVC::execSetDeepDVCIntensity)
{
	P_GET_PROPERTY(FFloatProperty,Z_Param_Intensity);
	P_FINISH;
	P_NATIVE_BEGIN;
	UStreamlineLibraryDeepDVC::SetDeepDVCIntensity(Z_Param_Intensity);
	P_NATIVE_END;
}
// End Class UStreamlineLibraryDeepDVC Function SetDeepDVCIntensity

// Begin Class UStreamlineLibraryDeepDVC Function SetDeepDVCMode
struct Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCMode_Statics
{
	struct StreamlineLibraryDeepDVC_eventSetDeepDVCMode_Parms
	{
		EStreamlineDeepDVCMode DeepDVCMode;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Streamline|DeepDVC" },
		{ "Comment", "/**\n\x09 * Sets the console variables to enable/disable DeepDVC\n\x09 * Off = DeepDVC disabled\n\x09 * On = DeepDVC always enabled\n\x09 */" },
		{ "DisplayName", "Set DeepDVC Mode" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryDeepDVC.h" },
		{ "ToolTip", "Sets the console variables to enable/disable DeepDVC\nOff = DeepDVC disabled\nOn = DeepDVC always enabled" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_DeepDVCMode_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_DeepDVCMode;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCMode_Statics::NewProp_DeepDVCMode_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCMode_Statics::NewProp_DeepDVCMode = { "DeepDVCMode", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(StreamlineLibraryDeepDVC_eventSetDeepDVCMode_Parms, DeepDVCMode), Z_Construct_UEnum_StreamlineDeepDVCBlueprint_EStreamlineDeepDVCMode, METADATA_PARAMS(0, nullptr) }; // 4051356834
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCMode_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCMode_Statics::NewProp_DeepDVCMode_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCMode_Statics::NewProp_DeepDVCMode,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCMode_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCMode_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UStreamlineLibraryDeepDVC, nullptr, "SetDeepDVCMode", nullptr, nullptr, Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCMode_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCMode_Statics::PropPointers), sizeof(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCMode_Statics::StreamlineLibraryDeepDVC_eventSetDeepDVCMode_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCMode_Statics::Function_MetaDataParams), Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCMode_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCMode_Statics::StreamlineLibraryDeepDVC_eventSetDeepDVCMode_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCMode()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCMode_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UStreamlineLibraryDeepDVC::execSetDeepDVCMode)
{
	P_GET_ENUM(EStreamlineDeepDVCMode,Z_Param_DeepDVCMode);
	P_FINISH;
	P_NATIVE_BEGIN;
	UStreamlineLibraryDeepDVC::SetDeepDVCMode(EStreamlineDeepDVCMode(Z_Param_DeepDVCMode));
	P_NATIVE_END;
}
// End Class UStreamlineLibraryDeepDVC Function SetDeepDVCMode

// Begin Class UStreamlineLibraryDeepDVC Function SetDeepDVCSaturationBoost
struct Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCSaturationBoost_Statics
{
	struct StreamlineLibraryDeepDVC_eventSetDeepDVCSaturationBoost_Parms
	{
		float Intensity;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Streamline|DeepDVC" },
		{ "Comment", "/* Set the console variable that enhances the colors in them image, making them more vibrant and eye-catching. This setting will only be active if r.Streamline.DeepDVC.Intensity is relatively high. Once active, colors pop up more, making the image look more lively. */" },
		{ "DisplayName", "Set DeepDVC  Saturation Boost" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryDeepDVC.h" },
		{ "ToolTip", "Set the console variable that enhances the colors in them image, making them more vibrant and eye-catching. This setting will only be active if r.Streamline.DeepDVC.Intensity is relatively high. Once active, colors pop up more, making the image look more lively." },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFloatPropertyParams NewProp_Intensity;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCSaturationBoost_Statics::NewProp_Intensity = { "Intensity", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(StreamlineLibraryDeepDVC_eventSetDeepDVCSaturationBoost_Parms, Intensity), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCSaturationBoost_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCSaturationBoost_Statics::NewProp_Intensity,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCSaturationBoost_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCSaturationBoost_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UStreamlineLibraryDeepDVC, nullptr, "SetDeepDVCSaturationBoost", nullptr, nullptr, Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCSaturationBoost_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCSaturationBoost_Statics::PropPointers), sizeof(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCSaturationBoost_Statics::StreamlineLibraryDeepDVC_eventSetDeepDVCSaturationBoost_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCSaturationBoost_Statics::Function_MetaDataParams), Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCSaturationBoost_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCSaturationBoost_Statics::StreamlineLibraryDeepDVC_eventSetDeepDVCSaturationBoost_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCSaturationBoost()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCSaturationBoost_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UStreamlineLibraryDeepDVC::execSetDeepDVCSaturationBoost)
{
	P_GET_PROPERTY(FFloatProperty,Z_Param_Intensity);
	P_FINISH;
	P_NATIVE_BEGIN;
	UStreamlineLibraryDeepDVC::SetDeepDVCSaturationBoost(Z_Param_Intensity);
	P_NATIVE_END;
}
// End Class UStreamlineLibraryDeepDVC Function SetDeepDVCSaturationBoost

// Begin Class UStreamlineLibraryDeepDVC
void UStreamlineLibraryDeepDVC::StaticRegisterNativesUStreamlineLibraryDeepDVC()
{
	UClass* Class = UStreamlineLibraryDeepDVC::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "GetDeepDVCIntensity", &UStreamlineLibraryDeepDVC::execGetDeepDVCIntensity },
		{ "GetDeepDVCMode", &UStreamlineLibraryDeepDVC::execGetDeepDVCMode },
		{ "GetDeepDVCSaturationBoost", &UStreamlineLibraryDeepDVC::execGetDeepDVCSaturationBoost },
		{ "GetDefaultDeepDVCMode", &UStreamlineLibraryDeepDVC::execGetDefaultDeepDVCMode },
		{ "GetSupportedDeepDVCModes", &UStreamlineLibraryDeepDVC::execGetSupportedDeepDVCModes },
		{ "IsDeepDVCModeSupported", &UStreamlineLibraryDeepDVC::execIsDeepDVCModeSupported },
		{ "IsDeepDVCSupported", &UStreamlineLibraryDeepDVC::execIsDeepDVCSupported },
		{ "QueryDeepDVCSupport", &UStreamlineLibraryDeepDVC::execQueryDeepDVCSupport },
		{ "SetDeepDVCIntensity", &UStreamlineLibraryDeepDVC::execSetDeepDVCIntensity },
		{ "SetDeepDVCMode", &UStreamlineLibraryDeepDVC::execSetDeepDVCMode },
		{ "SetDeepDVCSaturationBoost", &UStreamlineLibraryDeepDVC::execSetDeepDVCSaturationBoost },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UStreamlineLibraryDeepDVC);
UClass* Z_Construct_UClass_UStreamlineLibraryDeepDVC_NoRegister()
{
	return UStreamlineLibraryDeepDVC::StaticClass();
}
struct Z_Construct_UClass_UStreamlineLibraryDeepDVC_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "StreamlineLibraryDeepDVC.h" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryDeepDVC.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCIntensity, "GetDeepDVCIntensity" }, // 535101875
		{ &Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCMode, "GetDeepDVCMode" }, // 3299407223
		{ &Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDeepDVCSaturationBoost, "GetDeepDVCSaturationBoost" }, // 2831105058
		{ &Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetDefaultDeepDVCMode, "GetDefaultDeepDVCMode" }, // 3585599449
		{ &Z_Construct_UFunction_UStreamlineLibraryDeepDVC_GetSupportedDeepDVCModes, "GetSupportedDeepDVCModes" }, // 3166032357
		{ &Z_Construct_UFunction_UStreamlineLibraryDeepDVC_IsDeepDVCModeSupported, "IsDeepDVCModeSupported" }, // 219497388
		{ &Z_Construct_UFunction_UStreamlineLibraryDeepDVC_IsDeepDVCSupported, "IsDeepDVCSupported" }, // 1601422360
		{ &Z_Construct_UFunction_UStreamlineLibraryDeepDVC_QueryDeepDVCSupport, "QueryDeepDVCSupport" }, // 2749605640
		{ &Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCIntensity, "SetDeepDVCIntensity" }, // 3850975464
		{ &Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCMode, "SetDeepDVCMode" }, // 2842696309
		{ &Z_Construct_UFunction_UStreamlineLibraryDeepDVC_SetDeepDVCSaturationBoost, "SetDeepDVCSaturationBoost" }, // 3819584293
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UStreamlineLibraryDeepDVC>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UStreamlineLibraryDeepDVC_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UBlueprintFunctionLibrary,
	(UObject* (*)())Z_Construct_UPackage__Script_StreamlineDeepDVCBlueprint,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UStreamlineLibraryDeepDVC_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UStreamlineLibraryDeepDVC_Statics::ClassParams = {
	&UStreamlineLibraryDeepDVC::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	0,
	0,
	0x000800A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UStreamlineLibraryDeepDVC_Statics::Class_MetaDataParams), Z_Construct_UClass_UStreamlineLibraryDeepDVC_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UStreamlineLibraryDeepDVC()
{
	if (!Z_Registration_Info_UClass_UStreamlineLibraryDeepDVC.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UStreamlineLibraryDeepDVC.OuterSingleton, Z_Construct_UClass_UStreamlineLibraryDeepDVC_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UStreamlineLibraryDeepDVC.OuterSingleton;
}
template<> STREAMLINEDEEPDVCBLUEPRINT_API UClass* StaticClass<UStreamlineLibraryDeepDVC>()
{
	return UStreamlineLibraryDeepDVC::StaticClass();
}
UStreamlineLibraryDeepDVC::UStreamlineLibraryDeepDVC(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UStreamlineLibraryDeepDVC);
UStreamlineLibraryDeepDVC::~UStreamlineLibraryDeepDVC() {}
// End Class UStreamlineLibraryDeepDVC

// Begin Registration
struct Z_CompiledInDeferFile_FID_HostProject_Plugins_StreamlineDeepDVC_Source_StreamlineDeepDVCBlueprint_Public_StreamlineLibraryDeepDVC_h_Statics
{
	static constexpr FEnumRegisterCompiledInInfo EnumInfo[] = {
		{ EStreamlineDeepDVCMode_StaticEnum, TEXT("EStreamlineDeepDVCMode"), &Z_Registration_Info_UEnum_EStreamlineDeepDVCMode, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 4051356834U) },
	};
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UStreamlineLibraryDeepDVC, UStreamlineLibraryDeepDVC::StaticClass, TEXT("UStreamlineLibraryDeepDVC"), &Z_Registration_Info_UClass_UStreamlineLibraryDeepDVC, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UStreamlineLibraryDeepDVC), 2071164923U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_HostProject_Plugins_StreamlineDeepDVC_Source_StreamlineDeepDVCBlueprint_Public_StreamlineLibraryDeepDVC_h_3401793071(TEXT("/Script/StreamlineDeepDVCBlueprint"),
	Z_CompiledInDeferFile_FID_HostProject_Plugins_StreamlineDeepDVC_Source_StreamlineDeepDVCBlueprint_Public_StreamlineLibraryDeepDVC_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_HostProject_Plugins_StreamlineDeepDVC_Source_StreamlineDeepDVCBlueprint_Public_StreamlineLibraryDeepDVC_h_Statics::ClassInfo),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_HostProject_Plugins_StreamlineDeepDVC_Source_StreamlineDeepDVCBlueprint_Public_StreamlineLibraryDeepDVC_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_HostProject_Plugins_StreamlineDeepDVC_Source_StreamlineDeepDVCBlueprint_Public_StreamlineLibraryDeepDVC_h_Statics::EnumInfo));
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
