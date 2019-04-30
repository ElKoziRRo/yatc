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

#ifndef __UI_LOGIN_H
#define __UI_LOGIN_H

#if defined(HAVE_CONFIG_H)
    #include "../config.h"
#endif
#if defined(HAVE_LIBINTL_H)
    #include <libintl.h>
#else
    #define gettext(x) (x)
#endif
class winLogin_t {
public:

	glictWindow window;
	glictPanel pnlUsername, pnlPassword, pnlCreateAc;
	glictTextbox txtUsername, txtPassword;
	glictButton btnCreateAc;

	glictPanel pnlSeparator;

	glictButton btnOk, btnCancel;

	winLogin_t() 
	{

		window.SetWidth(231-4);
		window.SetHeight(173-17 - 2);
		window.SetCaption(gettext("Enter Game"));
		window.SetVisible(false);
		window.SetBGColor(.4, .4, .4, 1.);

		window.AddObject(&pnlUsername);
		window.AddObject(&txtUsername);
		window.AddObject(&pnlPassword);
		window.AddObject(&txtPassword);
		window.AddObject(&pnlCreateAc);
		window.AddObject(&btnCreateAc);
		window.AddObject(&pnlSeparator); /* **** */
		window.AddObject(&btnOk);
		window.AddObject(&btnCancel);

		pnlUsername.SetWidth(131-17);
		pnlUsername.SetHeight(12);
		pnlUsername.SetCaption(gettext("Account number:"));
		pnlUsername.SetPos(13,17);
		pnlUsername.SetBGActiveness(false);
		pnlUsername.SetFont("aafont");

		txtUsername.SetPos(128,15 - 1);
		txtUsername.SetWidth(216-134);
		txtUsername.SetHeight(46-33 + 2);
		txtUsername.SetPassProtectCharacter(128+('u'-96)); // circular *, just under 'u' which is at column ('u'-96)
		txtUsername.SetAllowedChars("0123456789");
		txtUsername.SetPrevious(!options.ui_compat ? (glictContainer*)&txtPassword : (glictContainer*)&btnCancel);
		txtUsername.SetNext(&txtPassword);

		pnlPassword.SetWidth(131-17);
		pnlPassword.SetHeight(12);
		pnlPassword.SetCaption(gettext("Password:"));
		pnlPassword.SetPos(13,47);
		pnlPassword.SetBGActiveness(false);
		pnlPassword.SetFont("aafont");

		txtPassword.SetPos(128,45-1);
		txtPassword.SetWidth(216-134);
		txtPassword.SetHeight(46-33+2);
		txtPassword.SetPassProtectCharacter(128+('u'-96));
		txtPassword.SetAllowedChars(" !\"#$%/()=?*'+<>[]{}`~\\|;:_,.-0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
		txtPassword.SetPrevious(&txtUsername);
		txtPassword.SetNext(!options.ui_compat ? (glictContainer*)&txtUsername : (glictContainer*)&btnCreateAc);

		pnlCreateAc.SetWidth(131-17);
		pnlCreateAc.SetHeight(24);
		pnlCreateAc.SetCaption(gettext("If you don't have\nan account yet:"));
		pnlCreateAc.SetPos(13,85-12);
		pnlCreateAc.SetBGActiveness(false);
		pnlCreateAc.SetFont("aafont");

		btnCreateAc.SetPos(128,94-12);
		btnCreateAc.SetWidth(216-133);
		btnCreateAc.SetHeight(112-95);
		btnCreateAc.SetCaption(gettext("Create Account"));
		btnCreateAc.SetBGColor(.8,.8,.8,1.);
		btnCreateAc.SetFont("minifont",8);
		btnCreateAc.SetPrevious(!options.ui_compat ? NULL : &txtPassword);
		btnCreateAc.SetNext(!options.ui_compat ? NULL : &btnOk);

		pnlSeparator.SetPos(9, 121);
		pnlSeparator.SetBGColor(.1,.1,.1,1.);
		pnlSeparator.SetWidth(222-13);
		pnlSeparator.SetHeight(2);
		pnlSeparator.SetSkin(&g_skin.chk);

		btnOk.SetPos(127-4,148-17);
		btnOk.SetWidth(40);
		btnOk.SetHeight(166-149);
		btnOk.SetCaption(gettext("Ok"));
		btnOk.SetBGColor(.8,.8,.8,1.);
		btnOk.SetFont("minifont",8);
		btnOk.SetPrevious(!options.ui_compat ? NULL : &btnCreateAc);
		btnOk.SetNext(!options.ui_compat ? NULL : &btnCancel);

		btnCancel.SetPos(180-4,148-17);
		btnCancel.SetWidth(40);
		btnCancel.SetHeight(166-149);
		btnCancel.SetCaption(gettext("Cancel"));
		btnCancel.SetBGColor(.8,.8,.8,1.);
		btnCancel.SetFont("minifont",8);
		btnCancel.SetPrevious(!options.ui_compat ? NULL : &btnOk);
		btnCancel.SetNext(!options.ui_compat ? NULL : &txtUsername);

	}

	void permitAccountName(bool yes) 
	{
		if(yes)
		{
			// we can't have weird-ass symbols in account names either. ;)
			txtUsername.SetAllowedChars(" !\"#$%/()=?*'+<>[]{}`~\\|;:_,.-0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
			pnlUsername.SetCaption(gettext("Account name:"));
		} 
		else 
		{
			txtUsername.SetAllowedChars("0123456789");
			pnlUsername.SetCaption(gettext("Account number:"));
		}
	}
};

#endif
