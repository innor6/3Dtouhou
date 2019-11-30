// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Enemy.generated.h"

UCLASS()
class GAME_API AEnemy : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// 定时器
	// 定时器重复次数
	UPROPERTY(EditAnywhere)
	int32 RepeatingCallsRemaining;

	// 回调函数
	void TimerCallback();

	// 定时结束后
	//UFUNCTION(BlueprintNativeEvent)
	void CountdownHasFinished();

	// 定时器句柄
	FTimerHandle MemberTimerHandle;



	// 发射弹幕
	UFUNCTION()
	void Fire();

	// 从摄像机位置的枪口偏移
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

	// 生成的发射物类
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AProjectile_Normal> ProjectileClass;
};
