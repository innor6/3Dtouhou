// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Projectile_Normal.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true; //如果不需要tick可以为false

	// 设置计时器重复次数
	RepeatingCallsRemaining = 100;

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	// 定时器：每2秒调用一次TimerCallback
	GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AEnemy::TimerCallback, 2.0f, true);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// 定时器的callback
void AEnemy::TimerCallback()
{
	// 调用该函数达到足够次数后
	if (--RepeatingCallsRemaining <= 0)
	{
		// 清空定时器：MemberTimerHandle现在可以复用于其他任意定时器。
		GetWorldTimerManager().ClearTimer(MemberTimerHandle); 
		// 出口
		CountdownHasFinished();
	}
	// 在此进行一些操作
	
	Fire(); //发射弹幕
}

// 计时重复结束后的出口【暂时什么都不做】
void AEnemy::CountdownHasFinished()
{

}

/*
// 发射弹幕
void AEnemy::Fire()
{
	// 尝试发射物体。
	if (ProjectileClass)
	{
		// 获取摄像机变换。
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		// 将 MuzzleOffset 从摄像机空间变换到世界空间。
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CameraRotation;
		// 将准星稍微上抬。
		MuzzleRotation.Pitch += 10.0f;
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			// 在枪口处生成发射物。
			AProjectile_Normal* Projectile = World->SpawnActor<AProjectile_Normal>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				// 设置发射物的初始轨道。
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}
		}
	}
}*/

// 发射弹幕【改】
void AEnemy::Fire()
{
	// 尝试发射物体。
	if (ProjectileClass)
	{
		// 获取摄像机变换。
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		// 将 MuzzleOffset 从摄像机空间变换到世界空间。
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CameraRotation;
		// 将准星稍微上抬。
		// MuzzleRotation.Pitch += 10.0f;
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			// 在枪口处生成发射物。
			AProjectile_Normal* Projectile;

			// 生成多个发射物
			// 修改MuzzleRotation
			MuzzleRotation.Pitch -= 90.0f;
			for (int i = 0; i < 17; ++i)
			{
				MuzzleRotation.Pitch += 10.0f;
				MuzzleRotation.Yaw += 15.0f;

				for (int j = 0; j < 12; ++j)
				{
					MuzzleRotation.Yaw += 30.0f;

					Projectile = World->SpawnActor<AProjectile_Normal>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
					if (Projectile)
					{
						// 设置发射物的初始轨道。
						FVector LaunchDirection = MuzzleRotation.Vector();
						Projectile->FireInDirection(LaunchDirection);
					}
				}

			}

		}
	}
}

