// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/EnemyPatrolRoute.h"
#include "Components/SplineComponent.h"


// Sets default values
AEnemyPatrolRoute::AEnemyPatrolRoute()
{
	PatrolRoute = CreateDefaultSubobject<USplineComponent>("Patrol Route");
}

void AEnemyPatrolRoute::IncrementPatrolRoute()
{
	_PatrolIndex += _Direction;

	if (_PatrolIndex == PatrolRoute->GetSplineLength() - 1)
		_Direction = -1; // move back if reached the end
	else if (_PatrolIndex == 0)
		_Direction = 1; // move forward if reached the start
}

FVector AEnemyPatrolRoute::GetSplinePointAsWorldPosition() const
{
	return PatrolRoute->GetLocationAtSplinePoint(_PatrolIndex, ESplineCoordinateSpace::World);
}
