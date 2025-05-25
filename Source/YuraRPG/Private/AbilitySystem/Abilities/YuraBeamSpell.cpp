// Copyright Muci Yun.


#include "AbilitySystem/Abilities/YuraBeamSpell.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Interaction/CombatInterface.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AbilitySystem/YuraAbilitySystemLibrary.h"

void UYuraBeamSpell::StoreCursorHitInfo(const FHitResult& CursorHitResult)
{
	if (CursorHitResult.bBlockingHit)
	{
		CursorHitLocation = CursorHitResult.Location;

		CursorHitActor = CursorHitResult.GetActor();
	}
	else
	{
		// 如果没有命中目标，就直接取消这个技能，并复制
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}
	
}

void UYuraBeamSpell::StoreOwnerOwnerVariables()
{
	if (CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
		OwnerPlayerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor.Get());
	}
}

void UYuraBeamSpell::FirstTraceTarget(const FVector& BeamTargetLocation)
{
	check(OwnerPlayerCharacter);
	if (OwnerPlayerCharacter->Implements<UCombatInterface>())
	{
		// 首先拿到发射光束的位置
		FVector StartLocation = ICombatInterface::Execute_GetWeaponComponent(OwnerPlayerCharacter)->GetSocketLocation(FName("TipSocket"));

		// 追踪
		TArray<AActor*> IgnoreActors;
		IgnoreActors.Add(OwnerPlayerCharacter);
		FHitResult HitResult;

		UKismetSystemLibrary::SphereTraceSingle(OwnerPlayerCharacter, 
			StartLocation, 
			BeamTargetLocation,
			5.f,
			ETraceTypeQuery::TraceTypeQuery1, 
			false, 
			IgnoreActors,
			EDrawDebugTrace::None,
			HitResult, 
			true);

		if (HitResult.bBlockingHit)
		{
			CursorHitLocation = HitResult.ImpactPoint;
			CursorHitActor = HitResult.GetActor();
		}
	}

	// 绑定死亡回调
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(CursorHitActor))
	{
		// 因为要避免多次绑定，同一个目标只绑定一次这个代理
		if (!CombatInterface->GetOnActorDeathDelegate().IsAlreadyBound(this, &ThisClass::OnPrimaryTargetDeath))
		{
			CombatInterface->GetOnActorDeathDelegate().AddDynamic(this, &ThisClass::OnPrimaryTargetDeath);
		}
	}
}

void UYuraBeamSpell::StoreAdditionalTargets(TArray<AActor*>& OutTargets)
{
	TArray<AActor*> LiveTargetInRadius;
	// 忽略自身和点击的目标--那么在多人游戏中，友方如何拿到呢？
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
	ActorsToIgnore.Add(CursorHitActor);
	
	// 这个半径可以随着技能等级提升而提升，这里我们先写死
	UYuraAbilitySystemLibrary::GetLivePlayersWithinRadius(GetAvatarActorFromActorInfo(), LiveTargetInRadius, ActorsToIgnore, 800.f, CursorHitActor->GetActorLocation());

	// 额外电击的目标数量
	const int32 AdditionalNumToShock = FMath::Min(MaxNumShocked, GetAbilityLevel()) - 1;
	// 找到离目标最近的几个
	UYuraAbilitySystemLibrary::GetClosetActors(AdditionalNumToShock, CursorHitActor->GetActorLocation(), LiveTargetInRadius, OutTargets);

	// 绑定额外目标死亡回调
	for (AActor* AdditionalTargets : OutTargets)
	{
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(AdditionalTargets))
		{
			// 因为要避免多次绑定，同一个目标只绑定一次这个代理
			if (!CombatInterface->GetOnActorDeathDelegate().IsAlreadyBound(this, &ThisClass::OnAdditionalTargetDeath))
			{
				CombatInterface->GetOnActorDeathDelegate().AddDynamic(this, &ThisClass::OnAdditionalTargetDeath);
			}
		}
	}
}

