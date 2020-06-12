#include "Tween.h"

#include "TweenerSubsystem.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "AHEasing/easing.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"

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
	UTween* Tween = NewObject<UTween>();

	Tween->ObjectPtr = ObjectPtr;
	Tween->Duration = Duration;
	Tween->bIsRelativeTween = bIsRelative;
	Tween->EaseType = EaseType;
	Tween->LoopType = LoopType;
	Tween->Loops = Loops;
	Tween->DelayBetweenLoops = DelayBetweenLoops;
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

UTween* UTween::NewTweenWidget(UWidget* Widget, ETweenType TweenType, FVector4 Target, bool bIsRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	if (Widget)
	{
		return NewTween(FWeakObjectPtr(Widget), ETweenTargetObjectType::Widget, TweenType, Target, bIsRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
	}

	return nullptr;
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
	if(!SceneComponent)
	{
		return nullptr;
	}
	
	const FVector CurrentLocation = SceneComponent->GetComponentLocation();

	SceneComponent->SetWorldLocation(Location);

	return NewTweenSceneComponent( SceneComponent, ETweenType::Location, FVector4(CurrentLocation), bIsLocationRelative, 
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
	if (!SceneComponent)
	{
		return nullptr;
	}

	const FVector CurrentRelativeLocation = SceneComponent->RelativeLocation;
	SceneComponent->SetRelativeLocation(Location);
	
	return NewTweenSceneComponent( SceneComponent, ETweenType::RelativeLocation, FVector4(CurrentRelativeLocation), bIsLocationRelative, 
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
	if (!SceneComponent)
	{
		return nullptr;
	}

	const FVector CurrentScale = SceneComponent->GetComponentScale();
	SceneComponent->SetWorldScale3D(Scale);

	return NewTweenSceneComponent( SceneComponent, ETweenType::Scale, FVector4(CurrentScale), bIsScaleRelative, 
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::ComponentRelativeScaleTo(USceneComponent* SceneComponent, FVector Scale,
	bool bIsScaleRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, 
	float DelayBetweenLoops, const UObject* WorldContextObject)
{
	return NewTweenSceneComponent(SceneComponent, ETweenType::Scale, FVector4(Scale), bIsScaleRelative, 
		Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);
}

UTween* UTween::ComponentRelativeScaleFrom(USceneComponent* SceneComponent, FVector Scale,
	bool bIsScaleRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, 
	float DelayBetweenLoops, const UObject* WorldContextObject)
{
	if (!SceneComponent)
	{
		return nullptr;
	}

	const FVector CurrentScale = SceneComponent->RelativeScale3D;
	SceneComponent->SetRelativeScale3D(Scale);

	return NewTweenSceneComponent( SceneComponent, ETweenType::RelativeScale, FVector4(CurrentScale), bIsScaleRelative, 
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
	if (!SceneComponent)
	{
		return nullptr;
	}

	const FQuat CurrentRotation = SceneComponent->GetComponentQuat();
	SceneComponent->SetWorldRotation(Rotation);

	return NewTweenSceneComponent( SceneComponent, ETweenType::Rotation, FVector4(CurrentRotation.X, CurrentRotation.Y, CurrentRotation.Z, CurrentRotation.W), bIsRotationRelative, 
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
	if (!SceneComponent)
	{
		return nullptr;
	}

	const FQuat RelativeRotation = FQuat(SceneComponent->RelativeRotation);
	SceneComponent->SetRelativeRotation(Rotation);

	return NewTweenSceneComponent( SceneComponent, ETweenType::RelativeRotation, FVector4(RelativeRotation.X, RelativeRotation.Y, RelativeRotation.Z, RelativeRotation.W), bIsRotationRelative,
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
	if (!Widget)
	{
		return nullptr;
	}

	const FVector2D CurrentLocation = Widget->RenderTransform.Translation;

	Widget->SetRenderTranslation(Location);

	return NewTweenWidget(Widget, ETweenType::RelativeLocation, FVector4(CurrentLocation.X, CurrentLocation.Y), bIsLocationRelative, 
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
	if (!Widget)
	{
		return nullptr;
	}
	const FVector2D CurrentScale = Widget->RenderTransform.Translation;

	Widget->SetRenderScale(Scale);

	return NewTweenWidget(Widget, ETweenType::RelativeScale, FVector4(CurrentScale.X, CurrentScale.Y), bIsScaleRelative, 
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
	if (!Widget)
	{
		return nullptr;
	}
	
	const FVector2D CurrentShear = Widget->RenderTransform.Shear;

	Widget->SetRenderShear(Shear);

	return NewTweenWidget(Widget, ETweenType::RelativeShear, FVector4(CurrentShear.X, CurrentShear.Y), bIsShearRelative, 
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
	if (!Widget)
	{
		return nullptr;
	}
	
	const float CurrentAngle = Widget->GetRenderTransformAngle();

	Widget->SetRenderTransformAngle(Angle);

	return NewTweenWidget(Widget, ETweenType::RelativeAngle, FVector4(CurrentAngle), bIsAngleRelative, 
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
	if (!Widget)
	{
		return nullptr;
	}
	
	const float CurrentOpacity = Widget->GetRenderOpacity();

	Widget->SetRenderOpacity(Opacity);

	return NewTweenWidget(Widget, ETweenType::Opacity, FVector4(CurrentOpacity), bIsOpacityRelative, 
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
	if (!Widget)
	{
		return nullptr;
	}
	
	const FLinearColor CurrentColor = GetWidgetColorAndOpacity(*Widget);

	SetWidgetColorAndOpacity(*Widget, Color);

	return NewTweenWidget(Widget, ETweenType::Color, CurrentColor, bIsColorRelative, 
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
	if (!Object)
	{
		return nullptr;
	}

	if (UFloatProperty* FloatProperty = FindField<UFloatProperty>(Object->GetClass(), PropertyName))
	{
		const float ValueTo = FloatProperty->GetPropertyValue_InContainer(Object);

		FloatProperty->SetPropertyValue_InContainer(Object, Value);

		UTween* Tween = NewTween(FWeakObjectPtr(Object), ETweenTargetObjectType::Property, ETweenType::Scalar, FVector4(ValueTo), bIsValueRelative, 
			Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

		Tween->ParameterName = PropertyName;

		return Tween;
	}

	return nullptr;
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
	if (!Object)
	{
		return nullptr;
	}
	
	if (UStructProperty* StructProperty = FindField<UStructProperty>(Object->GetClass(), PropertyName))
	{
		FVector VectorTo = *StructProperty->ContainerPtrToValuePtr<FVector>(Object);

		*StructProperty->ContainerPtrToValuePtr<FVector>(Object) = Vector;

		UTween* Tween = NewTween(FWeakObjectPtr(Object), ETweenTargetObjectType::Property, ETweenType::Vector, VectorTo, bIsVectorRelative, 
			Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

		Tween->ParameterName = PropertyName;

		return Tween;
	}

	return nullptr;
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
				return CacheInitialValuesSceneComponent(*SceneComponent);
			}
			break;
			
		case ETweenTargetObjectType::Material:
			if (UMaterialInstanceDynamic* Material = Cast<UMaterialInstanceDynamic>(Object))
			{
				return CacheInitialValuesMaterial(*Material);
			}
			break;
			
		case ETweenTargetObjectType::Widget:
			if (const UWidget* Widget = Cast<UWidget>(Object))
			{
				return CacheInitialValuesWidget(*Widget);
			}
			break;

		case ETweenTargetObjectType::Property:
			return CacheInitialValuesProperty(*Object);

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

	switch (TargetValueType)
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


bool UTween::CacheInitialValuesSceneComponent(const USceneComponent& SceneComponent)
{
	switch (TweenType)
	{
		case ETweenType::Location:
			{
				TargetValueType = ETargetValueType::Vector;
				StartValue = SceneComponent.GetComponentLocation();
				return true;
			}
		case ETweenType::RelativeLocation:
			{
				TargetValueType = ETargetValueType::Vector;
				StartValue = SceneComponent.RelativeLocation;
				return true;
			}
		case ETweenType::Rotation:
			{
				TargetValueType = ETargetValueType::Quat;
				const FQuat Rotation = SceneComponent.GetComponentQuat();
				StartValue = FVector4(Rotation.X, Rotation.Y, Rotation.Z, Rotation.W);
				return true;
			}
		case ETweenType::RelativeRotation:
			{
				TargetValueType = ETargetValueType::Quat;
				const FQuat RelativeRotation = FQuat(SceneComponent.RelativeRotation);
				StartValue = FVector4(RelativeRotation.X, RelativeRotation.Y, RelativeRotation.Z, RelativeRotation.W);
				return true;
			}
		case ETweenType::Scale:
			{
				TargetValueType = ETargetValueType::Vector;
				StartValue = SceneComponent.GetComponentScale();
				return true;
			}
		case ETweenType::RelativeScale:
			{
				TargetValueType = ETargetValueType::Vector;
				StartValue = SceneComponent.RelativeScale3D;
				return true;
			}
		default:
			return false;
	}
}

bool UTween::CacheInitialValuesMaterial(UMaterialInstanceDynamic& Material)
{
	const FMaterialParameterInfo ParameterInfo(ParameterName);
	
	switch (TweenType)
	{
		case ETweenType::Color:
			{
				FLinearColor Color(0, 0, 0, 0);

				TargetValueType = ETargetValueType::Color;
				
				if (Material.GetVectorParameterValue(ParameterInfo, Color) && 
					Material.InitializeVectorParameterAndGetIndex(ParameterName, Color, ParameterIndex))
				{
					StartValue = FVector4(Color.R, Color.G, Color.B, Color.A);

					return true;
				}
			
				return false;
			}
		case ETweenType::Scalar:
			{
				float Scalar;
				
				TargetValueType = ETargetValueType::Scalar;

				if (Material.GetScalarParameterValue(ParameterInfo, Scalar) &&
					Material.InitializeScalarParameterAndGetIndex(ParameterName, Scalar, ParameterIndex))
				{
					StartValue = FVector4(Scalar);

					return true;
				}

				return false;
			}
		default:
			return false;
	}
}

FLinearColor UTween::GetWidgetColorAndOpacity(const UWidget& Widget)
{
	if(const UImage *Image = Cast<UImage>(&Widget))
	{
		return Image->ColorAndOpacity;
	}
	else if(const UBorder *Border = Cast<UBorder>(&Widget))
	{
		return Border->ContentColorAndOpacity;
	}
	else if(const UButton *Button = Cast<UButton>(&Widget))
	{
		return Button->ColorAndOpacity;
	}

	return FLinearColor::Black;
}

bool UTween::CacheInitialValuesWidget(const UWidget& Widget)
{
	switch (TweenType)
	{
		case ETweenType::RelativeLocation:
			TargetValueType = ETargetValueType::Vector;
			StartValue = FVector4(Widget.RenderTransform.Translation.X, Widget.RenderTransform.Translation.Y);
			return true;
		case ETweenType::RelativeScale:
			TargetValueType = ETargetValueType::Vector;
			StartValue = FVector4(Widget.RenderTransform.Scale.X, Widget.RenderTransform.Scale.Y);
			return true;
		case ETweenType::Color:
			TargetValueType = ETargetValueType::Color;
			StartValue = GetWidgetColorAndOpacity(Widget);
			return true;
		case ETweenType::RelativeShear:
			TargetValueType = ETargetValueType::Vector;
			StartValue = FVector4(Widget.RenderTransform.Shear.X, Widget.RenderTransform.Shear.Y);
			return true;
		case ETweenType::RelativeAngle:
			TargetValueType = ETargetValueType::Scalar;
			StartValue = FVector4(Widget.GetRenderTransformAngle());
			return true;
		case ETweenType::Opacity:
			TargetValueType = ETargetValueType::Scalar;
			StartValue = FVector4(Widget.RenderOpacity);
			return true;
		default:
			return false;
	}
}

bool UTween::CacheInitialValuesProperty(const UObject& Object)
{
	switch (TweenType)
	{
		case ETweenType::Scalar:
			if (UFloatProperty* FloatProperty = FindField<UFloatProperty>(Object.GetClass(), ParameterName))
			{
				const float Value = FloatProperty->GetPropertyValue_InContainer(&Object);
				TargetValueType = ETargetValueType::Scalar;
				StartValue = FVector4(Value);
				CachedProperty = FloatProperty;
				return true;
			}

			break;
	
		case ETweenType::Vector:
			if (UStructProperty* StructProperty = FindField<UStructProperty>(Object.GetClass(), ParameterName))
			{
				TargetValueType = ETargetValueType::Vector;
				StartValue = *StructProperty->ContainerPtrToValuePtr<FVector>(&Object);
				CachedProperty = StructProperty;
				return true;
			}

			break;
		default:
			return false;
	}

	return false;
}

bool UTween::Tick(float DeltaTime, float UnscaledDeltaTime, bool bCompleteTweenThisStep)
{
	// fetch our deltaTime. It will either be taking this to completion or standard delta/unscaledDelta
	DeltaTime = bCompleteTweenThisStep ? TNumericLimits< float >::Max() : (bIsTimeScaleIndependent ? UnscaledDeltaTime : DeltaTime);

	// add deltaTime to our elapsed time and clamp it from -delay to duration
	ElapsedTime = FMath::Clamp(ElapsedTime + DeltaTime, -Delay, Duration);

	// if we have a delay, we will have a negative elapsedTime until the delay is complete
	if (ElapsedTime <= 0)
	{
		return false;
	}

	const float ModifiedElapsedTime = bIsRunningInReverse ? Duration - ElapsedTime : ElapsedTime;

	const float EasedTime = EaseHelper(ModifiedElapsedTime / Duration, EaseType);//(animCurve == null) ? EaseHelper.ease(easeType, modifiedElapsedTime, duration) : animCurve.Evaluate(modifiedElapsedTime / duration);

	// special case: Action tweens
	/*if (tweenType == TweenType::Action)
		customAction(transform, easedTime);*/

	FVector4 Vec;
	
	switch (TargetValueType)
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

	SetAsRequiredPerCurrentTweenType(Vec);

	// if we have a loopType and we are done do the loop
	if (LoopType != ELoopType::None && FMath::IsNearlyEqual(ElapsedTime, Duration))
	{
		HandleLooping();
	}

	return FMath::IsNearlyEqual(ElapsedTime, Duration);
}

void UTween::Activate()
{
	UWorld* World = WorldContextObject->GetWorld();

	UGameInstance* GameInstance = World->GetGameInstance();

	UTweenerSubsystem* Subsystem = GameInstance->GetSubsystem<UTweenerSubsystem>();

	if(PrepareForUse())
	{
		Subsystem->AddTween(this);
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
	}

	// kill our loop if we have no loops left and zero out the delay then prepare for use
	if (Loops == 0)
		LoopType = ELoopType::None;

	Delay = DelayBetweenLoops;
	ElapsedTime = -Delay;
}

void UTween::SetAsRequiredSceneComponent(const FVector4& Vec, USceneComponent& SceneComponent) const
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

void UTween::SetAsRequiredMaterial(const FVector4& Vec, UMaterialInstanceDynamic& Material) const
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

void UTween::SetWidgetColorAndOpacity(UWidget& Widget, FLinearColor ColorAndOpacity)
{
	if (UImage* Image = Cast<UImage>(&Widget))
	{
		Image->SetColorAndOpacity(ColorAndOpacity);
	}
	else if (UBorder* Border = Cast<UBorder>(&Widget))
	{
		Border->SetContentColorAndOpacity(ColorAndOpacity);
	}
	else if (UButton* Button = Cast<UButton>(&Widget))
	{
		Button->SetColorAndOpacity(ColorAndOpacity);
	}
}

void UTween::SetAsRequiredWidget(const FVector4& Vec, UWidget& Widget) const
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
		{	
			SetWidgetColorAndOpacity(Widget, FLinearColor(Vec));
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
	default: ;
	}
}

void UTween::SetAsRequiredProperty(const FVector4& Vec, UObject& Object) const
{
	switch (TweenType)
	{
		case ETweenType::Scalar:
			if(UFloatProperty *FloatProperty = Cast<UFloatProperty>(CachedProperty))
			{
				FloatProperty->SetPropertyValue_InContainer(&Object, static_cast<float>(Vec.X));
			}

			break;
		case ETweenType::Vector:
			if (UStructProperty* StructProperty = Cast<UStructProperty>(CachedProperty))
			{
				*StructProperty->ContainerPtrToValuePtr<FVector>(&Object) = FVector(Vec.X,Vec.Y,Vec.Z);
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
				SetAsRequiredSceneComponent(Vec, *SceneComponent);
			}
			break;
		case ETweenTargetObjectType::Material:
			if (UMaterialInstanceDynamic* Material = Cast<UMaterialInstanceDynamic>(Object))
			{
				SetAsRequiredMaterial(Vec, *Material);
			}
			break;
		case ETweenTargetObjectType::Widget:
			if (UWidget* Widget = Cast<UWidget>(Object))
			{
				SetAsRequiredWidget(Vec, *Widget);
			}
		case ETweenTargetObjectType::Property:
			SetAsRequiredProperty(Vec, *Object);

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

bool UTween::Stop(bool bBringToCompletion)
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
		return Subsystem->StopTween(this, bBringToCompletion);
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


