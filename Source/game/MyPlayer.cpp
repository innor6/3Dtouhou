// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayer.h"

// Sets default values
AMyPlayer::AMyPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 根组件
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	// 组件：弹簧臂
	OurCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	OurCameraSpringArm->SetupAttachment(RootComponent);
	OurCameraSpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 10.0f), FRotator(0.0f, 0.0f, 0.0f)); //FVector(0.0f, 0.0f, 50.0f), FRotator(-60.0f, 0.0f, 0.0f)
	OurCameraSpringArm->TargetArmLength = 400.f;
	OurCameraSpringArm->bEnableCameraLag = true;
	OurCameraSpringArm->CameraLagSpeed = 8.0f; //弹簧臂滞后速度
	// 组件：摄像机
	OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	OurCamera->SetupAttachment(OurCameraSpringArm, USpringArmComponent::SocketName);
	
	//控制默认玩家
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	//数据成员初始化
	BasicSpeed = 500;
	Speed = BasicSpeed;
}

// Called when the game starts or when spawned
void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// 每帧调用
void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//按下鼠标右键按钮时放大，松开时恢复
	/*
	{
		if (bZoomingIn)
		{
			ZoomFactor += DeltaTime / 0.5f;         //Zoom in over half a second
		}
		else
		{
			ZoomFactor -= DeltaTime / 0.25f;        //缩小四分之一秒
		}
		ZoomFactor = FMath::Clamp<float>(ZoomFactor, 0.0f, 1.0f);
		//基于ZoomFactor混合相机的FOV和SpringArm长度
		OurCamera->FieldOfView = FMath::Lerp<float>(90.0f, 60.0f, ZoomFactor);
		OurCameraSpringArm->TargetArmLength = FMath::Lerp<float>(400.0f, 300.0f, ZoomFactor);
	}*/

	//按下鼠标右键变为高速移动，视角变化，松开时恢复
	{
		if (bSpeedUp)
		{
			Speed = BasicSpeed*1.5;
			ZoomFactor -= DeltaTime / 0.5f;
		}
		else
		{
			Speed = BasicSpeed;
			ZoomFactor += DeltaTime / 0.25f;
		}
		ZoomFactor = FMath::Clamp<float>(ZoomFactor, 0.0f, 1.0f);
		//基于ZoomFactor混合相机的FOV和SpringArm长度
		OurCamera->FieldOfView = FMath::Lerp<float>(100.0f, 90.0f, ZoomFactor);
		OurCameraSpringArm->TargetArmLength = FMath::Lerp<float>(600.0f, 400.0f, ZoomFactor);
	}

	//旋转Actor的Yaw和Pitch，由于已与其附加，因此将转动相机
	{
		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw += CameraInput.X;
		NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + CameraInput.Y, -80.0f, 80.0f);
		SetActorRotation(NewRotation);
	}

	//旋转相机的Pitch
	/*{
		FRotator NewRotation = OurCameraSpringArm->GetComponentRotation();
		NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + CameraInput.Y, -80.0f, 80.0f);
		OurCameraSpringArm->SetWorldRotation(NewRotation);

	}*/

	//根据“MoveX”和“MoveY”轴处理移动
	{
		if (!MovementInput.IsZero())
		{
			MovementInput = MovementInput.GetSafeNormal() * Speed;
			FVector NewLocation = GetActorLocation();
			NewLocation += GetActorForwardVector() * MovementInput.X * DeltaTime;
			NewLocation += GetActorRightVector() * MovementInput.Y * DeltaTime;
			NewLocation += GetActorUpVector() * MovementInput.Z * DeltaTime;
			SetActorLocation(NewLocation);
		}
	}


}

// 将输入函数绑定到输入事件
// 当输入事件发送时，调用回调函数，并将值传入
void AMyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//连接“ZoomIn”的事件
	//InputComponent->BindAction("RMB", IE_Pressed, this, &AMyPlayer::ZoomIn);
	//InputComponent->BindAction("RMB", IE_Released, this, &AMyPlayer::ZoomOut);
	InputComponent->BindAction("RMB", IE_Pressed, this, &AMyPlayer::SpeedUp);
	InputComponent->BindAction("RMB", IE_Released, this, &AMyPlayer::SpeedDown);

	//连接四个轴的逐帧处理
	InputComponent->BindAxis("MoveForward", this, &AMyPlayer::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AMyPlayer::MoveRight);
	InputComponent->BindAxis("MoveUpward", this, &AMyPlayer::MoveUpward);
	InputComponent->BindAxis("MouseY", this, &AMyPlayer::PitchCamera);
	InputComponent->BindAxis("MouseX", this, &AMyPlayer::YawCamera);

}

// 输入回调函数
void AMyPlayer::MoveForward(float AxisValue)
{
	MovementInput.X = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void AMyPlayer::MoveRight(float AxisValue)
{
	MovementInput.Y = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void AMyPlayer::MoveUpward(float AxisValue)
{
	MovementInput.Z = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void AMyPlayer::PitchCamera(float AxisValue)
{
	CameraInput.Y = AxisValue;
}

void AMyPlayer::YawCamera(float AxisValue)
{
	CameraInput.X = AxisValue;
}

void AMyPlayer::ZoomIn()
{
	bZoomingIn = true;
}

void AMyPlayer::ZoomOut()
{
	bZoomingIn = false;
}

void AMyPlayer::SpeedUp()
{
	bSpeedUp = true;
}

void AMyPlayer::SpeedDown()
{
	bSpeedUp = false;
}
