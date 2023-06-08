// Created by Andrew Gresyk, see https://hfsm.dev.

#include "FightCharacter.h"

#include "Fight/UI/FightHUD.h"

#include "Net/UnrealNetwork.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"

//UE_DISABLE_OPTIMIZATION

DECLARE_LOG_CATEGORY_CLASS(LogFightCharacter, Log, All);

#include "FightCharacterFsm.inl"

#pragma region

void FFightCharacterFsmHost::Initialize(AFightCharacter* const InFightCharacter)
{
	FightCharacter = InFightCharacter;

	//hfsm2::StaticPrintConstT<FightCharacter::FSM_SIZE			> dummy;
	static_assert(sizeof(FsmStorage)		  == FightCharacter::FSM_SIZE		   , "");
	new (&FsmStorage		) FightCharacter::FSM{*FightCharacter};

	//hfsm2::StaticPrintConstT<FightCharacter::SERIAL_BUFFER_SIZE> dummy;
	static_assert(sizeof(SerialBufferStorage) == FightCharacter::SERIAL_BUFFER_SIZE, "");
	new (&SerialBufferStorage) FightCharacter::SerialBuffer;
}

void FFightCharacterFsmHost::Terminate()
{
	FightCharacter::GetFSM(FsmStorage).~InstanceT();
}

bool FFightCharacterFsmHost::Identical(const FFightCharacterFsmHost* InOther, uint32 InPortFlags) const
{
	check(IsInitialized());

	if (InOther->IsInitialized() == false)
	{
		UE_LOG(LogFightCharacter, Warning, TEXT("FFightCharacterFsmHost::Identical() // Initialize()"));
		return false;
	}
	else
	{
		const FightCharacter::SerialBuffer& SerialBuffer = FightCharacter::GetSerialBuffer(SerialBufferStorage);

		FightCharacter::SerialBuffer OtherSerialBuffer;
		const FightCharacter::FSM& OtherFsm = FightCharacter::GetFSM(InOther->FsmStorage);
		OtherFsm.save(OtherSerialBuffer);

		if (SerialBuffer == OtherSerialBuffer)
		{
			//UE_LOG(LogFightCharacter, Warning, TEXT("FFightCharacterFsmHost::Identical() // Identical"));
			return true;
		}
		else
		{
			//UE_LOG(LogFightCharacter, Warning, TEXT("FFightCharacterFsmHost::Identical() // Different"));
			return false;
		}
	}
}

bool FFightCharacterFsmHost::NetSerialize(FArchive& InOutArchive, class UPackageMap* InPackageMap, bool& bOutSuccess)
{
	check(FightCharacter);

	if (InOutArchive.IsSaving())
	{
		UE_LOG(LogFightCharacter, Warning, TEXT("FFightCharacterFsmHost::NetSerialize() // IsSaving()"));

		FightCharacter::SerialBuffer& SerialBuffer = FightCharacter::GetSerialBuffer(SerialBufferStorage);

		const FightCharacter::FSM& Fsm = FightCharacter::GetFSM(FsmStorage);
		Fsm.save(SerialBuffer);

		InOutArchive.SerializeBits(&SerialBuffer, SerialBuffer.BIT_CAPACITY);
	}
	else
	{
		UE_LOG(LogFightCharacter, Warning, TEXT("FFightCharacterFsmHost::NetSerialize() // IsLoading()"));

		FightCharacter::SerialBuffer& SerialBuffer = FightCharacter::GetSerialBuffer(SerialBufferStorage);

		InOutArchive.SerializeBits(&SerialBuffer, SerialBuffer.BIT_CAPACITY);

		FightCharacter::FSM& Fsm = FightCharacter::GetFSM(FsmStorage);
		Fsm.load(SerialBuffer);
	}

	bOutSuccess = true;
	return true;
}

#pragma endregion FFightCharacterFsmHost

#pragma region

AFightCharacter::AFightCharacter(const FObjectInitializer& InObjectInitializer)
	: Super{InObjectInitializer}
{
	bReplicates = true;
	//SetNetDormancy(ENetDormancy::DORM_DormantAll);
}

void AFightCharacter::SetupPlayerInputComponent(UInputComponent* InPlayerInputComponent)
{
	Super::SetupPlayerInputComponent(InPlayerInputComponent);

	InPlayerInputComponent->BindAxis("HorizontalMove", this, &AFightCharacter::HorizontalMove);

	InPlayerInputComponent->BindAction("Punch", IE_Pressed, this, &AFightCharacter::InputActionPunch);
	InPlayerInputComponent->BindAction("Jab"  , IE_Pressed, this, &AFightCharacter::InputActionJab	);
	InPlayerInputComponent->BindAction("Jump" , IE_Pressed, this, &AFightCharacter::InputActionJump	);
}

void AFightCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFightCharacter, FsmHost);
}

void AFightCharacter::PostInitProperties()
{
	Super::PostInitProperties();

	FsmHost.Initialize(this);
}

void AFightCharacter::BeginDestroy()
{
	FsmHost.Terminate();

	Super::BeginDestroy();
}

void AFightCharacter::BeginPlay()
{
	Super::BeginPlay();

	PopulateFSMDraw();

	FightCharacter::GetFSM(FsmHost).enter();

	GetSprite()->OnFinishedPlaying.AddUniqueDynamic(this, &AFightCharacter::OnFinishedPlaying);
}

void AFightCharacter::EndPlay(const EEndPlayReason::Type InEndPlayReason)
{
	GetSprite()->OnFinishedPlaying.RemoveAll(this);

	FightCharacter::GetFSM(FsmHost).exit();

	Super::EndPlay(InEndPlayReason);
}

void AFightCharacter::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);

	UpdateFSMDraw();
}

#if WITH_EDITOR

EDataValidationResult AFightCharacter::IsDataValid(TArray<FText>& ValidationErrors)
{
	EDataValidationResult SuperResult = Super::IsDataValid(ValidationErrors);
	EDataValidationResult Result = EDataValidationResult::Valid;

	if (IdleFlipbook == nullptr)
	{
		Result = EDataValidationResult::Invalid;
		ValidationErrors.Add(FText::FromString(TEXT("IdleFlipbook unset.")));
	}

	if (JabFlipbook == nullptr)
	{
		Result = EDataValidationResult::Invalid;
		ValidationErrors.Add(FText::FromString(TEXT("JabFlipbook unset.")));
	}

	if (PunchFlipbook == nullptr)
	{
		Result = EDataValidationResult::Invalid;
		ValidationErrors.Add(FText::FromString(TEXT("PunchFlipbook unset.")));
	}

	if (JumpFlipbook == nullptr)
	{
		Result = EDataValidationResult::Invalid;
		ValidationErrors.Add(FText::FromString(TEXT("JumpFlipbook unset.")));
	}

	return CombineDataValidationResults(SuperResult, Result);
}

#endif

void AFightCharacter::PopulateFSMDraw()
{
	if (APlayerController* PlayerController = GetController<APlayerController>())
	{
		if (AFightHUD* HUD = PlayerController->GetHUD<AFightHUD>())
		{
			uint8 Index = 0;
			for (auto Entry : FightCharacter::GetFSM(FsmHost).structure())
			{
				HUD->SetText(Index++, FText::FromString(FString::Printf(TEXT("%s%S"),
																		Entry.prefix,
																		Entry.name)));
			}
		}
	}
}

void AFightCharacter::UpdateFSMDraw()
{
	if (APlayerController* PlayerController = GetController<APlayerController>())
	{
		if (AFightHUD* HUD = PlayerController->GetHUD<AFightHUD>())
		{
			uint8 Index = 0;
			for (auto Entry : FightCharacter::GetFSM(FsmHost).structure())
			{
				FSlateColor Color{Entry.isActive ? FLinearColor::Red : FLinearColor::Yellow};

				HUD->SetColorAndOpacity(Index++, Color);
			}
		}
	}
}

void AFightCharacter::OnFinishedPlaying()
{
	UE_LOG(LogFightCharacter, Warning, TEXT("AFightCharacter::OnFinishedPlaying()"));

	FightCharacter::GetFSM(FsmHost).react(FightCharacter::EventFinishedPlaying{});
	ForceNetUpdate();
}

void AFightCharacter::HorizontalMove(const float InUnits)
{
	const FVector Direction = FVector::ForwardVector;
	AddMovementInput(Direction, InUnits);
}

void AFightCharacter::InputActionPunch()
{
	FightCharacter::GetFSM(FsmHost).react(FightCharacter::EventPunch{});
}

void AFightCharacter::InputActionJab()
{
	FightCharacter::GetFSM(FsmHost).react(FightCharacter::EventJab{});
}

void AFightCharacter::InputActionJump()
{
	FightCharacter::GetFSM(FsmHost).react(FightCharacter::EventJump{});
}

void AFightCharacter::Server_Punch_Implementation()
{
	FightCharacter::GetFSM(FsmHost).react(FightCharacter::EventPunch{});
	ForceNetUpdate();
}

void AFightCharacter::Server_Jab_Implementation()
{
	FightCharacter::GetFSM(FsmHost).react(FightCharacter::EventJab{});
	ForceNetUpdate();
}

void AFightCharacter::Server_Jump_Implementation()
{
	FightCharacter::GetFSM(FsmHost).react(FightCharacter::EventJump{});
	ForceNetUpdate();
}

#pragma endregion AFightCharacter

//UE_ENABLE_OPTIMIZATION
