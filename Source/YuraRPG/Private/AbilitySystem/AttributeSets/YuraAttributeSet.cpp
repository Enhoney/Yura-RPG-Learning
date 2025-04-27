// Copyright Muci Yun.


#include "AttributeSets/YuraAttributeSet.h"

#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"

#include "YuraGameplayTags.h"
#include "Interaction/CombatInterface.h"

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
				if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(EffectProps.TargetAvatorActor))
				{
					CombatInterface->Die();
				}
			}
		}
		
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


