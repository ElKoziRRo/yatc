//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////

#ifndef __UI_VIPWINDOW_H
#define __UI_VIPWINDOW_H

#include <GLICT/list.h>
#include <GLICT/progressbar.h>
#include <map>

#include "stackpanel.h"
#include "../popup.h"

class sbvlPanel_t;


class winVIP_t : public yatcStackPanelWindow
{
public:
	std::map<uint32_t, glictPanel*> m_entries;
	glictList container;

    uint32_t selectedcreature;

	sbvlPanel_t* controller;

	winVIP_t();
	~winVIP_t();

	void addVIP(uint32_t);
	void updateVIP(uint32_t);
	void removeVIP(uint32_t);

	virtual float GetDefaultHeight();
	virtual void OnClose();

    static void makeVIPPopup(Popup* popup, void* owner, void* arg);

	static void OnListbox(glictPos* pos, glictContainer *caller);

    static void onUnimplemented(Popup::Item *parent);
    static void onRemoveVIP(Popup::Item *parent);
};

#endif //__UI_VIPWINDOW_H
