/* ########################################################################

   PICsimLab - PIC laboratory simulator

   ########################################################################

   Copyright (c) : 2019-2022  Luis Claudio Gambôa Lopes

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

#include "other_IO_74xx573.h"
#include "../oscilloscope.h"
#include "../picsimlab.h"
#include "../spareparts.h"

/* outputs */
enum {
    O_P1,
    O_P2,
    O_P3,
    O_P4,
    O_P5,
    O_P6,
    O_P7,
    O_P8,
    O_P9,
    O_P10,
    O_P11,
    O_P12,
    O_P13,
    O_P14,
    O_P15,
    O_P16,
    O_P17,
    O_P18,
    O_P19,
    O_P20,
    O_IC
};

const char pin_names[20][10] = {"/OE", "D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7", "GND",
                                "LE",  "O7", "O6", "O5", "O4", "O3", "O2", "O1", "O0", "VCC"};

const char pin_values[20][10] = {"GND", {0}, {1},  {2},  {3},  {4},  {5},  {6},  {7},  "GND",
                                 {8},   {9}, {10}, {11}, {12}, {13}, {14}, {15}, {16}, "+5V"};

static PCWProp pcwprop[20] = {
    {PCW_LABEL, "1-/OE,GND"}, {PCW_COMBO, "2-D0"},       {PCW_COMBO, "3-D1"},     {PCW_COMBO, "4-D2"},
    {PCW_COMBO, "5-D3"},      {PCW_COMBO, "6-D4"},       {PCW_COMBO, "7-D5"},     {PCW_COMBO, "8-D6"},
    {PCW_COMBO, "9-D7"},      {PCW_LABEL, "10-GND,GND"}, {PCW_COMBO, "11-LE"},    {PCW_LABEL, "12-O7,NC"},
    {PCW_LABEL, "13-O6,NC"},  {PCW_LABEL, "14-O5,NC"},   {PCW_LABEL, "15-O4,NC"}, {PCW_LABEL, "16-O3,NC"},
    {PCW_LABEL, "17-O2,NC"},  {PCW_LABEL, "18-O1,NC"},   {PCW_LABEL, "19-O0,NC"}, {PCW_LABEL, "20-VCC,+5V"}};

cpart_IO_74xx573::cpart_IO_74xx573(const unsigned x, const unsigned y, const char* name, const char* type)
    : part(x, y, name, type), font(8, lxFONTFAMILY_TELETYPE, lxFONTSTYLE_NORMAL, lxFONTWEIGHT_BOLD) {
    X = x;
    Y = y;
    aways_update = 1;
    ReadMaps();
    Bitmap = NULL;

    LoadImage();

    io_74xx573_init(&lt8);
    io_74xx573_rst(&lt8);

    input_pins[0] = 0;
    input_pins[1] = 0;
    input_pins[2] = 0;
    input_pins[3] = 0;
    input_pins[4] = 0;
    input_pins[5] = 0;
    input_pins[6] = 0;
    input_pins[7] = 0;
    input_pins[8] = 0;

    output_pins[0] = SpareParts.RegisterIOpin(lxT("O7"));
    output_pins[1] = SpareParts.RegisterIOpin(lxT("O6"));
    output_pins[2] = SpareParts.RegisterIOpin(lxT("O5"));
    output_pins[3] = SpareParts.RegisterIOpin(lxT("O4"));
    output_pins[4] = SpareParts.RegisterIOpin(lxT("O3"));
    output_pins[5] = SpareParts.RegisterIOpin(lxT("O2"));
    output_pins[6] = SpareParts.RegisterIOpin(lxT("O1"));
    output_pins[7] = SpareParts.RegisterIOpin(lxT("O0"));

    mcount = 0;
    memset(output_pins_alm, 0, 8 * sizeof(unsigned long));
    _ret = 0;

    SetPCWProperties(pcwprop, 20);
}

cpart_IO_74xx573::~cpart_IO_74xx573(void) {
    for (int i = 0; i < 8; i++)
        SpareParts.UnregisterIOpin(output_pins[i]);
    delete Bitmap;
    canvas.Destroy();
}

void cpart_IO_74xx573::Draw(void) {
    int i;

    Update = 0;

    for (i = 0; i < outputc; i++) {
        if (output[i].update)  // only if need update
        {
            output[i].update = 0;

            if (!Update) {
                canvas.Init(Scale, Scale, Orientation);
                canvas.SetFont(font);
            }
            Update++;  // set to update buffer

            switch (output[i].id) {
                case O_IC:
                    canvas.SetColor(26, 26, 26);
                    canvas.Rectangle(1, output[i].x1, output[i].y1, output[i].x2 - output[i].x1,
                                     output[i].y2 - output[i].y1);
                    canvas.SetFgColor(255, 255, 255);
                    canvas.RotatedText("74XX573", output[i].x1 + 36, output[i].y2 - 15, 0.0);
                    break;
                default:
                    canvas.SetColor(49, 61, 99);
                    canvas.Rectangle(1, output[i].x1, output[i].y1, output[i].x2 - output[i].x1,
                                     output[i].y2 - output[i].y1);

                    canvas.SetFgColor(255, 255, 255);
                    canvas.RotatedText(pin_names[output[i].id - O_P1], output[i].x1, output[i].y2, 90.0);

                    int pinv = pin_values[output[i].id - O_P1][0];
                    if (pinv > 16) {
                        canvas.SetFgColor(155, 155, 155);
                        canvas.RotatedText(pin_values[output[i].id - O_P1], output[i].x1, output[i].y2 - 30, 90.0);
                    } else if (pinv < 9) {
                        if (input_pins[pinv] == 0)
                            canvas.RotatedText("NC", output[i].x1, output[i].y2 - 30, 90.0);
                        else
                            canvas.RotatedText(SpareParts.GetPinName(input_pins[pinv]), output[i].x1, output[i].y2 - 30,
                                               90.0);
                    } else {
                        if (output_pins[pinv - 9] == 0)
                            canvas.RotatedText("NC", output[i].x1, output[i].y2 - 30, 90.0);
                        else
                            canvas.RotatedText(
                                itoa(output_pins[pinv -
                                                 9]) /* + lxT (" ") + SpareParts.GetPinName (output_pins[pinv - 4])*/,
                                output[i].x1, output[i].y2 - 30, 90.0);
                    }
                    break;
            }
        }
    }

    if (Update) {
        canvas.End();
    }
}

unsigned short cpart_IO_74xx573::GetInputId(char* name) {
    printf("Erro input '%s' don't have a valid id! \n", name);
    return -1;
};

unsigned short cpart_IO_74xx573::GetOutputId(char* name) {
    if (strcmp(name, "PN_1") == 0)
        return O_P1;
    if (strcmp(name, "PN_2") == 0)
        return O_P2;
    if (strcmp(name, "PN_3") == 0)
        return O_P3;
    if (strcmp(name, "PN_4") == 0)
        return O_P4;
    if (strcmp(name, "PN_5") == 0)
        return O_P5;
    if (strcmp(name, "PN_6") == 0)
        return O_P6;
    if (strcmp(name, "PN_7") == 0)
        return O_P7;
    if (strcmp(name, "PN_8") == 0)
        return O_P8;
    if (strcmp(name, "PN_9") == 0)
        return O_P9;
    if (strcmp(name, "PN_10") == 0)
        return O_P10;
    if (strcmp(name, "PN_11") == 0)
        return O_P11;
    if (strcmp(name, "PN_12") == 0)
        return O_P12;
    if (strcmp(name, "PN_13") == 0)
        return O_P13;
    if (strcmp(name, "PN_14") == 0)
        return O_P14;
    if (strcmp(name, "PN_15") == 0)
        return O_P15;
    if (strcmp(name, "PN_16") == 0)
        return O_P16;
    if (strcmp(name, "PN_17") == 0)
        return O_P17;
    if (strcmp(name, "PN_18") == 0)
        return O_P18;
    if (strcmp(name, "PN_19") == 0)
        return O_P19;
    if (strcmp(name, "PN_20") == 0)
        return O_P20;
    if (strcmp(name, "IC_20") == 0)
        return O_IC;

    printf("Erro output '%s' don't have a valid id! \n", name);
    return 1;
};

lxString cpart_IO_74xx573::WritePreferences(void) {
    char prefs[256];

    sprintf(prefs, "%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu", input_pins[0], input_pins[1], input_pins[2],
            input_pins[3], input_pins[4], input_pins[5], input_pins[6], input_pins[7], input_pins[8], output_pins[0]);

    return prefs;
}

void cpart_IO_74xx573::ReadPreferences(lxString value) {
    unsigned char outp;
    sscanf(value.c_str(), "%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu", &input_pins[0], &input_pins[1],
           &input_pins[2], &input_pins[3], &input_pins[4], &input_pins[5], &input_pins[6], &input_pins[7],
           &input_pins[8], &outp);

    if (output_pins[0] != outp) {
        for (int i = 0; i < 8; i++)
            SpareParts.UnregisterIOpin(output_pins[i]);

        output_pins[0] = SpareParts.RegisterIOpin(lxT("O7"), outp++);
        output_pins[1] = SpareParts.RegisterIOpin(lxT("O6"), outp++);
        output_pins[2] = SpareParts.RegisterIOpin(lxT("O5"), outp++);
        output_pins[3] = SpareParts.RegisterIOpin(lxT("O4"), outp++);
        output_pins[4] = SpareParts.RegisterIOpin(lxT("O3"), outp++);
        output_pins[5] = SpareParts.RegisterIOpin(lxT("O2"), outp++);
        output_pins[6] = SpareParts.RegisterIOpin(lxT("O1"), outp++);
        output_pins[7] = SpareParts.RegisterIOpin(lxT("O0"), outp++);
    }

    Reset();
}

void cpart_IO_74xx573::ConfigurePropertiesWindow(CPWindow* WProp) {
    lxString spin;

    for (int i = 0; i < 20; i++) {
        lxString value = "";

        int pinv = pin_values[i][0];
        if (pinv > 16) {
            value = lxString(pin_values[i]);
        } else if (pinv > 8) {
            if (output_pins[pinv - 9] == 0)
                value = "NC";
            else
                value = itoa(output_pins[pinv - 9]);  // + lxT (" ") + SpareParts.GetPinName (output_pins[pinv - 4]);
        }

        ((CLabel*)WProp->GetChildByName("label" + itoa(i + 1)))->SetText(itoa(i + 1) + lxT("-") + pin_names[i]);
        CLabel* label = (CLabel*)WProp->GetChildByName("label_" + itoa(i + 1));
        if (label) {
            label->SetText(value);
        }
    }

    SetPCWComboWithPinNames(WProp, "combo2", input_pins[0]);
    SetPCWComboWithPinNames(WProp, "combo3", input_pins[1]);
    SetPCWComboWithPinNames(WProp, "combo4", input_pins[2]);
    SetPCWComboWithPinNames(WProp, "combo5", input_pins[3]);
    SetPCWComboWithPinNames(WProp, "combo6", input_pins[4]);
    SetPCWComboWithPinNames(WProp, "combo7", input_pins[5]);
    SetPCWComboWithPinNames(WProp, "combo8", input_pins[6]);
    SetPCWComboWithPinNames(WProp, "combo9", input_pins[7]);
    SetPCWComboWithPinNames(WProp, "combo11", input_pins[8]);
}

void cpart_IO_74xx573::ReadPropertiesWindow(CPWindow* WProp) {
    input_pins[0] = GetPWCComboSelectedPin(WProp, "combo2");
    input_pins[1] = GetPWCComboSelectedPin(WProp, "combo3");
    input_pins[2] = GetPWCComboSelectedPin(WProp, "combo4");
    input_pins[3] = GetPWCComboSelectedPin(WProp, "combo5");
    input_pins[4] = GetPWCComboSelectedPin(WProp, "combo6");
    input_pins[5] = GetPWCComboSelectedPin(WProp, "combo7");
    input_pins[6] = GetPWCComboSelectedPin(WProp, "combo8");
    input_pins[7] = GetPWCComboSelectedPin(WProp, "combo9");
    input_pins[8] = GetPWCComboSelectedPin(WProp, "combo11");
}

void cpart_IO_74xx573::PreProcess(void) {
    memset(output_pins_alm, 0, 8 * sizeof(unsigned long));

    JUMPSTEPS_ = PICSimLab.GetJUMPSTEPS();
    mcount = JUMPSTEPS_;
}

void cpart_IO_74xx573::Process(void) {
    const picpin* ppins = SpareParts.GetPinsValues();

    unsigned short ret;
    unsigned char data = 0;

    data |= ppins[input_pins[7] - 1].value;
    data |= (ppins[input_pins[6] - 1].value << 1);
    data |= (ppins[input_pins[5] - 1].value << 2);
    data |= (ppins[input_pins[4] - 1].value << 3);
    data |= (ppins[input_pins[3] - 1].value << 4);
    data |= (ppins[input_pins[2] - 1].value << 5);
    data |= (ppins[input_pins[1] - 1].value << 6);
    data |= (ppins[input_pins[0] - 1].value << 7);

    ret = io_74xx573_io(&lt8, ppins[input_pins[8] - 1].value, data);

    if (_ret != ret) {
        SpareParts.WritePin(output_pins[0], (ret & 0x01) != 0);
        SpareParts.WritePin(output_pins[1], (ret & 0x02) != 0);
        SpareParts.WritePin(output_pins[2], (ret & 0x04) != 0);
        SpareParts.WritePin(output_pins[3], (ret & 0x08) != 0);
        SpareParts.WritePin(output_pins[4], (ret & 0x10) != 0);
        SpareParts.WritePin(output_pins[5], (ret & 0x20) != 0);
        SpareParts.WritePin(output_pins[6], (ret & 0x40) != 0);
        SpareParts.WritePin(output_pins[7], (ret & 0x80) != 0);
    }
    _ret = ret;

    mcount++;
    if (mcount >= JUMPSTEPS_) {
        if (ppins[output_pins[0] - 1].value)
            output_pins_alm[0]++;
        if (ppins[output_pins[1] - 1].value)
            output_pins_alm[1]++;
        if (ppins[output_pins[2] - 1].value)
            output_pins_alm[2]++;
        if (ppins[output_pins[3] - 1].value)
            output_pins_alm[3]++;
        if (ppins[output_pins[4] - 1].value)
            output_pins_alm[4]++;
        if (ppins[output_pins[5] - 1].value)
            output_pins_alm[5]++;
        if (ppins[output_pins[6] - 1].value)
            output_pins_alm[6]++;
        if (ppins[output_pins[7] - 1].value)
            output_pins_alm[7]++;

        mcount = -1;
    }
}

void cpart_IO_74xx573::PostProcess(void) {
    long int NSTEPJ = PICSimLab.GetNSTEPJ();
    const picpin* ppins = SpareParts.GetPinsValues();

    SpareParts.WritePinOA(output_pins[0],
                          (ppins[output_pins[0] - 1].oavalue + ((output_pins_alm[0] * 200.0) / NSTEPJ) + 55) / 2);
    SpareParts.WritePinOA(output_pins[1],
                          (ppins[output_pins[1] - 1].oavalue + ((output_pins_alm[1] * 200.0) / NSTEPJ) + 55) / 2);
    SpareParts.WritePinOA(output_pins[2],
                          (ppins[output_pins[2] - 1].oavalue + ((output_pins_alm[2] * 200.0) / NSTEPJ) + 55) / 2);
    SpareParts.WritePinOA(output_pins[3],
                          (ppins[output_pins[3] - 1].oavalue + ((output_pins_alm[3] * 200.0) / NSTEPJ) + 55) / 2);
    SpareParts.WritePinOA(output_pins[4],
                          (ppins[output_pins[4] - 1].oavalue + ((output_pins_alm[4] * 200.0) / NSTEPJ) + 55) / 2);
    SpareParts.WritePinOA(output_pins[5],
                          (ppins[output_pins[5] - 1].oavalue + ((output_pins_alm[5] * 200.0) / NSTEPJ) + 55) / 2);
    SpareParts.WritePinOA(output_pins[6],
                          (ppins[output_pins[6] - 1].oavalue + ((output_pins_alm[6] * 200.0) / NSTEPJ) + 55) / 2);
    SpareParts.WritePinOA(output_pins[7],
                          (ppins[output_pins[7] - 1].oavalue + ((output_pins_alm[7] * 200.0) / NSTEPJ) + 55) / 2);
}

part_init(PART_IO_74XX573_Name, cpart_IO_74xx573, "Other");
