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
    FILE:         RadioMessage.h
    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    RadioMessage (radio comms) class declaration
*/

#ifndef RadioMessage_h
#define RadioMessage_h

#include "Types.h"
#include "Geometry.h"
#include "SimObject.h"
#include "Instruction.h"
#include "List.h"
#include "Text.h"

// +--------------------------------------------------------------------+

class Element;
class Ship;
class SimObject;

// +--------------------------------------------------------------------+

class RadioMessage
{
public:
    enum  ACTION 
    {
        NONE        = 0,

        DOCK_WITH   = Instruction::DOCK,
        RTB         = Instruction::RTB,
        QUANTUM_TO  = Instruction::NUM_ACTIONS,
        FARCAST_TO,

        // protocol:
        ACK,
        NACK,

        // target mgt:
        ATTACK,
        ESCORT,
        BRACKET,
        IDENTIFY,

        // combat mgt:
        COVER_ME,
        WEP_FREE,
        WEP_HOLD,
        FORM_UP,       // alias for wep_hold
        SAY_POSITION,

        // sensor mgt:
        LAUNCH_PROBE,
        GO_EMCON1,
        GO_EMCON2,
        GO_EMCON3,

        // formation mgt:
        GO_DIAMOND,
        GO_SPREAD,
        GO_BOX,
        GO_TRAIL,

        // mission mgt:
        MOVE_PATROL,
        SKIP_NAVPOINT,
        RESUME_MISSION,

        // misc announcements:
        CALL_ENGAGING,
        FOX_1,
        FOX_2,
        FOX_3,
        SPLASH_1,
        SPLASH_2,
        SPLASH_3,
        SPLASH_4,
        SPLASH_5,   // target destroyed
        SPLASH_6,   // enemy destroyed
        SPLASH_7,   // confirmed kill
        DISTRESS,
        BREAK_ORBIT,
        MAKE_ORBIT,
        QUANTUM_JUMP,

        // friendly fire:
        WARN_ACCIDENT,
        WARN_TARGETED,
        DECLARE_ROGUE,

        // support:
        PICTURE,
        REQUEST_PICTURE,
        REQUEST_SUPPORT,

        // traffic control:
        CALL_INBOUND,
        CALL_APPROACH,
        CALL_CLEARANCE,
        CALL_FINALS,
        CALL_WAVE_OFF,

        NUM_ACTIONS
    };

    RadioMessage(Ship*    dst, const Ship* sender, int action);
    RadioMessage(Element* dst, const Ship* sender, int action);
    RadioMessage(const RadioMessage& rm);
    virtual ~RadioMessage();

    // accessors:
    static const char*   ActionName(int a);

    const Ship*       Sender()          const { return sender;        }
    Ship*             DestinationShip() const { return dst_ship;      }
    Element*          DestinationElem() const { return dst_elem;      }
    int               Action()          const { return action;        }
    List<SimObject>&  TargetList()            { return target_list;   }
    const Point&      Location()        const { return location;      }
    const Text&       Info()            const { return info;          }
    int               Channel()         const { return channel;       }

    // mutators:
    void              SetDestinationShip(Ship* s)      { dst_ship = s;  }
    void              SetDestinationElem(Element* e)   { dst_elem = e;  }
    void              AddTarget(SimObject* s);
    void              SetLocation(const Point& l)      { location  = l; }
    void              SetInfo(Text msg)                { info = msg;    }
    void              SetChannel(int c)                { channel = c;   }

protected:
    const Ship*       sender;
    Ship*             dst_ship;
    Element*          dst_elem;
    int               action;
    List<SimObject>   target_list;
    Point             location;
    Text              info;
    int               channel;
};

// +--------------------------------------------------------------------+

#endif RadioMessage_h

