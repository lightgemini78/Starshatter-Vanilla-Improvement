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
    FILE:         MenuView.h
    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    View class for displaying right-click context menus
*/

#ifndef MenuView_h
#define MenuView_h

#include "Types.h"
#include "View.h"
#include "Bitmap.h"
#include "Font.h"
#include "Text.h"

// +--------------------------------------------------------------------+

class Menu;
class MenuItem;

// +--------------------------------------------------------------------+

class MenuView : public View
{
public:
    MenuView(Window* c);
    virtual ~MenuView();

    // Operations:
    virtual void      Refresh();
    virtual void      OnWindowMove();
    virtual void      DoMouseFrame();
    virtual void      DrawMenu();
    virtual void      DrawMenu(int x, int y, Menu* menu);
    virtual int       ProcessMenuItem();
    virtual void      ClearMenuSelection(Menu* menu);

    virtual bool      IsShown()         { return show_menu != 0;   }
    virtual int       GetAction()       { return action;           }
    virtual Menu*     GetMenu()         { return menu;             }
    virtual void      SetMenu(Menu* m)  { menu = m;                }
    virtual MenuItem* GetMenuItem()     { return menu_item;        }

    virtual Color     GetBackColor()          { return back_color; }
    virtual void      SetBackColor(Color c)   { back_color = c;    }
    virtual Color     GetTextColor()          { return text_color; }
    virtual void      SetTextColor(Color c)   { text_color = c;    }

protected:
    int         width, height;

    int         shift_down;
    int         mouse_down;
    int         right_down;
    int         show_menu;
    POINT       right_start;
    POINT       offset;

    int         action;
    Menu*       menu;
    MenuItem*   menu_item;
    MenuItem*   selected;

    Color       back_color;
    Color       text_color;
};

#endif MenuView_h

