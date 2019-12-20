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

	// �������λ�õ�ǹ��ƫ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

	// ��������1������
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AProjectile_Normal> ProjectileClass;
	// ��������2��С�۵�
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AProjectile_Normal> Projectile_Pink;
	// ��������3������
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AProjectile_Normal> Projectile_Butterfly;
	// ��������4���ӳٹ���
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AProjectile_DelayeSnipe> Projectile_DelayedTama;
	// ��������5���ӳٺ���
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AProjectile_DelayeSnipe> Projectile_DelayedButterfly;

private:
	// ����ʱ����
	// ��ʱ���ظ�����
	UPROPERTY(EditAnywhere) int32 RepeatingCallsRemaining;

	// �ص�����
	void TimerCallback();
	void TimerCallback2();
	void TimerCallback3();
	void TimerCallback4();
	void TimerCallback5();

	// ��ʱ�����
	FTimerHandle MemberTimerHandle;

	// ��ʱ������
	//UFUNCTION(BlueprintNativeEvent)
	void CountdownHasFinished();

	// ���䵯Ļ
	UFUNCTION() void Fire(); //��ʼ
	UFUNCTION() void Fire2_1(); //����-���ǹ���
	UFUNCTION() void Fire2_2(); //����-����۵�
	UFUNCTION() void Fire3_1(); //���߱���֮��-���ٹ���
	UFUNCTION() void Fire3_2(); //���߱���֮��-����������
	UFUNCTION() void Fire4_1(); //����ں봨-�̶�����
	UFUNCTION() void Fire4_2(); //����ں봨-���ٺ���
	UFUNCTION() void Fire5_1(); //�շ�-�������
	UFUNCTION() void Fire5_2(); //�շ�-����۵�

	// �������
	int spellcard = 1;
	int count3_1 = 6;
	int count3_2 = 0;
	int count4_1 = 0;
	int count4_2 = 1; //���ҷ������������任
};
