// Created by Andrew Gresyk, see https://hfsm.dev.

#pragma once

#include "GameFramework/GameMode.h"
//#include "GameFramework/GameModeBase.h"

#include "FightGameModeBase.generated.h"

UCLASS()
class FIGHT_API AFightGameModeBase : public AGameMode // Base
{
	GENERATED_BODY()

	// AGameModeBase
	//virtual void PostLogin(APlayerController* InNewPlayer) override final;
	//virtual void Logout(AController* InExiting) override final;
};
