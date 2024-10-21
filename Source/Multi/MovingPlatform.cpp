// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	// SetMobility�� �־����� �ʰ� �����̷� �ϸ� ������ ����.
	SetMobility(EComponentMobility::Movable);
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();


	if (HasAuthority())
	{
		// ��� �ޱ� ���
		// �𸮾� �������� ���Ͱ� ��Ʈ��ũ �󿡼� �������� ���θ� �����ϴ� �Լ�
		SetReplicates(true);
		SetReplicateMovement(true);
	}

	globalStartLocation = GetActorLocation();
	globalTargetLocation = GetTransform().TransformPosition(targetLocation);
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Ʈ���� �ۿ��� ���� ���� �÷����� �۵�
	if (activeTriggers < 1) { return; }
	// Authority�� ���� ���� �������� Ȯ�� ��, ����� �ϰ� Ŭ���̾�Ʈ��� �� ����� �����Ѵ�. 
	// Authority() �ڵ尡 �ִ� ���� �����̴�. !Authority()�� Ŭ���̾�Ʈ�̴�. Not on server == client	
	if (HasAuthority())
	{
		FVector location = GetActorLocation();
		float JourneyLength = (globalTargetLocation - globalStartLocation).Size();
		float JourneyTravelled = (location - globalStartLocation).Size();
		if (JourneyTravelled >= JourneyLength)
		{
			FVector Swap = globalStartLocation;
			globalStartLocation = globalTargetLocation;
			globalTargetLocation = Swap;
		}

		// GetSafeNormal�� GetNnsafeNormal�� ���̴� GetUnsafeNormal�� ���ڰ� ���� ���� �� ��ȣ���� �ʴ´�.
		// GetSafeNormal : ������ ũ�Ⱑ �ſ� �۰ų� 0�� ����� ���(���� ���, ������ ���̰� �ε��Ҽ��� ���� ������ �ִ� ���) ����ȭ�� �����ϴ� ��� 0 ���͸� ��ȯ�ϰų� ���������� ó���Ѵ�.
		// GetUnsafeNormal : ���̰� ���� 0�� ���͸� ����ȭ�ϸ�, �ε��Ҽ��� ���꿡 ���� �����÷ο� �Ǵ� ����Ȯ�� ����� ���� �� �ֽ��ϴ�. �� �Լ��� �׷� ��Ȳ�� ��ȣ���� �ʰ� �״�� ����Ѵ�.
		FVector direction = (globalTargetLocation - globalStartLocation).GetSafeNormal();
		location += speed * DeltaTime * direction;
		SetActorLocation(location);
	}

}

void AMovingPlatform::AddActiveTrigger()
{
	activeTriggers++;
}

void AMovingPlatform::RemoveActiveTrigger()
{
	if (activeTriggers > 0)
	{
		activeTriggers--;
	}

}
