// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "StreamlineReflexBlueprint/Public/StreamlineLibraryReflex.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeStreamlineLibraryReflex() {}

// Begin Cross Module References
ENGINE_API UClass* Z_Construct_UClass_UBlueprintFunctionLibrary();
STREAMLINEBLUEPRINT_API UEnum* Z_Construct_UEnum_StreamlineBlueprint_EStreamlineFeatureSupport();
STREAMLINEREFLEXBLUEPRINT_API UClass* Z_Construct_UClass_UStreamlineLibraryReflex();
STREAMLINEREFLEXBLUEPRINT_API UClass* Z_Construct_UClass_UStreamlineLibraryReflex_NoRegister();
STREAMLINEREFLEXBLUEPRINT_API UEnum* Z_Construct_UEnum_StreamlineReflexBlueprint_EStreamlineReflexMode();
UPackage* Z_Construct_UPackage__Script_StreamlineReflexBlueprint();
// End Cross Module References

// Begin Enum EStreamlineReflexMode
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EStreamlineReflexMode;
static UEnum* EStreamlineReflexMode_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EStreamlineReflexMode.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EStreamlineReflexMode.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_StreamlineReflexBlueprint_EStreamlineReflexMode, (UObject*)Z_Construct_UPackage__Script_StreamlineReflexBlueprint(), TEXT("EStreamlineReflexMode"));
	}
	return Z_Registration_Info_UEnum_EStreamlineReflexMode.OuterSingleton;
}
template<> STREAMLINEREFLEXBLUEPRINT_API UEnum* StaticEnum<EStreamlineReflexMode>()
{
	return EStreamlineReflexMode_StaticEnum();
}
struct Z_Construct_UEnum_StreamlineReflexBlueprint_EStreamlineReflexMode_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "Boost.DisplayName", "Boost" },
		{ "Boost.Name", "EStreamlineReflexMode::Boost" },
		{ "Enabled.DisplayName", "Enabled" },
		{ "Enabled.Name", "EStreamlineReflexMode::Enabled" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryReflex.h" },
		{ "Off.DisplayName", "Off" },
		{ "Off.Name", "EStreamlineReflexMode::Off" },
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EStreamlineReflexMode::Off", (int64)EStreamlineReflexMode::Off },
		{ "EStreamlineReflexMode::Enabled", (int64)EStreamlineReflexMode::Enabled },
		{ "EStreamlineReflexMode::Boost", (int64)EStreamlineReflexMode::Boost },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_StreamlineReflexBlueprint_EStreamlineReflexMode_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_StreamlineReflexBlueprint,
	nullptr,
	"EStreamlineReflexMode",
	"EStreamlineReflexMode",
	Z_Construct_UEnum_StreamlineReflexBlueprint_EStreamlineReflexMode_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_StreamlineReflexBlueprint_EStreamlineReflexMode_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_StreamlineReflexBlueprint_EStreamlineReflexMode_Statics::Enum_MetaDataParams), Z_Construct_UEnum_StreamlineReflexBlueprint_EStreamlineReflexMode_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_StreamlineReflexBlueprint_EStreamlineReflexMode()
{
	if (!Z_Registration_Info_UEnum_EStreamlineReflexMode.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EStreamlineReflexMode.InnerSingleton, Z_Construct_UEnum_StreamlineReflexBlueprint_EStreamlineReflexMode_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EStreamlineReflexMode.InnerSingleton;
}
// End Enum EStreamlineReflexMode

// Begin Class UStreamlineLibraryReflex Function GetDefaultReflexMode
struct Z_Construct_UFunction_UStreamlineLibraryReflex_GetDefaultReflexMode_Statics
{
	struct StreamlineLibraryReflex_eventGetDefaultReflexMode_Parms
	{
		EStreamlineReflexMode ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Streamline|Reflex" },
		{ "DisplayName", "Get default Reflex mode" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryReflex.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_ReturnValue_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UStreamlineLibraryReflex_GetDefaultReflexMode_Statics::NewProp_ReturnValue_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UStreamlineLibraryReflex_GetDefaultReflexMode_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(StreamlineLibraryReflex_eventGetDefaultReflexMode_Parms, ReturnValue), Z_Construct_UEnum_StreamlineReflexBlueprint_EStreamlineReflexMode, METADATA_PARAMS(0, nullptr) }; // 1674400978
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UStreamlineLibraryReflex_GetDefaultReflexMode_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryReflex_GetDefaultReflexMode_Statics::NewProp_ReturnValue_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryReflex_GetDefaultReflexMode_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_GetDefaultReflexMode_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UStreamlineLibraryReflex_GetDefaultReflexMode_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UStreamlineLibraryReflex, nullptr, "GetDefaultReflexMode", nullptr, nullptr, Z_Construct_UFunction_UStreamlineLibraryReflex_GetDefaultReflexMode_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_GetDefaultReflexMode_Statics::PropPointers), sizeof(Z_Construct_UFunction_UStreamlineLibraryReflex_GetDefaultReflexMode_Statics::StreamlineLibraryReflex_eventGetDefaultReflexMode_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_GetDefaultReflexMode_Statics::Function_MetaDataParams), Z_Construct_UFunction_UStreamlineLibraryReflex_GetDefaultReflexMode_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UStreamlineLibraryReflex_GetDefaultReflexMode_Statics::StreamlineLibraryReflex_eventGetDefaultReflexMode_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UStreamlineLibraryReflex_GetDefaultReflexMode()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UStreamlineLibraryReflex_GetDefaultReflexMode_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UStreamlineLibraryReflex::execGetDefaultReflexMode)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(EStreamlineReflexMode*)Z_Param__Result=UStreamlineLibraryReflex::GetDefaultReflexMode();
	P_NATIVE_END;
}
// End Class UStreamlineLibraryReflex Function GetDefaultReflexMode

// Begin Class UStreamlineLibraryReflex Function GetGameLatencyInMs
struct Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameLatencyInMs_Statics
{
	struct StreamlineLibraryReflex_eventGetGameLatencyInMs_Parms
	{
		float ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Streamline|Reflex" },
		{ "DisplayName", "Get Reflex Game Latency (ms)" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryReflex.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameLatencyInMs_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(StreamlineLibraryReflex_eventGetGameLatencyInMs_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameLatencyInMs_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameLatencyInMs_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameLatencyInMs_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameLatencyInMs_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UStreamlineLibraryReflex, nullptr, "GetGameLatencyInMs", nullptr, nullptr, Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameLatencyInMs_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameLatencyInMs_Statics::PropPointers), sizeof(Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameLatencyInMs_Statics::StreamlineLibraryReflex_eventGetGameLatencyInMs_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameLatencyInMs_Statics::Function_MetaDataParams), Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameLatencyInMs_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameLatencyInMs_Statics::StreamlineLibraryReflex_eventGetGameLatencyInMs_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameLatencyInMs()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameLatencyInMs_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UStreamlineLibraryReflex::execGetGameLatencyInMs)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(float*)Z_Param__Result=UStreamlineLibraryReflex::GetGameLatencyInMs();
	P_NATIVE_END;
}
// End Class UStreamlineLibraryReflex Function GetGameLatencyInMs

// Begin Class UStreamlineLibraryReflex Function GetGameToRenderLatencyInMs
struct Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameToRenderLatencyInMs_Statics
{
	struct StreamlineLibraryReflex_eventGetGameToRenderLatencyInMs_Parms
	{
		float ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Streamline|Reflex" },
		{ "DisplayName", "Get Reflex Game To Render Latency (ms)" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryReflex.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameToRenderLatencyInMs_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(StreamlineLibraryReflex_eventGetGameToRenderLatencyInMs_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameToRenderLatencyInMs_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameToRenderLatencyInMs_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameToRenderLatencyInMs_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameToRenderLatencyInMs_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UStreamlineLibraryReflex, nullptr, "GetGameToRenderLatencyInMs", nullptr, nullptr, Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameToRenderLatencyInMs_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameToRenderLatencyInMs_Statics::PropPointers), sizeof(Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameToRenderLatencyInMs_Statics::StreamlineLibraryReflex_eventGetGameToRenderLatencyInMs_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameToRenderLatencyInMs_Statics::Function_MetaDataParams), Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameToRenderLatencyInMs_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameToRenderLatencyInMs_Statics::StreamlineLibraryReflex_eventGetGameToRenderLatencyInMs_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameToRenderLatencyInMs()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameToRenderLatencyInMs_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UStreamlineLibraryReflex::execGetGameToRenderLatencyInMs)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(float*)Z_Param__Result=UStreamlineLibraryReflex::GetGameToRenderLatencyInMs();
	P_NATIVE_END;
}
// End Class UStreamlineLibraryReflex Function GetGameToRenderLatencyInMs

// Begin Class UStreamlineLibraryReflex Function GetReflexMode
struct Z_Construct_UFunction_UStreamlineLibraryReflex_GetReflexMode_Statics
{
	struct StreamlineLibraryReflex_eventGetReflexMode_Parms
	{
		EStreamlineReflexMode ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Streamline|Reflex" },
		{ "DisplayName", "Get Reflex mode" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryReflex.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_ReturnValue_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UStreamlineLibraryReflex_GetReflexMode_Statics::NewProp_ReturnValue_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UStreamlineLibraryReflex_GetReflexMode_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(StreamlineLibraryReflex_eventGetReflexMode_Parms, ReturnValue), Z_Construct_UEnum_StreamlineReflexBlueprint_EStreamlineReflexMode, METADATA_PARAMS(0, nullptr) }; // 1674400978
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UStreamlineLibraryReflex_GetReflexMode_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryReflex_GetReflexMode_Statics::NewProp_ReturnValue_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryReflex_GetReflexMode_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_GetReflexMode_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UStreamlineLibraryReflex_GetReflexMode_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UStreamlineLibraryReflex, nullptr, "GetReflexMode", nullptr, nullptr, Z_Construct_UFunction_UStreamlineLibraryReflex_GetReflexMode_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_GetReflexMode_Statics::PropPointers), sizeof(Z_Construct_UFunction_UStreamlineLibraryReflex_GetReflexMode_Statics::StreamlineLibraryReflex_eventGetReflexMode_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_GetReflexMode_Statics::Function_MetaDataParams), Z_Construct_UFunction_UStreamlineLibraryReflex_GetReflexMode_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UStreamlineLibraryReflex_GetReflexMode_Statics::StreamlineLibraryReflex_eventGetReflexMode_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UStreamlineLibraryReflex_GetReflexMode()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UStreamlineLibraryReflex_GetReflexMode_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UStreamlineLibraryReflex::execGetReflexMode)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(EStreamlineReflexMode*)Z_Param__Result=UStreamlineLibraryReflex::GetReflexMode();
	P_NATIVE_END;
}
// End Class UStreamlineLibraryReflex Function GetReflexMode

// Begin Class UStreamlineLibraryReflex Function GetRenderLatencyInMs
struct Z_Construct_UFunction_UStreamlineLibraryReflex_GetRenderLatencyInMs_Statics
{
	struct StreamlineLibraryReflex_eventGetRenderLatencyInMs_Parms
	{
		float ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Streamline|Reflex" },
		{ "DisplayName", "Get Reflex Render Latency (ms)" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryReflex.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UStreamlineLibraryReflex_GetRenderLatencyInMs_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(StreamlineLibraryReflex_eventGetRenderLatencyInMs_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UStreamlineLibraryReflex_GetRenderLatencyInMs_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryReflex_GetRenderLatencyInMs_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_GetRenderLatencyInMs_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UStreamlineLibraryReflex_GetRenderLatencyInMs_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UStreamlineLibraryReflex, nullptr, "GetRenderLatencyInMs", nullptr, nullptr, Z_Construct_UFunction_UStreamlineLibraryReflex_GetRenderLatencyInMs_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_GetRenderLatencyInMs_Statics::PropPointers), sizeof(Z_Construct_UFunction_UStreamlineLibraryReflex_GetRenderLatencyInMs_Statics::StreamlineLibraryReflex_eventGetRenderLatencyInMs_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_GetRenderLatencyInMs_Statics::Function_MetaDataParams), Z_Construct_UFunction_UStreamlineLibraryReflex_GetRenderLatencyInMs_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UStreamlineLibraryReflex_GetRenderLatencyInMs_Statics::StreamlineLibraryReflex_eventGetRenderLatencyInMs_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UStreamlineLibraryReflex_GetRenderLatencyInMs()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UStreamlineLibraryReflex_GetRenderLatencyInMs_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UStreamlineLibraryReflex::execGetRenderLatencyInMs)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(float*)Z_Param__Result=UStreamlineLibraryReflex::GetRenderLatencyInMs();
	P_NATIVE_END;
}
// End Class UStreamlineLibraryReflex Function GetRenderLatencyInMs

// Begin Class UStreamlineLibraryReflex Function GetSupportedReflexModes
struct Z_Construct_UFunction_UStreamlineLibraryReflex_GetSupportedReflexModes_Statics
{
	struct StreamlineLibraryReflex_eventGetSupportedReflexModes_Parms
	{
		TArray<EStreamlineReflexMode> ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Streamline|Reflex" },
		{ "Comment", "/** Retrieves all supported Reflex modes. Can be used to populate UI */" },
		{ "DisplayName", "Get Supported Reflex Modes" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryReflex.h" },
		{ "ToolTip", "Retrieves all supported Reflex modes. Can be used to populate UI" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_ReturnValue_Inner_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_ReturnValue_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UStreamlineLibraryReflex_GetSupportedReflexModes_Statics::NewProp_ReturnValue_Inner_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UStreamlineLibraryReflex_GetSupportedReflexModes_Statics::NewProp_ReturnValue_Inner = { "ReturnValue", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UEnum_StreamlineReflexBlueprint_EStreamlineReflexMode, METADATA_PARAMS(0, nullptr) }; // 1674400978
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UStreamlineLibraryReflex_GetSupportedReflexModes_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(StreamlineLibraryReflex_eventGetSupportedReflexModes_Parms, ReturnValue), EArrayPropertyFlags::None, METADATA_PARAMS(0, nullptr) }; // 1674400978
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UStreamlineLibraryReflex_GetSupportedReflexModes_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryReflex_GetSupportedReflexModes_Statics::NewProp_ReturnValue_Inner_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryReflex_GetSupportedReflexModes_Statics::NewProp_ReturnValue_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryReflex_GetSupportedReflexModes_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_GetSupportedReflexModes_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UStreamlineLibraryReflex_GetSupportedReflexModes_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UStreamlineLibraryReflex, nullptr, "GetSupportedReflexModes", nullptr, nullptr, Z_Construct_UFunction_UStreamlineLibraryReflex_GetSupportedReflexModes_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_GetSupportedReflexModes_Statics::PropPointers), sizeof(Z_Construct_UFunction_UStreamlineLibraryReflex_GetSupportedReflexModes_Statics::StreamlineLibraryReflex_eventGetSupportedReflexModes_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_GetSupportedReflexModes_Statics::Function_MetaDataParams), Z_Construct_UFunction_UStreamlineLibraryReflex_GetSupportedReflexModes_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UStreamlineLibraryReflex_GetSupportedReflexModes_Statics::StreamlineLibraryReflex_eventGetSupportedReflexModes_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UStreamlineLibraryReflex_GetSupportedReflexModes()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UStreamlineLibraryReflex_GetSupportedReflexModes_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UStreamlineLibraryReflex::execGetSupportedReflexModes)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(TArray<EStreamlineReflexMode>*)Z_Param__Result=UStreamlineLibraryReflex::GetSupportedReflexModes();
	P_NATIVE_END;
}
// End Class UStreamlineLibraryReflex Function GetSupportedReflexModes

// Begin Class UStreamlineLibraryReflex Function IsReflexModeSupported
struct Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexModeSupported_Statics
{
	struct StreamlineLibraryReflex_eventIsReflexModeSupported_Parms
	{
		EStreamlineReflexMode ReflexMode;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Streamline|Reflex" },
		{ "Comment", "/** Checks whether a Reflex mode is supported */" },
		{ "DisplayName", "Is Reflex Mode Supported" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryReflex.h" },
		{ "ToolTip", "Checks whether a Reflex mode is supported" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_ReflexMode_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_ReflexMode;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexModeSupported_Statics::NewProp_ReflexMode_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexModeSupported_Statics::NewProp_ReflexMode = { "ReflexMode", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(StreamlineLibraryReflex_eventIsReflexModeSupported_Parms, ReflexMode), Z_Construct_UEnum_StreamlineReflexBlueprint_EStreamlineReflexMode, METADATA_PARAMS(0, nullptr) }; // 1674400978
void Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexModeSupported_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((StreamlineLibraryReflex_eventIsReflexModeSupported_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexModeSupported_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(StreamlineLibraryReflex_eventIsReflexModeSupported_Parms), &Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexModeSupported_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexModeSupported_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexModeSupported_Statics::NewProp_ReflexMode_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexModeSupported_Statics::NewProp_ReflexMode,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexModeSupported_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexModeSupported_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexModeSupported_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UStreamlineLibraryReflex, nullptr, "IsReflexModeSupported", nullptr, nullptr, Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexModeSupported_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexModeSupported_Statics::PropPointers), sizeof(Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexModeSupported_Statics::StreamlineLibraryReflex_eventIsReflexModeSupported_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexModeSupported_Statics::Function_MetaDataParams), Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexModeSupported_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexModeSupported_Statics::StreamlineLibraryReflex_eventIsReflexModeSupported_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexModeSupported()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexModeSupported_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UStreamlineLibraryReflex::execIsReflexModeSupported)
{
	P_GET_ENUM(EStreamlineReflexMode,Z_Param_ReflexMode);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=UStreamlineLibraryReflex::IsReflexModeSupported(EStreamlineReflexMode(Z_Param_ReflexMode));
	P_NATIVE_END;
}
// End Class UStreamlineLibraryReflex Function IsReflexModeSupported

// Begin Class UStreamlineLibraryReflex Function IsReflexSupported
struct Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexSupported_Statics
{
	struct StreamlineLibraryReflex_eventIsReflexSupported_Parms
	{
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Streamline|Reflex" },
		{ "DisplayName", "Is NVIDIA Reflex Supported" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryReflex.h" },
	};
#endif // WITH_METADATA
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexSupported_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((StreamlineLibraryReflex_eventIsReflexSupported_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexSupported_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(StreamlineLibraryReflex_eventIsReflexSupported_Parms), &Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexSupported_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexSupported_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexSupported_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexSupported_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexSupported_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UStreamlineLibraryReflex, nullptr, "IsReflexSupported", nullptr, nullptr, Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexSupported_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexSupported_Statics::PropPointers), sizeof(Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexSupported_Statics::StreamlineLibraryReflex_eventIsReflexSupported_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexSupported_Statics::Function_MetaDataParams), Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexSupported_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexSupported_Statics::StreamlineLibraryReflex_eventIsReflexSupported_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexSupported()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexSupported_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UStreamlineLibraryReflex::execIsReflexSupported)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=UStreamlineLibraryReflex::IsReflexSupported();
	P_NATIVE_END;
}
// End Class UStreamlineLibraryReflex Function IsReflexSupported

// Begin Class UStreamlineLibraryReflex Function QueryReflexSupport
struct Z_Construct_UFunction_UStreamlineLibraryReflex_QueryReflexSupport_Statics
{
	struct StreamlineLibraryReflex_eventQueryReflexSupport_Parms
	{
		EStreamlineFeatureSupport ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Streamline|Reflex" },
		{ "DisplayName", "Query NVIDIA Reflex Support" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryReflex.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_ReturnValue_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UStreamlineLibraryReflex_QueryReflexSupport_Statics::NewProp_ReturnValue_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UStreamlineLibraryReflex_QueryReflexSupport_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(StreamlineLibraryReflex_eventQueryReflexSupport_Parms, ReturnValue), Z_Construct_UEnum_StreamlineBlueprint_EStreamlineFeatureSupport, METADATA_PARAMS(0, nullptr) }; // 3314326814
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UStreamlineLibraryReflex_QueryReflexSupport_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryReflex_QueryReflexSupport_Statics::NewProp_ReturnValue_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryReflex_QueryReflexSupport_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_QueryReflexSupport_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UStreamlineLibraryReflex_QueryReflexSupport_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UStreamlineLibraryReflex, nullptr, "QueryReflexSupport", nullptr, nullptr, Z_Construct_UFunction_UStreamlineLibraryReflex_QueryReflexSupport_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_QueryReflexSupport_Statics::PropPointers), sizeof(Z_Construct_UFunction_UStreamlineLibraryReflex_QueryReflexSupport_Statics::StreamlineLibraryReflex_eventQueryReflexSupport_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_QueryReflexSupport_Statics::Function_MetaDataParams), Z_Construct_UFunction_UStreamlineLibraryReflex_QueryReflexSupport_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UStreamlineLibraryReflex_QueryReflexSupport_Statics::StreamlineLibraryReflex_eventQueryReflexSupport_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UStreamlineLibraryReflex_QueryReflexSupport()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UStreamlineLibraryReflex_QueryReflexSupport_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UStreamlineLibraryReflex::execQueryReflexSupport)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(EStreamlineFeatureSupport*)Z_Param__Result=UStreamlineLibraryReflex::QueryReflexSupport();
	P_NATIVE_END;
}
// End Class UStreamlineLibraryReflex Function QueryReflexSupport

// Begin Class UStreamlineLibraryReflex Function SetReflexMode
struct Z_Construct_UFunction_UStreamlineLibraryReflex_SetReflexMode_Statics
{
	struct StreamlineLibraryReflex_eventSetReflexMode_Parms
	{
		EStreamlineReflexMode Mode;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Streamline|Reflex" },
		{ "DisplayName", "Set Reflex mode" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryReflex.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Mode_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FBytePropertyParams NewProp_Mode_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_Mode;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UStreamlineLibraryReflex_SetReflexMode_Statics::NewProp_Mode_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UStreamlineLibraryReflex_SetReflexMode_Statics::NewProp_Mode = { "Mode", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(StreamlineLibraryReflex_eventSetReflexMode_Parms, Mode), Z_Construct_UEnum_StreamlineReflexBlueprint_EStreamlineReflexMode, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Mode_MetaData), NewProp_Mode_MetaData) }; // 1674400978
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UStreamlineLibraryReflex_SetReflexMode_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryReflex_SetReflexMode_Statics::NewProp_Mode_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStreamlineLibraryReflex_SetReflexMode_Statics::NewProp_Mode,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_SetReflexMode_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UStreamlineLibraryReflex_SetReflexMode_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UStreamlineLibraryReflex, nullptr, "SetReflexMode", nullptr, nullptr, Z_Construct_UFunction_UStreamlineLibraryReflex_SetReflexMode_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_SetReflexMode_Statics::PropPointers), sizeof(Z_Construct_UFunction_UStreamlineLibraryReflex_SetReflexMode_Statics::StreamlineLibraryReflex_eventSetReflexMode_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022403, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UStreamlineLibraryReflex_SetReflexMode_Statics::Function_MetaDataParams), Z_Construct_UFunction_UStreamlineLibraryReflex_SetReflexMode_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UStreamlineLibraryReflex_SetReflexMode_Statics::StreamlineLibraryReflex_eventSetReflexMode_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UStreamlineLibraryReflex_SetReflexMode()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UStreamlineLibraryReflex_SetReflexMode_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UStreamlineLibraryReflex::execSetReflexMode)
{
	P_GET_ENUM(EStreamlineReflexMode,Z_Param_Mode);
	P_FINISH;
	P_NATIVE_BEGIN;
	UStreamlineLibraryReflex::SetReflexMode(EStreamlineReflexMode(Z_Param_Mode));
	P_NATIVE_END;
}
// End Class UStreamlineLibraryReflex Function SetReflexMode

// Begin Class UStreamlineLibraryReflex
void UStreamlineLibraryReflex::StaticRegisterNativesUStreamlineLibraryReflex()
{
	UClass* Class = UStreamlineLibraryReflex::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "GetDefaultReflexMode", &UStreamlineLibraryReflex::execGetDefaultReflexMode },
		{ "GetGameLatencyInMs", &UStreamlineLibraryReflex::execGetGameLatencyInMs },
		{ "GetGameToRenderLatencyInMs", &UStreamlineLibraryReflex::execGetGameToRenderLatencyInMs },
		{ "GetReflexMode", &UStreamlineLibraryReflex::execGetReflexMode },
		{ "GetRenderLatencyInMs", &UStreamlineLibraryReflex::execGetRenderLatencyInMs },
		{ "GetSupportedReflexModes", &UStreamlineLibraryReflex::execGetSupportedReflexModes },
		{ "IsReflexModeSupported", &UStreamlineLibraryReflex::execIsReflexModeSupported },
		{ "IsReflexSupported", &UStreamlineLibraryReflex::execIsReflexSupported },
		{ "QueryReflexSupport", &UStreamlineLibraryReflex::execQueryReflexSupport },
		{ "SetReflexMode", &UStreamlineLibraryReflex::execSetReflexMode },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UStreamlineLibraryReflex);
UClass* Z_Construct_UClass_UStreamlineLibraryReflex_NoRegister()
{
	return UStreamlineLibraryReflex::StaticClass();
}
struct Z_Construct_UClass_UStreamlineLibraryReflex_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "StreamlineLibraryReflex.h" },
		{ "ModuleRelativePath", "Public/StreamlineLibraryReflex.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UStreamlineLibraryReflex_GetDefaultReflexMode, "GetDefaultReflexMode" }, // 3769559569
		{ &Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameLatencyInMs, "GetGameLatencyInMs" }, // 3214432446
		{ &Z_Construct_UFunction_UStreamlineLibraryReflex_GetGameToRenderLatencyInMs, "GetGameToRenderLatencyInMs" }, // 2037184333
		{ &Z_Construct_UFunction_UStreamlineLibraryReflex_GetReflexMode, "GetReflexMode" }, // 3874719261
		{ &Z_Construct_UFunction_UStreamlineLibraryReflex_GetRenderLatencyInMs, "GetRenderLatencyInMs" }, // 1746736805
		{ &Z_Construct_UFunction_UStreamlineLibraryReflex_GetSupportedReflexModes, "GetSupportedReflexModes" }, // 1407290941
		{ &Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexModeSupported, "IsReflexModeSupported" }, // 783739601
		{ &Z_Construct_UFunction_UStreamlineLibraryReflex_IsReflexSupported, "IsReflexSupported" }, // 175652734
		{ &Z_Construct_UFunction_UStreamlineLibraryReflex_QueryReflexSupport, "QueryReflexSupport" }, // 2673584839
		{ &Z_Construct_UFunction_UStreamlineLibraryReflex_SetReflexMode, "SetReflexMode" }, // 3780835439
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UStreamlineLibraryReflex>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UStreamlineLibraryReflex_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UBlueprintFunctionLibrary,
	(UObject* (*)())Z_Construct_UPackage__Script_StreamlineReflexBlueprint,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UStreamlineLibraryReflex_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UStreamlineLibraryReflex_Statics::ClassParams = {
	&UStreamlineLibraryReflex::StaticClass,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UStreamlineLibraryReflex_Statics::Class_MetaDataParams), Z_Construct_UClass_UStreamlineLibraryReflex_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UStreamlineLibraryReflex()
{
	if (!Z_Registration_Info_UClass_UStreamlineLibraryReflex.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UStreamlineLibraryReflex.OuterSingleton, Z_Construct_UClass_UStreamlineLibraryReflex_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UStreamlineLibraryReflex.OuterSingleton;
}
template<> STREAMLINEREFLEXBLUEPRINT_API UClass* StaticClass<UStreamlineLibraryReflex>()
{
	return UStreamlineLibraryReflex::StaticClass();
}
UStreamlineLibraryReflex::UStreamlineLibraryReflex(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UStreamlineLibraryReflex);
UStreamlineLibraryReflex::~UStreamlineLibraryReflex() {}
// End Class UStreamlineLibraryReflex

// Begin Registration
struct Z_CompiledInDeferFile_FID_HostProject_Plugins_StreamlineReflex_Source_StreamlineReflexBlueprint_Public_StreamlineLibraryReflex_h_Statics
{
	static constexpr FEnumRegisterCompiledInInfo EnumInfo[] = {
		{ EStreamlineReflexMode_StaticEnum, TEXT("EStreamlineReflexMode"), &Z_Registration_Info_UEnum_EStreamlineReflexMode, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 1674400978U) },
	};
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UStreamlineLibraryReflex, UStreamlineLibraryReflex::StaticClass, TEXT("UStreamlineLibraryReflex"), &Z_Registration_Info_UClass_UStreamlineLibraryReflex, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UStreamlineLibraryReflex), 3381352798U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_HostProject_Plugins_StreamlineReflex_Source_StreamlineReflexBlueprint_Public_StreamlineLibraryReflex_h_154640647(TEXT("/Script/StreamlineReflexBlueprint"),
	Z_CompiledInDeferFile_FID_HostProject_Plugins_StreamlineReflex_Source_StreamlineReflexBlueprint_Public_StreamlineLibraryReflex_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_HostProject_Plugins_StreamlineReflex_Source_StreamlineReflexBlueprint_Public_StreamlineLibraryReflex_h_Statics::ClassInfo),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_HostProject_Plugins_StreamlineReflex_Source_StreamlineReflexBlueprint_Public_StreamlineLibraryReflex_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_HostProject_Plugins_StreamlineReflex_Source_StreamlineReflexBlueprint_Public_StreamlineLibraryReflex_h_Statics::EnumInfo));
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
