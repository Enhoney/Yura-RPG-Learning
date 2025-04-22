
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "YuraPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class UYuraAbilitySystemComponent;
class IEnemyInterface;
class UYuraInputConfig;
class USplineComponent;
struct FGameplayTag;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class YURARPG_API AYuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AYuraPlayerController();

	virtual void PlayerTick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

private:

	void Move(const FInputActionValue& InputActionValue);

	// 执行调用敌人高亮逻辑
	void CursorTrace();

	// Ability Action
	void AbilityInputTagPressed(FGameplayTag AbilityActionTag);
	void AbilityInputTagReleased(FGameplayTag AbilityActionTag);
	void AbilityInputTagHeld(FGameplayTag AbilityActionTag);

	// 获取AbilitySystemComponent
	UYuraAbilitySystemComponent* GetAbilitySystemComponent();

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> YuraContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	// 记录上一帧和这一帧命中的对象
	IEnemyInterface* LastActor =  nullptr;
	IEnemyInterface* ThisActor = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UYuraInputConfig> AbilityInputConfig;

	UPROPERTY()
	TObjectPtr<UYuraAbilitySystemComponent> AbilitySystemComponent;

	/**
	 * Properties for Click to Move
	 */
	// 缓存的鼠标点击位置的世界坐标
	FVector CachedDestinationLocation = FVector::ZeroVector;
	// 鼠标按下到抬起的时间
	float FollowingTime = 0.f;
	// 短按的时间阈值
	float ShortPressThreshould = 0.5f;
	// 是否需要绕路，如果不需要绕路，走的就是直线，不需要改变方向
	bool bAutoRunning;
	// 鼠标位置是否为一个敌人
	bool bTargeting = false;

	// 到达目标点距离阈值
	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptenceRadius = 50.f;

	// 这个组件可以存储一个路径曲线
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> SplineComponent;

};
