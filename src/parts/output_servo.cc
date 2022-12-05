/* ########################################################################

   PICsimLab - PIC laboratory simulator

   ########################################################################

   Copyright (c) : 2010-2022  Luis Claudio Gambôa Lopes

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

#include "output_servo.h"
#include "../oscilloscope.h"
#include "../picsimlab.h"
#include "../spareparts.h"

/* outputs */
enum { O_P1, O_AXIS };

static PCWProp pcwprop[1] = {{PCW_COMBO, "Pin 1"}};

cpart_servo::cpart_servo(const unsigned x, const unsigned y, const char* name, const char* type)
    : part(x, y, name, type), font(9, lxFONTFAMILY_TELETYPE, lxFONTSTYLE_NORMAL, lxFONTWEIGHT_BOLD) {
    aways_update = 1;
    input_pin = 0;
    angle = 0;
    angle_ = 0;
    in_[0] = 0;
    in_[1] = 0;
    time = 0;
    BackGround = NULL;
    SetPCWProperties(pcwprop, 1);
}

cpart_servo::~cpart_servo(void) {
    delete Bitmap;
    delete BackGround;
    canvas.Destroy();
}

void cpart_servo::Draw(void) {
    int i;

    Update = 0;

    for (i = 0; i < outputc; i++) {
        if (output[i].update)  // only if need update
        {
            output[i].update = 0;

            if (!Update) {
                canvas.SetBitmap(BackGround, 1.0, 1.0);  // FIXME draw servo error on scale or rotate
                canvas.Init(Scale, Scale, Orientation);
                canvas.SetFont(font);
            }
            Update++;  // set to update buffer

            if (output[i].id == O_P1) {
                canvas.SetFgColor(255, 255, 255);
                if (input_pin == 0)
                    canvas.RotatedText("NC", output[i].x1, output[i].y1, 0);
                else
                    canvas.RotatedText(SpareParts.GetPinName(input_pin), output[i].x1, output[i].y1, 0);
            }

            if (output[i].id == O_AXIS) {
                float x2 = output[i].x1 + output[i].r * sin(angle);
                float y2 = output[i].y1 - output[i].r * cos(angle);
                canvas.SetFgColor(0, 0, 0);
                canvas.SetLineWidth(20);
                canvas.Line(output[i].x1, output[i].y1, x2, y2);
                canvas.SetFgColor(255, 255, 255);
                canvas.SetLineWidth(18);
                canvas.Line(output[i].x1, output[i].y1, x2, y2);
            }
        }
    }

    if (Update) {
        canvas.End();
    }
}

void cpart_servo::Process(void) {
    const picpin* ppins = SpareParts.GetPinsValues();

    if (input_pin == 0)
        return;

    in_[1] = in_[0];
    in_[0] = ppins[input_pin - 1].value;

    if ((in_[0] == 1) && (in_[1] == 0))  // rise
    {
        time = 0;
    }

    if ((in_[0] == 0) && (in_[1] == 1))  // low
    {
        angle_ = ((time / PICSimLab.GetBoard()->MGetInstClockFreq()) - 0.0015) * 3141.59265359;

        if (angle_ > M_PI / 2.0)
            angle_ = M_PI / 2.0;
        if (angle_ < -M_PI / 2.0)
            angle_ = -M_PI / 2.0;
    }

    time++;
}

void cpart_servo::PostProcess(void) {
    if (angle > angle_) {
        angle -= 0.2;
        if (angle < angle_)
            angle = angle_;
    }

    if (angle < angle_) {
        angle += 0.2;
        if (angle > angle_)
            angle = angle_;
    }

    if (output_ids[O_AXIS]->value_f != angle) {
        output_ids[O_AXIS]->value_f = angle;
        output_ids[O_AXIS]->update = 1;
        output_ids[O_P1]->update = 1;
    }
}

unsigned short cpart_servo::GetInputId(char* name) {
    printf("Erro input '%s' don't have a valid id! \n", name);
    return -1;
}

unsigned short cpart_servo::GetOutputId(char* name) {
    if (strcmp(name, "PN_1") == 0)
        return O_P1;
    if (strcmp(name, "DG_AXIS") == 0)
        return O_AXIS;

    printf("Erro output '%s' don't have a valid id! \n", name);
    return 1;
}

lxString cpart_servo::WritePreferences(void) {
    char prefs[256];

    sprintf(prefs, "%hhu", input_pin);

    return prefs;
}

void cpart_servo::ReadPreferences(lxString value) {
    sscanf(value.c_str(), "%hhu", &input_pin);
}

void cpart_servo::RegisterRemoteControl(void) {
    output_ids[O_AXIS]->status = (void*)&angle;
}

void cpart_servo::ConfigurePropertiesWindow(CPWindow* WProp) {
    SetPCWComboWithPinNames(WProp, "combo1", input_pin);
}

void cpart_servo::ReadPropertiesWindow(CPWindow* WProp) {
    input_pin = GetPWCComboSelectedPin(WProp, "combo1");
}

void cpart_servo::LoadImage(void) {
    lxImage image(SpareParts.GetWindow());
    image.LoadFile(lxGetLocalFile(PICSimLab.GetSharePath() + lxT("parts/") + Type + "/" + GetPictureFileName()),
                   Orientation, Scale, Scale);

    Bitmap = new lxBitmap(&image, (SpareParts.GetWindow()));
    image.Destroy();

    image.LoadFile(lxGetLocalFile(PICSimLab.GetSharePath() + lxT("parts/") + Type + "/" + GetPictureFileName()),
                   Orientation, Scale, Scale);

    if (BackGround) {
        delete BackGround;
    }
    BackGround = new lxBitmap(&image, (SpareParts.GetWindow()));
    image.Destroy();

    canvas.Destroy();
    canvas.Create(SpareParts.GetWindow()->GetWWidget(), Bitmap);
}

// Register the part in PICSimLab spare parts list
part_init(PART_SERVO_Name, cpart_servo, "Output");
