// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	// SetMobility를 넣어주지 않고 움직이려 하면 에러가 난다.
	SetMobility(EComponentMobility::Movable);
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();


	if (HasAuthority())
	{
		// 통신 받기 허용
		// 언리얼 엔진에서 액터가 네트워크 상에서 복제될지 여부를 설정하는 함수
		SetReplicates(true);
		SetReplicateMovement(true);
	}

	globalStartLocation = GetActorLocation();
	globalTargetLocation = GetTransform().TransformPosition(targetLocation);
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 트리거 작용이 있을 때만 플랫폼이 작동
	if (activeTriggers < 1) { return; }
	// Authority를 통해 서버 권한인지 확인 후, 계산을 하고 클라이언트들로 그 결과를 복제한다. 
	// Authority() 코드가 있는 쪽은 서버이다. !Authority()는 클라이언트이다. Not on server == client	
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

		// GetSafeNormal과 GetNnsafeNormal의 차이는 GetUnsafeNormal은 숫자가 아주 작을 때 보호하지 않는다.
		// GetSafeNormal : 벡터의 크기가 매우 작거나 0에 가까운 경우(예를 들어, 벡터의 길이가 부동소수점 오차 범위에 있는 경우) 정규화를 수행하는 대신 0 벡터를 반환하거나 예외적으로 처리한다.
		// GetUnsafeNormal : 길이가 거의 0인 벡터를 정규화하면, 부동소수점 연산에 따라 오버플로우 또는 부정확한 결과가 나올 수 있습니다. 이 함수는 그런 상황을 보호하지 않고 그대로 계산한다.
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
