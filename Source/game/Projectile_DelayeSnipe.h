// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Projectile_DelayeSnipe.generated.h"

UCLASS()
class GAME_API AProjectile_DelayeSnipe : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile_DelayeSnipe();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ������ײ���
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComponent;

	// �������˶����
	UPROPERTY(VisibleAnywhere, Category = Movement)
	UProjectileMovementComponent* ProjectileMovementComponent;

	// ���÷��䷽��
	void FireInDirection(const FVector& ShootDirection);
	// ���÷����ٶ�
	void SetSpeed(const float Speed);

	// ��ʱ���ص�����
	void TimerCallback();
	// ��ʱ�����
	FTimerHandle MemberTimerHandle;
	// ��ʱ����ʱ
	float DelayTime = 2.0f;
	// �ӳٺ���ٶ�
	float SnipeSpeed = 3000.0f;

private:
	// ��ʱ��״̬
	bool timer_state = true;
	
};
