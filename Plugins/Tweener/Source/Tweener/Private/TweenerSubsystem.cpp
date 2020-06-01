#include "TweenerSubsystem.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Blueprint/UserWidget.h"


// Called every frame
void UTweenerSubsystem::Tick(float DeltaTime)
{
	float UnscaledDeltaTime = DeltaTime;
	
	if(UWorld* TickableGameObjectWorld = GetTickableGameObjectWorld())
	{
		if (UWorld* World = TickableGameObjectWorld->GetWorld())
		{
			const float CurrentWorldTime = World->GetTimeSeconds();

			DeltaTime = CurrentWorldTime - LastTickGameTimeSeconds;

			LastTickGameTimeSeconds = CurrentWorldTime;
		}
	}

	
	// loop backwards so we can remove completed tweens
	for (int32 Index = ActiveTweens.Num() -1 ; Index >= 0; --Index)
	{
		UTween *Tween = ActiveTweens[Index];
		
		if (Tween->bIsPaused)
		{
			continue;
		}

		if (!Tween->ObjectPtr.IsValid() || Tween->Tick(DeltaTime, UnscaledDeltaTime))
		{
			Tween->Complete.Broadcast();

			//// handle nextTween if we have a chain
			if (Tween->NextTween != nullptr)
			{
				if(Tween->NextTween->PrepareForUse())
				{
					ActiveTweens.Add(Tween->NextTween);
				}
				
				// null out the nextTween so that the reset method doesn't remove it!
				Tween->NextTween = nullptr;
			}

			ActiveTweens.RemoveAt(Index);
		}
	}
}

ETickableTickType UTweenerSubsystem::GetTickableTickType() const
{
	return ETickableTickType::Always;
}

TStatId UTweenerSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UTweenerSubsystem, STATGROUP_Tickables);
}

bool UTweenerSubsystem::StopTween(UTween* Tween, bool bBringToCompletion)
{
	if(ActiveTweens.Remove(Tween) > 0)
	{
		if (bBringToCompletion && Tween->ObjectPtr.IsValid())
		{
			Tween->Tick(0.f, 0.f, true);
		}

		return true;
	}

	return false;
}

bool UTweenerSubsystem::StopTweenForObject(UObject *Object, bool bBringToCompletion)
{
	const FWeakObjectPtr ObjectPtr = FWeakObjectPtr(Object);

	const bool bComplete = ObjectPtr.IsValid() && bBringToCompletion;

	bool bDidRemoveTween = false;
	
	for (int32 Index = ActiveTweens.Num() - 1; Index >= 0; --Index)
	{
		UTween* Tween = ActiveTweens[Index];

		if(Tween->ObjectPtr == ObjectPtr)
		{
			if (bComplete)
			{
				Tween->Tick(0.f, 0.f, true);
			}

			ActiveTweens.RemoveAt(Index);
			bDidRemoveTween = true;
		}
	}

	return bDidRemoveTween;
}

void UTweenerSubsystem::StopAllTweens(bool bBringToCompletion)
{
	if(bBringToCompletion)
	{
		for (auto ActiveTween : ActiveTweens)
		{
			if (ActiveTween->ObjectPtr.IsValid())
			{
				ActiveTween->Tick(0.f, 0.f, true);
			}
		}
	}

	ActiveTweens.Empty();
}

void UTweenerSubsystem::SetAllTweenPauseState(bool bIsPaused)
{
	for (auto ActiveTween : ActiveTweens)
	{
		ActiveTween->bIsPaused = bIsPaused;
	}
}

UTween* UTweenerSubsystem::ComponentLocationTo(USceneComponent* SceneComponent, FVector Location,
                                               bool bIsLocationRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops,
                                               float DelayBetweenLoops)
{
	UTween* Tween = UTween::ComponentLocationTo( SceneComponent, Location, bIsLocationRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops);

	return PrepareAndAdd(Tween);
}

UTween* UTweenerSubsystem::ComponentLocationFrom(USceneComponent* SceneComponent, FVector Location,
	bool bIsLocationRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops,
	float DelayBetweenLoops)
{
	UTween* Tween = UTween::ComponentLocationFrom( SceneComponent, Location, bIsLocationRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops);

	return PrepareAndAdd(Tween);
}

UTween* UTweenerSubsystem::ComponentRelativeLocationTo(USceneComponent* SceneComponent, FVector Location,
	bool bIsLocationRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops,
	float DelayBetweenLoops)
{
	UTween* Tween = UTween::ComponentRelativeLocationTo( SceneComponent, Location, bIsLocationRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops);

	return PrepareAndAdd(Tween);
}

UTween* UTweenerSubsystem::ComponentRelativeLocationFrom(USceneComponent* SceneComponent, FVector Location,
	bool bIsLocationRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops,
	float DelayBetweenLoops)
{
	UTween* Tween = UTween::ComponentRelativeLocationFrom( SceneComponent, Location, bIsLocationRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops);

	return PrepareAndAdd(Tween);
}

UTween* UTweenerSubsystem::ComponentScaleTo(USceneComponent* SceneComponent, FVector Scale, bool bIsScaleRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops)
{
	UTween* Tween = UTween::ComponentScaleTo( SceneComponent, Scale, bIsScaleRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops);

	return PrepareAndAdd(Tween);
}

UTween* UTweenerSubsystem::ComponentScaleFrom(USceneComponent* SceneComponent, FVector Scale, bool bIsScaleRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops)
{
	UTween* Tween = UTween::ComponentScaleFrom( SceneComponent, Scale, bIsScaleRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops);

	return PrepareAndAdd(Tween);
}

UTween* UTweenerSubsystem::ComponentRelativeScaleTo(USceneComponent* SceneComponent, FVector Scale,
	bool bIsScaleRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops)
{
	UTween* Tween = UTween::ComponentRelativeScaleTo( SceneComponent, Scale, bIsScaleRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops);

	return PrepareAndAdd(Tween);
}

UTween* UTweenerSubsystem::ComponentRelativeScaleFrom(USceneComponent* SceneComponent, FVector Scale,
	bool bIsScaleRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops)
{
	UTween* Tween = UTween::ComponentRelativeScaleFrom( SceneComponent, Scale, bIsScaleRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops);

	return PrepareAndAdd(Tween);
}

UTween* UTweenerSubsystem::ComponentRotationTo(USceneComponent* SceneComponent, FQuat Rotation,
	bool bIsRotationRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops,
	float DelayBetweenLoops)
{
	UTween* Tween = UTween::ComponentRotationTo( SceneComponent, Rotation, bIsRotationRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops);

	return PrepareAndAdd(Tween);
}

UTween* UTweenerSubsystem::ComponentRotationFrom(USceneComponent* SceneComponent, FQuat Rotation,
	bool bIsRotationRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops,
	float DelayBetweenLoops)
{
	UTween* Tween = UTween::ComponentRotationFrom( SceneComponent, Rotation, bIsRotationRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops);

	return PrepareAndAdd(Tween);
}

UTween* UTweenerSubsystem::ComponentRelativeRotationTo(USceneComponent* SceneComponent, FQuat Rotation,
	bool bIsRotationRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops,
	float DelayBetweenLoops)
{
	UTween* Tween = UTween::ComponentRelativeRotationTo( SceneComponent, Rotation, bIsRotationRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops);

	return PrepareAndAdd(Tween);
}

UTween* UTweenerSubsystem::ComponentRelativeRotationFrom(USceneComponent* SceneComponent, FQuat Rotation,
	bool bIsRotationRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops,
	float DelayBetweenLoops)
{
	UTween* Tween = UTween::ComponentRelativeRotationFrom( SceneComponent, Rotation, bIsRotationRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops);

	return PrepareAndAdd(Tween);
}

UTween* UTweenerSubsystem::ActorLocationTo(AActor* Actor, FVector Location, bool bIsLocationRelative, float Duration,
	EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops)
{
	UTween* Tween = UTween::ActorLocationTo( Actor, Location, bIsLocationRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops);

	return PrepareAndAdd(Tween);
}

UTween* UTweenerSubsystem::ActorLocationFrom(AActor* Actor, FVector Location, bool bIsLocationRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops)
{
	UTween* Tween = UTween::ActorLocationFrom( Actor, Location, bIsLocationRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops);

	return PrepareAndAdd(Tween);
}

UTween* UTweenerSubsystem::ActorRelativeLocationTo(AActor* Actor, FVector Location, bool bIsLocationRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops)
{
	UTween* Tween = UTween::ActorRelativeLocationTo( Actor, Location, bIsLocationRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops);

	return PrepareAndAdd(Tween);
}

UTween* UTweenerSubsystem::ActorRelativeLocationFrom(AActor* Actor, FVector Location, bool bIsLocationRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops)
{
	UTween* Tween = UTween::ActorRelativeLocationFrom( Actor, Location, bIsLocationRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops);

	return PrepareAndAdd(Tween);
}

UTween* UTweenerSubsystem::ActorScaleTo(AActor* Actor, FVector Scale, bool bIsScaleRelative, float Duration,
	EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops)
{
	UTween* Tween = UTween::ActorScaleTo( Actor, Scale, bIsScaleRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops);

	return PrepareAndAdd(Tween);
}

UTween* UTweenerSubsystem::ActorScaleFrom(AActor* Actor, FVector Scale, bool bIsScaleRelative, float Duration,
	EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops)
{
	UTween* Tween = UTween::ActorScaleFrom( Actor, Scale, bIsScaleRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops);

	return PrepareAndAdd(Tween);
}

UTween* UTweenerSubsystem::ActorRelativeScaleTo(AActor* Actor, FVector Scale, bool bIsScaleRelative, float Duration,
	EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops)
{
	UTween* Tween = UTween::ActorRelativeScaleTo( Actor, Scale, bIsScaleRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops);

	return PrepareAndAdd(Tween);
}

UTween* UTweenerSubsystem::ActorRelativeScaleFrom(AActor* Actor, FVector Scale, bool bIsScaleRelative, float Duration,
	EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops)
{
	UTween* Tween = UTween::ActorRelativeScaleFrom( Actor, Scale, bIsScaleRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops);

	return PrepareAndAdd(Tween);
}

UTween* UTweenerSubsystem::ActorRotationTo(AActor* Actor, FQuat Rotation, bool bIsRotationRelative, float Duration,
	EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops)
{
	UTween* Tween = UTween::ActorRotationTo( Actor, Rotation, bIsRotationRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops);

	return PrepareAndAdd(Tween);
}

UTween* UTweenerSubsystem::ActorRotationFrom(AActor* Actor, FQuat Rotation, bool bIsRotationRelative, float Duration,
	EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops)
{
	UTween* Tween = UTween::ActorRotationFrom( Actor, Rotation, bIsRotationRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops);

	return PrepareAndAdd(Tween);
}

UTween* UTweenerSubsystem::ActorRelativeRotationTo(AActor* Actor, FQuat Rotation, bool bIsRotationRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops)
{
	UTween* Tween = UTween::ActorRelativeRotationTo( Actor, Rotation, bIsRotationRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops);

	return PrepareAndAdd(Tween);
}

UTween* UTweenerSubsystem::ActorRelativeRotationFrom(AActor* Actor, FQuat Rotation, bool bIsRotationRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops)
{
	UTween* Tween = UTween::ActorRelativeRotationFrom( Actor, Rotation, bIsRotationRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops);

	return PrepareAndAdd(Tween);
}

UTween* UTweenerSubsystem::WidgetRenderLocationTo(UWidget* Widget, FVector2D Location,
	bool bIsLocationRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops,
	float DelayBetweenLoops)
{
	UTween* Tween = UTween::WidgetRenderLocationTo( Widget, Location, bIsLocationRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops);

	return PrepareAndAdd(Tween);
}

UTween* UTweenerSubsystem::WidgetRenderLocationFrom(UWidget* Widget, FVector2D Location,
	bool bIsLocationRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops,
	float DelayBetweenLoops)
{
	UTween* Tween = UTween::WidgetRenderLocationFrom( Widget, Location, bIsLocationRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops);

	return PrepareAndAdd(Tween);
}

UTween* UTweenerSubsystem::WidgetRenderScaleTo(UWidget* Widget, FVector2D Scale, bool bIsScaleRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops)
{
	UTween* Tween = UTween::WidgetRenderScaleTo( Widget, Scale, bIsScaleRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops);

	return PrepareAndAdd(Tween);
}

UTween* UTweenerSubsystem::WidgetRenderScaleFrom(UWidget* Widget, FVector2D Scale, bool bIsScaleRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops)
{
	UTween* Tween = UTween::WidgetRenderScaleFrom( Widget, Scale, bIsScaleRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops);

	return PrepareAndAdd(Tween);
}

UTween* UTweenerSubsystem::WidgetRenderShearTo(UWidget* Widget, FVector2D Shear, bool bIsShearRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops)
{
	UTween* Tween = UTween::WidgetRenderShearTo( Widget, Shear, bIsShearRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops);

	return PrepareAndAdd(Tween);
}

UTween* UTweenerSubsystem::WidgetRenderShearFrom(UWidget* Widget, FVector2D Shear, bool bIsShearRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops)
{
	UTween* Tween = UTween::WidgetRenderShearFrom( Widget, Shear, bIsShearRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops);

	return PrepareAndAdd(Tween);
}

UTween* UTweenerSubsystem::WidgetRenderAngleTo(UWidget* Widget, float Angle, bool bIsAngleRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops)
{
	UTween* Tween = UTween::WidgetRenderAngleTo( Widget, Angle, bIsAngleRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops);

	return PrepareAndAdd(Tween);
}

UTween* UTweenerSubsystem::WidgetRenderAngleFrom(UWidget* Widget, float Angle, bool bIsAngleRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops)
{
	UTween* Tween = UTween::WidgetRenderAngleFrom( Widget, Angle, bIsAngleRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops);

	return PrepareAndAdd(Tween);
}

UTween* UTweenerSubsystem::WidgetRenderOpacityTo(UWidget* Widget, float Opacity, bool bIsOpacityRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops)
{
	UTween* Tween = UTween::WidgetRenderOpacityTo( Widget, Opacity, bIsOpacityRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops);

	return PrepareAndAdd(Tween);
}

UTween* UTweenerSubsystem::WidgetRenderOpacityFrom(UWidget* Widget, float Opacity, bool bIsOpacityRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops)
{
	UTween* Tween = UTween::WidgetRenderOpacityFrom( Widget, Opacity, bIsOpacityRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops);

	return PrepareAndAdd(Tween);
}

UTween* UTweenerSubsystem::WidgetColorTo(UWidget* Widget, FLinearColor Color, bool bIsColorRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops)
{
	UTween* Tween = UTween::WidgetColorTo( Widget, Color, bIsColorRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops);

	return PrepareAndAdd(Tween);
}

UTween* UTweenerSubsystem::WidgetColorFrom(UWidget* Widget, const FLinearColor& Color, bool bIsColorRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops)
{
	UTween* Tween = UTween::WidgetColorFrom( Widget, Color, bIsColorRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops);

	return PrepareAndAdd(Tween);
}

UTween* UTweenerSubsystem::MaterialColorTo(UMaterialInstanceDynamic* Material, FName MaterialProperty,
	FLinearColor Color, bool bIsColorRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops,
	float DelayBetweenLoops)
{
	UTween* Tween = UTween::MaterialColorTo( Material, MaterialProperty, Color, bIsColorRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops);

	return PrepareAndAdd(Tween);
}

UTween* UTweenerSubsystem::MaterialScalarTo(UMaterialInstanceDynamic* Material, FName MaterialProperty,
	float Scalar, bool bIsScalarRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops,
	float DelayBetweenLoops)
{
	UTween* Tween = UTween::MaterialScalarTo( Material, MaterialProperty, Scalar, bIsScalarRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops);

	return PrepareAndAdd(Tween);
}

UTween* UTweenerSubsystem::FloatTo(UObject* Object, FName PropertyName, float Value, bool bIsValueRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops)
{
	UTween* Tween = UTween::FloatTo(Object, PropertyName, Value, bIsValueRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops);

	return PrepareAndAdd(Tween);
}

UTween* UTweenerSubsystem::FloatFrom(UObject* Object, FName PropertyName, float Value, bool bIsValueRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops)
{
	UTween* Tween = UTween::FloatFrom(Object, PropertyName, Value, bIsValueRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops);

	return PrepareAndAdd(Tween);
}

UTween* UTweenerSubsystem::VectorTo(UObject* Object, FName PropertyName, FVector ValueTo, bool bIsVectorRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops)
{
	UTween* Tween = UTween::VectorTo(Object, PropertyName, ValueTo, bIsVectorRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops);

	return PrepareAndAdd(Tween);
}

UTween* UTweenerSubsystem::VectorFrom(UObject* Object, FName PropertyName, FVector ValueFrom, bool bIsVectorRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops)
{
	UTween* Tween = UTween::VectorFrom(Object, PropertyName, ValueFrom, bIsVectorRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops);

	return PrepareAndAdd(Tween);
}

void UTweenerSubsystem::AddTween(UTween* Tween)
{
	ActiveTweens.Add(Tween);
}

UTween* UTweenerSubsystem::PrepareAndAdd(UTween* Tween)
{
	if (Tween && Tween->PrepareForUse())
	{
		AddTween(Tween);
		return Tween;
	}
	else
	{
		return nullptr;
	}
}







