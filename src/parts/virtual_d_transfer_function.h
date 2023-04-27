/* ########################################################################

   PICSimLab - Programmable IC Simulator Laboratory

   ########################################################################

   Copyright (c) : 2010-2023  Luis Claudio Gambôa Lopes <lcgamboa@yahoo.com>

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

#ifndef PART_DTRANSFERF_H
#define PART_DTRANSFERF_H

#include <lxrad.h>
#include "../lib/part.h"

#define PART_DTRANSFERF_Name "D. Transfer Function"

class cpart_dtfunc : public part {
public:
    lxString GetAboutInfo(void) override { return lxT("L.C. Gamboa \n <lcgamboa@yahoo.com>"); };
    cpart_dtfunc(const unsigned x, const unsigned y, const char* name, const char* type, board* pboard_);
    ~cpart_dtfunc(void);
    void DrawOutput(const unsigned int index) override;
    void PreProcess(void) override;
    void Process(void) override;
    void Reset(void) override;
    void EvKeyPress(uint key, uint mask) override;
    void EvKeyRelease(uint key, uint mask) override;
    void ConfigurePropertiesWindow(CPWindow* WProp) override;
    void ReadPropertiesWindow(CPWindow* WProp) override;
    lxString WritePreferences(void) override;
    void ReadPreferences(lxString value) override;
    unsigned short GetInputId(char* name) override;
    unsigned short GetOutputId(char* name) override;

private:
    unsigned char pins[2];
    int ordern;
    int orderd;
    float num[4];
    float den[4];
    float v[4];
    float sample;
    float in_gain;
    float in_off;
    float out_gain;
    float out_off;
    long unsigned int nsamples;
    lxFont font;
};

#endif /* PART_DTRANSFERF_H */
