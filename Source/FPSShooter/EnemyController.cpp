// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyController.h"
#include "FPSShooter.h"
#include "FPSShooterGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"


// Sets default values
AEnemyController::AEnemyController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	RootBox->SetGenerateOverlapEvents(true);
	RootBox->OnComponentBeginOverlap.AddDynamic(this,&AEnemyController::OnOverlap);

	//RootComponent = RootBox;

}

// Called when the game starts or when spawned
void AEnemyController::BeginPlay()
{
	Super::BeginPlay();
	
	this->SetLifeSpan(10);

}

// Called every frame
void AEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FVector NewLocation = GetActorLocation();//the position of the enemy
	NewLocation.X += Direction.X* Speed * DeltaTime;// the enemy is going to move in the x axis
	NewLocation.Y += Direction.Y* Speed * DeltaTime;// the enemy is going to move in the y axis
	SetActorLocation(NewLocation);// to make it move

}

void AEnemyController::OnOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

	if (OtherActor == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)) {

		((AFPSShooterGameMode*)GetWorld()->GetAuthGameMode())->OnGameOver();
		//this code pauses the game
		UGameplayStatics::SetGamePaused(GetWorld(), true);

	}
	else {
		if (OtherActor->GetName().Contains("Projectile")) {

			((AFPSShooterGameMode*)GetWorld()->GetAuthGameMode())->IncrementScore();
			OtherActor->Destroy();
			this->Destroy();
		}
	}

}

