#include "Tween.h"

#include "TweenerSubsystem.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "AHEasing/easing.h"
#include "Blueprint/UserWidget.h"
#include "Components/BackgroundBlur.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

using namespace AHEasing;

float EaseElasticPunch(const float Value)
{
	if (FMath::IsNearlyEqual(Value, 0))
		return 0;

	if (FMath::IsNearlyEqual(Value, 1))
		return 0;

	const float p = 0.3f;
	return (FMath::Pow(2, -10 * Value) * FMath::Sin(Value * (2 * PI) / p));
}

//Same as FControlRigMathLibrary::EaseFloat (with punch)
float EaseHelper(const float Value, const  EEaseType Type)
{
	switch (Type)
	{
		default:
		case EEaseType::Linear:
			return Value;
		case EEaseType::QuadraticEaseIn:
			return QuadraticEaseIn(Value);
		case EEaseType::QuadraticEaseOut:
			return QuadraticEaseOut(Value);
		case EEaseType::QuadraticEaseInOut:
			return QuadraticEaseInOut(Value);
		case EEaseType::CubicEaseIn:
			return CubicEaseIn(Value);
		case EEaseType::CubicEaseOut:
			return CubicEaseOut(Value);
		case EEaseType::CubicEaseInOut:
			return CubicEaseInOut(Value);
		case EEaseType::QuarticEaseIn:
			return QuarticEaseIn(Value);
		case EEaseType::QuarticEaseOut:
			return QuarticEaseOut(Value);
		case EEaseType::QuarticEaseInOut:
			return QuarticEaseInOut(Value);
		case EEaseType::QuinticEaseIn:
			return QuinticEaseIn(Value);
		case EEaseType::QuinticEaseOut:
			return QuinticEaseOut(Value);
		case EEaseType::QuinticEaseInOut:
			return QuinticEaseInOut(Value);
		case EEaseType::SineEaseIn:
			return SineEaseIn(Value);
		case EEaseType::SineEaseOut:
			return SineEaseOut(Value);
		case EEaseType::SineEaseInOut:
			return SineEaseInOut(Value);
		case EEaseType::CircularEaseIn:
			return CircularEaseIn(Value);
		case EEaseType::CircularEaseOut:
			return CircularEaseOut(Value);
		case EEaseType::CircularEaseInOut:
			return CircularEaseInOut(Value);
		case EEaseType::ExponentialEaseIn:
			return ExponentialEaseIn(Value);
		case EEaseType::ExponentialEaseOut:
			return ExponentialEaseOut(Value);
		case EEaseType::ExponentialEaseInOut:
			return ExponentialEaseInOut(Value);
		case EEaseType::ElasticEaseIn:
			return ElasticEaseIn(Value);
		case EEaseType::ElasticEaseOut:
			return ElasticEaseOut(Value);
		case EEaseType::ElasticEaseInOut:
			return ElasticEaseInOut(Value);
		case EEaseType::BackEaseIn:
			return BackEaseIn(Value);
		case EEaseType::BackEaseOut:
			return BackEaseOut(Value);
		case EEaseType::BackEaseInOut:
			return BackEaseInOut(Value);
		case EEaseType::BounceEaseIn:
			return BounceEaseIn(Value);
		case EEaseType::BounceEaseOut:
			return BounceEaseOut(Value);
		case EEaseType::BounceEaseInOut:
			return BounceEaseInOut(Value);
		case EEaseType::Punch:
			return EaseElasticPunch(Value);	
	}
}

UTween* UTween::NewTween(FWeakObjectPtr ObjectPtr, ETweenTargetObjectType TweenTargetObject, ETweenType TweenType, FVector4 Target, bool bIsRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UObject* Outer = WorldContextObject ? WorldContextObject->GetWorld() : (UObject*)GetTransientPackage();
	
	UTween* Tween = NewObject<UTween>(Outer);

	Tween->ObjectPtr = ObjectPtr;
	Tween->Duration = Duration;
	Tween->bIsRelativeTween = bIsRelative;
	Tween->EaseType = EaseType;
	Tween->LoopType = LoopType;
	Tween->Loops = Loops;
	Tween->DelayBetweenLoops = DelayBetweenLoops;
	Tween->Delay = DelayBetweenLoops; //if loops == 0?
	Tween->TargetValue = Target;
	Tween->TweenType = TweenType;
	Tween->TargetObjectType = TweenTargetObject;
	Tween->WorldContextObject = WorldContextObject;
	
	return Tween;
}

UTween* UTween::NewTweenSceneComponent(USceneComponent* SceneComponent, ETweenType TweenType, FVector4 Target, bool bIsRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	if(SceneComponent)
	{
		return NewTween(FWeakObjectPtr(SceneComponent), ETweenTargetObjectType::SceneComponent, TweenType, Target, bIsRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
	}
	
	return nullptr;
}

UTween* UTween::NewTweenSceneComponentFrom(USceneComponent* SceneComponent, ETweenType TweenType, FVector4 From, bool bIsRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	FVector4 Target;

	if (!SceneComponent || !GetValueSceneComponent(Target, *SceneComponent, TweenType))
	{
		return nullptr;
	}

	SetValueSceneComponent(From, *SceneComponent, TweenType);

	return NewTweenSceneComponent(SceneComponent, TweenType, Target, bIsRelative,
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}


UTween* UTween::NewTweenWidget(UWidget* Widget, ETweenType TweenType, FVector4 Target, bool bIsRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	if (Widget)
	{
		return NewTween(FWeakObjectPtr(Widget), ETweenTargetObjectType::Widget, TweenType, Target, bIsRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
	}

	return nullptr;
}

UTween* UTween::NewTweenWidgetFrom(UWidget* Widget, ETweenType TweenType, FVector4 From, bool bIsRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	FVector4 Target;

	if (!Widget || !GetValueWidget(Target, *Widget, TweenType))
	{
		return nullptr;
	}

	SetValueWidget(From, *Widget, TweenType);

	return NewTweenWidget(Widget, TweenType, Target, bIsRelative,
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::ComponentLocationTo(USceneComponent* SceneComponent, FVector Location, 
	bool bIsLocationRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, 
	float DelayBetweenLoops, const UObject* WorldContextObject)
{
	return NewTweenSceneComponent( SceneComponent, ETweenType::Location, FVector4(Location), bIsLocationRelative, 
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::ComponentLocationFrom(USceneComponent* SceneComponent, FVector Location,
	bool bIsLocationRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops,
	float DelayBetweenLoops, const UObject* WorldContextObject)
{
	return NewTweenSceneComponentFrom(SceneComponent, ETweenType::Location, FVector4(Location), bIsLocationRelative,
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::ComponentRelativeLocationTo(USceneComponent* SceneComponent, FVector Location,
	bool bIsLocationRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops,
	float DelayBetweenLoops, const UObject* WorldContextObject)
{
	return NewTweenSceneComponent( SceneComponent, ETweenType::RelativeLocation, FVector4(Location), bIsLocationRelative, 
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::ComponentRelativeLocationFrom(USceneComponent* SceneComponent, FVector Location,
	bool bIsLocationRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops,
	float DelayBetweenLoops, const UObject* WorldContextObject)
{
	return NewTweenSceneComponentFrom(SceneComponent, ETweenType::RelativeLocation, FVector4(Location), bIsLocationRelative,
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::ComponentScaleTo(USceneComponent* SceneComponent, FVector Scale,
	bool bIsScaleRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, 
	float DelayBetweenLoops, const UObject* WorldContextObject)
{
	return NewTweenSceneComponent( SceneComponent, ETweenType::Scale, FVector4(Scale), bIsScaleRelative, 
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::ComponentScaleFrom(USceneComponent* SceneComponent, FVector Scale,
	bool bIsScaleRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, 
	float DelayBetweenLoops, const UObject* WorldContextObject)
{
	return NewTweenSceneComponentFrom(SceneComponent, ETweenType::Scale, FVector4(Scale), bIsScaleRelative,
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::ComponentRelativeScaleTo(USceneComponent* SceneComponent, FVector Scale,
	bool bIsScaleRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, 
	float DelayBetweenLoops, const UObject* WorldContextObject)
{
	return NewTweenSceneComponent(SceneComponent, ETweenType::RelativeScale, FVector4(Scale), bIsScaleRelative, 
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::ComponentRelativeScaleFrom(USceneComponent* SceneComponent, FVector Scale,
	bool bIsScaleRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, 
	float DelayBetweenLoops, const UObject* WorldContextObject)
{
	return NewTweenSceneComponentFrom(SceneComponent, ETweenType::RelativeScale, FVector4(Scale), bIsScaleRelative,
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::ComponentRotationTo(USceneComponent* SceneComponent, FQuat Rotation,
	bool bIsRotationRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops,
	float DelayBetweenLoops, const UObject* WorldContextObject)
{
	return NewTweenSceneComponent( SceneComponent, ETweenType::Rotation, FVector4(Rotation.X, Rotation.Y, Rotation.Z, Rotation.W), bIsRotationRelative, 
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::ComponentRotationFrom(USceneComponent* SceneComponent, FQuat Rotation,
	bool bIsRotationRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops,
	float DelayBetweenLoops, const UObject* WorldContextObject)
{
	return NewTweenSceneComponentFrom(SceneComponent, ETweenType::Rotation, FVector4(Rotation.X, Rotation.Y, Rotation.Z, Rotation.W), bIsRotationRelative,
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::ComponentRelativeRotationTo(USceneComponent* SceneComponent, FQuat Rotation,
	bool bIsRotationRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops,
	float DelayBetweenLoops, const UObject* WorldContextObject)
{
	return NewTweenSceneComponent(SceneComponent, ETweenType::RelativeRotation, FVector4(Rotation.X, Rotation.Y, Rotation.Z, Rotation.W), bIsRotationRelative,
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::ComponentRelativeRotationFrom(USceneComponent* SceneComponent, FQuat Rotation,
	bool bIsRotationRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops,
	float DelayBetweenLoops, const UObject* WorldContextObject)
{
	return NewTweenSceneComponentFrom(SceneComponent, ETweenType::RelativeRotation, FVector4(Rotation.X, Rotation.Y, Rotation.Z, Rotation.W), bIsRotationRelative,
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::ActorLocationTo(AActor* Actor, FVector Location, 
	bool bIsLocationRelative, float Duration, EEaseType EaseType, ELoopType LoopType, 
	int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	if (!Actor)
	{
		return nullptr;
	}

	return ComponentLocationTo( Actor->GetRootComponent(), Location, bIsLocationRelative, 
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::ActorLocationFrom(AActor* Actor, FVector Location, 
	bool bIsLocationRelative, float Duration, EEaseType EaseType, ELoopType LoopType, 
	int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	if (!Actor)
	{
		return nullptr;
	}

	return ComponentLocationFrom( Actor->GetRootComponent(), Location, bIsLocationRelative, 
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::ActorRelativeLocationTo(AActor* Actor, FVector Location, 
	bool bIsLocationRelative, float Duration, EEaseType EaseType, ELoopType LoopType, 
	int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	if (!Actor)
	{
		return nullptr;
	}

	return ComponentRelativeLocationTo( Actor->GetRootComponent(), Location, bIsLocationRelative, 
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::ActorRelativeLocationFrom(AActor* Actor, FVector Location, 
	bool bIsLocationRelative, float Duration, EEaseType EaseType, ELoopType LoopType, 
	int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	if (!Actor)
	{
		return nullptr;
	}

	return ComponentRelativeLocationFrom( Actor->GetRootComponent(), Location, bIsLocationRelative, 
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::ActorScaleTo(AActor* Actor, FVector Scale, 
	bool bIsScaleRelative, float Duration, EEaseType EaseType, ELoopType LoopType, 
	int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	if (!Actor)
	{
		return nullptr;
	}

	return ComponentScaleTo( Actor->GetRootComponent(), Scale, bIsScaleRelative, 
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::ActorScaleFrom(AActor* Actor, FVector Scale, 
	bool bIsScaleRelative, float Duration, EEaseType EaseType, ELoopType LoopType, 
	int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	if (!Actor)
	{
		return nullptr;
	}

	return ComponentScaleFrom( Actor->GetRootComponent(), Scale, bIsScaleRelative, 
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::ActorRelativeScaleTo(AActor* Actor, FVector Scale, 
	bool bIsScaleRelative, float Duration, EEaseType EaseType, ELoopType LoopType, 
	int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	if (!Actor)
	{
		return nullptr;
	}

	return ComponentRelativeScaleTo( Actor->GetRootComponent(), Scale, bIsScaleRelative, 
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::ActorRelativeScaleFrom(AActor* Actor, FVector Scale, 
	bool bIsScaleRelative, float Duration, EEaseType EaseType, ELoopType LoopType, 
	int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	if (!Actor)
	{
		return nullptr;
	}

	return ComponentRelativeScaleFrom( Actor->GetRootComponent(), Scale, bIsScaleRelative, 
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::ActorRotationTo(AActor* Actor, FQuat Rotation, 
	bool bIsRotationRelative, float Duration, EEaseType EaseType, ELoopType LoopType, 
	int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	if (!Actor)
	{
		return nullptr;
	}

	return ComponentRotationTo( Actor->GetRootComponent(), Rotation, bIsRotationRelative, 
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::ActorRotationFrom(AActor* Actor, FQuat Rotation, 
	bool bIsRotationRelative, float Duration, EEaseType EaseType, ELoopType LoopType, 
	int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	if (!Actor)
	{
		return nullptr;
	}

	return ComponentRotationFrom( Actor->GetRootComponent(), Rotation, bIsRotationRelative, 
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::ActorRelativeRotationTo(AActor* Actor, FQuat Rotation, 
	bool bIsRotationRelative, float Duration, EEaseType EaseType, ELoopType LoopType, 
	int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	if (!Actor)
	{
		return nullptr;
	}

	return ComponentRelativeRotationTo( Actor->GetRootComponent(), Rotation, bIsRotationRelative, 
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::ActorRelativeRotationFrom(AActor* Actor, FQuat Rotation, 
	bool bIsRotationRelative, float Duration, EEaseType EaseType, ELoopType LoopType, 
	int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	if (!Actor)
	{
		return nullptr;
	}

	return ComponentRelativeRotationFrom( Actor->GetRootComponent(), Rotation, bIsRotationRelative, 
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::WidgetSlotPositionTo(UWidget* Widget, FVector2D Location, bool bIsLocationRelative, float Duration,
	EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	return NewTweenWidget(Widget, ETweenType::SlotPosition, FVector4(Location.X, Location.Y), bIsLocationRelative,
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::WidgetSlotPositionFrom(UWidget* Widget, FVector2D Location, bool bIsLocationRelative, float Duration,
	EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	return NewTweenWidgetFrom(Widget, ETweenType::SlotPosition, FVector4(Location.X, Location.Y), bIsLocationRelative,
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::WidgetRenderLocationTo(UWidget* Widget, FVector2D Location,
     bool bIsLocationRelative, float Duration, EEaseType EaseType, ELoopType LoopType, 
	int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	return NewTweenWidget( Widget, ETweenType::RelativeLocation, FVector4(Location.X, Location.Y), bIsLocationRelative, 
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::WidgetRenderLocationFrom(UWidget* Widget, FVector2D Location,
	bool bIsLocationRelative, float Duration, EEaseType EaseType, ELoopType LoopType, 
	int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	return NewTweenWidgetFrom(Widget, ETweenType::RelativeLocation, FVector4(Location.X, Location.Y), bIsLocationRelative,
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::WidgetRenderScaleTo(UWidget* Widget, FVector2D Scale,
	bool bIsScaleRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	return NewTweenWidget(Widget, ETweenType::RelativeScale, FVector4(Scale.X, Scale.Y), bIsScaleRelative, 
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::WidgetRenderScaleFrom(UWidget* Widget, FVector2D Scale,
	bool bIsScaleRelative, float Duration, EEaseType EaseType, ELoopType LoopType, 
	int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	return NewTweenWidgetFrom(Widget, ETweenType::RelativeScale, FVector4(Scale.X, Scale.Y), bIsScaleRelative,
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::WidgetRenderShearTo(UWidget* Widget, FVector2D Shear,
	bool bIsShearRelative, float Duration, EEaseType EaseType, ELoopType LoopType, 
	int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	return NewTweenWidget(Widget, ETweenType::RelativeShear, FVector4(Shear.X, Shear.Y), bIsShearRelative, 
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::WidgetRenderShearFrom(UWidget* Widget, FVector2D Shear,
	bool bIsShearRelative, float Duration, EEaseType EaseType, ELoopType LoopType, 
	int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	return NewTweenWidgetFrom(Widget, ETweenType::RelativeShear, FVector4(Shear.X, Shear.Y), bIsShearRelative,
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::WidgetRenderAngleTo(UWidget* Widget, float Angle, 
	bool bIsAngleRelative, float Duration, EEaseType EaseType, ELoopType LoopType, 
	int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	return NewTweenWidget(Widget, ETweenType::RelativeAngle, FVector4(Angle), bIsAngleRelative, 
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::WidgetRenderAngleFrom(UWidget* Widget, float Angle, 
	bool bIsAngleRelative, float Duration, EEaseType EaseType, ELoopType LoopType, 
	int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	return NewTweenWidgetFrom(Widget, ETweenType::RelativeAngle, FVector4(Angle), bIsAngleRelative,
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::WidgetRenderOpacityTo(UWidget* Widget, float Opacity, 
	bool bIsOpacityRelative, float Duration, EEaseType EaseType, ELoopType LoopType, 
	int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	return NewTweenWidget(Widget, ETweenType::Opacity, FVector4(Opacity), bIsOpacityRelative, 
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::WidgetRenderOpacityFrom(UWidget* Widget, float Opacity, 
	bool bIsOpacityRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, 
	float DelayBetweenLoops, const UObject* WorldContextObject)
{
	return NewTweenWidgetFrom(Widget, ETweenType::Opacity, FVector4(Opacity), bIsOpacityRelative,
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::WidgetColorTo(UWidget* Widget, FLinearColor Color, 
	bool bIsColorRelative, float Duration, EEaseType EaseType, ELoopType LoopType, 
	int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	return NewTweenWidget(Widget, ETweenType::Color, Color, bIsColorRelative, 
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::WidgetColorFrom(UWidget* Widget, const FLinearColor& Color, 
	bool bIsColorRelative, float Duration, EEaseType EaseType, ELoopType LoopType, 
	int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	return NewTweenWidgetFrom(Widget, ETweenType::Color, Color, bIsColorRelative,
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::WidgetBackgroundColorTo(UWidget* Widget, FLinearColor Color,
	bool bIsColorRelative, float Duration, EEaseType EaseType, ELoopType LoopType,
	int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	return NewTweenWidget(Widget, ETweenType::BackgroundColor, Color, bIsColorRelative,
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::WidgetBackgroundColorFrom(UWidget* Widget, const FLinearColor& Color,
	bool bIsColorRelative, float Duration, EEaseType EaseType, ELoopType LoopType,
	int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	return NewTweenWidgetFrom(Widget, ETweenType::BackgroundColor, Color, bIsColorRelative,
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}


UTween* UTween::WidgetBlurStrengthTo(UWidget* Widget, float BlurStrength,
	bool bIsBlurStrengthRelative, float Duration, EEaseType EaseType, ELoopType LoopType,
	int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	return NewTweenWidget(Widget, ETweenType::BlurStrength, FVector4(BlurStrength), bIsBlurStrengthRelative,
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::WidgetBlurStrengthFrom(UWidget* Widget, float BlurStrength,
	bool bIsBlurStrengthRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops,
	float DelayBetweenLoops, const UObject* WorldContextObject)
{
	return NewTweenWidgetFrom(Widget, ETweenType::BlurStrength, FVector4(BlurStrength), bIsBlurStrengthRelative,
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::DesiredSizeScaleTo(UWidget* Widget, FVector2D SizeScale,
	bool bIsDesiredSizeScaleRelative, float Duration, EEaseType EaseType, ELoopType LoopType,
	int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	return NewTweenWidget(Widget, ETweenType::DesiredSizeScale, FVector4(SizeScale.X, SizeScale.Y), bIsDesiredSizeScaleRelative,
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::DesiredSizeScaleFrom(UWidget* Widget, FVector2D SizeScale,
	bool bIsDesiredSizeScaleRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops,
	float DelayBetweenLoops, const UObject* WorldContextObject)
{
	return NewTweenWidgetFrom(Widget, ETweenType::DesiredSizeScale, FVector4(SizeScale.X, SizeScale.Y), bIsDesiredSizeScaleRelative,
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}


UTween* UTween::MaterialColorTo(UMaterialInstanceDynamic* Material, FName MaterialProperty, FLinearColor Color, 
	bool bIsColorRelative, float Duration, EEaseType EaseType, ELoopType LoopType, 
	int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	if (!Material)
	{
		return nullptr;
	}
	
	UTween* Tween = NewTween( FWeakObjectPtr(Material), ETweenTargetObjectType::Material, ETweenType::Color, Color, bIsColorRelative, 
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	Tween->ParameterName = MaterialProperty;

	return Tween;
}

UTween* UTween::MaterialScalarTo(UMaterialInstanceDynamic* Material, FName MaterialProperty, float Scalar, 
	bool bIsScalarRelative, float Duration, EEaseType EaseType, ELoopType LoopType, 
	int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	if (!Material)
	{
		return nullptr;
	}

	UTween* Tween = NewTween( FWeakObjectPtr(Material), ETweenTargetObjectType::Material, ETweenType::Scalar, FVector4(Scalar), bIsScalarRelative, 
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	Tween->ParameterName = MaterialProperty;

	return Tween;
}

UTween* UTween::FloatTo(UObject* Object, FName PropertyName, float Value, 
	bool bIsValueRelative, float Duration, EEaseType EaseType, ELoopType LoopType, 
	int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	if (!Object)
	{
		return nullptr;
	}

	UTween* Tween = NewTween(FWeakObjectPtr(Object), ETweenTargetObjectType::Property, ETweenType::Scalar, FVector4(Value), bIsValueRelative, 
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	Tween->ParameterName = PropertyName;

	return Tween;
}

UTween* UTween::FloatFrom(UObject* Object, FName PropertyName, float Value, 
	bool bIsValueRelative, float Duration, EEaseType EaseType, ELoopType LoopType, 
	int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	FVector4 CurrentValue;
	FProperty* Property;

	const ETweenType TweenType = ETweenType::Scalar;
	if (!Object || !GetValueProperty(CurrentValue, Property, *Object, PropertyName, TweenType))
	{
		return nullptr;
	}

	SetValueProperty(FVector4(Value), *Object, TweenType, Property);
	
	UTween* Tween = NewTween(FWeakObjectPtr(Object), ETweenTargetObjectType::Property, TweenType, CurrentValue, bIsValueRelative,
	                         Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	Tween->ParameterName = PropertyName;
	Tween->CachedProperty = Property;

	return Tween;
}

UTween* UTween::VectorTo(UObject* Object, FName PropertyName, FVector ValueTo, 
	bool bIsVectorRelative, float Duration, EEaseType EaseType, ELoopType LoopType, 
	int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	if (!Object)
	{
		return nullptr;
	}
	
	UTween* Tween = NewTween(FWeakObjectPtr(Object), ETweenTargetObjectType::Property, ETweenType::Vector, ValueTo, bIsVectorRelative, 
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	Tween->ParameterName = PropertyName;

	return Tween;
}

UTween* UTween::VectorFrom(UObject* Object, FName PropertyName, FVector Vector, 
	bool bIsVectorRelative, float Duration, EEaseType EaseType, ELoopType LoopType, 
	int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	FVector4 CurrentValue;
	FProperty* Property;

	const ETweenType TweenType = ETweenType::Vector;
	if (!Object || !GetValueProperty(CurrentValue, Property, *Object, PropertyName, TweenType))
	{
		return nullptr;
	}

	SetValueProperty(Vector, *Object, TweenType, Property);

	UTween* Tween = NewTween(FWeakObjectPtr(Object), ETweenTargetObjectType::Property, TweenType, CurrentValue, bIsVectorRelative,
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	Tween->ParameterName = PropertyName;
	Tween->CachedProperty = Property;

	return Tween;
}

UTween* UTween::CustomAction(UObject* Object, float From, float To, float Duration, EEaseType EaseType,
                             ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = NewTween(FWeakObjectPtr(Object), ETweenTargetObjectType::CustomAction, ETweenType::Scalar, FVector4(To),false,
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	Tween->StartValue = FVector4(From);
	
	return Tween;
}

UTween* UTween::NextVectorTo(FVector VectorTo, float InDuration, float InDelay)
{
	//ensure(TweenType == ETweenType::Vector);
	
	NextTween = NewTween(ObjectPtr, this->TargetObjectType, TweenType, VectorTo, bIsRelativeTween, 
		InDuration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	NextTween->ParameterName = ParameterName;
	NextTween->Delay = InDelay;

	return NextTween;
}

UTween* UTween::NextRotationTo(FQuat RotationTo, float InDuration, float InDelay)
{
	//ensure(TweenType == ETweenType::Rotation);

	NextTween = NewTween(ObjectPtr, this->TargetObjectType, TweenType,FVector4(RotationTo.X, RotationTo.Y, RotationTo.Z, RotationTo.W), bIsRelativeTween, 
		InDuration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	NextTween->ParameterName = ParameterName;
	NextTween->Delay = InDelay;

	return NextTween;
	
}

UTween* UTween::NextColorTo(FLinearColor ColorTo, float InDuration, float InDelay)
{
	NextTween = NewTween(ObjectPtr, this->TargetObjectType, TweenType, ColorTo, bIsRelativeTween, 
		InDuration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	NextTween->ParameterName = ParameterName;
	NextTween->Delay = InDelay;

	return NextTween;
}

UTween* UTween::NextScalarTo(float ScalarTo, float InDuration, float InDelay)
{
	NextTween = NewTween(ObjectPtr, this->TargetObjectType, TweenType, FVector4(ScalarTo), bIsRelativeTween, 
		InDuration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	NextTween->ParameterName = ParameterName;
	NextTween->Delay = InDelay;

	return NextTween;
}

//UTween* UTween::PropertyTo(UObject* Object, TFieldPath<FProperty> PropertyToEdit, float Value, bool bIsValueRelative,
//	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops,
//	const UObject* WorldContextObject)
//{
//
//	UTween* Tween = NewTween(FWeakObjectPtr(Object), ETweenTargetObjectType::Property, ETweenType::Scalar, FVector4(Value), bIsValueRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
//
//	//Tween->ParameterName = PropertyToEdit.Get();
//
//	return Tween;
//}

bool UTween::CacheInitialValues()
{
	UObject* Object = ObjectPtr.Get();

	if (!Object)
	{
		return false;
	}
	
	switch(TargetObjectType)
	{
		case ETweenTargetObjectType::SceneComponent:
			if (const USceneComponent * SceneComponent = Cast<USceneComponent>(Object))
			{
				return GetValueSceneComponent(StartValue, *SceneComponent, TweenType);
			}
			break;
			
		case ETweenTargetObjectType::Material:
			if (UMaterialInstanceDynamic* Material = Cast<UMaterialInstanceDynamic>(Object))
			{
				return GetValueMaterial(StartValue, ParameterIndex, *Material, ParameterName, TweenType);
			}
			break;
			
		case ETweenTargetObjectType::Widget:
			if (const UWidget* Widget = Cast<UWidget>(Object))
			{
				return GetValueWidget(StartValue,*Widget,TweenType);
			}
			break;

		case ETweenTargetObjectType::Property:
			return GetValueProperty(StartValue,CachedProperty,*Object,ParameterName,TweenType);

		case ETweenTargetObjectType::CustomAction:
			return true;

		default:
			return false;
	}

	return false;
}

bool UTween::PrepareForUse()
{
	if (!CacheInitialValues())
	{
		return false;
	}
		
	ElapsedTime = -Delay;

	switch (GetTargetValueType())
	{
		case ETargetValueType::Vector:
		case ETargetValueType::Color: 
		case ETargetValueType::Scalar:
			if (bIsRelativeTween)
			{
				EndValue = StartValue + TargetValue;
			}
			else
			{
				EndValue = TargetValue;
			}
			break;
		
		case ETargetValueType::Quat:
			if (bIsRelativeTween)
			{
				const FQuat Start = FQuat(StartValue.X, StartValue.Y, StartValue.Z, StartValue.W);
				const FQuat Target = FQuat(TargetValue.X, TargetValue.Y, TargetValue.Z, TargetValue.W);
				const FQuat End = Start * Target;

				EndValue = FVector4(End.X, End.Y, End.Z, End.W);
			}
			else
			{
				EndValue = TargetValue;
			}

			break;
	}

	return true;
}


ETargetValueType UTween::GetTargetValueType() const
{
	switch (TweenType)
	{
	case ETweenType::Rotation:
	case ETweenType::RelativeRotation:
		return ETargetValueType::Quat;
	case ETweenType::Color:	
		return  ETargetValueType::Color;
	case ETweenType::Scalar:
	case ETweenType::RelativeAngle:
	case ETweenType::Opacity:
		return ETargetValueType::Scalar;
	default:
		return ETargetValueType::Vector;
	}
			
	
}

bool UTween::GetValueSceneComponent(FVector4 &OutVec, const USceneComponent& SceneComponent, ETweenType TweenType)
{
	switch (TweenType)
	{
		case ETweenType::Location:
			{
				OutVec = SceneComponent.GetComponentLocation();
				return true;
			}
		case ETweenType::RelativeLocation:
			{
				OutVec = SceneComponent.GetRelativeLocation();
				return true;
			}
		case ETweenType::Rotation:
			{
				const FQuat Rotation = SceneComponent.GetComponentQuat();
				OutVec = FVector4(Rotation.X, Rotation.Y, Rotation.Z, Rotation.W);
				return true;
			}
		case ETweenType::RelativeRotation:
			{
				const FQuat RelativeRotation = FQuat(SceneComponent.GetRelativeRotation());
				OutVec = FVector4(RelativeRotation.X, RelativeRotation.Y, RelativeRotation.Z, RelativeRotation.W);
				return true;
			}
		case ETweenType::Scale:
			{
				OutVec = SceneComponent.GetComponentScale();
				return true;
			}
		case ETweenType::RelativeScale:
			{
				OutVec = SceneComponent.GetRelativeScale3D();
				return true;
			}
		default:
			return false;
	}
}

bool UTween::GetValueMaterial(FVector4& OutVec,int32 &OutParameterIndex, UMaterialInstanceDynamic& Material, FName ParameterName, ETweenType TweenType)
{
	const FMaterialParameterInfo ParameterInfo(ParameterName);
	
	switch (TweenType)
	{
		case ETweenType::Color:
			{
				FLinearColor Color(0, 0, 0, 0);
				
				if (Material.GetVectorParameterValue(ParameterInfo, Color) && 
					Material.InitializeVectorParameterAndGetIndex(ParameterName, Color, OutParameterIndex))
				{
					OutVec = FVector4(Color.R, Color.G, Color.B, Color.A);

					return true;
				}
			
				return false;
			}
		case ETweenType::Scalar:
			{
				float Scalar;
				
				if (Material.GetScalarParameterValue(ParameterInfo, Scalar) &&
					Material.InitializeScalarParameterAndGetIndex(ParameterName, Scalar, OutParameterIndex))
				{
					OutVec = FVector4(Scalar);

					return true;
				}

				return false;
			}
		default:
			return false;
	}
}

//if (UImage* Image = Cast<UImage>(&Widget))
//{
//	Image->SetColorAndOpacity(ColorAndOpacity);
//}
//else if (UBorder* Border = Cast<UBorder>(&Widget))
//{
//	switch (TweenType)
//	{
//	case ETweenType::ContentColor:
//		Border->SetContentColorAndOpacity(ColorAndOpacity);
//		break;
//	default:
//	case ETweenType::BrushColor:
//		Border->SetBrushColor(ColorAndOpacity);
//		break;
//	}
//
//}
//else if (UButton* Button = Cast<UButton>(&Widget))
//{
//	switch (TweenType)
//	{
//	default:
//	case ETweenType::ContentColor:
//		Button->SetColorAndOpacity(ColorAndOpacity);
//		break;
//	case  ETweenType::BackgroundColor:
//		Button->SetBackgroundColor(ColorAndOpacity);
//		break;
//		//Button->WidgetStyle.Hovered.TintColor
//	}
//
//}

FLinearColor UTween::GetWidgetColorAndOpacity(const UWidget& Widget, ETweenType TweenType)
{
	if(const UImage *Image = Cast<UImage>(&Widget))
	{
		return Image->ColorAndOpacity;
	}
	else if(const UBorder *Border = Cast<UBorder>(&Widget))
	{
			switch (TweenType)
			{
			case ETweenType::ContentColor:
				return Border->GetContentColorAndOpacity();
			default:
			case ETweenType::BrushColor:
				return Border->GetBrushColor();
			}

	}
	else if(const UButton *Button = Cast<UButton>(&Widget))
	{
		switch (TweenType)
		{
		default:
		case ETweenType::ContentColor:
			return Button->ColorAndOpacity;
		case  ETweenType::BackgroundColor:
			return Button->BackgroundColor;
		}
	}
	else if (const UTextBlock* TextBlock = Cast<UTextBlock>(&Widget))
	{
		return TextBlock->GetColorAndOpacity().GetSpecifiedColor();
	}

	return FLinearColor::Black;
}

bool UTween::GetValueWidget(FVector4& OutVec, const UWidget& Widget, ETweenType TweenType)
{
	switch (TweenType)
	{
		case ETweenType::RelativeLocation:
			OutVec = FVector4(Widget.GetRenderTransform().Translation.X, Widget.GetRenderTransform().Translation.Y);
			return true;
		case ETweenType::RelativeScale:
			OutVec = FVector4(Widget.GetRenderTransform().Scale.X, Widget.GetRenderTransform().Scale.Y);
			return true;
		case ETweenType::Color:
		case ETweenType::BackgroundColor:
		case ETweenType::ContentColor:
		case ETweenType::BrushColor:
			OutVec = GetWidgetColorAndOpacity(Widget, TweenType);
			return true;
		case ETweenType::RelativeShear:
			OutVec = FVector4(Widget.GetRenderTransform().Shear.X, Widget.GetRenderTransform().Shear.Y);
			return true;
		case ETweenType::RelativeAngle:
			OutVec = FVector4(Widget.GetRenderTransformAngle());
			return true;
		case ETweenType::Opacity:
			OutVec = FVector4(Widget.GetRenderOpacity());
			return true;
		case ETweenType::SlotPosition:
			if (const UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(Widget.Slot))
			{
				const FVector2D Position = CanvasPanelSlot->GetPosition();
				OutVec = FVector4(Position.X,Position.Y);
				return true;
			}
			return false;
		case ETweenType::BlurStrength:
		{
			if(const UBackgroundBlur* BackgroundBlur = Cast<UBackgroundBlur>(&Widget))
			{
				OutVec = FVector4(BackgroundBlur->GetBlurStrength());
				return true;
			}
			return false;
		}
		case ETweenType::DesiredSizeScale:
		{
			if (const UBorder* Border = Cast<UBorder>(&Widget))
			{
				FVector2D DesiredSizeScale =Border->GetDesiredSizeScale();
				OutVec = FVector4(DesiredSizeScale.X, DesiredSizeScale.Y, 0.0f,0.0f);
				return true;
			}
			return false;
		}
		
		default:
			return false;
	}
}

bool UTween::GetValueProperty(FVector4& OutVec, FProperty*& OutProperty, const UObject& Object, FName ParameterName, ETweenType TweenType)
{
	switch (TweenType)
	{
		case ETweenType::Scalar:
			if (FFloatProperty* FloatProperty = FindFProperty<FFloatProperty>(Object.GetClass(), ParameterName))
			{
				const float Value = FloatProperty->GetPropertyValue_InContainer(&Object);
				OutVec = FVector4(Value);
				OutProperty = FloatProperty;
				return true;
			}

			break;
	
		case ETweenType::Vector:
			if (FStructProperty* StructProperty = FindFProperty<FStructProperty>(Object.GetClass(), ParameterName))
			{
				OutVec = *StructProperty->ContainerPtrToValuePtr<FVector>(&Object);
				OutProperty = StructProperty;
				return true;
			}

			break;
		default:
			return false;
	}

	return false;
}

float UTween::PercentComplete() const
{
	// add deltaTime to our elapsed time and clamp it from -delay to duration
	const float LocalElapsedTime = FMath::Clamp(ElapsedTime, -Delay, Duration);

	// if we have a delay, we will have a negative elapsedTime until the delay is complete
	if (LocalElapsedTime <= 0)
	{
		return 0.0f;
	}

	const float ModifiedElapsedTime = bIsRunningInReverse ? Duration - ElapsedTime : ElapsedTime;

	return ModifiedElapsedTime / Duration;
}

bool UTween::Tick(float DeltaTime, float UnscaledDeltaTime, bool bCompleteTweenThisStep)
{
	// fetch our deltaTime. It will either be taking this to completion or standard delta/unscaledDelta
	if (bCompleteTweenThisStep)
	{
		DeltaTime = TNumericLimits< float >::Max();
		if (LoopType == ELoopType::PingPong)
		{
			if (Loops % 2 == 1)
			{
				bIsRunningInReverse = !bIsRunningInReverse;
			}
		}
	}
	else
	{
		DeltaTime = bIsTimeScaleIndependent ? UnscaledDeltaTime : DeltaTime;
	}

	// add deltaTime to our elapsed time and clamp it from -delay to duration
	ElapsedTime = FMath::Clamp(ElapsedTime + DeltaTime, -Delay, Duration);

	// if we have a delay, we will have a negative elapsedTime until the delay is complete
	if (ElapsedTime <= 0)
	{
		return false;
	}

	const float ModifiedElapsedTime = bIsRunningInReverse ? Duration - ElapsedTime : ElapsedTime;

	const float EasedTime = EaseHelper(ModifiedElapsedTime / Duration, EaseType);//(animCurve == null) ? EaseHelper.ease(easeType, modifiedElapsedTime, duration) : animCurve.Evaluate(modifiedElapsedTime / duration);

	
	FVector4 Vec;
	
	switch (GetTargetValueType())
	{
	case ETargetValueType::Vector: 
	case ETargetValueType::Color: 
	case ETargetValueType::Scalar:
		Vec = (StartValue * (1.0f-EasedTime)) + (EndValue * EasedTime);
		break;
	case ETargetValueType::Quat:
		{
			const FQuat StartRotation = FQuat(StartValue.X, StartValue.Y, StartValue.Z, StartValue.W);
			const FQuat EndRotation = FQuat(EndValue.X, EndValue.Y, EndValue.Z, EndValue.W);
			const FQuat InterpolatedRotation = FQuat::Slerp(StartRotation, EndRotation, EasedTime);
			Vec = FVector4(InterpolatedRotation.X, InterpolatedRotation.Y, InterpolatedRotation.Z, InterpolatedRotation.W);
		}

		break;
	default: ;
	}

	if (Action.IsBound())
	{
		Action.Broadcast(ObjectPtr.Get(false), EasedTime,Vec.X );
	}

	
	SetAsRequiredPerCurrentTweenType(Vec);

	// if we have a loopType and we are done do the loop
	if (LoopType != ELoopType::None && FMath::IsNearlyEqual(ElapsedTime, Duration))
	{
		HandleLooping();
	}

	return FMath::IsNearlyEqual(ElapsedTime, Duration);
}

void UTween::SetValueEnd() const
{
	SetAsRequiredPerCurrentTweenType(EndValue);

	//Same as
	//bCompleteTweenThisStep = true;
	//Tick(0.f, 0.f, bool bCompleteTweenThisStep)
}

void UTween::Activate()
{
	UWorld* World = WorldContextObject->GetWorld();

	UGameInstance* GameInstance = World->GetGameInstance();

	if (UTweenerSubsystem* Subsystem = GameInstance->GetSubsystem<UTweenerSubsystem>())
	{
		Subsystem->StartTween(this);
	}
}

/// <summary>
/// handles loop logic
/// </summary>
void UTween::HandleLooping()
{
	Loops--;
	if (LoopType == ELoopType::RestartFromBeginning)
	{
		SetAsRequiredPerCurrentTweenType(StartValue);

		PrepareForUse();
	}
	else // ping-pong
	{
		bIsRunningInReverse = !bIsRunningInReverse;
	}

	if (LoopType == ELoopType::RestartFromBeginning || Loops % 2 == 1)
	{
		LoopComplete.Broadcast();
		LoopCompleteDelegate.Broadcast();
	}

	// kill our loop if we have no loops left and zero out the delay then prepare for use
	if (Loops == 0)
		LoopType = ELoopType::None;

	Delay = DelayBetweenLoops;
	ElapsedTime = -Delay;
}

void UTween::SetValueSceneComponent(const FVector4& Vec, USceneComponent& SceneComponent, ETweenType TweenType) 
{
	switch (TweenType)
	{
	case ETweenType::Location:
		SceneComponent.SetWorldLocation(Vec);
		break;
	case ETweenType::RelativeLocation:
		SceneComponent.SetRelativeLocation(Vec);
		break;
	case ETweenType::Scale:
		SceneComponent.SetWorldScale3D(Vec);
		break;
	case ETweenType::RelativeScale:
		SceneComponent.SetRelativeScale3D(Vec);
		break;
	case ETweenType::Rotation:
		SceneComponent.SetWorldRotation(FQuat(Vec.X,Vec.Y,Vec.Z,Vec.W));
		break;
	case ETweenType::RelativeRotation:
		SceneComponent.SetRelativeRotation(FQuat(Vec.X, Vec.Y, Vec.Z, Vec.W));
		break;
	default:
		break;
	}
}

void UTween::SetValueMaterial(const FVector4& Vec, UMaterialInstanceDynamic& Material, ETweenType TweenType, int32 ParameterIndex)
{
	switch (TweenType)
	{
		case ETweenType::Color:
			Material.SetVectorParameterByIndex(ParameterIndex, FLinearColor(Vec));
			break;
		case ETweenType::Scalar:
			Material.SetScalarParameterByIndex(ParameterIndex, Vec.X);
			break;
		default:
			break;
	}	
}

void UTween::SetWidgetColorAndOpacity(UWidget& Widget, FLinearColor ColorAndOpacity, ETweenType TweenType)
{
	if (UImage* Image = Cast<UImage>(&Widget))
	{
		Image->SetColorAndOpacity(ColorAndOpacity);
	}
	else if (UBorder* Border = Cast<UBorder>(&Widget))
	{
		switch(TweenType)
		{
		case ETweenType::ContentColor:
			Border->SetContentColorAndOpacity(ColorAndOpacity);
			break;
		default:
		case ETweenType::BrushColor:
			Border->SetBrushColor(ColorAndOpacity);
			break;
		}
		
	}
	else if (UButton* Button = Cast<UButton>(&Widget))
	{
		switch (TweenType)
		{
		default:
		case ETweenType::ContentColor:
			Button->SetColorAndOpacity(ColorAndOpacity);
			break;
		case  ETweenType::BackgroundColor:
			Button->SetBackgroundColor(ColorAndOpacity);
			break;
		}
	}
	else if (UTextBlock* TextBlock = Cast<UTextBlock>(&Widget))
	{
		return TextBlock->SetColorAndOpacity(FSlateColor(ColorAndOpacity));
	}
}

void UTween::SetValueWidget(const FVector4& Vec, UWidget& Widget, ETweenType TweenType) 
{
	switch (TweenType)
	{
	case ETweenType::RelativeLocation:
		Widget.SetRenderTranslation(FVector2D(Vec.X, Vec.Y));
		break;
	case ETweenType::RelativeScale:
		Widget.SetRenderScale(FVector2D(Vec.X, Vec.Y));
		break;
	case ETweenType::Color:
	case ETweenType::BackgroundColor:
	case ETweenType::ContentColor:
		{	
			SetWidgetColorAndOpacity(Widget, FLinearColor(Vec), TweenType);
		}
		break;
	case ETweenType::RelativeShear:
		Widget.SetRenderShear(FVector2D(Vec.X, Vec.Y));
		break;
	case ETweenType::RelativeAngle:
		Widget.SetRenderTransformAngle(Vec.X);
		break;
	case ETweenType::Opacity:
		Widget.SetRenderOpacity(Vec.X);
		break;
	case ETweenType::SlotPosition:
		if (UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(Widget.Slot))
		{
			CanvasPanelSlot->SetPosition(FVector2D(Vec.X, Vec.Y));
		}
		break;

	case ETweenType::BlurStrength:
	{
		if (UBackgroundBlur* BackgroundBlur = Cast<UBackgroundBlur>(&Widget))
		{
			BackgroundBlur->SetBlurStrength(Vec.X);
		}
	}
	case ETweenType::DesiredSizeScale:
	{
		if (UBorder* Border = Cast<UBorder>(&Widget))
		{
			Border->SetDesiredSizeScale(FVector2D(Vec.X, Vec.Y));
		}
	}
	default: ;
	}
}

void UTween::SetValueProperty(const FVector4& Vec, UObject& Object, ETweenType TweenType, FProperty* CachedProperty)
{
	switch (TweenType)
	{
		case ETweenType::Scalar:
			if(const FFloatProperty *FloatProperty = CastField<FFloatProperty>(CachedProperty))
			{
				FloatProperty->SetPropertyValue_InContainer(&Object, static_cast<float>(Vec.X));

				Object.PostInterpChange(CachedProperty);
			}

			break;
		case ETweenType::Vector:
			if (const FStructProperty* StructProperty = CastField<FStructProperty>(CachedProperty))
			{
				*StructProperty->ContainerPtrToValuePtr<FVector>(&Object) = FVector(Vec.X,Vec.Y,Vec.Z);

				Object.PostInterpChange(CachedProperty);
			}
			break;
		default: ;
	}
}


void UTween::SetAsRequiredPerCurrentTweenType(const FVector4& Vec) const
{
	UObject* Object = ObjectPtr.Get();

	if(!Object)
	{
		return;
	}

	switch (TargetObjectType)
	{
		case ETweenTargetObjectType::SceneComponent:
			if (USceneComponent* SceneComponent = Cast<USceneComponent>(Object))
			{
				SetValueSceneComponent(Vec, *SceneComponent, TweenType);
			}
			break;
		case ETweenTargetObjectType::Material:
			if (UMaterialInstanceDynamic* Material = Cast<UMaterialInstanceDynamic>(Object))
			{
				SetValueMaterial(Vec, *Material, TweenType, ParameterIndex);
			}
			break;
		case ETweenTargetObjectType::Widget:
			if (UWidget* Widget = Cast<UWidget>(Object))
			{
				SetValueWidget(Vec, *Widget, TweenType);
			}
		case ETweenTargetObjectType::Property:
			SetValueProperty(Vec, *Object, TweenType, CachedProperty);

			break;
		default:
			break;
	}
}

/// <summary>
/// reverses the current tween. if it was going forward it will be going backwards and vice versa.
/// </summary>
void UTween::ReverseTween()
{
	bIsRunningInReverse = !bIsRunningInReverse;
	ElapsedTime = Duration - ElapsedTime;
}

bool UTween::Stop(bool bBringToCompletion, bool bIncludeChain)
{
	UWorld* World = GetWorld();

	if(!World)
	{
		return false;
	}
	
	UGameInstance* GameInstance = World->GetGameInstance();

	if (!GameInstance)
	{
		return false;
	}

	if(UTweenerSubsystem *Subsystem = GameInstance->GetSubsystem<UTweenerSubsystem>())
	{
		return Subsystem->StopTween(this, bBringToCompletion, bIncludeChain);
	}

	return false;
}

bool UTween::IsActive() const
{
	UWorld* World = GetWorld();

	if (!World)
	{
		return false;
	}

	UGameInstance* GameInstance = World->GetGameInstance();

	if (!GameInstance)
	{
		return false;
	}

	if (UTweenerSubsystem* Subsystem = GameInstance->GetSubsystem<UTweenerSubsystem>())
	{
		return Subsystem->IsTweenActive(this);
	}

	return false;
}


/// <summary>
/// chainable. Sets the EaseType used by the tween.
/// </summary>
UTween * UTween::SetEaseType(const EEaseType InEaseType)
{
	this->EaseType = InEaseType;
	return this;
}

// <summary>
/// chainable. set the loop type for the tween. a single pingpong loop means going from start-finish-start.
/// </summary>
UTween* UTween::SetLoopType(ELoopType InLoopType, int InLoops = 1, float InDelayBetweenLoops = 0.0f)
{
	LoopType = InLoopType;
	DelayBetweenLoops = InDelayBetweenLoops;

	// double the loop count for ping-pong
	if (InLoopType == ELoopType::PingPong)
	{
		InLoops = InLoops * 2 - 1;
	}
	
	Loops = InLoops;

	return this;
}


/// <summary>
/// chainable. sets the delay for the tween.
/// </summary>
UTween* UTween::SetDelay(float InDelay)
{
	Delay = InDelay;
	ElapsedTime = -InDelay;
	return this;
}


/// <summary>
/// sets the tween to be time scale independent
/// </summary>
/// <returns>The Tween</returns>
UTween* UTween::SetTimeScaleIndependent()
{
	bIsTimeScaleIndependent = true;
	return this;
}


