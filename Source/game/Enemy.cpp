// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Projectile_Normal.h"
#include "Projectile_DelayeSnipe.h"
#include "Kismet/GameplayStatics.h" //��ȡ��ǰ���

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

	// ���ü�ʱ���ظ�����
	RepeatingCallsRemaining = 6;
	spellcard = 1;
	// ��ʱ����ÿx�����һ��TimerCallback
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

// ��ʱ�ظ�������ĳ���
void AEnemy::CountdownHasFinished()
{
	++spellcard;//������һ�׶�
	MuzzleOffset = FVector(0, 0, 0);
	if (spellcard == 2)
	{
		RepeatingCallsRemaining = 70;
		GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AEnemy::TimerCallback2, 0.5f, true, 2.0f);
	}
	else if (spellcard == 3) {
		RepeatingCallsRemaining = 380;
		GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AEnemy::TimerCallback3, 0.1f, true, 2.0f);
	}
	else if (spellcard == 4) {
		RepeatingCallsRemaining = 235;//310
		GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AEnemy::TimerCallback4, 0.1f, true, 2.0f);
	}
	else if (spellcard == 5) {
		RepeatingCallsRemaining = 9;
		GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AEnemy::TimerCallback5, 2.0f, true);
	}
}


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

void AEnemy::TimerCallback2()
{
	// ����
	if (--RepeatingCallsRemaining <= 0)
	{
		GetWorldTimerManager().ClearTimer(MemberTimerHandle);
		CountdownHasFinished();
	}

	// ���䵯Ļ
	if (RepeatingCallsRemaining % 4 == 0)
		Fire2_1();
	else
		Fire2_2();
	
}

void AEnemy::TimerCallback3()
{
	// ����
	if (--RepeatingCallsRemaining <= 0)
	{
		GetWorldTimerManager().ClearTimer(MemberTimerHandle);
		CountdownHasFinished();
	}

	// ���䵯Ļ
	if (RepeatingCallsRemaining % 4 == 0) Fire3_1();
	Fire3_2();
	
	
}

void AEnemy::TimerCallback4()
{
	// ����
	if (--RepeatingCallsRemaining <= -1)
	{
		GetWorldTimerManager().ClearTimer(MemberTimerHandle);
		CountdownHasFinished();
	}

	//���䵯Ļ
	if (RepeatingCallsRemaining % 75 < 10 && (RepeatingCallsRemaining % 75) % 2 == 0)
		Fire4_1(); 
	else if (RepeatingCallsRemaining % 15 == 0)
		Fire4_2();
}

void AEnemy::TimerCallback5() //2sһ��
{
	// ����
	if (--RepeatingCallsRemaining <= 0)
	{
		GetWorldTimerManager().ClearTimer(MemberTimerHandle);
		CountdownHasFinished();
	}

	//���䵯Ļ
	if (RepeatingCallsRemaining % 2 == 0)
		Fire5_1();
	else if (RepeatingCallsRemaining % 2 == 1)
		Fire5_2();
}





// ���䵯Ļ
void AEnemy::Fire()
{
	// ���Է�������
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
						// ���ó�ʼ�ٶ�
						Projectile->SetSpeed(800.0f);
					}
				}

			}

		}
	}
}

// ���䵯Ļ������-���ǹ���
void AEnemy::Fire2_1()
{
	if (ProjectileClass)
	{
		// ��ȡ������任��
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);
		// �� MuzzleOffset ��������ռ�任������ռ�
		MuzzleOffset = FVector(0, FMath::FRandRange(-500.0f, +500.0f), FMath::FRandRange(-300.0f, +300.0f));
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CameraRotation;

		// ���÷������
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			
			// �������ָ��
			AProjectile_Normal* Projectile;

			// �Ի��ѣ�����ָ���ɫ������focus
			APawn* player = UGameplayStatics::GetPlayerPawn(World, 0);
			FVector focus = (player->GetActorLocation() - MuzzleLocation);
			//FVector left = FVector::CrossProduct(focus, FVector(0, 0, 1)); //������
			//FVector up = FVector::CrossProduct(left, focus);               //������������ϵ��
			focus.Normalize();
			//left.Normalize();
			//up.Normalize();

			MuzzleRotation = focus.Rotation();
			// ����һ��
			Projectile = World->SpawnActor<AProjectile_Normal>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
				Projectile->SetSpeed(800.0f);
			}


			MuzzleRotation = focus.Rotation();
			// ������������
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
						// ���÷�����ĳ�ʼ���
						FVector LaunchDirection = MuzzleRotation.Vector();
						Projectile->FireInDirection(LaunchDirection);
						// ���ó�ʼ�ٶ�
						Projectile->SetSpeed(800.0f);
					}
				}
			}
		}
	}
}

// ���䵯Ļ������-����۵�
void AEnemy::Fire2_2()
{
	if (Projectile_Pink)
	{
		// ����任
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

			// �Ի��ѣ�����ָ���ɫ������focus
			APawn* player = UGameplayStatics::GetPlayerPawn(World, 0);
			FVector focus = (player->GetActorLocation() - MuzzleLocation);
			focus.Normalize();
			

			// ���ɶ��������
			for (int i = 0; i < 25; ++i)
			{
				MuzzleRotation = FMath::VRandCone(focus, 1.2f).Rotation();

				Projectile = World->SpawnActor<AProjectile_Normal>(Projectile_Pink, MuzzleLocation, MuzzleRotation, SpawnParams);
				if (Projectile)
				{
					// ���÷�����ĳ�ʼ���
					FVector LaunchDirection = MuzzleRotation.Vector();
					Projectile->FireInDirection(LaunchDirection);
					// ���ó�ʼ�ٶ�
					Projectile->SetSpeed(300.0f);
				}
			}
		}
	}
}

// ���䵯Ļ�����߱���֮��-���ٹ���
void AEnemy::Fire3_1()
{
	if (Projectile_DelayedTama)
	{
		// ��ȡ������任��
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		// �� MuzzleOffset ��������ռ�任������ռ䡣
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CameraRotation;

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			
			// ���÷������
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


			// ���ɷ�����
			AProjectile_DelayeSnipe* Projectile;
			Projectile = World->SpawnActor<AProjectile_DelayeSnipe>(Projectile_DelayedTama, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				// ���ö�ʱ����ʱ
				Projectile->DelayTime = 6.0f;
				// �ѻ��ٶ�
				Projectile->SnipeSpeed = 2000.0f;
				// ���÷�����ĳ�ʼ���
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
				// ���ó�ʼ�ٶ�
				Projectile->SetSpeed(300.0f);
			}
		}
	}
}

// ���䵯Ļ�����߱���֮��-���������� (-cosa, sina * cosb, sina * sinb)
void AEnemy::Fire3_2()
{
	if (Projectile_Butterfly)
	{
		// ����任
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
			for (int i = 0; i < 2; ++i)
			{
				float theta = FMath::DegreesToRadians(10.0f* i - 6.0f * count3_2 + 30.0f); //Բ׶�Ƕ�
				FMath::SinCos(&sina, &cosa, theta);
				for (int j = 0; j < 3; ++j)
				{
					float alpha = FMath::DegreesToRadians(120.0f * j + 6.0f * count3_2 + 60.0f * i);
					FMath::SinCos(&sinb, &cosb, alpha);
					direction.X = -cosa;
					direction.Y = sina * cosb;
					direction.Z = sina * sinb;
					direction.Normalize();
					MuzzleRotation = direction.Rotation();
					
					Projectile = World->SpawnActor<AProjectile_Normal>(Projectile_Butterfly, MuzzleLocation, MuzzleRotation, SpawnParams);
					if (Projectile)
					{
						// ���÷�����ĳ�ʼ���
						FVector LaunchDirection = MuzzleRotation.Vector();
						Projectile->FireInDirection(LaunchDirection);
						// ���ó�ʼ�ٶ�
						Projectile->SetSpeed(500.0f);
					}

					// ��������
					direction = -direction;
					MuzzleRotation = direction.Rotation();
					Projectile = World->SpawnActor<AProjectile_Normal>(Projectile_Butterfly, MuzzleLocation, MuzzleRotation, SpawnParams);
					if (Projectile)
					{
						FVector LaunchDirection = MuzzleRotation.Vector();
						Projectile->FireInDirection(LaunchDirection);
						Projectile->SetSpeed(500.0f);
					}
				}
			}
		}
	}
	//if (++count3_2 >= 20) count3_2 = 0;
	++count3_2;
}

// ���䵯Ļ������ں봨-�̶�����:6s/��
void AEnemy::Fire4_1()
{
	if (Projectile_Pink)
	{
		// ����任
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
			FVector offsets[5] = { FVector(0.0f,0.0f,500.0f),FVector(0.0f,1000.0f,500.0f),FVector(0.0f,-1000.0f,500.0f),FVector(0.0f,500.0f,-500.0f),FVector(0.0f,-500.0f,-500.0f) };

			MuzzleOffset = offsets[count4_1];
			if (++count4_1 >= 5) count4_1 = 0;

			// �� MuzzleOffset ��������ռ�任������ռ�
			FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
			FRotator MuzzleRotation = CameraRotation;

			MuzzleRotation.Pitch -= 90.0f;
			for (int i = 0; i < 11; ++i)
			{
				MuzzleRotation.Pitch += 15.0f;
				MuzzleRotation.Yaw += 15.0f;
				for (int j = 0; j < 12; ++j)
				{
					MuzzleRotation.Yaw += 30.0f;
					Projectile = World->SpawnActor<AProjectile_Normal>(Projectile_Pink, MuzzleLocation, MuzzleRotation, SpawnParams);
					if (Projectile)
					{
						// ���ó�ʼ���
						FVector LaunchDirection = MuzzleRotation.Vector();
						Projectile->FireInDirection(LaunchDirection);
						// ���ó�ʼ�ٶ�
						Projectile->SetSpeed(500.0f);
					}
				}
			}



		}
	}
}

// ���䵯Ļ������ں봨-���ٺ�����1.5s/��*4
void AEnemy::Fire4_2()
{
	if (Projectile_DelayedButterfly)
	{
		// ����任
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
					// ���ɷ�����
					AProjectile_DelayeSnipe* Projectile;
					Projectile = World->SpawnActor<AProjectile_DelayeSnipe>(Projectile_DelayedButterfly, MuzzleLocation, MuzzleRotation, SpawnParams);
					if (Projectile)
					{
						// ���ö�ʱ����ʱ
						Projectile->DelayTime = 2.0f;
						// �ѻ��ٶ�
						Projectile->SnipeSpeed = -200.0f * j + 3000.0f;
						// ���÷�����ĳ�ʼ���
						FVector LaunchDirection = MuzzleRotation.Vector();
						Projectile->FireInDirection(LaunchDirection);
						// ���ó�ʼ�ٶ�
						//Projectile->SetSpeed(1000.0f);
					}
				}
			}
		}
	}
	//����ת
	count4_2 = -count4_2;
}

// ���䵯Ļ���շ�-�������
void AEnemy::Fire5_1()
{
	if (ProjectileClass)
	{
		// ����任
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CameraRotation;

		//���ɷ�����
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

						// ���÷�����ĳ�ʼ�����
						FVector LaunchDirection = MuzzleRotation.Vector();
						Projectile->FireInDirection(LaunchDirection);
						// ���ó�ʼ�ٶ�
						Projectile->SetSpeed(600.0f);
					}

				}
			}
		}
	}
}

// ���䵯Ļ���շ�-����۵�
void AEnemy::Fire5_2()
{
	if (Projectile_Pink)
	{
		// ����任
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CameraRotation;

		//���ɷ�����
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;

			AProjectile_Normal* Projectile;
			MuzzleRotation.Pitch -= 90.0f;
			MuzzleRotation.Yaw += 15.0f;//
			for (int i = 0; i < 17; ++i)
			{
				MuzzleRotation.Pitch += 10.0f;
				MuzzleRotation.Yaw += 15.0f;

				for (int j = 0; j < 12; ++j)
				{
					MuzzleRotation.Yaw += 30.0f;

					Projectile = World->SpawnActor<AProjectile_Normal>(Projectile_Pink, MuzzleLocation, MuzzleRotation, SpawnParams);
					if (Projectile)
					{

						// ���÷�����ĳ�ʼ�����
						FVector LaunchDirection = MuzzleRotation.Vector();
						Projectile->FireInDirection(LaunchDirection);
						// ���ó�ʼ�ٶ�
						Projectile->SetSpeed(300.0f);
					}

				}
			}
		}
	}
}