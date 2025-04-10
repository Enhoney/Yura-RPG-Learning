// Copyright Muci Yun.


#include "AttributeSets/YuraAttributeSet.h"

#include "Net/UnrealNetwork.h"

UYuraAttributeSet::UYuraAttributeSet()
{
	InitHealth(50.f);
	InitMaxHealth(100.f);
	InitMana(150.f);
	InitMaxMana(150.f);
}

void UYuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 无条件复制并执行通知，所谓的通知就是OnRep_xxx函数回调
	// REPNOTIFY_Always和REPNOTIFY_OnChanged的区别就是，如果我们修改后的值和原来的一样，
	// 那么REPNOTIFY_OnChanged将不会执行通知，而REPNOTIFY_Always会执行通知
	DOREPLIFETIME_CONDITION_NOTIFY(UYuraAttributeSet, Health,		COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UYuraAttributeSet, MaxHealth,	COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UYuraAttributeSet, Mana,			COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UYuraAttributeSet, MaxMana,		COND_None, REPNOTIFY_Always);
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

void UYuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UYuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UYuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UYuraAttributeSet, Mana, OldMana);
}

void UYuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UYuraAttributeSet, MaxMana, OldMaxMana);
}
