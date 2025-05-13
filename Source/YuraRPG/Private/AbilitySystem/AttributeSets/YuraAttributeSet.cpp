// Copyright Muci Yun.


#include "AttributeSets/YuraAttributeSet.h"

#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"

#include "YuraGameplayTags.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Player/YuraPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "YuraAbilitySystemLibrary.h"
#include "YuraLogChannel.h"

UYuraAttributeSet::UYuraAttributeSet()
{
	const FYuraGameplayTags& YuraGameplayTags = FYuraGameplayTags::Get();

	// Primary Attribute
	TagsToAttributes.Add(YuraGameplayTags.Attribute_Primary_Strength,				GetStrengthAttribute);
	TagsToAttributes.Add(YuraGameplayTags.Attribute_Primary_Intelligence,			GetIntelligenceAttribute);
	TagsToAttributes.Add(YuraGameplayTags.Attribute_Primary_Resilience,				GetResilienceAttribute);
	TagsToAttributes.Add(YuraGameplayTags.Attribute_Primary_Vigor,					GetVigorAttribute);

	// Secondary Attribute
	TagsToAttributes.Add(YuraGameplayTags.Attribute_Secondary_Armor,				GetArmorAttribute);
	TagsToAttributes.Add(YuraGameplayTags.Attribute_Secondary_ArmorPenetration,		GetArmorPenetrationAttribute);
	TagsToAttributes.Add(YuraGameplayTags.Attribute_Secondary_BlockChance,			GetBlockChanceAttribute);
	TagsToAttributes.Add(YuraGameplayTags.Attribute_Secondary_CriticalHitChance,	GetCriticalHitChanceAttribute);
	TagsToAttributes.Add(YuraGameplayTags.Attribute_Secondary_CriticalHitDamage,	GetCriticalHitDamageAttribute);
	TagsToAttributes.Add(YuraGameplayTags.Attribute_Secondary_CriticalHitResistance, GetCriticalHitResistanceAttribute);
	TagsToAttributes.Add(YuraGameplayTags.Attribute_Secondary_HealthRegeneration,	GetHealthRegenerationAttribute);
	TagsToAttributes.Add(YuraGameplayTags.Attribute_Secondary_ManaRegeneration,		GetManaRegenerationAttribute);
	TagsToAttributes.Add(YuraGameplayTags.Attribute_Secondary_MaxHealth,			GetMaxHealthAttribute);
	TagsToAttributes.Add(YuraGameplayTags.Attribute_Secondary_MaxMana,				GetMaxManaAttribute);
	// 伤害抗性
	TagsToAttributes.Add(YuraGameplayTags.Attribute_Resilience_Fire,				GetFireResistanceAttribute);
	TagsToAttributes.Add(YuraGameplayTags.Attribute_Resilience_Physic,				GetPhysicResistanceAttribute);
	TagsToAttributes.Add(YuraGameplayTags.Attribute_Resilience_Lighting,			GetLightingResistanceAttribute);
	TagsToAttributes.Add(YuraGameplayTags.Attribute_Resilience_Arcane,				GetArcaneResistanceAttribute);

}

void UYuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 无条件复制并执行通知，所谓的通知就是OnRep_xxx函数回调
	// REPNOTIFY_Always和REPNOTIFY_OnChanged的区别就是，如果我们修改后的值和原来的一样，
	// 那么REPNOTIFY_OnChanged将不会执行通知，而REPNOTIFY_Always会执行通知
	DOREPLIFETIME_CONDITION_NOTIFY(UYuraAttributeSet, Strength,		COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UYuraAttributeSet, Intelligence,	COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UYuraAttributeSet, Resilience,	COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UYuraAttributeSet, Vigor,		COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UYuraAttributeSet, Health,		COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UYuraAttributeSet, Mana,			COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UYuraAttributeSet, MaxHealth,				COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UYuraAttributeSet, MaxMana,					COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UYuraAttributeSet, Armor,					COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UYuraAttributeSet, ArmorPenetration,			COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UYuraAttributeSet, BlockChance,				COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UYuraAttributeSet, CriticalHitChance,		COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UYuraAttributeSet, CriticalHitDamage,		COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UYuraAttributeSet, CriticalHitResistance,	COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UYuraAttributeSet, HealthRegeneration,		COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UYuraAttributeSet, ManaRegeneration,			COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UYuraAttributeSet, FireResistance,			COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UYuraAttributeSet, PhysicResistance,			COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UYuraAttributeSet, LightingResistance,		COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UYuraAttributeSet, ArcaneResistance,			COND_None, REPNOTIFY_Always);
}

void UYuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& OutProps) const
{
	OutProps.GEContectHandle = Data.EffectSpec.GetContext();
	OutProps.SourceASC = OutProps.GEContectHandle.GetInstigatorAbilitySystemComponent();

	if (IsValid(OutProps.SourceASC) && OutProps.SourceASC->AbilityActorInfo.IsValid() && OutProps.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		OutProps.SourceAvatorActor = OutProps.SourceASC->AbilityActorInfo->AvatarActor.Get();
		OutProps.SourceController = OutProps.SourceASC->AbilityActorInfo->PlayerController.Get();

		if (OutProps.SourceController == nullptr && OutProps.SourceAvatorActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(OutProps.SourceAvatorActor))
			{
				OutProps.SourceController = Pawn->GetController();
			}
		}

		if (OutProps.SourceController)
		{
			OutProps.SourceCharacter = Cast<ACharacter>(OutProps.SourceController->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		OutProps.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		OutProps.TargetAvatorActor = Data.Target.AbilityActorInfo->AvatarActor.Get();

		OutProps.TargetCharacter = Cast<ACharacter>(OutProps.TargetAvatorActor);
		OutProps.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OutProps.TargetAvatorActor);

	}

}

void UYuraAttributeSet::ShowDamageText(const float DamageNum, const FEffectProperties& Props, bool bDamageBlock, bool bCriticalHit) const
{
	if (Props.TargetCharacter != Props.SourceCharacter)
	{
		// 玩家攻击敌人
		if (AYuraPlayerController* PlayerController = Cast<AYuraPlayerController>(Props.SourceController))
		{
			PlayerController->ShowDamageText(DamageNum, Props.TargetCharacter, bDamageBlock, bCriticalHit);
			return;
		}
		// 敌人攻击玩家
		if (AYuraPlayerController* PlayerController = Cast<AYuraPlayerController>(Props.TargetController))
		{
			PlayerController->ShowDamageText(DamageNum, Props.TargetCharacter, bDamageBlock, bCriticalHit);
		}
	}
}

void UYuraAttributeSet::SendExpEvent(const FEffectProperties& OutProps) const
{
	// 注意这里的Soruce和Target
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(OutProps.TargetAvatorActor))
	{
		const FGameplayTag& ExpTag = FYuraGameplayTags::Get().Attribute_Meta_IncomingExp;
		FGameplayEventData Payload;
		Payload.EventTag = ExpTag;

		Payload.EventMagnitude = UYuraAbilitySystemLibrary::FindEnemyExpReward(OutProps.SourceAvatorActor, 
			CombatInterface->GetCharacterClass(), CombatInterface->GetCharacterLevel());

		// 造成伤害时，来源就是玩家
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OutProps.SourceAvatorActor, ExpTag, Payload);
	}
}

void UYuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}

	if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Max(0.f, NewValue);
	}

	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}

	if (Attribute == GetMaxManaAttribute())
	{
		NewValue = FMath::Max(0.f, NewValue);
	}
}

void UYuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// 通过EvaluatedData可以拿到被修改的属性
	// 这个FGameplayEffectModCallbackData结构体十分复杂，嵌套层数很多
	// 几乎可以通过它拿到和GE相关的任何信息
	FEffectProperties EffectProps;
	SetEffectProperties(Data, EffectProps);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}

	if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		SetMaxHealth(FMath::Max(0.f, GetMaxHealth()));
	}

	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMana()));
	}

	if (Data.EvaluatedData.Attribute == GetMaxManaAttribute())
	{
		SetMaxMana(FMath::Max(0.f, GetMaxMana()));
	}

	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.f);
		if (LocalIncomingDamage > 0.f)
		{
			const float NewHealth = GetHealth() - LocalIncomingDamage;

			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

			// 是否为致命伤害
			const bool bFatal = (NewHealth <= 0);

			// 激活受击能力
			if (!bFatal)
			{
				FGameplayTagContainer TagContainer;
				// 直接就使用Effect.HitReact这个Tag即可，激活能力不会给ASC授予上面的标签吧
				TagContainer.AddTag(FYuraGameplayTags::Get().Effects_HitReact);
				EffectProps.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			}
			else
			{
				// 死亡
				SendExpEvent(EffectProps);
				if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(EffectProps.TargetAvatorActor))
				{
					// 发起GameplayEvent，向伤害发起者奖励经验
					CombatInterface->Die();
				}
			}

			// 判断是否暴击，是否格挡
			const bool bDamageBlock = UYuraAbilitySystemLibrary::IsDamageBlock(EffectProps.GEContectHandle);
			const bool bCriticalHit = UYuraAbilitySystemLibrary::IsCriticalHit(EffectProps.GEContectHandle);

			// 伤害飘字
			ShowDamageText(LocalIncomingDamage, EffectProps, bDamageBlock, bCriticalHit);
			
		}
		
	}

	if (Data.EvaluatedData.Attribute == GetIncomingExpAttribute())
	{
		// 获取经验
		const float LocalIncomingExp = GetIncomingExp();
		SetIncomingExp(0.f);

		if (EffectProps.SourceCharacter->Implements<UPlayerInterface>() && EffectProps.SourceCharacter->Implements<UCombatInterface>())
		{
			// 判断是否可以升级
			const int32 CurExp = IPlayerInterface::Execute_GetCurrentExp(EffectProps.SourceCharacter);
			const int32 CurLevel = Cast<ICombatInterface>(EffectProps.SourceCharacter)->GetCharacterLevel();
			const int32 NewLevel = IPlayerInterface::Execute_FindCurrentLevelByExp(EffectProps.SourceCharacter, (CurExp + LocalIncomingExp));

			const int32 NumLevelUps = NewLevel - CurLevel;
			if (NumLevelUps > 0)
			{
				// 获得属性点和技能点
				int32 AttributePointsReward = 0;
				int32 SpellPointsReward = 0;
				// 处理跳级的问题
				for (int32 Level = CurLevel + 1; Level <= NewLevel; ++Level)
				{
					AttributePointsReward += IPlayerInterface::Execute_GetAttributePointReward(EffectProps.SourceCharacter, Level);
					SpellPointsReward += IPlayerInterface::Execute_GetSpellPointReward(EffectProps.SourceCharacter, Level);
				}
				// 升级
				IPlayerInterface::Execute_AddToPlayerLevel(EffectProps.SourceCharacter, NumLevelUps);
				// 赋予技能点和属性点
				IPlayerInterface::Execute_AddAttributePoints(EffectProps.SourceCharacter, AttributePointsReward);
				IPlayerInterface::Execute_AddSpellPoints(EffectProps.SourceCharacter, SpellPointsReward);

				// 加满血量和蓝量
				bTopOffHealth = true;
				bTopOffMana = true;

				// 升级--这里主要是播放效果
				IPlayerInterface::Execute_LevelUp(EffectProps.SourceCharacter);

			}

			IPlayerInterface::Execute_AddToExp(EffectProps.SourceCharacter, LocalIncomingExp);
		}
	}

}

void UYuraAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	// 最大血量被修改--在这个项目中，就只有升级的时候才会改动
	if (Attribute == GetMaxHealthAttribute() && bTopOffHealth)
	{
		SetHealth(GetMaxHealth());
		bTopOffHealth = false;
	}

	if (Attribute == GetMaxManaAttribute() && bTopOffMana)
	{
		SetMana(GetMaxMana());
		bTopOffMana = false;
	}
}

void UYuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UYuraAttributeSet, Strength, OldStrength);
}

void UYuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UYuraAttributeSet, Intelligence, OldIntelligence);
}

void UYuraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UYuraAttributeSet, Resilience, OldResilence);
}

void UYuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UYuraAttributeSet, Vigor, OldVigor);
}

void UYuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	/**
	 * 这个宏GAMEPLAYATTRIBUTE_REPNOTIFY的作用是：
	 *
	 * 处理 属性（Attribute）的网络同步（Replication） 和 客户端回调。
	 * 它的核心作用是简化属性复制逻辑，确保服务器和客户端之间的属性同步能够触发自定义逻辑（如UI更新、特效播放等）。
	*/
	GAMEPLAYATTRIBUTE_REPNOTIFY(UYuraAttributeSet, Health, OldHealth);
}


void UYuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UYuraAttributeSet, Mana, OldMana);
}

void UYuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UYuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UYuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UYuraAttributeSet, MaxMana, OldMaxMana);
}

void UYuraAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UYuraAttributeSet, Armor, OldArmor);
}

void UYuraAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UYuraAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UYuraAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UYuraAttributeSet, BlockChance, OldBlockChance);
}

void UYuraAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UYuraAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UYuraAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UYuraAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void UYuraAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UYuraAttributeSet, CriticalHitResistance, OldCriticalHitResistance);
}

void UYuraAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UYuraAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UYuraAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UYuraAttributeSet, ManaRegeneration, OldManaRegeneration);
}

void UYuraAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UYuraAttributeSet, FireResistance, OldFireResistance);
}

void UYuraAttributeSet::OnRep_PhysicResistance(const FGameplayAttributeData& OldPhysicResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UYuraAttributeSet, PhysicResistance, OldPhysicResistance);
}

void UYuraAttributeSet::OnRep_LightingResistance(const FGameplayAttributeData& OldLightingResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UYuraAttributeSet, LightingResistance, OldLightingResistance);
}

void UYuraAttributeSet::OnRep_ArcaneResistance(const FGameplayAttributeData& OldArcaneResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UYuraAttributeSet, ArcaneResistance, OldArcaneResistance);
}


