// Created by Andrew Gresyk, see https://hfsm.dev.

#define HFSM2_ENABLE_STRUCTURE_REPORT
#define HFSM2_ENABLE_SERIALIZATION
#include "hfsm2/machine.hpp"

namespace FightCharacter
{
using Config = hfsm2::Config
					::ContextT<AFightCharacter&>
					::ManualActivation;

using M = hfsm2::MachineT<Config>;

using Topology = M::PeerRoot<
	Idling,
					 Walking,
					 Punching,
					 Jabbing,
					 Jumping
				 >;

struct EventPunch			{};
struct EventJab				{};
struct EventJump			{};
struct EventFinishedPlaying	{};

struct Idling
	: Topology::State
{
	void enter(PlanControl& control) {
		AFightCharacter& FightCharacter = control.context();

		FightCharacter.GetSprite()->SetFlipbook(FightCharacter.IdleFlipbook);
		FightCharacter.GetSprite()->SetLooping(true);
		FightCharacter.GetSprite()->Play();
	}

	using Topology::State::react;

	void react(const EventPunch&, FullControl& control) {
		control.changeTo<Punching>();
	}

	void react(const EventJab&, FullControl& control) {
		control.changeTo<Jabbing>();
	}

	void react(const EventJump&, FullControl& control) {
		control.changeTo<Jumping>();
	}
};

struct Walking
	: Topology::State
{};

struct ActingBase
	: Topology::State
{
	using Topology::State::react;

	void react(const EventFinishedPlaying&, FullControl& control) {
		control.changeTo<Idling>();
	}
};

struct Punching
	: Topology::StateT<ActingBase>
{
	void enter(PlanControl& control) {
		AFightCharacter& FightCharacter = control.context();

		if (FightCharacter.IsNetMode(NM_Client))
		{
			FightCharacter.Server_Punch();
		}

		FightCharacter.GetSprite()->SetFlipbook(FightCharacter.PunchFlipbook);
		FightCharacter.GetSprite()->SetLooping(false);
		FightCharacter.GetSprite()->Play();
	}
};

struct Jabbing
	: Topology::StateT<ActingBase>
{
	void enter(PlanControl& control) {
		AFightCharacter& FightCharacter = control.context();

		if (FightCharacter.IsNetMode(NM_Client))
		{
			FightCharacter.Server_Jab();
		}

		FightCharacter.GetSprite()->SetFlipbook(FightCharacter.JabFlipbook);
		FightCharacter.GetSprite()->SetLooping(false);
		FightCharacter.GetSprite()->Play();
	}
};

struct Jumping
	: Topology::StateT<ActingBase>
{
	void enter(PlanControl& control) {
		AFightCharacter& FightCharacter = control.context();

		if (FightCharacter.IsNetMode(NM_Client))
		{
			FightCharacter.Server_Jump();
		}

		FightCharacter.GetSprite()->SetFlipbook(FightCharacter.JumpFlipbook);
		FightCharacter.GetSprite()->SetLooping(false);
		FightCharacter.GetSprite()->Play();
	}
};

using FSM			= Topology::Instance;
using SerialBuffer	= FSM::SerialBuffer;

enum : size_t
{
	FSM_SIZE			= sizeof(FSM),
	SERIAL_BUFFER_SIZE	= sizeof(SerialBuffer),
};

	  FSM& GetFSM(		FFightCharacterFsmHost::FSM_STORAGE& InStorage) { return *reinterpret_cast<		 FSM*>(&InStorage			); }
const FSM& GetFSM(const FFightCharacterFsmHost::FSM_STORAGE& InStorage) { return *reinterpret_cast<const FSM*>(&InStorage			); }

	  FSM& GetFSM(		FFightCharacterFsmHost			   & InFsmHost) { return *reinterpret_cast<		 FSM*>(&InFsmHost.FsmStorage); }
const FSM& GetFSM(const FFightCharacterFsmHost			   & InFsmHost) { return *reinterpret_cast<const FSM*>(&InFsmHost.FsmStorage); }

	  SerialBuffer& GetSerialBuffer(	  FFightCharacterFsmHost::BUFFER_STORAGE& InStorage) { return *reinterpret_cast<	  SerialBuffer*>(&InStorage); }
const SerialBuffer& GetSerialBuffer(const FFightCharacterFsmHost::BUFFER_STORAGE& InStorage) { return *reinterpret_cast<const SerialBuffer*>(&InStorage); }
}
