// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayer.h"

// Sets default values
AMyPlayer::AMyPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// �����
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	// ��������ɱ�
	OurCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	OurCameraSpringArm->SetupAttachment(RootComponent);
	OurCameraSpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 10.0f), FRotator(0.0f, 0.0f, 0.0f)); //FVector(0.0f, 0.0f, 50.0f), FRotator(-60.0f, 0.0f, 0.0f)
	OurCameraSpringArm->TargetArmLength = 400.f;
	OurCameraSpringArm->bEnableCameraLag = true;
	OurCameraSpringArm->CameraLagSpeed = 8.0f; //���ɱ��ͺ��ٶ�
	// ����������
	OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	OurCamera->SetupAttachment(OurCameraSpringArm, USpringArmComponent::SocketName);
	
	//����Ĭ�����
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	//���ݳ�Ա��ʼ��
	BasicSpeed = 500;
	Speed = BasicSpeed;
}

// Called when the game starts or when spawned
void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// ÿ֡����
void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//��������Ҽ���ťʱ�Ŵ��ɿ�ʱ�ָ�
	/*
	{
		if (bZoomingIn)
		{
			ZoomFactor += DeltaTime / 0.5f;         //Zoom in over half a second
		}
		else
		{
			ZoomFactor -= DeltaTime / 0.25f;        //��С�ķ�֮һ��
		}
		ZoomFactor = FMath::Clamp<float>(ZoomFactor, 0.0f, 1.0f);
		//����ZoomFactor��������FOV��SpringArm����
		OurCamera->FieldOfView = FMath::Lerp<float>(90.0f, 60.0f, ZoomFactor);
		OurCameraSpringArm->TargetArmLength = FMath::Lerp<float>(400.0f, 300.0f, ZoomFactor);
	}*/

	//��������Ҽ���Ϊ�����ƶ����ӽǱ仯���ɿ�ʱ�ָ�
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
		//����ZoomFactor��������FOV��SpringArm����
		OurCamera->FieldOfView = FMath::Lerp<float>(100.0f, 90.0f, ZoomFactor);
		OurCameraSpringArm->TargetArmLength = FMath::Lerp<float>(600.0f, 400.0f, ZoomFactor);
	}

	//��תActor��Yaw��Pitch�����������丽�ӣ���˽�ת�����
	{
		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw += CameraInput.X;
		NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + CameraInput.Y, -80.0f, 80.0f);
		SetActorRotation(NewRotation);
	}

	//��ת�����Pitch
	/*{
		FRotator NewRotation = OurCameraSpringArm->GetComponentRotation();
		NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + CameraInput.Y, -80.0f, 80.0f);
		OurCameraSpringArm->SetWorldRotation(NewRotation);

	}*/

	//���ݡ�MoveX���͡�MoveY���ᴦ���ƶ�
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

// �����뺯���󶨵������¼�
// �������¼�����ʱ�����ûص�����������ֵ����
void AMyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//���ӡ�ZoomIn�����¼�
	//InputComponent->BindAction("RMB", IE_Pressed, this, &AMyPlayer::ZoomIn);
	//InputComponent->BindAction("RMB", IE_Released, this, &AMyPlayer::ZoomOut);
	InputComponent->BindAction("RMB", IE_Pressed, this, &AMyPlayer::SpeedUp);
	InputComponent->BindAction("RMB", IE_Released, this, &AMyPlayer::SpeedDown);

	//�����ĸ������֡����
	InputComponent->BindAxis("MoveForward", this, &AMyPlayer::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AMyPlayer::MoveRight);
	InputComponent->BindAxis("MoveUpward", this, &AMyPlayer::MoveUpward);
	InputComponent->BindAxis("MouseY", this, &AMyPlayer::PitchCamera);
	InputComponent->BindAxis("MouseX", this, &AMyPlayer::YawCamera);

}

// ����ص�����
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
