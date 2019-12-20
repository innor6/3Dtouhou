// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile_Normal.h"


// Sets default values
AProjectile_Normal::AProjectile_Normal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// �������ʹ������������ײ
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	// ������ײͨ������ײ�뾶
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	CollisionComponent->InitSphereRadius(15.0f);
	RootComponent = CollisionComponent;

	// Ͷ���������������������˶�
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 1000.0f;
	ProjectileMovementComponent->MaxSpeed = 0.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	//ProjectileMovementComponent->bShouldBounce = true;
	//ProjectileMovementComponent->Bounciness = 0.3f;

	// 10 ���������
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

// ���÷��䷽���ٶ���ProjectileMovementComponent->InitialSpeed���壩
void AProjectile_Normal::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

// ���÷����ٶ�
void AProjectile_Normal::SetSpeed(const float Speed)
{
	ProjectileMovementComponent->Velocity *= (Speed / ProjectileMovementComponent->InitialSpeed);
}