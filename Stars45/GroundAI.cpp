/*  Starshatter OpenSource Distribution
    Copyright (c) 1997-2004, Destroyer Studios LLC.
    All Rights Reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name "Destroyer Studios" nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.

    SUBSYSTEM:    Stars.exe
    FILE:         GroundAI.cpp
    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Low-Level Artificial Intelligence class for Ground Units
*/

#include "MemDebug.h"
#include "GroundAI.h"
#include "SteerAI.h"
#include "System.h"
#include "Ship.h"
#include "ShipDesign.h"
#include "Shield.h"
#include "Sim.h"
#include "Player.h"
#include "CarrierAI.h"
#include "Contact.h"
#include "Weapon.h"
#include "WeaponGroup.h"

#include "Game.h"
#include "Physical.h"

// +----------------------------------------------------------------------+

GroundAI::GroundAI(SimObject* s)
: ship((Ship*) s), target(0), subtarget(0), exec_time(0), carrier_ai(0)
{
    Sim*  sim         = Sim::GetSim();
    Ship* pship       = sim->GetPlayerShip();
    int   player_team = 1;
    int   ai_level    = 1;

    if (pship)
    player_team = pship->GetIFF();

    Player* player = Player::GetCurrentPlayer();
    if (player) {
        if (ship && ship->GetIFF() && ship->GetIFF() != player_team) {
            ai_level = player->AILevel();
        }
        else if (player->AILevel() == 0) {
            ai_level = 1;
        }
    }

    // evil alien ships are *always* smart:
    if (ship && ship->GetIFF() > 1 && ship->Design()->auto_roll > 1) {
        ai_level = 2;
    }

    if (ship && ship->GetHangar() && ship->GetCommandAILevel() > 0)
    carrier_ai = new(__FILE__,__LINE__) CarrierAI(ship, ai_level);
}


// +--------------------------------------------------------------------+

GroundAI::~GroundAI()
{
    delete carrier_ai;
}

// +--------------------------------------------------------------------+

void
GroundAI::SetTarget(SimObject* targ, System* sub)
{
    if (target != targ) {
        target = targ;
        
        if (target)
        Observe(target);
    }

    subtarget = sub;
}

// +--------------------------------------------------------------------+

bool
GroundAI::Update(SimObject* obj)
{
    if (obj == target) {
        target = 0;
        subtarget = 0;
    }

    return SimObserver::Update(obj);
}

const char*
GroundAI::GetObserverName() const
{
    static char name[64];
    sprintf_s(name, "GroundAI(%s)", ship->Name());
    return name;
}

// +--------------------------------------------------------------------+

void
GroundAI::SelectTarget()
{
    SimObject* potential_target = 0;

    // pick the closest combatant ship with a different IFF code:
    double target_dist = 1.0e15;

    Ship* current_ship_target = 0;

    ListIter<Contact> c_iter = ship->ContactList();
    while (++c_iter) {
        Contact* contact = c_iter.value();
        int      c_iff   = contact->GetIFF(ship);
        Ship*    c_ship  = contact->GetShip();
        Shot*    c_shot  = contact->GetShot();
        bool     rogue   = false;

        if (c_ship)
        rogue = c_ship->IsRogue();

        if (rogue || c_iff > 0 && c_iff != ship->GetIFF() && c_iff < 1000) {
            if (c_ship && !c_ship->InTransition()) {
                // found an enemy, check distance:
                double dist = (ship->Location() - c_ship->Location()).length();

                if (!current_ship_target || (c_ship->Class() <= current_ship_target->Class() &&
                            dist < target_dist)) {
                    current_ship_target = c_ship;
                    target_dist = dist;
                }
            }
        }

        potential_target = current_ship_target;
    }

    SetTarget(potential_target);
}

// +--------------------------------------------------------------------+

int
GroundAI::Type() const
{
    return SteerAI::GROUND;
}

// +--------------------------------------------------------------------+

void
GroundAI::ExecFrame(double secs)
{
    const int exec_period = 1000;

    if ((int) Game::GameTime() - exec_time > exec_period) {
        exec_time = (int) Game::GameTime();
        SelectTarget();
    }

    if (ship) {
        Shield* shield = ship->GetShield();

        if (shield)
        shield->SetPowerLevel(100);

        ListIter<WeaponGroup> iter = ship->Weapons();
        while (++iter) {
            WeaponGroup* group = (WeaponGroup*) iter.value();

            if (group->NumWeapons() > 1 && group->CanTarget(Ship::DROPSHIPS))
            group->SetFiringOrders(Weapon::POINT_DEFENSE);
            else
            group->SetFiringOrders(Weapon::AUTO);

            group->SetTarget((Ship*) target, 0);
        }

        if (carrier_ai)
        carrier_ai->ExecFrame(secs);
    }
}
