// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile_Normal.h"


// Sets default values
AProjectile_Normal::AProjectile_Normal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// 根组件：使用球体代表简单碰撞
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	// 设置碰撞通道、碰撞半径
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	CollisionComponent->InitSphereRadius(15.0f);
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
void AProjectile_Normal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile_Normal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 设置发射方向（速度由ProjectileMovementComponent->InitialSpeed定义）
void AProjectile_Normal::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

// 设置发射速度
void AProjectile_Normal::SetSpeed(const float Speed)
{
	ProjectileMovementComponent->Velocity *= (Speed / ProjectileMovementComponent->InitialSpeed);
}