// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile_DelayeSnipe.h"
#include "Kismet/GameplayStatics.h" //��ȡ��ǰ���

// Sets default values
AProjectile_DelayeSnipe::AProjectile_DelayeSnipe()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// �������ʹ������������ײ
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	//������ӡ�������ײͨ��
	CollisionComponent->InitSphereRadius(15.0f); //��ײ�뾶
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
void AProjectile_DelayeSnipe::BeginPlay()
{
	Super::BeginPlay();
	
	// ��ʱ����DelayTime������TimerCallback
	timer_state = true;
	GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AProjectile_DelayeSnipe::TimerCallback, DelayTime);
}

// Called every frame
void AProjectile_DelayeSnipe::Tick(float DeltaTime)
{
	//����ڼ�ʱ��״̬������
	if (timer_state)
	{
		FVector deltaVelocity = (DeltaTime / DelayTime) * (ProjectileMovementComponent->Velocity);
		ProjectileMovementComponent->Velocity -= deltaVelocity;
	}

	Super::Tick(DeltaTime);

}


// ���÷��䷽���ٶ���ProjectileMovementComponent->InitialSpeed���壩
void AProjectile_DelayeSnipe::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

// ���÷����ٶ�
void AProjectile_DelayeSnipe::SetSpeed(const float Speed)
{
	ProjectileMovementComponent->Velocity *= (Speed / ProjectileMovementComponent->InitialSpeed);
}

// ��ʱ����callback
void AProjectile_DelayeSnipe::TimerCallback()
{
	// �ѻ�Ŀ�꣺����ָ���ɫ������focus
	APawn* player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	FVector focus = (player->GetActorLocation() - GetActorLocation());
	focus.Normalize();

	// �ı䷽���ٶ�
	ProjectileMovementComponent->Velocity = focus * SnipeSpeed;

	// ��ն�ʱ����MemberTimerHandle���ڿ��Ը������������ⶨʱ����
	timer_state = false;
	GetWorldTimerManager().ClearTimer(MemberTimerHandle);
}