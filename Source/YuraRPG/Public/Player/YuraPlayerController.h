
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
class UDamageFloatingComponent;
class UNiagaraSystem;
class AMagicCircle;

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

	// 在敌人头上显示伤害--Client RPC，只在当前客户端显示
	UFUNCTION(Client, Reliable)
	void ShowDamageText(float DamageFloating, ACharacter* TargetCharacter, bool bDamageBlock, bool bCriticalHit);

	UFUNCTION(BlueprintCallable, Category = "MagicCircle")
	void ShowMagicCircle();

	UFUNCTION(BlueprintCallable, Category = "MagicCircle")
	void HideMagicCircle();

	UFUNCTION(BlueprintCallable, Category = "MagicCircle")
	void SetMagicCircleMaterial(UMaterialInterface* InMagicCircleMat);

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

private:

	void Move(const FInputActionValue& InputActionValue);

	// Shift的按下和抬起
	// 可以不传递参数，如果不需要这个参数的话
	void ShiftPressed();
	void ShiftReleased();

	// 执行调用敌人高亮逻辑
	void CursorTrace();

	// Ability Action
	void AbilityInputTagPressed(FGameplayTag AbilityActionTag);
	void AbilityInputTagReleased(FGameplayTag AbilityActionTag);
	void AbilityInputTagHeld(FGameplayTag AbilityActionTag);

	// 获取AbilitySystemComponent
	UYuraAbilitySystemComponent* GetAbilitySystemComponent();

	// 分帧移动--自动
	void YuraAutoRunning();

	// 让MagicCircl跟随鼠标移动
	void UpdateMacicCircleLocation();

private:
	// 鼠标命中结果
	FHitResult CursorHit;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> YuraContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction;

	// 记录按键是否按下
	bool bShiftDown = false;

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
	float AutoRunAcceptenceRadius = 100.f;

	// 这个组件可以存储一个路径曲线
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> SplineComponent;

	// 用于创建伤害数字组件
	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<UDamageFloatingComponent> DamageFloatingCompClass;

	// 点击位置的箭头
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UNiagaraSystem> ClickNiagaraSystem;

	// Magic Circle
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMagicCircle> MagicCircleClass;

	UPROPERTY()
	TObjectPtr<AMagicCircle> MagicCircle;
};
