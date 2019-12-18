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

	// 球体碰撞组件
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComponent;

	// 发射物运动组件
	UPROPERTY(VisibleAnywhere, Category = Movement)
	UProjectileMovementComponent* ProjectileMovementComponent;

	// 设置发射方向
	void FireInDirection(const FVector& ShootDirection);
	// 设置发射速度
	void SetSpeed(const float Speed);

	// 定时器回调函数
	void TimerCallback();
	// 定时器句柄
	FTimerHandle MemberTimerHandle;
	// 定时器延时
	float DelayTime = 2.0f;
	// 延迟后的速度
	float SnipeSpeed = 3000.0f;

private:
	// 定时器状态
	bool timer_state = true;
	
};
