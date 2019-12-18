// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Projectile_Normal.h"
#include "Projectile_DelayeSnipe.h"
#include "Kismet/GameplayStatics.h" //获取当前玩家

// Sets default values
AEnemy::AEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	// 设置计时器重复次数
	RepeatingCallsRemaining = 1000;
	// 定时器：每x秒调用一次TimerCallback
	GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AEnemy::TimerCallback3, 0.1f, true);

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

// 计时重复结束后的出口
void AEnemy::CountdownHasFinished()
{
	++spellcard;//进入下一阶段
	MuzzleOffset = FVector(0, 0, 0);
	if (spellcard == 2)
	{
		RepeatingCallsRemaining = 80;
		GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AEnemy::TimerCallback2, 0.5f, true);
	}
	else if (spellcard == 3) {
		RepeatingCallsRemaining = 20;
		GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AEnemy::TimerCallback3, 1.0f, true);
	}
	else if (spellcard == 4) {
		RepeatingCallsRemaining = 40;
		GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AEnemy::TimerCallback4, 1.5f, true);
	}
	else if (spellcard == 5) {
		RepeatingCallsRemaining = 20;
		GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AEnemy::TimerCallback5, 4.0f, true);
	}
}


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

void AEnemy::TimerCallback2()
{
	// 出口
	if (--RepeatingCallsRemaining <= 0)
	{
		GetWorldTimerManager().ClearTimer(MemberTimerHandle);
		CountdownHasFinished();
	}

	// 发射弹幕
	if (RepeatingCallsRemaining % 4 == 0)
		Fire2_1();
	else
		Fire2_2();
	
}

void AEnemy::TimerCallback3()
{
	// 出口
	if (--RepeatingCallsRemaining <= 0)
	{
		GetWorldTimerManager().ClearTimer(MemberTimerHandle);
		CountdownHasFinished();
	}

	// 发射弹幕
	if (RepeatingCallsRemaining % 4 == 0) Fire3_1();
	Fire3_2();
	
	
}

void AEnemy::TimerCallback4() //1.5s一次
{
	// 出口
	if (--RepeatingCallsRemaining <= 0)
	{
		GetWorldTimerManager().ClearTimer(MemberTimerHandle);
		CountdownHasFinished();
	}

	//发射弹幕
	if (RepeatingCallsRemaining % 5 == 0)
		Fire4_1(); 
	else
		Fire4_2();
}

void AEnemy::TimerCallback5() //2s一次
{
	// 出口
	if (--RepeatingCallsRemaining <= 0)
	{
		GetWorldTimerManager().ClearTimer(MemberTimerHandle);
		CountdownHasFinished();
	}

	//发射弹幕
	Fire5_1();
}



// FVector MuzzleLocation：发射坐标
// FRotator MuzzleRotation：发射方向

// 发射弹幕【改】
void AEnemy::Fire()
{
	// 尝试发射物体
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



// 发射弹幕：二非-六角光玉
void AEnemy::Fire2_1()
{
	if (ProjectileClass)
	{
		// 获取摄像机变换。
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);
		// 将 MuzzleOffset 从摄像机空间变换到世界空间
		MuzzleOffset = FVector(0, FMath::FRandRange(-1000.0f, +1000.0f), FMath::FRandRange(-500.0f, +500.0f));
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CameraRotation;

		// 配置发射参数
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			
			// 发射物的指针
			AProjectile_Normal* Projectile;

			// 自机狙：计算指向角色的向量focus
			APawn* player = UGameplayStatics::GetPlayerPawn(World, 0);
			FVector focus = (player->GetActorLocation() - MuzzleLocation);
			//FVector left = FVector::CrossProduct(focus, FVector(0, 0, 1)); //上向量
			//FVector up = FVector::CrossProduct(left, focus);               //左向量（左手系）
			focus.Normalize();
			//left.Normalize();
			//up.Normalize();

			MuzzleRotation = focus.Rotation();
			// 发射一次
			Projectile = World->SpawnActor<AProjectile_Normal>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}


			MuzzleRotation = focus.Rotation();
			// 发射多个发射物
			for (int i = 0; i < 13; ++i)
			{
				if (i == 6) continue;
				MuzzleRotation.Yaw = focus.Rotation().Yaw - 24.0f + 4.0f * i;
				
				for (int j = 0; j < 3; ++j)
				{
					if (j == 0) MuzzleRotation.Pitch = focus.Rotation().Pitch + 6.0f * (i-6);
					if (j == 1) MuzzleRotation.Pitch = focus.Rotation().Pitch - 6.0f * (i-6);
					if (j == 2)
					{
						MuzzleRotation.Pitch = focus.Rotation().Pitch;
						MuzzleRotation.Yaw = focus.Rotation().Yaw - 48.0f + 8.0f * i;
					}

					Projectile = World->SpawnActor<AProjectile_Normal>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
					if (Projectile)
					{
						// 设置发射物的初始轨道
						FVector LaunchDirection = MuzzleRotation.Vector();
						Projectile->FireInDirection(LaunchDirection);
					}
				}
			}
		}
	}
}

// 发射弹幕：二非-随机粉弹
void AEnemy::Fire2_2()
{
	if (ProjectileClass)
	{
		// 坐标变换
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);
		MuzzleOffset = FVector(0,FMath::FRandRange(-1000.0f,+1000.0f), FMath::FRandRange(-500.0f, +500.0f));
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CameraRotation;


		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
	
			AProjectile_Normal* Projectile;

			// 自机狙：计算指向角色的向量focus
			APawn* player = UGameplayStatics::GetPlayerPawn(World, 0);
			FVector focus = (player->GetActorLocation() - MuzzleLocation);
			focus.Normalize();
			

			// 生成多个发射物
			for (int i = 0; i < 25; ++i)
			{
				MuzzleRotation = FMath::VRandCone(focus, 1.0f).Rotation();

				Projectile = World->SpawnActor<AProjectile_Normal>(Projectile_Pink, MuzzleLocation, MuzzleRotation, SpawnParams);
				if (Projectile)
				{
					// 设置发射物的初始轨道
					FVector LaunchDirection = MuzzleRotation.Vector();
					Projectile->FireInDirection(LaunchDirection);
					// 设置初始速度
					Projectile->SetSpeed(300.0f);
				}
			}
		}
	}
}

// 发射弹幕：生者必灭之理-跟踪光玉
void AEnemy::Fire3_1()
{
	if (ProjectileClass)
	{
		// 获取摄像机变换。
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		// 将 MuzzleOffset 从摄像机空间变换到世界空间。
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CameraRotation;

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			
			// 设置发射参数
			if (count3_1 < 12)
			{
				MuzzleRotation.Yaw -= 90.0f;
				MuzzleRotation.Pitch += 15.0f * (count3_1 - 6);
			}
			else
			{
				MuzzleRotation.Yaw += 90.0f;
				MuzzleRotation.Pitch += 15.0f * (18 - count3_1);
			}
			++count3_1;
			if (count3_1 >= 24) { count3_1 = 0; }


			// 生成发射物
			AProjectile_DelayeSnipe* Projectile;
			Projectile = World->SpawnActor<AProjectile_DelayeSnipe>(Projectile_DelayedTama, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				// 设置定时器延时
				Projectile->DelayTime = 6.0f;
				// 狙击速度
				Projectile->SnipeSpeed = 3000.0f;
				// 设置发射物的初始轨道
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
				// 设置初始速度
				Projectile->SetSpeed(300.0f);
			}
		}
	}
}

// 发射弹幕：生者必灭之理-蝴蝶三旋臂
// (1,sin(theta)cos(alpha),sin(theta)sin(alpha))
void AEnemy::Fire3_2()
{
	if (ProjectileClass)
	{
		// 坐标变换
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CameraRotation;

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;

			AProjectile_Normal* Projectile;

			FVector direction(1,0,0);
			float sina;
			float cosa;
			float sinb;
			float cosb;
			for (int i = 0; i < 3; ++i)
			{
				float theta = FMath::DegreesToRadians(10.0f* i + 30.0f); //圆锥角度
				FMath::SinCos(&sina, &cosa, theta);
				for (int j = 0; j < 3; ++j)
				{
					float alpha = FMath::DegreesToRadians(60.0f * j + 6.0f * count3_2 + 40.0f * i);
					FMath::SinCos(&sinb, &cosb, alpha);
					direction.X = -cosa;
					direction.Y = sina * cosb;
					direction.Z = sina * sinb;
					direction.Normalize();
					MuzzleRotation = direction.Rotation();
					Projectile = World->SpawnActor<AProjectile_Normal>(Projectile_Butterfly, MuzzleLocation, MuzzleRotation, SpawnParams);
					if (Projectile)
					{
						// 设置发射物的初始轨道
						FVector LaunchDirection = MuzzleRotation.Vector();
						Projectile->FireInDirection(LaunchDirection);
						// 设置初始速度
						//Projectile->SetSpeed(1000.0f);
					}
				}
			}
		}
	}
	//if (++count3_2 >= 20) count3_2 = 0;
	++count3_2;
}
// 发射弹幕：埋骨于弘川-固定蝴蝶:6s/次
void AEnemy::Fire4_1()
{
	if (ProjectileClass)
	{
		// 坐标变换
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;

			AProjectile_Normal* Projectile;
			FVector offsets[5] = { FVector(0.0f,1000.0f,500.0f),FVector(0.0f,0.0f,500.0f),FVector(0.0f,-1000.0f,500.0f),FVector(0.0f,500.0f,-500.0f),FVector(0.0f,-500.0f,-500.0f) };
			// 生成多个发射物
			for (int k = 0; k < 5; ++k)
			{
				MuzzleOffset = offsets[k];
				// 将 MuzzleOffset 从摄像机空间变换到世界空间
				FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
				FRotator MuzzleRotation = CameraRotation;
				
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
							// 设置初始轨道
							FVector LaunchDirection = MuzzleRotation.Vector();
							Projectile->FireInDirection(LaunchDirection);
							// 设置初始速度
							Projectile->SetSpeed(800.0f);
						}
					}

				}
			}
			

		}
	}
}

// 发射弹幕：埋骨于弘川-跟踪蝴蝶：1.5s/次*4
void AEnemy::Fire4_2()
{
	if (ProjectileClass)
	{
		// 坐标变换
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CameraRotation;

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;

			MuzzleRotation.Yaw += 90.0f * count4_2;
			MuzzleRotation.Pitch -= 36.0f;
			for (int i = 0; i < 12; ++i)
			{
				MuzzleRotation.Pitch += 6.0f;
				for (int j = 0; j < 8; ++j)
				{
					// 生成发射物
					AProjectile_DelayeSnipe* Projectile;
					Projectile = World->SpawnActor<AProjectile_DelayeSnipe>(Projectile_DelayedButterfly, MuzzleLocation, MuzzleRotation, SpawnParams);
					if (Projectile)
					{
						// 设置定时器延时
						Projectile->DelayTime = 2.0f;
						// 狙击速度
						Projectile->SnipeSpeed = -200.0f * j + 3000.0f;
						// 设置发射物的初始轨道
						FVector LaunchDirection = MuzzleRotation.Vector();
						Projectile->FireInDirection(LaunchDirection);
						// 设置初始速度
						//Projectile->SetSpeed(1000.0f);
					}
				}
			}
		}
	}
	//方向反转
	count4_2 = -count4_2;
}

// 发射弹幕：返魂蝶-球面光玉
void AEnemy::Fire5_1()
{
	if (ProjectileClass)
	{
		// 坐标变换
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CameraRotation;

		//生成发射物
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;

			AProjectile_Normal* Projectile;
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
						// 设置初始速度
						Projectile->SetSpeed(600.0f);
					}

				}
			}
		}
	}
}