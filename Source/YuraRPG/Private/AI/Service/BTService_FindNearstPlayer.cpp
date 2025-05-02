// Copyright Muci Yun.


#include "AI/Service/BTService_FindNearstPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"

void UBTService_FindNearstPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	// 目标Tag，因为有可能玩家会拥有宠物，宠物也是由AI控制的
	// 这个AIOwner是AIController
	APawn* OwnerPawn = AIOwner->GetPawn();
	const FName TargetTag = 
		OwnerPawn->ActorHasTag(FName("YuraCharacter.Player")) ? FName("YuraCharacter.Enemy") : FName("YuraCharacter.Player");
	// 存储找到的TargetActor
	TArray<AActor*> ActorsWithTag;
	// 根据Tag查找Actor
	UGameplayStatics::GetAllActorsWithTag(OwnerPawn, TargetTag, ActorsWithTag);

	// 找到最近的Actor 
	float Distance = 0.f;
	AActor* TargetActor = UGameplayStatics::FindNearestActor(OwnerPawn->GetActorLocation(), ActorsWithTag, Distance);

	// 写入黑板
	UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowKeySelector, TargetActor);
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetKeySelector, Distance);
}
