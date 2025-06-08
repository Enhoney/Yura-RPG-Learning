// Copyright Muci Yun.

#pragma once

#include "CoreMinimal.h"
#include "Character/YuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "YuraCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UNiagaraComponent;
class UPassiveNiagaraComponent;

/**
 * 
 */
UCLASS()
class YURARPG_API AYuraCharacter : public AYuraCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()

public:
	AYuraCharacter();

	// 被Controlelr掌控的时候--服务端在这里初始化ASC
	virtual void PossessedBy(AController* NewController) override;

	// PlayerState网络复制过来的时候
	virtual void OnRep_PlayerState() override;

	/** Combat Interface start*/
	virtual int32 GetCharacterLevel() const override;
	/** Combat Interface end*/

	/** IPlayerInterface start*/
	virtual void AddToExp_Implementation(int32 ExpToAdd) override;
	virtual void AddToPlayerLevel_Implementation(int32 LevelToAdd) override;
	virtual void LevelUp_Implementation() override;
	virtual int32 GetCurrentExp_Implementation() const override;
	virtual int32 FindCurrentLevelByExp_Implementation(int32 CurExp) const override;

	virtual int32 GetAttributePointReward_Implementation(int32 Level) const override;
	virtual int32 GetSpellPointReward_Implementation(int32 Level) const override;

	virtual void AddAttributePoints_Implementation(int32 AttributePointToAdd) override;
	virtual void AddSpellPoints_Implementation(int32 SpellPointToAdd) override;

	virtual int32 GetAttributePoint_Implementation() const override;
	virtual void ConsumeAttributePoint_Implementation(int32 AttributePointToUse) override;
	virtual void ConsumeSpellPoint_Implementation(int32 SpellPointToUse) override;

	virtual void ShowMagicCircle_Implementation() override;
	virtual void HideMagicCircle_Implementation() override;
	virtual void SetMagicCircleMaterial_Implementation(UMaterialInterface* InMagicMaterial) override;

	virtual void SaveProgress_Implementation(const FName& CheckpointTag) override;
	/** IPlayerInterface end*/

protected:
	virtual void OnRep_Stunned() override;

	virtual void OnRep_Burned() override;

	virtual void InitializeDefaultAttributes() const override;

private:

	virtual void InitAbilityActorInfo() override;

	void LoadProgress();

	// 播放升级的粒子特效
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastActivateLevelUpNiagara();

private:

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> Camera;

	// 升级特效组件
	UPROPERTY(VisibleAnywhere, Category = "Level Up")
	TObjectPtr<UNiagaraComponent> LevelingUpNiagaraComp;

	// 被动技能的粒子效果--技能装备上之后显示

	UPROPERTY(VisibleAnywhere, Category = "Passive Ability")
	TObjectPtr<UPassiveNiagaraComponent> HaloOfProtectionNiagara;

	UPROPERTY(VisibleAnywhere, Category = "Passive Ability")
	TObjectPtr<UPassiveNiagaraComponent> HealthSiphonNiagara;

	UPROPERTY(VisibleAnywhere, Category = "Passive Ability")
	TObjectPtr<UPassiveNiagaraComponent> ManaSiphonNiagara;

	// 读档使用的GE--设置主要属性
	UPROPERTY(EditDefaultsOnly, Category = "SaveGame")
	TSubclassOf<UGameplayEffect> LoadAttributeEffectClass;
};
