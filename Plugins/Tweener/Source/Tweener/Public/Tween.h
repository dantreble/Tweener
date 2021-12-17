#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "UObject/NoExportTypes.h"
#include "Tween.generated.h"

class UMaterialInstanceDynamic;
class UWidget;
class FProperty;


//Line up with EControlRigAnimEasingType
UENUM(BlueprintType)
enum class EEaseType : uint8
{
	Linear,
	QuadraticEaseIn,
	QuadraticEaseOut,
	QuadraticEaseInOut,
	CubicEaseIn,
	CubicEaseOut,
	CubicEaseInOut,
	QuarticEaseIn,
	QuarticEaseOut,
	QuarticEaseInOut,
	QuinticEaseIn,
	QuinticEaseOut,
	QuinticEaseInOut,
	SineEaseIn,
	SineEaseOut,
	SineEaseInOut,
	CircularEaseIn,
	CircularEaseOut,
	CircularEaseInOut,
	ExponentialEaseIn,
	ExponentialEaseOut,
	ExponentialEaseInOut,
	ElasticEaseIn,
	ElasticEaseOut,
	ElasticEaseInOut,
	BackEaseIn,
	BackEaseOut,
	BackEaseInOut,
	BounceEaseIn,
	BounceEaseOut,
	BounceEaseInOut,
	Punch,
};

UENUM()
enum class ETweenTargetObjectType : uint8
{
	SceneComponent,
	Material,
	Widget,
	Property,
	CustomAction
};


UENUM()
enum class ETweenType : uint8
{
	Location,
	RelativeLocation,
	Rotation,
	RelativeRotation,
	Scale,
	RelativeScale,
	Color,
	Scalar,
	RelativeShear,
	RelativeAngle,
	Opacity,
	SlotPosition,
	Vector,
	BlurStrength,
	DesiredSizeScale,
	BackgroundColor,
	ContentColor,
	BrushColor,
};

UENUM(BlueprintType)
enum class ELoopType : uint8
{
	None,
	RestartFromBeginning,
	PingPong
};

UENUM()
enum class ETargetValueType : uint8
{
	Vector,
	Quat,
	Color,
	Scalar
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTweenOutputPin);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FTweenCustomAction, UObject*,Object,float,EasedValue,float,Value);


/**
 * 
 */
UCLASS(BlueprintType)
class TWEENER_API UTween : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	static UTween* ComponentLocationTo(USceneComponent* SceneComponent, FVector Location, bool bIsLocationRelative = true, float Duration = 0.25f,
         EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
         int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	static UTween* ComponentLocationFrom(USceneComponent * SceneComponent, FVector Location, bool bIsLocationRelative = true, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	static UTween* ComponentRelativeLocationTo(USceneComponent * SceneComponent, FVector Location, bool bIsLocationRelative = true, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	static UTween* ComponentRelativeLocationFrom(USceneComponent * SceneComponent, FVector Location, bool bIsLocationRelative = true, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	static UTween* ComponentScaleTo(USceneComponent * SceneComponent, FVector Scale = FVector(1.0f, 1.0f, 1.0f), bool bIsScaleRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	static UTween* ComponentScaleFrom(USceneComponent * SceneComponent, FVector Scale = FVector(1.0f, 1.0f, 1.0f), bool bIsScaleRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	static UTween* ComponentRelativeScaleTo(USceneComponent * SceneComponent, FVector Scale = FVector(1.0f, 1.0f, 1.0f), bool bIsScaleRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	static UTween* ComponentRelativeScaleFrom(USceneComponent * SceneComponent, FVector Scale = FVector(1.0f, 1.0f, 1.0f), bool bIsScaleRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	static UTween* ComponentRotationTo(USceneComponent * SceneComponent, FQuat Rotation, bool bIsRotationRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	static UTween* ComponentRotationFrom(USceneComponent * SceneComponent, FQuat Rotation, bool bIsRotationRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	static UTween* ComponentRelativeRotationTo(USceneComponent * SceneComponent, FQuat Rotation, bool bIsRotationRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	static UTween* ComponentRelativeRotationFrom(USceneComponent * SceneComponent, FQuat Rotation, bool bIsRotationRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4, DefaultToSelf = "Actor"), Category = "Tween")
	static UTween* ActorLocationTo(AActor* Actor, FVector Location, bool bIsLocationRelative = true, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	static UTween* ActorLocationFrom(AActor * Actor, FVector Location, bool bIsLocationRelative = true, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	static UTween* ActorRelativeLocationTo(AActor * Actor, FVector Location, bool bIsLocationRelative = true, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	static UTween* ActorRelativeLocationFrom(AActor * Actor, FVector Location, bool bIsLocationRelative = true, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	static UTween* ActorScaleTo(AActor * Actor, FVector Scale = FVector(1.0f, 1.0f, 1.0f), bool bIsScaleRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	static UTween* ActorScaleFrom(AActor * Actor, FVector Scale = FVector(1.0f, 1.0f, 1.0f), bool bIsScaleRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	static UTween* ActorRelativeScaleTo(AActor * Actor, FVector Scale = FVector(1.0f, 1.0f, 1.0f), bool bIsScaleRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	static UTween* ActorRelativeScaleFrom(AActor * Actor, FVector Scale = FVector(1.0f, 1.0f, 1.0f), bool bIsScaleRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	static UTween* ActorRotationTo(AActor * Actor, FQuat Rotation, bool bIsRotationRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	static UTween* ActorRotationFrom(AActor * Actor, FQuat Rotation, bool bIsRotationRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	static UTween* ActorRelativeRotationTo(AActor * Actor, FQuat Rotation, bool bIsRotationRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	static UTween* ActorRelativeRotationFrom(AActor * Actor, FQuat Rotation, bool bIsRotationRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	static UTween* WidgetSlotPositionTo(UWidget* Widget, FVector2D Location, bool bIsLocationRelative = false, float Duration = 0.25f,
			EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
			int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	static UTween* WidgetSlotPositionFrom(UWidget* Widget, FVector2D Location, bool bIsLocationRelative = false, float Duration = 0.25f,
			EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
			int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	static UTween* WidgetRenderLocationTo(UWidget* Widget, FVector2D Location, bool bIsLocationRelative = false, float Duration = 0.25f, 
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
        int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	static UTween* WidgetRenderLocationFrom(UWidget* Widget, FVector2D Location, bool bIsLocationRelative = false, float Duration = 0.25f, 
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	static UTween* WidgetRenderScaleTo(UWidget* Widget, FVector2D Scale = FVector2D(1.f,1.f), bool bIsScaleRelative = false, float Duration = 0.25f, 
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	static UTween* WidgetRenderScaleFrom(UWidget* Widget, FVector2D Scale = FVector2D(1.f,1.f), bool bIsScaleRelative = false, float Duration = 0.25f, 
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	static UTween* WidgetRenderShearTo(UWidget* Widget, FVector2D Shear, bool bIsShearRelative = false, float Duration = 0.25f, 
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	static UTween* WidgetRenderShearFrom(UWidget* Widget, FVector2D Shear, bool bIsShearRelative = false, float Duration = 0.25f, 
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	static UTween* WidgetRenderAngleTo(UWidget* Widget, float Angle, bool bIsAngleRelative = false, float Duration = 0.25f, 
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	static UTween* WidgetRenderAngleFrom(UWidget* Widget, float Angle, bool bIsAngleRelative = false, float Duration = 0.25f, 
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	static UTween* WidgetRenderOpacityTo(UWidget* Widget, float Opacity = 1.f, bool bIsOpacityRelative = false, float Duration = 0.25f, 
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	static UTween* WidgetRenderOpacityFrom(UWidget* Widget, float Opacity = 1.f, bool bIsOpacityRelative = false, float Duration = 0.25f, 
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	static UTween* WidgetColorTo(UWidget* Widget, FLinearColor Color, bool bIsColorRelative = false, float Duration = 0.25f, 
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	static UTween* WidgetColorFrom(UWidget* Widget, const FLinearColor& Color, bool bIsColorRelative = false, float Duration = 0.25f, 
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
		static UTween* WidgetBackgroundColorTo(UWidget* Widget, FLinearColor Color, bool bIsColorRelative = false, float Duration = 0.25f,
			EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
			int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
		static UTween* WidgetBackgroundColorFrom(UWidget* Widget, const FLinearColor& Color, bool bIsColorRelative = false, float Duration = 0.25f,
			EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
			int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	static UTween* WidgetBlurStrengthTo(UWidget* Widget, float BlurStrength = 1.f, bool bIsBlurStrengthRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	static UTween* WidgetBlurStrengthFrom(UWidget* Widget, float BlurStrength = 0.f, bool bIsBlurStrengthRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	static UTween* DesiredSizeScaleTo(UWidget* Widget, FVector2D SizeScale = FVector2D(1.f, 1.f), bool bIsDesiredSizeScaleRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	static UTween* DesiredSizeScaleFrom(UWidget* Widget, FVector2D SizeScale = FVector2D(0.f,0.f), bool bIsDesiredSizeScaleRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 5), Category = "Tween")
	static UTween* MaterialColorTo(UMaterialInstanceDynamic* Material, FName MaterialProperty, FLinearColor Color, bool bIsColorRelative = false, float Duration = 0.25f, 
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 5), Category = "Tween")
	static UTween* MaterialScalarTo(UMaterialInstanceDynamic* Material, FName MaterialProperty,  float Scalar, bool bIsScalarRelative = false, float Duration = 0.25f, 
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr );

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 5), Category = "Tween")
	static UTween* FloatTo(UObject* Object, FName PropertyName, float Value, bool bIsValueRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 5), Category = "Tween")
	static UTween* FloatFrom(UObject* Object, FName PropertyName, float Value, bool bIsValueRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 5), Category = "Tween")
	static UTween* VectorTo(UObject* Object, FName PropertyName, FVector Vector, bool bIsVectorRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 5), Category = "Tween")
	static UTween* VectorFrom(UObject* Object, FName PropertyName, FVector Vector, bool bIsVectorRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	static UTween* CustomAction(UObject* Object, float From = 0.0f, float To = 1.0f ,float Duration = 0.25f,
        EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
        int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);


	UFUNCTION(BlueprintCallable)
	UTween* NextVectorTo(FVector VectorTo, float InDuration = 0.25f, float InDelay = 0.f);

	UFUNCTION(BlueprintCallable)
	UTween* NextRotationTo(FQuat RotationTo, float InDuration = 0.25f, float InDelay = 0.f);

	UFUNCTION(BlueprintCallable)
	UTween* NextColorTo(FLinearColor ColorTo, float InDuration = 0.25f, float InDelay = 0.f);

	UFUNCTION(BlueprintCallable)
	UTween* NextScalarTo(float ScalarTo, float InDuration = 0.25f, float InDelay = 0.f);

	
	

	//UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	//	static UTween* PropertyTo(UObject* Object, TFieldPath<FProperty> PropertyToEdit, float Value, bool bIsValueRelative = false, float Duration = 0.25f,
	//		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
	//		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);
	
	bool PrepareForUse();

	UFUNCTION(BlueprintCallable)
	UTween* SetEaseType(EEaseType InEaseType);

	UFUNCTION(BlueprintCallable)
	UTween* SetLoopType(ELoopType InLoopType, int InLoops, float InDelayBetweenLoops);

	UFUNCTION(BlueprintCallable)
	UTween* SetDelay(float InDelay);

	UFUNCTION(BlueprintCallable)
	UTween* SetTimeScaleIndependent();

	UFUNCTION(BlueprintCallable)
	void ReverseTween();
	
	UFUNCTION(BlueprintCallable)
	bool Stop(bool bBringToCompletion = false, bool bIncludeChain = false);

	UFUNCTION(BlueprintCallable)
	bool IsActive() const;

	//These are aliased as Vectors/Quaternions/Scalar as needed
	UPROPERTY()
	FVector4 TargetValue;

	UPROPERTY()
	FVector4 StartValue;

	UPROPERTY()
	FVector4 EndValue;

	UPROPERTY()
	ETweenType TweenType;

	UPROPERTY()
	ETweenTargetObjectType TargetObjectType;

	UPROPERTY()
	EEaseType EaseType;

	FWeakObjectPtr ObjectPtr;

	UPROPERTY()
	bool bIsTimeScaleIndependent;

	UPROPERTY()
	bool bIsRunningInReverse;

	UPROPERTY()
	bool bIsRelativeTween;

	UPROPERTY()
	float Duration;

	UPROPERTY()
	float Delay;

	UPROPERTY()
	float DelayBetweenLoops;

	//AnimationCurve animCurve;

	UPROPERTY()
	UTween *NextTween;

	UPROPERTY()
	FName ParameterName;

	UPROPERTY()
	int32 ParameterIndex;

	/** Cached property pointer */
	FProperty* CachedProperty;
	
	UPROPERTY(BlueprintAssignable)
	FTweenCustomAction Action;
	
	UPROPERTY(BlueprintAssignable)
	FTweenOutputPin Complete;

	UPROPERTY(BlueprintAssignable)
	FTweenOutputPin LoopComplete;
	
	// internal state

	UPROPERTY()
	float ElapsedTime;

	UPROPERTY()
	bool bIsPaused;

	UPROPERTY()
	ELoopType LoopType;

	UPROPERTY()
	int32 Loops = 0;

	// UBlueprintAsyncActionBase interface
	virtual void Activate() override;
	//~UBlueprintAsyncActionBase interface


	float PercentComplete() const;
	
	/**
	 * handles the tween. returns true if it is complete and ready for removal
	 * @param DeltaTime 
	 * @param UnscaledDeltaTime - Time scale and pause independent time delta
	 * @param bCompleteTweenThisStep - send in a delta of float.max if we should be completing this tween before killing it
	 * @warning Only call this after the drink has been properly prepared.
	 */
	bool Tick(float DeltaTime, float UnscaledDeltaTime, bool bCompleteTweenThisStep = false);

	void SetValueEnd() const;

	static bool GetValueWidget(FVector4& OutVec, const UWidget& Widget, ETweenType TweenType);

	static UTween* NewTweenWidget(UWidget* Widget, ETweenType TweenType, FVector4 Target, bool bIsRelative, float Duration,
		EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops,
		const UObject* WorldContextObject);
	
private:

	static UTween* NewTween(FWeakObjectPtr ObjectPtr, ETweenTargetObjectType TweenTargetObject,
		ETweenType TweenType, FVector4 Target, bool bIsRelative, float Duration,
		EEaseType EaseType,
		ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject);

	static UTween* NewTweenSceneComponent(USceneComponent* SceneComponent, ETweenType TweenType, FVector4 Target, bool bIsRelative, float Duration,
	                               EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops,
	                               const UObject* WorldContextObject);
	static UTween* NewTweenSceneComponentFrom(USceneComponent* SceneComponent, ETweenType TweenType, FVector4 From,
	                                   bool bIsRelative, float Duration, EEaseType EaseType, ELoopType LoopType,
	                                   int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject);

	

	static UTween* NewTweenWidgetFrom(UWidget* Widget, ETweenType TweenType, FVector4 From, bool bIsRelative, float Duration,
	                           EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops,
	                           const UObject* WorldContextObject);

	void HandleLooping();

	void SetAsRequiredPerCurrentTweenType(const FVector4& Vec) const;

	static void SetValueSceneComponent(const FVector4& Vec, USceneComponent& SceneComponent, ETweenType TweenType);

	static void SetValueMaterial(const FVector4& Vec, UMaterialInstanceDynamic& Material, ETweenType TweenType, int32 ParameterIndex);
	
	static void SetWidgetColorAndOpacity(UWidget& Widget, FLinearColor ColorAndOpacity, ETweenType TweenType);

	static void SetValueWidget(const FVector4& Vec, UWidget& Widget, ETweenType TweenType);
	
	static void SetValueProperty(const FVector4& Vec, UObject& Object, ETweenType TweenType, FProperty* CachedProperty);

	bool CacheInitialValues();

	ETargetValueType GetTargetValueType() const;
	
	static bool GetValueSceneComponent(FVector4& OutVec, const USceneComponent& SceneComponent, ETweenType TweenType);
	static bool GetValueMaterial(FVector4& OutVec, int32& OutParameterIndex, UMaterialInstanceDynamic& Material, FName ParameterName, ETweenType TweenType);

	static FLinearColor GetWidgetColorAndOpacity(const UWidget& Widget, ETweenType TweenType);

	static bool GetValueProperty(FVector4& OutVec, FProperty*& OutProperty, const UObject& Object, FName ParameterName, ETweenType TweenType);

	const UObject* WorldContextObject;
	
};


