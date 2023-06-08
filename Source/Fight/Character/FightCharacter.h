// Created by Andrew Gresyk, see https://hfsm.dev.

#pragma once

#include "PaperCharacter.h"

#include "FightCharacter.generated.h"

#pragma region

class FIGHT_API AFightCharacter;

USTRUCT()
struct FFightCharacterFsmHost
{
	using FSM_STORAGE	 = uint8[304];
	using BUFFER_STORAGE = uint8[  1];

	GENERATED_BODY()

	void Initialize(AFightCharacter* const InFightCharacter);
	void Terminate();

	bool IsInitialized() const { return FightCharacter != nullptr; }

	bool Identical(const FFightCharacterFsmHost* InOther, uint32 InPortFlags) const;
	bool NetSerialize(FArchive& InOutArchive, class UPackageMap* InPackageMap, bool& bOutSuccess);

	AFightCharacter* FightCharacter = nullptr;

	FSM_STORAGE FsmStorage;
	BUFFER_STORAGE SerialBufferStorage;
};

template<>
struct TStructOpsTypeTraits<FFightCharacterFsmHost>
	: public TStructOpsTypeTraitsBase2<FFightCharacterFsmHost>
{
	enum
	{
	//	WithIdenticalViaEquality   = false,	// struct can be compared via its operator==.  This should be mutually exclusive with WithIdentical.
		WithIdentical              = true,	// struct can be compared via an Identical(const T* Other, uint32 PortFlags) function.  This should be mutually exclusive with WithIdenticalViaEquality.

		WithNetSerializer          = true,	// struct has a NetSerialize function for serializing its state to an FArchive used for network replication.
	//	WithNetDeltaSerializer     = false,	// struct has a NetDeltaSerialize function for serializing differences in state from a previous NetSerialize operation.
	//	WithNetSharedSerialization = false,	// struct has a NetSerialize function that does not require the package map to serialize its state.
	};
};

#pragma endregion FFightCharacterFsmHost

#pragma region

namespace FightCharacter
{
struct Idling;
struct Walking;
struct ActingBase;
struct Punching;
struct Jabbing;
struct Jumping;
}

class UPaperFlipbook;

UCLASS()
class FIGHT_API AFightCharacter
	: public APaperCharacter
{
	GENERATED_BODY()

	friend FFightCharacterFsmHost;

	friend FightCharacter::Idling;
	friend FightCharacter::Walking;
	friend FightCharacter::ActingBase;
	friend FightCharacter::Punching;
	friend FightCharacter::Jabbing;
	friend FightCharacter::Jumping;

public:
	AFightCharacter(const FObjectInitializer& InObjectInitializer);

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* InPlayerInputComponent) override final;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override final;

	virtual void PostInitProperties() override final;
	virtual void BeginDestroy() override final;

	virtual void BeginPlay() override final;
	virtual void EndPlay(const EEndPlayReason::Type InEndPlayReason) override final;

	virtual void Tick(float InDeltaTime) override final;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override final;
#endif

private:
	void PopulateFSMDraw();
	void UpdateFSMDraw();

	UFUNCTION()
	void OnFinishedPlaying();

	UFUNCTION()
	void HorizontalMove(const float InUnits);

	UFUNCTION()
	void InputActionPunch();

	UFUNCTION()
	void InputActionJab();

	UFUNCTION()
	void InputActionJump();

	UFUNCTION(Server, Reliable)
	void Server_Punch();

	UFUNCTION(Server, Reliable)
	void Server_Jab();

	UFUNCTION(Server, Reliable)
	void Server_Jump();

private:
	UPROPERTY(EditAnywhere, Category="Fight|Flipbooks")
	UPaperFlipbook* IdleFlipbook;

	UPROPERTY(EditAnywhere, Category="Fight|Flipbooks")
	UPaperFlipbook* JabFlipbook;

	UPROPERTY(EditAnywhere, Category="Fight|Flipbooks")
	UPaperFlipbook* PunchFlipbook;

	UPROPERTY(EditAnywhere, Category="Fight|Flipbooks")
	UPaperFlipbook* JumpFlipbook;

	UPROPERTY(Replicated)
	FFightCharacterFsmHost FsmHost;
};

#pragma endregion AFightCharacter
