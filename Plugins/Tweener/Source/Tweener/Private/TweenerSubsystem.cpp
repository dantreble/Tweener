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

		if(Tween == nullptr)
		{
			ActiveTweens.RemoveAt(Index);
			continue;
		}
		
		if (Tween->bIsPaused)
		{
			continue;
		}

		if (!Tween->ObjectPtr.IsValid() || Tween->Tick(DeltaTime, UnscaledDeltaTime))
		{
			ActiveTweens.RemoveAt(Index);

			Tween->Complete.Broadcast();
			Tween->CompleteDelegate.Broadcast();

			//// handle nextTween if we have a chain
			if (Tween->NextTween != nullptr)
			{
				if(Tween->NextTween->PrepareForUse())
				{
					ActiveTweens.Add(Tween->NextTween);
				}
			}
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

bool UTweenerSubsystem::StopTween(UTween* Tween, bool bBringToCompletion, bool bIncludeChain)
{
	UTween* TweenItr = Tween;

	bool bFoundActive = false;

	while (TweenItr)
	{
		const bool bWasActive = ActiveTweens.Remove(TweenItr) > 0;
		
		if (bFoundActive || bWasActive)
		{
			if (bBringToCompletion)
			{
				if (TweenItr->ObjectPtr.IsValid())
				{
					if(!bWasActive)
					{
						TweenItr->PrepareForUse();
					}

					TweenItr->Tick(0.f, 0.f, true);
				}

				TweenItr->Complete.Broadcast();
				TweenItr->CompleteDelegate.Broadcast();
			}

			//Every linked tween after the active tween will need to be Completed
			bFoundActive = true;
		}

		TweenItr = bIncludeChain ? TweenItr->NextTween : nullptr;
	}

	return bFoundActive;
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

				Tween->Complete.Broadcast();
				Tween->CompleteDelegate.Broadcast();
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

				ActiveTween->Complete.Broadcast();
				ActiveTween->CompleteDelegate.Broadcast();
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

bool UTweenerSubsystem::IsTweenActive(const UTween* Tween) const
{
	return ActiveTweens.Contains(Tween);
}

bool UTweenerSubsystem::ObjectHasActiveTweens(const UObject* Object) const
{
	const FWeakObjectPtr ObjectPtr = FWeakObjectPtr(Object);
	
	for (auto ActiveTween : ActiveTweens)
	{
		if (ActiveTween->ObjectPtr == ObjectPtr)
		{
			return true;
		}
	}

	return false;
}


UTween* UTweenerSubsystem::ComponentLocationTo(USceneComponent* SceneComponent, FVector Location,
                                               bool bIsLocationRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops,
                                               float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::ComponentLocationTo( SceneComponent, Location, bIsLocationRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::ComponentLocationFrom(USceneComponent* SceneComponent, FVector Location,
	bool bIsLocationRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops,
	float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::ComponentLocationFrom( SceneComponent, Location, bIsLocationRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::ComponentRelativeLocationTo(USceneComponent* SceneComponent, FVector Location,
	bool bIsLocationRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops,
	float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::ComponentRelativeLocationTo( SceneComponent, Location, bIsLocationRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::ComponentRelativeLocationFrom(USceneComponent* SceneComponent, FVector Location,
	bool bIsLocationRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops,
	float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::ComponentRelativeLocationFrom( SceneComponent, Location, bIsLocationRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::ComponentScaleTo(USceneComponent* SceneComponent, FVector Scale, bool bIsScaleRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::ComponentScaleTo( SceneComponent, Scale, bIsScaleRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::ComponentScaleFrom(USceneComponent* SceneComponent, FVector Scale, bool bIsScaleRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::ComponentScaleFrom( SceneComponent, Scale, bIsScaleRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::ComponentRelativeScaleTo(USceneComponent* SceneComponent, FVector Scale,
	bool bIsScaleRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::ComponentRelativeScaleTo( SceneComponent, Scale, bIsScaleRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::ComponentRelativeScaleFrom(USceneComponent* SceneComponent, FVector Scale,
	bool bIsScaleRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::ComponentRelativeScaleFrom( SceneComponent, Scale, bIsScaleRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::ComponentRotationTo(USceneComponent* SceneComponent, FQuat Rotation,
	bool bIsRotationRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops,
	float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::ComponentRotationTo( SceneComponent, Rotation, bIsRotationRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::ComponentRotationFrom(USceneComponent* SceneComponent, FQuat Rotation,
	bool bIsRotationRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops,
	float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::ComponentRotationFrom( SceneComponent, Rotation, bIsRotationRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::ComponentRelativeRotationTo(USceneComponent* SceneComponent, FQuat Rotation,
	bool bIsRotationRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops,
	float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::ComponentRelativeRotationTo( SceneComponent, Rotation, bIsRotationRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::ComponentRelativeRotationFrom(USceneComponent* SceneComponent, FQuat Rotation,
	bool bIsRotationRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops,
	float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::ComponentRelativeRotationFrom( SceneComponent, Rotation, bIsRotationRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::ActorLocationTo(AActor* Actor, FVector Location, bool bIsLocationRelative, float Duration,
	EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::ActorLocationTo( Actor, Location, bIsLocationRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::ActorLocationFrom(AActor* Actor, FVector Location, bool bIsLocationRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::ActorLocationFrom( Actor, Location, bIsLocationRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::ActorRelativeLocationTo(AActor* Actor, FVector Location, bool bIsLocationRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::ActorRelativeLocationTo( Actor, Location, bIsLocationRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::ActorRelativeLocationFrom(AActor* Actor, FVector Location, bool bIsLocationRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::ActorRelativeLocationFrom( Actor, Location, bIsLocationRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::ActorScaleTo(AActor* Actor, FVector Scale, bool bIsScaleRelative, float Duration,
	EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::ActorScaleTo( Actor, Scale, bIsScaleRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::ActorScaleFrom(AActor* Actor, FVector Scale, bool bIsScaleRelative, float Duration,
	EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::ActorScaleFrom( Actor, Scale, bIsScaleRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::ActorRelativeScaleTo(AActor* Actor, FVector Scale, bool bIsScaleRelative, float Duration,
	EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::ActorRelativeScaleTo( Actor, Scale, bIsScaleRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::ActorRelativeScaleFrom(AActor* Actor, FVector Scale, bool bIsScaleRelative, float Duration,
	EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::ActorRelativeScaleFrom( Actor, Scale, bIsScaleRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::ActorRotationTo(AActor* Actor, FQuat Rotation, bool bIsRotationRelative, float Duration,
	EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::ActorRotationTo( Actor, Rotation, bIsRotationRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::ActorRotationFrom(AActor* Actor, FQuat Rotation, bool bIsRotationRelative, float Duration,
	EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::ActorRotationFrom( Actor, Rotation, bIsRotationRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::ActorRelativeRotationTo(AActor* Actor, FQuat Rotation, bool bIsRotationRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::ActorRelativeRotationTo( Actor, Rotation, bIsRotationRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::ActorRelativeRotationFrom(AActor* Actor, FQuat Rotation, bool bIsRotationRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::ActorRelativeRotationFrom( Actor, Rotation, bIsRotationRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::WidgetSlotPositionTo(UWidget* Widget, FVector2D Location, 
	bool bIsLocationRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, 
	float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::WidgetSlotPositionTo(Widget, Location, bIsLocationRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::WidgetSlotPositionFrom(UWidget* Widget, FVector2D Location, 
	bool bIsLocationRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, 
	float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::WidgetSlotPositionFrom(Widget, Location, bIsLocationRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::WidgetRenderLocationTo(UWidget* Widget, FVector2D Location,
	bool bIsLocationRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops,
	float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::WidgetRenderLocationTo( Widget, Location, bIsLocationRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::WidgetRenderLocationFrom(UWidget* Widget, FVector2D Location,
	bool bIsLocationRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops,
	float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::WidgetRenderLocationFrom( Widget, Location, bIsLocationRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::WidgetRenderScaleTo(UWidget* Widget, FVector2D Scale, bool bIsScaleRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::WidgetRenderScaleTo( Widget, Scale, bIsScaleRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::WidgetRenderScaleFrom(UWidget* Widget, FVector2D Scale, bool bIsScaleRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::WidgetRenderScaleFrom( Widget, Scale, bIsScaleRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::WidgetRenderShearTo(UWidget* Widget, FVector2D Shear, bool bIsShearRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::WidgetRenderShearTo( Widget, Shear, bIsShearRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::WidgetRenderShearFrom(UWidget* Widget, FVector2D Shear, bool bIsShearRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::WidgetRenderShearFrom( Widget, Shear, bIsShearRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::WidgetRenderAngleTo(UWidget* Widget, float Angle, bool bIsAngleRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::WidgetRenderAngleTo( Widget, Angle, bIsAngleRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::WidgetRenderAngleFrom(UWidget* Widget, float Angle, bool bIsAngleRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::WidgetRenderAngleFrom( Widget, Angle, bIsAngleRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::WidgetRenderOpacityTo(UWidget* Widget, float Opacity, bool bIsOpacityRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::WidgetRenderOpacityTo( Widget, Opacity, bIsOpacityRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::WidgetRenderOpacityFrom(UWidget* Widget, float Opacity, bool bIsOpacityRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::WidgetRenderOpacityFrom( Widget, Opacity, bIsOpacityRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::WidgetColorTo(UWidget* Widget, FLinearColor Color, bool bIsColorRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::WidgetColorTo( Widget, Color, bIsColorRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::WidgetColorFrom(UWidget* Widget, const FLinearColor& Color, bool bIsColorRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::WidgetColorFrom( Widget, Color, bIsColorRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::WidgetBackgroundColorTo(UWidget* Widget, FLinearColor Color, bool bIsColorRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::WidgetBackgroundColorTo(Widget, Color, bIsColorRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::WidgetBackgroundColorFrom(UWidget* Widget, const FLinearColor& Color, bool bIsColorRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::WidgetBackgroundColorFrom(Widget, Color, bIsColorRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}


UTween* UTweenerSubsystem::WidgetBlurStrengthTo(UWidget* Widget, float BlurStrength, bool bIsBlurStrengthRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::WidgetBlurStrengthTo(Widget, BlurStrength, bIsBlurStrengthRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::WidgetBlurStrengthFrom(UWidget* Widget, float BlurStrength, bool bIsBlurStrengthRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::WidgetBlurStrengthFrom(Widget, BlurStrength, bIsBlurStrengthRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}


UTween* UTweenerSubsystem::MaterialColorTo(UMaterialInstanceDynamic* Material, FName MaterialProperty,
	FLinearColor Color, bool bIsColorRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops,
	float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::MaterialColorTo( Material, MaterialProperty, Color, bIsColorRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::MaterialScalarTo(UMaterialInstanceDynamic* Material, FName MaterialProperty,
	float Scalar, bool bIsScalarRelative, float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops,
	float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::
		MaterialScalarTo( Material, MaterialProperty, Scalar, bIsScalarRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::FloatTo(UObject* Object, FName PropertyName, float Value, bool bIsValueRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::FloatTo(Object, PropertyName, Value, bIsValueRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::FloatFrom(UObject* Object, FName PropertyName, float Value, bool bIsValueRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::FloatFrom(Object, PropertyName, Value, bIsValueRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::VectorTo(UObject* Object, FName PropertyName, FVector ValueTo, bool bIsVectorRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::VectorTo(Object, PropertyName, ValueTo, bIsVectorRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::VectorFrom(UObject* Object, FName PropertyName, FVector ValueFrom, bool bIsVectorRelative,
	float Duration, EEaseType EaseType, ELoopType LoopType, int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::VectorFrom(Object, PropertyName, ValueFrom, bIsVectorRelative, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}

UTween* UTweenerSubsystem::CustomAction(UObject* Object, float From, float To, float Duration, EEaseType EaseType, ELoopType LoopType,
	int32 Loops, float DelayBetweenLoops, const UObject* WorldContextObject)
{
	UTween* Tween = UTween::CustomAction(Object,From, To, Duration, EaseType, LoopType, Loops, DelayBetweenLoops, WorldContextObject);

	return StartTween(Tween);
}


UTween* UTweenerSubsystem::StartTween(UTween* Tween)
{
	if (Tween && Tween->PrepareForUse())
	{
		ActiveTweens.AddUnique(Tween);
		return Tween;
	}

	return nullptr;
}







