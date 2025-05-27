// Copyright Muci Yun.


#include "Actor/PointCollectionActor.h"
#include "AbilitySystem/YuraAbilitySystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
APointCollectionActor::APointCollectionActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Point_0 = CreateDefaultSubobject<USceneComponent>("Point_0");
	SetRootComponent(Point_0);
	ImmutablePts.Add(Point_0);

	Point_1 = CreateDefaultSubobject<USceneComponent>("Point_1");
	Point_1->SetupAttachment(Point_0);
	ImmutablePts.Add(Point_1);

	Point_2 = CreateDefaultSubobject<USceneComponent>("Point_2");
	Point_2->SetupAttachment(Point_0);
	ImmutablePts.Add(Point_2);

	Point_3 = CreateDefaultSubobject<USceneComponent>("Point_3");
	Point_3->SetupAttachment(Point_0);
	ImmutablePts.Add(Point_3);

	Point_4 = CreateDefaultSubobject<USceneComponent>("Point_4");
	Point_4->SetupAttachment(Point_0);
	ImmutablePts.Add(Point_4);

	Point_5 = CreateDefaultSubobject<USceneComponent>("Point_5");
	Point_5->SetupAttachment(Point_0);
	ImmutablePts.Add(Point_5);

	Point_6 = CreateDefaultSubobject<USceneComponent>("Point_6");
	Point_6->SetupAttachment(Point_0);
	ImmutablePts.Add(Point_6);

	Point_7 = CreateDefaultSubobject<USceneComponent>("Point_7");
	Point_7->SetupAttachment(Point_0);
	ImmutablePts.Add(Point_7);

	Point_8 = CreateDefaultSubobject<USceneComponent>("Point_8");
	Point_8->SetupAttachment(Point_0);
	ImmutablePts.Add(Point_8);

	Point_9 = CreateDefaultSubobject<USceneComponent>("Point_9");
	Point_9->SetupAttachment(Point_0);
	ImmutablePts.Add(Point_9);

	Point_10 = CreateDefaultSubobject<USceneComponent>("Point_10");
	Point_10->SetupAttachment(Point_0);
	ImmutablePts.Add(Point_10);
		
}

TArray<USceneComponent*> APointCollectionActor::GetGroundPoints(const FVector& GroundLocation, int32 NumToGet, float YawOverride /*= 0.f*/)
{
	checkf(ImmutablePts.Num() >= NumToGet, TEXT("Attempted to access ImmutablePts out of bound!"));

	TArray<USceneComponent*> PointsCopy;
	for (USceneComponent* Point : ImmutablePts)
	{

		if (PointsCopy.Num() >= NumToGet)
		{
			return PointsCopy;
		}

		if (Point != Point_0)
		{
			// 让它绕着中心点，转动偏航角的角度
			// 目的是什么？
			FVector ToPoint = Point->GetComponentLocation() - Point_0->GetComponentLocation();
			ToPoint.RotateAngleAxis(YawOverride, FVector::UpVector);
			Point->SetWorldLocation(Point_0->GetComponentLocation() + ToPoint);
		}

		// 执行LineTrace
		const FVector RaiseLocation = FVector(Point->GetComponentLocation().X, Point->GetComponentLocation().Y, Point->GetComponentLocation().Z + 500);
		const FVector LowerLocation = FVector(Point->GetComponentLocation().X, Point->GetComponentLocation().Y, Point->GetComponentLocation().Z - 500);

		FHitResult HitResult;
		// 让它忽略范围内的角色，这个方法不止针对玩家
		TArray<AActor*> ActorsToIgnore;
		UYuraAbilitySystemLibrary::GetLivePlayersWithinRadius(this, ActorsToIgnore, TArray<AActor*>(), 1500.f, GetActorLocation());

		// 基于Profile的碰撞检测
		FCollisionQueryParams CollisionQueryParms;
		CollisionQueryParms.AddIgnoredActors(ActorsToIgnore);
		GetWorld()->LineTraceSingleByProfile(HitResult, RaiseLocation, LowerLocation, FName("BlockAll"), CollisionQueryParms);
		

		const FVector AdjustLocation = FVector(Point->GetComponentLocation().X, Point->GetComponentLocation().Y,HitResult.ImpactPoint.Z);
		Point->SetWorldLocation(AdjustLocation);
		// 设置它的方向为垂直向上
		Point->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal));


		PointsCopy.Add(Point);

		

	}

	return PointsCopy;
}

void APointCollectionActor::BeginPlay()
{
	Super::BeginPlay();
	
}


