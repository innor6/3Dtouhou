// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Pawn.h"
#include "MyPlayer.generated.h"



UCLASS()
class GAME_API AMyPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// 组件:球体碰撞
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComponent;
	// 组件：弹簧臂
	UPROPERTY(EditAnywhere)
		USpringArmComponent* OurCameraSpringArm;
	// 组件：摄像机
	UPROPERTY(EditAnywhere)
		UCameraComponent* OurCamera;
	
	// 输入变量
	FVector MovementInput;
	FVector2D CameraInput;
	float ZoomFactor;
	bool bZoomingIn;
	UPROPERTY(EditAnywhere) float BasicSpeed;
	UPROPERTY(EditAnywhere) float Speed;
	bool bSpeedUp;
	// 输入回调函数
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void MoveUpward(float AxisValue);
	void PitchCamera(float AxisValue);
	void YawCamera(float AxisValue);
	void ZoomIn();
	void ZoomOut();
	void SpeedUp();
	void SpeedDown();

	
};
