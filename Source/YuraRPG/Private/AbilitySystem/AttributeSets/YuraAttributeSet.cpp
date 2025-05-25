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
#include "GameplayEffectComponents\TargetTagsGameplayEffectComponent.h"
#include "YuraAbilityTypes.h"

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

void UYuraAttributeSet::ShowDamageText(const float DamageNum, const FEffectProperties& EffectProps, bool bDamageBlock, bool bCriticalHit) const
{
	if (EffectProps.TargetCharacter != EffectProps.SourceCharacter)
	{
		// 玩家攻击敌人
		if (AYuraPlayerController* PlayerController = Cast<AYuraPlayerController>(EffectProps.SourceController))
		{
			PlayerController->ShowDamageText(DamageNum, EffectProps.TargetCharacter, bDamageBlock, bCriticalHit);
			return;
		}
		// 敌人攻击玩家
		if (AYuraPlayerController* PlayerController = Cast<AYuraPlayerController>(EffectProps.TargetController))
		{
			PlayerController->ShowDamageText(DamageNum, EffectProps.TargetCharacter, bDamageBlock, bCriticalHit);
		}
	}
}

void UYuraAttributeSet::SendExpEvent(const FEffectProperties& EffectProps) const
{
	// 注意这里的Soruce和Target
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(EffectProps.TargetAvatorActor))
	{
		const FGameplayTag& ExpTag = FYuraGameplayTags::Get().Attribute_Meta_IncomingExp;
		FGameplayEventData Payload;
		Payload.EventTag = ExpTag;

		Payload.EventMagnitude = UYuraAbilitySystemLibrary::FindEnemyExpReward(EffectProps.SourceAvatorActor,
			CombatInterface->GetCharacterClass(), CombatInterface->GetCharacterLevel());

		// 造成伤害时，来源就是玩家
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(EffectProps.SourceAvatorActor, ExpTag, Payload);
	}
}

void UYuraAttributeSet::HandleIncomingDamage(const FEffectProperties& EffectProps)
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
			// 如果触发击退了
			FVector KnockbackVector = UYuraAbilitySystemLibrary::GetKnockbackVector(EffectProps.GEContectHandle);
			if (!KnockbackVector.IsZero())
			{
				EffectProps.TargetCharacter->LaunchCharacter(KnockbackVector, true, true);
			}
			else
			{
				// 如果是被电击，就不要执行受击能力
				if (EffectProps.TargetCharacter->Implements<UCombatInterface>() &&
					!ICombatInterface::Execute_GetIsBeingShocked(EffectProps.TargetCharacter))
				{
					// 击退和受击不会同时触发
					FGameplayTagContainer TagContainer;
					// 直接就使用Effect.HitReact这个Tag即可，激活能力不会给ASC授予上面的标签吧
					TagContainer.AddTag(FYuraGameplayTags::Get().Effects_HitReact);
					EffectProps.TargetASC->TryActivateAbilitiesByTag(TagContainer);
				}

			}
			
		}
		else
		{
			// 死亡
			SendExpEvent(EffectProps);
			if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(EffectProps.TargetAvatorActor))
			{
				// 发起GameplayEvent，向伤害发起者奖励经验
				CombatInterface->Die(UYuraAbilitySystemLibrary::GetDeathImpulse(EffectProps.GEContectHandle));
			}
		}

		// 判断是否暴击，是否格挡
		const bool bDamageBlock = UYuraAbilitySystemLibrary::IsDamageBlock(EffectProps.GEContectHandle);
		const bool bCriticalHit = UYuraAbilitySystemLibrary::IsCriticalHit(EffectProps.GEContectHandle);

		// 伤害飘字
		ShowDamageText(LocalIncomingDamage, EffectProps, bDamageBlock, bCriticalHit);

		// 如果成功施加负面效果
		if (UYuraAbilitySystemLibrary::IsSuccessfulDebuff(EffectProps.GEContectHandle))
		{
			HandleDebuffApply(EffectProps);
		}

	}
}

void UYuraAttributeSet::HandleIncomingExp(const FEffectProperties& EffectProps)
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
			for (int32 Level = CurLevel; Level < NewLevel; ++Level)
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

void UYuraAttributeSet::HandleDebuffApply(const FEffectProperties& EffectProps)
{
	// 获取DebuffInfo
	const FYuraGameplayTags YuraTags = FYuraGameplayTags::Get();
	const FGameplayTag DamageTypeTag = UYuraAbilitySystemLibrary::GetDamageTypeTag(EffectProps.GEContectHandle);
	const float DebuffBaseDamage = UYuraAbilitySystemLibrary::GetDebuffBaseDamage(EffectProps.GEContectHandle);
	const float DebuffDuration = UYuraAbilitySystemLibrary::GetDebuffDuration(EffectProps.GEContectHandle);
	const float DebuffFrequency = UYuraAbilitySystemLibrary::GetDebuffFrequency(EffectProps.GEContectHandle);
	
	FName DebuffName = FName(FString::Printf(TEXT("DynamicDebuff_%s"), *DamageTypeTag.ToString()));
	// 创建动态GE
	// GetTransientPackage()--获取暂态包--不知道干啥的，注释说是临时存储永远不会保存的对象的，这个位置是Outer
	UGameplayEffect* DebuffEffect = NewObject<UGameplayEffect>(GetTransientPackage(), DebuffName);

	// 配置
	// 持续时间
	DebuffEffect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	DebuffEffect->DurationMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(DebuffDuration));
	// 配置频率
	DebuffEffect->Period = FScalableFloat(DebuffFrequency);
	DebuffEffect->bExecutePeriodicEffectOnApplication = false;	// 施加后不立即执行
	// 让它给目标添加一个DebuffTag，用于知道这个Debuff的起止时间
	// 5.4之后就只能通过Component来给目标添加tag了
	UTargetTagsGameplayEffectComponent& TargetTagsComponent = DebuffEffect->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	FInheritedTagContainer TagContainerMods;
	const FGameplayTag DebuffTag = YuraTags.DamageTypeToDebuff[DamageTypeTag];
	TagContainerMods.AddTag(DebuffTag);
	// 如果是眩晕，就直接禁用玩家的各种行为
	if (DebuffTag.MatchesTag(YuraTags.Debuff_Lighting_Stun))
	{
		TagContainerMods.AddTag(YuraTags.Player_Block_CursorTrace);
		TagContainerMods.AddTag(YuraTags.Player_Block_InputHeld);
		TagContainerMods.AddTag(YuraTags.Player_Block_InputPressed);
		TagContainerMods.AddTag(YuraTags.Player_Block_InputReleased);
	}
	TargetTagsComponent.SetAndApplyTargetTagChanges(TagContainerMods);

	// 堆叠策略--按来源堆叠
	DebuffEffect->StackingType = EGameplayEffectStackingType::AggregateBySource;
	// 每个玩家最多对他施加一层Debuff
	DebuffEffect->StackLimitCount = 1;	
	// 施加成功刷新持续时间
	DebuffEffect->StackDurationRefreshPolicy = EGameplayEffectStackingDurationPolicy::RefreshOnSuccessfulApplication;
	// 施加成功刷新触发实际
	DebuffEffect->StackPeriodResetPolicy = EGameplayEffectStackingPeriodPolicy::NeverReset;

	// 添加一个默认的，并得到它的引用
	FGameplayModifierInfo& DebuffModifierInfo =  DebuffEffect->Modifiers.AddDefaulted_GetRef();
	DebuffModifierInfo.Attribute = UYuraAttributeSet::GetIncomingDamageAttribute();
	DebuffModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(DebuffBaseDamage));
	DebuffModifierInfo.ModifierOp = EGameplayModOp::Additive;

	/** 到这里，Effect就设置好了，接下来是如何施加它*/
	FGameplayEffectContextHandle DebuffEffectHandle = EffectProps.SourceASC->MakeEffectContext();
	DebuffEffectHandle.AddSourceObject(EffectProps.SourceAvatorActor);
	TSharedPtr<FGameplayEffectSpec> MutableDebuffEffectSpec = MakeShareable<FGameplayEffectSpec>(new FGameplayEffectSpec(DebuffEffect, DebuffEffectHandle, 1.f));
	// Ok啊，开始套娃，现在给他加这个是干嘛用的，我们捋一下，按照现在这个GE的配置它的伤害是不走GEEC的，那么它有什么用？
	// 后面会揭晓的（盲猜用于Debuff的独特提示）
	if (MutableDebuffEffectSpec)
	{
		FYuraGameplayEffectContext* YuraDebuffContext = static_cast<FYuraGameplayEffectContext*>(MutableDebuffEffectSpec->GetContext().Get());
		YuraDebuffContext->SetDamageTypeTag(DamageTypeTag);
		// 避免Debuff继续造成Debuff陷入无限循环--虽然默认也是false，这里仅仅是为了提示你而已
		YuraDebuffContext->SetIsSuccessfulDebuff(false);

		// 施加负面效果
		EffectProps.TargetASC->ApplyGameplayEffectSpecToSelf(*MutableDebuffEffectSpec);
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

	// 如果目标已经死亡，就啥也不干
	if (EffectProps.TargetAvatorActor->Implements<UCombatInterface>() &&
		ICombatInterface::Execute_IsDead(EffectProps.TargetAvatorActor))
	{
		return;
	}

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
		HandleIncomingDamage(EffectProps);
		
	}

	if (Data.EvaluatedData.Attribute == GetIncomingExpAttribute())
	{
		HandleIncomingExp(EffectProps);
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


