// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "FPSShooterGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FPSSHOOTER_API AFPSShooterGameMode : public AGameMode
{
	GENERATED_BODY()

		float MINIMUM_INTERVAL = 0.5f;
		float MAXIMUM_INTERVAL = 2.0f;
		float TIME_TO_MIMNIM_INTERVAL = 30.0f;
	
public:
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;
	
	void IncrementScore();

	void OnGameOver();

	void OnRestart();

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class AEnemyController> EnemyBlueprint;

	float EnemyTimer;
	float GameTimer;

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
	void ChangeMenuWidget(TSubclassOf<UUserWidget>NewWidgetClass);


protected:

	int Score = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
	TSubclassOf<UUserWidget>StartingWidgetClass;
	UPROPERTY()
	UUserWidget*CurrentWidget;

};
