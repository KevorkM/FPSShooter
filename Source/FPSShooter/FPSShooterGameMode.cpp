// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSShooterGameMode.h"
#include "FPSShooter.h"
#include "EnemyController.h"
#include "GameWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"



// Called when the game starts or when spawned
void AFPSShooterGameMode::BeginPlay()
{
	Super::BeginPlay();

	ChangeMenuWidget(StartingWidgetClass);

	((UGameWidget*)CurrentWidget)->Load();

	GetWorld()->GetFirstPlayerController()->InputComponent->BindAction("Restart", IE_Pressed, this, &AFPSShooterGameMode::OnRestart).bExecuteWhenPaused=true;

}

// Called every frame
void AFPSShooterGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	GameTimer += DeltaTime;
	EnemyTimer -= DeltaTime;

	if (EnemyTimer <= 0.0f) {

		float difficultPercentage = FMath::Min(GameTimer / TIME_TO_MIMNIM_INTERVAL, 1.0f);
		EnemyTimer = MAXIMUM_INTERVAL - (MAXIMUM_INTERVAL - MINIMUM_INTERVAL)*difficultPercentage;

		UWorld*World = GetWorld();

		if (World) {

			float distance = 800.0f;
			float RandomAngle = FMath::RandRange(0.0f, 360.0f);

			FVector playerLocation = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();

			FVector enemyLocation = playerLocation;

			enemyLocation.X += FMath::Cos(RandomAngle * 3.14f / 180.0f)*distance;
			enemyLocation.Y += FMath::Sin(RandomAngle * 3.14f / 180.0f)*distance;
			enemyLocation.Z = 220.0f;

			AEnemyController* enemy = World->SpawnActor<AEnemyController>
			(EnemyBlueprint, enemyLocation, FRotator::ZeroRotator);
			enemy->Direction = (playerLocation - enemyLocation).GetSafeNormal();

		}

	}
}

void AFPSShooterGameMode::IncrementScore()
{
	Score += 100;
	((UGameWidget*)CurrentWidget)->SetScore(Score);
}

void AFPSShooterGameMode::OnGameOver()
{
	((UGameWidget*)CurrentWidget)->OnGameOver(Score);

}

void AFPSShooterGameMode::OnRestart()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void AFPSShooterGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (CurrentWidget != nullptr) {

		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;

	}

	if (NewWidgetClass) {

		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
		if (CurrentWidget != nullptr) {
			CurrentWidget->AddToViewport();
		}

	}

}



