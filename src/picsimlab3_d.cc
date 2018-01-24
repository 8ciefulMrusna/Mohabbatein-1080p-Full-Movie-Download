/* ########################################################################

   PICsimLab - PIC laboratory simulator

   ########################################################################

   Copyright (c) : 2010-2015  Luis Claudio Gambôa Lopes

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

CPWindow3::CPWindow3(void)
{
//lxrad automatic generated block start, don't edit below!
  SetFOwner(this);
  SetClass(wxT("CPWindow"));
  SetName(wxT("window3"));
  SetTag(0);
  SetX(401);
  SetY(215);
  SetWidth(325);
  SetHeight(239);
  SetHint(wxT(""));
  SetEnable(1);
  SetVisible(0);
  SetPopupMenu(NULL);
  SetTitle(wxT("PICSimLab - Configuration"));
  SetOverrideRedirect(0);
  EvOnCreate=EVONCREATE & CPWindow3::_EvOnCreate;
  //label1
  label1.SetFOwner(this);
  label1.SetClass(wxT("CLabel"));
  label1.SetName(wxT("label1"));
  label1.SetTag(0);
  label1.SetX(50);
  label1.SetY(55);
  label1.SetWidth(87);
  label1.SetHeight(20);
  label1.SetHint(wxT(""));
  label1.SetEnable(1);
  label1.SetVisible(1);
  label1.SetPopupMenu(NULL);
  label1.SetText(wxT("Serial Port:"));
  label1.SetAlign(1);
  CreateChild(&label1);
  //label2
  label2.SetFOwner(this);
  label2.SetClass(wxT("CLabel"));
  label2.SetName(wxT("label2"));
  label2.SetTag(0);
  label2.SetX(51);
  label2.SetY(105);
  label2.SetWidth(88);
  label2.SetHeight(20);
  label2.SetHint(wxT(""));
  label2.SetEnable(1);
  label2.SetVisible(1);
  label2.SetPopupMenu(NULL);
  label2.SetText(wxT("Prog. Port:"));
  label2.SetAlign(1);
  CreateChild(&label2);
  //button1
  button1.SetFOwner(this);
  button1.SetClass(wxT("CButton"));
  button1.SetName(wxT("button1"));
  button1.SetTag(0);
  button1.SetX(126);
  button1.SetY(156);
  button1.SetWidth(65);
  button1.SetHeight(28);
  button1.SetHint(wxT(""));
  button1.SetEnable(1);
  button1.SetVisible(1);
  button1.SetPopupMenu(NULL);
  button1.EvMouseButtonClick=EVMOUSEBUTTONCLICK & CPWindow3::button1_EvMouseButtonClick;
  button1.SetText(wxT("Ok"));
  CreateChild(&button1);
  //combo1
  combo1.SetFOwner(this);
  combo1.SetClass(wxT("CCombo"));
  combo1.SetName(wxT("combo1"));
  combo1.SetTag(0);
  combo1.SetX(144);
  combo1.SetY(53);
  combo1.SetWidth(137);
  combo1.SetHeight(26);
  combo1.SetHint(wxT(""));
  combo1.SetEnable(1);
  combo1.SetVisible(1);
  combo1.SetPopupMenu(NULL);
  combo1.SetItems(wxT(""));
  combo1.SetText(wxT(""));
  combo1.SetReadOnly(0);
  CreateChild(&combo1);
  //combo2
  combo2.SetFOwner(this);
  combo2.SetClass(wxT("CCombo"));
  combo2.SetName(wxT("combo2"));
  combo2.SetTag(0);
  combo2.SetX(145);
  combo2.SetY(103);
  combo2.SetWidth(137);
  combo2.SetHeight(26);
  combo2.SetHint(wxT(""));
  combo2.SetEnable(0);
  combo2.SetVisible(1);
  combo2.SetPopupMenu(NULL);
  combo2.SetItems(wxT(""));
  combo2.SetText(wxT(""));
  combo2.SetReadOnly(0);
  CreateChild(&combo2);
  /*#Others*/
//lxrad automatic generated block end, don't edit above!
};
