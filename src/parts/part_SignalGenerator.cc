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

#include "../picsimlab1.h"
#include "../picsimlab4.h"
#include "../picsimlab5.h"
#include "part_SignalGenerator.h"

/* outputs */
enum
{
 O_P1, O_P2, O_PO1, O_PO2, O_TP, O_AMPL, O_FREQ
};

/* inputs */
enum
{
 I_PO1, I_PO2, I_TP
};

cpart_SignalGenerator::cpart_SignalGenerator(unsigned x, unsigned y)
{
 X = x;
 Y = y;
 ReadMaps ();

 lxImage image;
 image.LoadFile (Window1.GetSharePath () + lxT ("parts/") + GetPictureFileName ());

 Bitmap = new lxBitmap (image, &Window5);
 image.Destroy ();

 canvas.Create (Window5.GetWWidget (), Bitmap);


 input_pins[0] = 0;

 values[0] = 0;
 values[1] = 0;

 active[0] = 0;
 active[1] = 0;

 type = 0;
 ts = 0;
}

cpart_SignalGenerator::~cpart_SignalGenerator(void)
{
 delete Bitmap;
 canvas.Destroy ();
}

void
cpart_SignalGenerator::Draw(void)
{

 int i,j;
 String temp;
 float v[2];
 float tsi;
 int size;
 canvas.Init ();

 lxFont font (9, lxFONTFAMILY_TELETYPE, lxFONTSTYLE_NORMAL, lxFONTWEIGHT_BOLD);
 canvas.SetFont (font);

 for (i = 0; i < outputc; i++)
  {

   switch (output[i].id)
    {
    case O_P1:
     canvas.SetColor (49, 61, 99);
     canvas.Rectangle (1, output[i].x1, output[i].y1, output[i].x2 - output[i].x1, output[i].y2 - output[i].y1);
     canvas.SetFgColor (255, 255, 255);
     if (input_pins[output[i].id - O_P1] == 0)
      canvas.Text ("NC", output[i].x1, output[i].y1);
     else
      canvas.Text (Window5.GetPinName (input_pins[output[i].id - O_P1]), output[i].x1, output[i].y1);
     break;
    case O_P2:
     canvas.SetColor (49, 61, 99);
     canvas.Rectangle (1, output[i].x1, output[i].y1, output[i].x2 - output[i].x1, output[i].y2 - output[i].y1);
     canvas.SetFgColor (255, 255, 255);
     canvas.Text ("GND", output[i].x1, output[i].y1);
     break;
    case O_PO1:
    case O_PO2:
     canvas.SetColor (50, 50, 50);
     canvas.Rectangle (1, output[i].x1, output[i].y1, output[i].x2 - output[i].x1, output[i].y2 - output[i].y1);
     canvas.SetColor (250, 250, 250);
     canvas.Rectangle (1, output[i].x1 + 6, output[i].y1 + values[output[i].id - O_PO1], 20, 10);
     break;
    case O_TP:
     canvas.SetColor (255, 255, 255);
     canvas.Rectangle (1, output[i].x1, output[i].y1, output[i].x2 - output[i].x1, output[i].y2 - output[i].y1);
     canvas.SetFgColor (0, 0, 0);
    
     v[0]= 0;
     tsi = 0;
     size = output[i].x2 - output[i].x1;

     for (j = 1; j < size; j++)
      {
       v[1]=v[0];
       switch (type)
        {
        case 0:
         v[0] = (sin (tsi)) ;
         break;
        case 1:
         v[0] = ((sin (tsi) > 0) - 0.5)*2 ;
         break;
        case 2:
         v[0] = ((acos (sin (tsi)) / 1.5708) - 1) ;
         break;
        }
       tsi += 3*6.28/size;
       if(j > 0)
        {
          //canvas.Point (output[i].x1+j,output[i].y1+((v+2.0)*size/4.0));
          
          canvas.Line (output[i].x1+j-1,output[i].y1+((v[1]+2.0)*size/4.0),output[i].x1+j,output[i].y1+((v[0]+2.0)*size/4.0));
        }
      }

     break;
    case O_AMPL:
     temp.Printf ("A=%5.2f", ampl);
     canvas.SetColor (49, 61, 99);
     canvas.Rectangle (1, output[i].x1, output[i].y1, output[i].x2 - output[i].x1, output[i].y2 - output[i].y1);
     canvas.SetFgColor (255, 255, 255);
     canvas.Text (temp, output[i].x1, output[i].y1);
     break;
    case O_FREQ:
     temp.Printf ("F=%5.2f", freq);
     canvas.SetColor (49, 61, 99);
     canvas.Rectangle (1, output[i].x1, output[i].y1, output[i].x2 - output[i].x1, output[i].y2 - output[i].y1);
     canvas.SetFgColor (255, 255, 255);
     canvas.Text (temp, output[i].x1, output[i].y1);
     break;
    }
  }

 canvas.End ();

}

void
cpart_SignalGenerator::PreProcess(void)
{
 JUMPSTEPS_ = (Window1.GetBoard ()->MGetInstClock () / 250000);
 mcount = JUMPSTEPS_;

 freq = (10000 * values[1] / 148.0);
 ampl = (2.5 * values[0] / 148.0);
}

void
cpart_SignalGenerator::Process(void)
{

 mcount++;

 if (mcount > JUMPSTEPS_)
  {
   float v = 0;

   switch (type)
    {
    case 0:
     v = (ampl * sin (freq * 6.28 * ts)) + 2.5;
     break;
    case 1:
     v = ((sin (freq * 6.28 * ts) > 0) - 0.5)*2 * ampl + 2.5;
     break;
    case 2:
     v = ((acos (sin (freq * 6.28 * ts)) / 1.5708) - 1) * ampl + 2.5;
     break;
    }
   ts += 4e-6;

   if (ts > 1e6)ts = ts - 1e6;

   Window5.SetAPin (input_pins[0], v);

   mcount = -1;
  }

}

void
cpart_SignalGenerator::EvMouseButtonPress(uint button, uint x, uint y, uint state)
{

 int i;
 unsigned int l;

 for (i = 0; i < inputc; i++)
  {
   if (((input[i].x1 <= x)&&(input[i].x2 >= x))&&((input[i].y1 <= y)&&(input[i].y2 >= y)))
    {
     l = (input[i].y2 - input[i].y1 - 10);
     switch (input[i].id)
      {
      case I_PO1:
       values[0] = y - input[i].y1;
       if (values[0] >= l)values[0] = l;
       active[0] = 1;
       break;
      case I_PO2:
       values[1] = y - input[i].y1;
       if (values[1] >= l)values[1] = l;
       active[1] = 1;
       break;
      case I_TP:
       type++;
       if (type > 2)type = 0;
       break;
      }
    }
  }

}

void
cpart_SignalGenerator::EvMouseButtonRelease(uint button, uint x, uint y, uint state)
{
 int i;

 for (i = 0; i < inputc; i++)
  {
   if (((input[i].x1 <= x)&&(input[i].x2 >= x))&&((input[i].y1 <= y)&&(input[i].y2 >= y)))
    {
     switch (input[i].id)
      {
      case I_PO1:
       active[0] = 0;
       break;
      case I_PO2:
       active[1] = 0;
       break;
      }
    }
  }
}

void
cpart_SignalGenerator::EvMouseMove(uint button, uint x, uint y, uint state)
{

 int i;
 unsigned int l;

 for (i = 0; i < inputc; i++)
  {
   if (((input[i].x1 <= x)&&(input[i].x2 >= x))&&((input[i].y1 <= y)&&(input[i].y2 >= y)))
    {
     l = (input[i].y2 - input[i].y1 - 10);
     switch (input[i].id)
      {
      case I_PO1:
       if (active[0])
        {
         values[0] = y - input[i].y1;
         if (values[0] >= l)values[0] = l;
        }
       break;
      case I_PO2:
       if (active[1])
        {
         values[1] = y - input[i].y1;
         if (values[1] >= l)values[1] = l;
        }
       break;
      }
    }
  }
}

unsigned short
cpart_SignalGenerator::get_in_id(char * name)
{

 if (strcmp (name, "PO1") == 0)return I_PO1;
 if (strcmp (name, "PO2") == 0)return I_PO2;
 if (strcmp (name, "TP") == 0)return I_TP;

 printf ("Erro input '%s' don't have a valid id! \n", name);
 return -1;
};

unsigned short
cpart_SignalGenerator::get_out_id(char * name)
{

 if (strcmp (name, "P1") == 0)return O_P1;
 if (strcmp (name, "P2") == 0)return O_P2;

 if (strcmp (name, "PO1") == 0)return O_PO1;
 if (strcmp (name, "PO2") == 0)return O_PO2;
 if (strcmp (name, "TP") == 0)return O_TP;
 if (strcmp (name, "AMPL") == 0)return O_AMPL;
 if (strcmp (name, "FREQ") == 0)return O_FREQ;

 printf ("Erro output '%s' don't have a valid id! \n", name);
 return 1;
}

String
cpart_SignalGenerator::WritePreferences(void)
{
 char prefs[256];

 sprintf (prefs, "%hhu,%hhu,%hhu,%hhu", input_pins[0], values[0], values[1], type);

 return prefs;
}

void
cpart_SignalGenerator::ReadPreferences(String value)
{
 sscanf (value.c_str (), "%hhu,%hhu,%hhu,%hhu", &input_pins[0], &values[0], &values[1], &type);
}

CPWindow * WProp_SignalGenerator;

void
cpart_SignalGenerator::ConfigurePropertiesWindow(CPWindow * wprop)
{
 String Items = Window5.GetPinsNames ();
 String spin;
 WProp_SignalGenerator = wprop;

 ((CCombo*) WProp_SignalGenerator->GetChildByName ("combo1"))->SetItems (Items);
 if (input_pins[0] == 0)
  ((CCombo*) WProp_SignalGenerator->GetChildByName ("combo1"))->SetText ("0  NC");
 else
  {
   spin = Window5.GetPinName (input_pins[0]);
   ((CCombo*) WProp_SignalGenerator->GetChildByName ("combo1"))->SetText (itoa (input_pins[0]) + "  " + spin);
  }



 ((CButton*) WProp_SignalGenerator->GetChildByName ("button1"))->EvMouseButtonRelease = EVMOUSEBUTTONRELEASE & CPWindow5::PropButtonRelease;
 ((CButton*) WProp_SignalGenerator->GetChildByName ("button1"))->SetTag (1);

 ((CButton*) WProp_SignalGenerator->GetChildByName ("button2"))->EvMouseButtonRelease = EVMOUSEBUTTONRELEASE & CPWindow5::PropButtonRelease;
}

void
cpart_SignalGenerator::ReadPropertiesWindow(void)
{
 input_pins[0] = atoi (((CCombo*) WProp_SignalGenerator->GetChildByName ("combo1"))->GetText ());
}

