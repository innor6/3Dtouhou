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

	// 从摄像机位置的枪口偏移
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

	// 发射物类1：光玉
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AProjectile_Normal> ProjectileClass;
	// 发射物类2：小粉弹
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AProjectile_Normal> Projectile_Pink;
	// 发射物类3：蝴蝶
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AProjectile_Normal> Projectile_Butterfly;
	// 发射物类4：延迟光玉
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AProjectile_DelayeSnipe> Projectile_DelayedTama;
	// 发射物类5：延迟蝴蝶
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AProjectile_DelayeSnipe> Projectile_DelayedButterfly;

private:
	// 【定时器】
	// 定时器重复次数
	UPROPERTY(EditAnywhere) int32 RepeatingCallsRemaining;

	// 回调函数
	void TimerCallback();
	void TimerCallback2();
	void TimerCallback3();
	void TimerCallback4();
	void TimerCallback5();

	// 定时器句柄
	FTimerHandle MemberTimerHandle;

	// 定时结束后
	//UFUNCTION(BlueprintNativeEvent)
	void CountdownHasFinished();

	// 发射弹幕
	UFUNCTION() void Fire(); //初始
	UFUNCTION() void Fire2_1(); //二非-六角光玉
	UFUNCTION() void Fire2_2(); //二非-随机粉弹
	UFUNCTION() void Fire3_1(); //生者必灭之理-跟踪光玉
	UFUNCTION() void Fire3_2(); //生者必灭之理-蝴蝶三旋臂
	UFUNCTION() void Fire4_1(); //埋骨于弘川-固定蝴蝶
	UFUNCTION() void Fire4_2(); //埋骨于弘川-跟踪蝴蝶
	UFUNCTION() void Fire5_1(); //终符-球面光玉
	UFUNCTION() void Fire5_2(); //终符-球面粉弹

	// 发射参数
	int spellcard = 1;
	int count3_1 = 6;
	int count3_2 = 0;
	int count4_1 = 0;
	int count4_2 = 1; //左右发蝴蝶，正负变换
};
