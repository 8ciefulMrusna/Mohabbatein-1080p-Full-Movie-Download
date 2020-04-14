/* ########################################################################

   PICsimLab - PIC laboratory simulator

   ########################################################################

   Copyright (c) : 2019-2020  Luis Claudio Gambôa Lopes

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

#ifndef PART_MI2C_24CXXX_H
#define PART_MI2C_24CXXX_H

#include<lxrad.h>
#include"part.h"
#include"../devices/lcd_pcd8544.h"

class cpart_MI2C_24CXXX : public part {
public:

    String GetName(void) {
        return lxT("MEM 24CXXX");
    };
    cpart_MI2C_24CXXX(unsigned x, unsigned y);
    ~cpart_MI2C_24CXXX(void);
    void Draw(void);
    void PreProcess(void);
    void Process(void);

    String GetPictureFileName(void) {
        return lxT("MI2C_24CXXX.png");
    };

    String GetInputMapFile(void) {
        return lxT("MI2C_24CXXX_i.map");
    };

    String GetOutputMapFile(void) {
        return lxT("IC8_o.map");
    };

    String GetPropertiesWindowFile(void) {
        return lxT("MI2C_24CXXX.lxrad");
    };
    void EvMouseButtonPress(uint button, uint x, uint y,uint state);
    void ConfigurePropertiesWindow(CPWindow * wprop);
    void filedialog_EvOnClose(int retId);
    void ReadPropertiesWindow(void);
    String WritePreferences(void);
    void ReadPreferences(String value);
    unsigned short get_in_id(char * name);
    unsigned short get_out_id(char * name);
private:
    unsigned char input_pins[5];
    mi2c_t mi2c;
    int kbits;
    char f_mi2c_name[200];
    char f_mi2c_tmp_name[200];
    FILE * f_mi2c;
};




#endif /* PART_MI2C_24CXXX_H */

