// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Projectile_Normal.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true; //�������Ҫtick����Ϊfalse

	// ���ü�ʱ���ظ�����
	RepeatingCallsRemaining = 100;

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	// ��ʱ����ÿ2�����һ��TimerCallback
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

// ��ʱ����callback
void AEnemy::TimerCallback()
{
	// ���øú����ﵽ�㹻������
	if (--RepeatingCallsRemaining <= 0)
	{
		// ��ն�ʱ����MemberTimerHandle���ڿ��Ը������������ⶨʱ����
		GetWorldTimerManager().ClearTimer(MemberTimerHandle); 
		// ����
		CountdownHasFinished();
	}
	// �ڴ˽���һЩ����
	
	Fire(); //���䵯Ļ
}

// ��ʱ�ظ�������ĳ��ڡ���ʱʲô��������
void AEnemy::CountdownHasFinished()
{

}

/*
// ���䵯Ļ
void AEnemy::Fire()
{
	// ���Է������塣
	if (ProjectileClass)
	{
		// ��ȡ������任��
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		// �� MuzzleOffset ��������ռ�任������ռ䡣
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CameraRotation;
		// ��׼����΢��̧��
		MuzzleRotation.Pitch += 10.0f;
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			// ��ǹ�ڴ����ɷ����
			AProjectile_Normal* Projectile = World->SpawnActor<AProjectile_Normal>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				// ���÷�����ĳ�ʼ�����
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}
		}
	}
}*/

// ���䵯Ļ���ġ�
void AEnemy::Fire()
{
	// ���Է������塣
	if (ProjectileClass)
	{
		// ��ȡ������任��
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		// �� MuzzleOffset ��������ռ�任������ռ䡣
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CameraRotation;
		// ��׼����΢��̧��
		// MuzzleRotation.Pitch += 10.0f;
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			// ��ǹ�ڴ����ɷ����
			AProjectile_Normal* Projectile;

			// ���ɶ��������
			// �޸�MuzzleRotation
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
						// ���÷�����ĳ�ʼ�����
						FVector LaunchDirection = MuzzleRotation.Vector();
						Projectile->FireInDirection(LaunchDirection);
					}
				}

			}

		}
	}
}

