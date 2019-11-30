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


	// ��ʱ��
	// ��ʱ���ظ�����
	UPROPERTY(EditAnywhere)
	int32 RepeatingCallsRemaining;

	// �ص�����
	void TimerCallback();

	// ��ʱ������
	//UFUNCTION(BlueprintNativeEvent)
	void CountdownHasFinished();

	// ��ʱ�����
	FTimerHandle MemberTimerHandle;



	// ���䵯Ļ
	UFUNCTION()
	void Fire();

	// �������λ�õ�ǹ��ƫ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

	// ���ɵķ�������
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AProjectile_Normal> ProjectileClass;
};
