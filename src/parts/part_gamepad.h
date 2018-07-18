/* ########################################################################

   PICsimLab - PIC laboratory simulator

   ########################################################################

   Copyright (c) : 2010-2018  Luis Claudio Gambôa Lopes

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   For e-mail suggestions :  lcgamboa@yahoo.com
   ######################################################################## */

#ifndef PART_GAMEPAD_H
#define	PART_GAMEPAD_H

#include<lxrad.h>
#include"part.h"


class cpart_gamepad:public part
{
   public:
      String GetName(void){return lxT("Gamepad");};
      cpart_gamepad(unsigned x, unsigned y);
      ~cpart_gamepad(void);
      void Draw(void);
      void Process(void);
      String GetPictureFileName(void){return lxT("gamepad.png");};
      String GetInputMapFile(void){return lxT("gamepad_i.map");};
      String GetOutputMapFile(void){return lxT("gamepad_o.map");};
      String GetPropertiesWindowFile(void){return lxT("gamepad.lxrad");};
      void EvMouseButtonPress(uint button, uint x, uint y,uint state);
      void EvMouseButtonRelease(uint button, uint x, uint y,uint state);
      void EvMouseMove(uint button, uint x, uint y,uint state);
      void EvKeyPress(uint key, uint x, uint y,uint mask);
      void EvKeyRelease(uint key, uint x, uint y,uint mask);
      void ConfigurePropertiesWindow(CPWindow *  wprop);
      void ReadPropertiesWindow(void);
      String WritePreferences(void);
      void ReadPreferences(String value);
      unsigned short get_in_id(char * name);
      unsigned short get_out_id(char * name);
      void PropButton (CControl * control, uint button, uint x, uint y, uint state);
    private:
      unsigned char output_pins[8];    
      unsigned char output_value[8];
      unsigned char valuex;
      unsigned char valuey;
      unsigned char active;
      
      unsigned int jr;
};


#endif	/* PART_GAMEPAD_H */

