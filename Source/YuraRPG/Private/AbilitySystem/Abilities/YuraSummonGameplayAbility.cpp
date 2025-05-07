// Copyright Muci Yun.


#include "AbilitySystem/Abilities/YuraSummonGameplayAbility.h"

#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UYuraSummonGameplayAbility::GetSummonSpawnLocations()
{
	TArray<FVector> OutLocations;

	// 获取位置和向前向量
	const FVector AvatarLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
	const FVector AvatarForwardVector = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	// 每次循环偏移的角度--均匀
	const float DeltaSpread = SpawnSpread / MinionsNum;
	/** 开始生成一个扇形范围*/
	// 角度--让前向向量绕着Z轴旋转指定角度的一半
	// 这个ZAxisVector和UpVector是不是一样的呢
	const FVector LeftOfSpread = AvatarForwardVector.RotateAngleAxis(-SpawnSpread / 2, FVector::UpVector);
	for (int32 i = 0; i < MinionsNum; ++i)
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
		FVector RandLocation = AvatarLocation + Direction * FMath::RandRange(MinSpawnRadius, MaxSpawnRadius);

		// 处理不在一个水平面和上下方没有地形的情况
		FHitResult LineTraceResult;
		// 上方4m到下方4m范围
		GetWorld()->LineTraceSingleByChannel(LineTraceResult, RandLocation + FVector(0.f, 0.f, 400.f), RandLocation + FVector(0.f, 0.f, -400.f), ECollisionChannel::ECC_Visibility);

		if (LineTraceResult.bBlockingHit)
		{
			// 由命中的情况下才进添加，否则就是空的，无法进行添加--在这种情况下，没法生成指定的数量了
			RandLocation = LineTraceResult.ImpactPoint + FVector(0.f, 0.f, 50.f);
			OutLocations.Add(RandLocation);
		}
	}

	return OutLocations;
}

AActor* UYuraSummonGameplayAbility::SpawnRandMinionAtLocation(const FVector& SpawnLocation)
{
	const TSubclassOf<APawn> MinionClass = GetRandMinionClass();
	if (IsValid(MinionClass))
	{
		return SpawnPointMinionAtLocation(MinionClass, SpawnLocation);
	}
	return nullptr;
}

AActor* UYuraSummonGameplayAbility::SpawnPointMinionAtLocation(const TSubclassOf<APawn> PawnToSpawn, const FVector& SpawnLocation)
{
	const FRotator SpawnRotation = GetAvatarActorFromActorInfo()->GetActorRotation();
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AActor* ActorSpawned = GetWorld()->SpawnActor(PawnToSpawn, &SpawnLocation, &SpawnRotation, SpawnParameters);

	return ActorSpawned;
}

TSubclassOf<APawn> UYuraSummonGameplayAbility::GetRandMinionClass() const
{
	if (MinionClasses.Num() > 0)
	{
		return MinionClasses[FMath::RandRange(0, MinionClasses.Num() - 1)];
	}

	return TSubclassOf<APawn>();
}
