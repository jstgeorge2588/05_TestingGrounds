// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/ChooseNextWaypoint.h"
#include "AIController.h"
#include "PatrolRoute.h"
#include "BehaviorTree/BlackboardComponent.h"

 EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{

	 //Get Patrol Points

	 auto AIController = OwnerComp.GetAIOwner();
	 auto ControlledPawn = AIController->GetPawn();
	 auto PatrolRoute = ControlledPawn->FindComponentByClass<UPatrolRoute>();

	 if (!ensure(PatrolRoute)) { return EBTNodeResult::Failed; }

	 auto PatrolPoints = PatrolRoute->GetPatrolPoints();
	 if (PatrolPoints.Num() == 0)
	 {
		 UE_LOG(LogTemp, Warning, TEXT("A guard is missing patrol points"))
			 return EBTNodeResult::Failed;
	 }

	 // Set next waypoint

	auto BlackboardComp = OwnerComp.GetBlackboardComponent();
	auto Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);
	BlackboardComp->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);
	
	//Cycle through index
	auto NextIndex = (Index + 1) % PatrolPoints.Num();
	BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);
	//TODO PROTECT AGAINST EMPTY PATROL ROUTES
	return EBTNodeResult::Succeeded;
}
