// Copyright Muci Yun.


#include "YuraEnemy.h"
#include "Components/CapsuleComponent.h"
#include "YuraRPG.h"

AYuraEnemy::AYuraEnemy()
{
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AYuraEnemy::HighlightActor()
{
	// 开启高亮（这个设置和材质有关，不必深究）
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_FOR_HIGHLIGHT_RED);

	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_FOR_HIGHLIGHT_RED);
}

void AYuraEnemy::UnhighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}
