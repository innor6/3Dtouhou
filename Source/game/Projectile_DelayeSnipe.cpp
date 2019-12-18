// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile_DelayeSnipe.h"
#include "Kismet/GameplayStatics.h" //获取当前玩家

// Sets default values
AProjectile_DelayeSnipe::AProjectile_DelayeSnipe()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 根组件：使用球体代表简单碰撞
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	//【待添加】设置碰撞通道
	CollisionComponent->InitSphereRadius(15.0f); //碰撞半径
	RootComponent = CollisionComponent;

	// 投射物组件：驱动发射物的运动
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 1000.0f;
	ProjectileMovementComponent->MaxSpeed = 0.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	//ProjectileMovementComponent->bShouldBounce = true;
	//ProjectileMovementComponent->Bounciness = 0.3f;

	// 10 秒后消亡。
	InitialLifeSpan = 10.0f;
}

// Called when the game starts or when spawned
void AProjectile_DelayeSnipe::BeginPlay()
{
	Super::BeginPlay();
	
	// 定时器：DelayTime秒后调用TimerCallback
	timer_state = true;
	GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AProjectile_DelayeSnipe::TimerCallback, DelayTime);
}

// Called every frame
void AProjectile_DelayeSnipe::Tick(float DeltaTime)
{
	//如果在计时器状态，减速
	if (timer_state)
	{
		FVector deltaVelocity = (DeltaTime / DelayTime) * (ProjectileMovementComponent->Velocity);
		ProjectileMovementComponent->Velocity -= deltaVelocity;
	}

	Super::Tick(DeltaTime);

}


// 设置发射方向（速度由ProjectileMovementComponent->InitialSpeed定义）
void AProjectile_DelayeSnipe::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

// 设置发射速度
void AProjectile_DelayeSnipe::SetSpeed(const float Speed)
{
	ProjectileMovementComponent->Velocity *= (Speed / ProjectileMovementComponent->InitialSpeed);
}

// 定时器的callback
void AProjectile_DelayeSnipe::TimerCallback()
{
	// 狙击目标：计算指向角色的向量focus
	APawn* player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	FVector focus = (player->GetActorLocation() - GetActorLocation());
	focus.Normalize();

	// 改变方向、速度
	ProjectileMovementComponent->Velocity = focus * SnipeSpeed;

	// 清空定时器：MemberTimerHandle现在可以复用于其他任意定时器。
	timer_state = false;
	GetWorldTimerManager().ClearTimer(MemberTimerHandle);
}