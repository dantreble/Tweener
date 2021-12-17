#pragma once

#include "CoreMinimal.h"
#include "Tween.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Tickable.h"
#include "TweenerSubsystem.generated.h"

class UMaterialInstanceDynamic;


UCLASS()
class TWEENER_API UTweenerSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()
	
public:	
	UTweenerSubsystem() {}

	// FTickableGameObject begin
	virtual UWorld* GetTickableGameObjectWorld() const override { return GetOuter()->GetWorld();}
	virtual void Tick(float DeltaTime) override;
	virtual ETickableTickType GetTickableTickType() const override;
	virtual TStatId GetStatId() const override;
	virtual bool IsTickableWhenPaused() const override { return true; }
	// FTickableGameObject end

	UFUNCTION(BlueprintCallable, Category = "Tween Management")
	bool StopTween(UTween* Tween, bool bBringToCompletion=false, bool bIncludeChain=false);

	UFUNCTION(BlueprintCallable, Category = "Tween Management")
	bool StopTweenForObject(UObject* Object, bool bBringToCompletion);

	UFUNCTION(BlueprintCallable, Category = "Tween Management")
	UTween* StartTween(UTween* Tween);
	
	UFUNCTION(BlueprintCallable, Category = "Tween Management")
	void StopAllTweens(bool bBringToCompletion);

	UFUNCTION(BlueprintCallable, Category = "Tween Management")
	void SetAllTweenPauseState(bool bIsPaused);

	UFUNCTION(BlueprintCallable, Category = "Tween Management")
	bool IsTweenActive(const UTween* Tween) const;

	UFUNCTION(BlueprintCallable, Category = "Tween Management")
	bool ObjectHasActiveTweens(const UObject* Object) const ;

	
	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	UTween* ComponentLocationTo(USceneComponent * SceneComponent, FVector Location, bool bIsLocationRelative = true, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);
	
	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	UTween* ComponentLocationFrom(USceneComponent * SceneComponent, FVector Location, bool bIsLocationRelative = true, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	UTween* ComponentRelativeLocationTo(USceneComponent * SceneComponent, FVector Location, bool bIsLocationRelative = true, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	UTween* ComponentRelativeLocationFrom(USceneComponent * SceneComponent, FVector Location, bool bIsLocationRelative = true, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	UTween* ComponentScaleTo(USceneComponent * SceneComponent, FVector Scale = FVector(1.0f, 1.0f, 1.0f), bool bIsScaleRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	UTween* ComponentScaleFrom(USceneComponent * SceneComponent, FVector Scale = FVector(1.0f, 1.0f, 1.0f), bool bIsScaleRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	UTween* ComponentRelativeScaleTo(USceneComponent * SceneComponent, FVector Scale = FVector(1.0f, 1.0f, 1.0f), bool bIsScaleRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	UTween* ComponentRelativeScaleFrom(USceneComponent * SceneComponent, FVector Scale = FVector(1.0f, 1.0f, 1.0f), bool bIsScaleRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	UTween* ComponentRotationTo(USceneComponent * SceneComponent, FQuat Rotation, bool bIsRotationRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	UTween* ComponentRotationFrom(USceneComponent * SceneComponent, FQuat Rotation, bool bIsRotationRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	UTween* ComponentRelativeRotationTo(USceneComponent * SceneComponent, FQuat Rotation, bool bIsRotationRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	UTween* ComponentRelativeRotationFrom(USceneComponent * SceneComponent, FQuat Rotation, bool bIsRotationRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	UTween* ActorLocationTo(AActor * Actor, FVector Location, bool bIsLocationRelative = true, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	UTween* ActorLocationFrom(AActor * Actor, FVector Location, bool bIsLocationRelative = true, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	UTween* ActorRelativeLocationTo(AActor * Actor, FVector Location, bool bIsLocationRelative = true, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	UTween* ActorRelativeLocationFrom(AActor * Actor, FVector Location, bool bIsLocationRelative = true, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	UTween* ActorScaleTo(AActor * Actor, FVector Scale = FVector(1.0f, 1.0f, 1.0f), bool bIsScaleRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	UTween* ActorScaleFrom(AActor * Actor, FVector Scale = FVector(1.0f, 1.0f, 1.0f), bool bIsScaleRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	UTween* ActorRelativeScaleTo(AActor * Actor, FVector Scale = FVector(1.0f, 1.0f, 1.0f), bool bIsScaleRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	UTween* ActorRelativeScaleFrom(AActor * Actor, FVector Scale = FVector(1.0f, 1.0f, 1.0f), bool bIsScaleRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	UTween* ActorRotationTo(AActor * Actor, FQuat Rotation, bool bIsRotationRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	UTween* ActorRotationFrom(AActor * Actor, FQuat Rotation, bool bIsRotationRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	UTween* ActorRelativeRotationTo(AActor * Actor, FQuat Rotation, bool bIsRotationRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	UTween* ActorRelativeRotationFrom(AActor * Actor, FQuat Rotation, bool bIsRotationRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	UTween* WidgetSlotPositionTo(UWidget* Widget, FVector2D Location, bool bIsLocationRelative = false, float Duration = 0.25f,
			EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
			int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	UTween* WidgetSlotPositionFrom(UWidget* Widget, FVector2D Location, bool bIsLocationRelative = false, float Duration = 0.25f,
			EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
			int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	UTween* WidgetRenderLocationTo(UWidget * Widget, FVector2D Location, bool bIsLocationRelative = false, float Duration = 0.25f, 
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	UTween* WidgetRenderLocationFrom(UWidget * Widget, FVector2D Location, bool bIsLocationRelative = false, float Duration = 0.25f, 
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	UTween* WidgetRenderScaleTo(UWidget * Widget, FVector2D Scale = FVector2D(1.f,1.f), bool bIsScaleRelative = false, float Duration = 0.25f, 
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	UTween* WidgetRenderScaleFrom(UWidget * Widget, FVector2D Scale = FVector2D(1.f,1.f), bool bIsScaleRelative = false, float Duration = 0.25f, 
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	UTween* WidgetRenderShearTo(UWidget * Widget, FVector2D Shear, bool bIsShearRelative = false, float Duration = 0.25f, 
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	UTween* WidgetRenderShearFrom(UWidget * Widget, FVector2D Shear, bool bIsShearRelative = false, float Duration = 0.25f, 
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	UTween* WidgetRenderAngleTo(UWidget * Widget, float Angle, bool bIsAngleRelative = false, float Duration = 0.25f, 
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	UTween* WidgetRenderAngleFrom(UWidget * Widget, float Angle, bool bIsAngleRelative = false, float Duration = 0.25f, 
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	UTween* WidgetRenderOpacityTo(UWidget * Widget, float Opacity = 1.f, bool bIsOpacityRelative = false, float Duration = 0.25f, 
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	UTween* WidgetRenderOpacityFrom(UWidget * Widget, float Opacity = 1.f, bool bIsOpacityRelative = false, float Duration = 0.25f, 
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	UTween* WidgetColorTo(UWidget * Widget, FLinearColor Color, bool bIsColorRelative = false, float Duration = 0.25f, 
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	UTween* WidgetColorFrom(UWidget *Widget, const FLinearColor & Color, bool bIsColorRelative = false, float Duration = 0.25f, 
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	UTween* WidgetBackgroundColorTo(UWidget* Widget, FLinearColor Color, bool bIsColorRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	UTween* WidgetBackgroundColorFrom(UWidget* Widget, const FLinearColor& Color, bool bIsColorRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	UTween* WidgetBlurStrengthTo(UWidget* Widget, float BlurStrength = 1.f, bool bIsBlurStrengthRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", AdvancedDisplay = 4), Category = "Tween")
	UTween* WidgetBlurStrengthFrom(UWidget* Widget, float BlurStrength = 0.f, bool bIsBlurStrengthRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject", AdvancedDisplay = 5), Category = "Tween")
	UTween* MaterialColorTo(UMaterialInstanceDynamic * Material, FName MaterialProperty, FLinearColor Color, bool bIsColorRelative = false, float Duration = 0.25f, 
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", AdvancedDisplay = 5), Category = "Tween")
	UTween* MaterialScalarTo(UMaterialInstanceDynamic * Material, FName MaterialProperty, float Scalar, bool bIsScalarRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject", AdvancedDisplay = 5), Category = "Tween")
	UTween* FloatTo(UObject* Object, FName PropertyName, float Value, bool bIsValueRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject", AdvancedDisplay = 5), Category = "Tween")
	UTween* FloatFrom(UObject* Object, FName PropertyName, float Value, bool bIsValueRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject", AdvancedDisplay = 5), Category = "Tween")
	UTween* VectorTo(UObject* Object, FName PropertyName, FVector Vector, bool bIsVectorRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject", AdvancedDisplay = 5), Category = "Tween")
	UTween* VectorFrom(UObject* Object, FName PropertyName, FVector Vector, bool bIsVectorRelative = false, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", AdvancedDisplay = 2), Category = "Tween")
	UTween* CustomAction(UObject* Object, float From = 0.0f, float To = 1.0f, float Duration = 0.25f,
		EEaseType EaseType = EEaseType::QuarticEaseIn, ELoopType LoopType = ELoopType::None,
		int32 Loops = 0, float DelayBetweenLoops = 0.0f, const UObject* WorldContextObject = nullptr);



private:
	

	UPROPERTY()
	TArray<UTween*> ActiveTweens;

	float LastTickGameTimeSeconds;
	
};




