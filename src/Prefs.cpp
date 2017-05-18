//
// Prefs
//
// Class for saving and loading preference information
// via BMessages.
//

#include <Message.h>
#include <Messenger.h>
#include <File.h>
#include <String.h>
#include <FindDirectory.h>
#include <Debug.h>

#include "TPreferences.h"
#include "Prefs.h"

// Open the settings file and read the data.
Prefs::Prefs()
{
	default_prefs.fontsize = 25;
	default_prefs.numcols = 3;
	default_prefs.drawheights = false;
	default_prefs.drawborder = true;
	default_prefs.autoadjust = false;
	default_prefs.displaytext = "The quick brown fox jumps over the lazy dog.";
	default_prefs.mbg_color = def_mbg_color;
	default_prefs.mdisplay_color = def_mdisplay_color;
	default_prefs.minfo_color = def_minfo_color;
	default_prefs.mstroke_color = def_mstroke_color;
	default_prefs.mheights_color = def_mheights_color;
	default_prefs.mselectedbg_color = def_mselectedbg_color;
	default_prefs.mselecteddisplay_color = def_mselecteddisplay_color;
	default_prefs.mselectedinfo_color = def_mselectedinfo_color;
	default_prefs.mselectedheights_color = def_mselectedheights_color;
	
	default_prefs.pbg_color = def_pbg_color;
	default_prefs.pdisplay_color = def_pdisplay_color;
	default_prefs.pstroke_color = def_pstroke_color;
	default_prefs.pselectedbg_color = def_pselectedbg_color;
	default_prefs.pselecteddisplay_color = def_pselecteddisplay_color;
	default_prefs.pinactiveselectedbg_color = def_pinactiveselectedbg_color;
	default_prefs.pinactiveselecteddisplay_color = def_pinactiveselecteddisplay_color;
	
	// create Preferences-Object
	prefs = new TPreferences("Fontboy_prefs");			
	LoadPrefs();
}

// Write the preferences to disk.
Prefs::~Prefs()
{
	SavePrefs();

	delete prefs;
}

void Prefs::Apply()
{
}

void Prefs::Update()
{
	font_height info_height, text_height;

	BFont font(be_plain_font);

	font.GetHeight(&info_height);
	font.SetSize(GetFontSize());
	font.GetHeight(&text_height);

	active_prefs.rowheight = FM_ROWHEIGHT_BUFFER + text_height.ascent + text_height.descent + info_height.ascent + info_height.descent;
	
	
}

bool Prefs::CanRevert()
{
	return 	!_CompareUserPrefs(active_prefs, revert_prefs);
}

void Prefs::Revert()
{
	_CopyUserPrefs(active_prefs, revert_prefs);
}

bool Prefs::IsDefault()
{
	return 	_CompareUserPrefs(active_prefs, default_prefs);
}

void Prefs::Default()
{
	_CopyUserPrefs(active_prefs, default_prefs);
}

bool Prefs::_CompareUserPrefs(prefs_data& prefs1, prefs_data& prefs2)
{
	return 	prefs1.fontsize == prefs2.fontsize &&
		prefs1.numcols == prefs2.numcols &&
		prefs1.drawheights == prefs2.drawheights &&
		prefs1.drawborder == prefs2.drawborder &&
		prefs1.autoadjust == prefs2.autoadjust &&
		prefs1.displaytext == prefs2.displaytext &&
		prefs1.mbg_color == prefs2.mbg_color &&
		prefs1.mdisplay_color == prefs2.mdisplay_color &&
		prefs1.minfo_color == prefs2.minfo_color &&
		prefs1.mstroke_color == prefs2.mstroke_color &&
		prefs1.mheights_color == prefs2.mheights_color &&
		prefs1.mselectedbg_color == prefs2.mselectedbg_color &&
		prefs1.mselecteddisplay_color == prefs2.mselecteddisplay_color &&
		prefs1.mselectedinfo_color == prefs2.mselectedinfo_color &&
		prefs1.mselectedheights_color == prefs2.mselectedheights_color &&
		prefs1.pbg_color == prefs2.pbg_color  &&
		prefs1.pdisplay_color == prefs2.pdisplay_color &&
		prefs1.pstroke_color == prefs2.pstroke_color &&
		prefs1.pselectedbg_color == prefs2.pselectedbg_color  &&
		prefs1.pselecteddisplay_color == prefs2.pselecteddisplay_color &&
		prefs1.pinactiveselectedbg_color == prefs2.pinactiveselectedbg_color  &&
		prefs1.pinactiveselecteddisplay_color == prefs2.pinactiveselecteddisplay_color;		
}

void Prefs::_CopyUserPrefs(prefs_data& prefs1, prefs_data& prefs2)
{
	// Display
	prefs1.fontsize = prefs2.fontsize;
	prefs1.numcols = prefs2.numcols;
	prefs1.drawheights = prefs2.drawheights;
	prefs1.drawborder = prefs2.drawborder;
	prefs1.autoadjust = prefs2.autoadjust;
	prefs1.displaytext = prefs2.displaytext;
	// Color
	prefs1.mbg_color = prefs2.mbg_color;
	prefs1.mdisplay_color = prefs2.mdisplay_color;
	prefs1.minfo_color = prefs2.minfo_color;
	prefs1.mstroke_color = prefs2.mstroke_color;
	prefs1.mheights_color = prefs2.mheights_color;
	
	prefs1.mselectedbg_color = prefs2.mselectedbg_color;
	prefs1.mselecteddisplay_color = prefs2.mselecteddisplay_color;
	prefs1.mselectedinfo_color = prefs2.mselectedinfo_color;
	prefs1.mselectedheights_color = prefs2.mselectedheights_color;
	
	prefs1.pbg_color = prefs2.pbg_color;
	prefs1.pdisplay_color = prefs2.pdisplay_color;
	prefs1.pstroke_color = prefs2.pstroke_color;
	prefs1.pselectedbg_color = prefs2.pselectedbg_color;
	prefs1.pselecteddisplay_color = prefs2.pselecteddisplay_color;
	prefs1.pinactiveselectedbg_color = prefs2.pinactiveselectedbg_color;
	prefs1.pinactiveselecteddisplay_color = prefs2.pinactiveselecteddisplay_color;
	// Skip windows splitview settings
}

void Prefs::FinishUpdate()
{
	update_listwindow = false;
	update_detailwindow = false;
}

void Prefs::LoadPrefs()
{
	if (prefs != NULL) {
		if (prefs->FindFloat("fontsize", &active_prefs.fontsize) != B_OK)
			active_prefs.fontsize = default_prefs.fontsize;
		if (prefs->FindFloat("numcols", &active_prefs.numcols) != B_OK)
			active_prefs.numcols = default_prefs.numcols;
		if (prefs->FindInt32("drawheights", &active_prefs.drawheights) != B_OK)
			active_prefs.drawheights = default_prefs.drawheights;
		if (prefs->FindInt32("drawborder", &active_prefs.drawborder) != B_OK)
			active_prefs.drawborder = default_prefs.drawborder;
		if (prefs->FindInt32("autoadjust", &active_prefs.autoadjust) != B_OK)
			active_prefs.autoadjust = default_prefs.autoadjust;

		const char *dtext;
		if (prefs->FindString("displaytext", &dtext) == B_OK)
			active_prefs.displaytext.SetTo(dtext);
		else
			active_prefs.displaytext = default_prefs.displaytext;
		
		rgb_color *col;
		ssize_t csize;
		status_t status;

		status = prefs->FindData("mbgcolor", B_RGB_COLOR_TYPE, (const void **)&col, &csize);
		active_prefs.mbg_color = (status == B_NO_ERROR) ? *col : def_mbg_color;
		status = prefs->FindData("mdisplaycolor", B_RGB_COLOR_TYPE, (const void **)&col, &csize);
		active_prefs.mdisplay_color = (status == B_NO_ERROR) ? *col : def_mdisplay_color;
		status = prefs->FindData("minfocolor", B_RGB_COLOR_TYPE, (const void **)&col, &csize);
		active_prefs.minfo_color = (status == B_NO_ERROR) ? *col : def_minfo_color;
		status = prefs->FindData("mstrokecolor", B_RGB_COLOR_TYPE, (const void **)&col, &csize);
		active_prefs.mstroke_color = (status == B_NO_ERROR) ? *col : def_mstroke_color;
		status = prefs->FindData("mheightscolor", B_RGB_COLOR_TYPE, (const void **)&col, &csize);
		active_prefs.mheights_color = (status == B_NO_ERROR) ? *col : def_mheights_color;

		status = prefs->FindData("mselectedbgcolor", B_RGB_COLOR_TYPE, (const void **)&col, &csize);
		active_prefs.mselectedbg_color = (status == B_NO_ERROR) ? *col : def_mselectedbg_color;
		status = prefs->FindData("mselecteddisplaycolor", B_RGB_COLOR_TYPE, (const void **)&col, &csize);
		active_prefs.mselecteddisplay_color = (status == B_NO_ERROR) ? *col : def_mselecteddisplay_color;
		status = prefs->FindData("mselectedinfocolor", B_RGB_COLOR_TYPE, (const void **)&col, &csize);
		active_prefs.mselectedinfo_color = (status == B_NO_ERROR) ? *col : def_mselectedinfo_color;
		status = prefs->FindData("mselectedheightscolor", B_RGB_COLOR_TYPE, (const void **)&col, &csize);
		active_prefs.mselectedheights_color = (status == B_NO_ERROR) ? *col : def_mselectedheights_color;
		
		status = prefs->FindData("pbgcolor", B_RGB_COLOR_TYPE, (const void **)&col, &csize);
		active_prefs.pbg_color = (status == B_NO_ERROR) ? *col : def_pbg_color;
		status = prefs->FindData("pdisplaycolor", B_RGB_COLOR_TYPE, (const void **)&col, &csize);
		active_prefs.pdisplay_color = (status == B_NO_ERROR) ? *col : def_pdisplay_color;
		status = prefs->FindData("pstrokecolor", B_RGB_COLOR_TYPE, (const void **)&col, &csize);
		active_prefs.pstroke_color = (status == B_NO_ERROR) ? *col : def_pstroke_color;
		
		status = prefs->FindData("pselectedbgcolor", B_RGB_COLOR_TYPE, (const void **)&col, &csize);
		active_prefs.pselectedbg_color = (status == B_NO_ERROR) ? *col : def_pselectedbg_color;
		status = prefs->FindData("pselecteddisplaycolor", B_RGB_COLOR_TYPE, (const void **)&col, &csize);
		active_prefs.pselecteddisplay_color = (status == B_NO_ERROR) ? *col : def_pselecteddisplay_color;
		
		status = prefs->FindData("pinactiveselectedbgcolor", B_RGB_COLOR_TYPE, (const void **)&col, &csize);
		active_prefs.pinactiveselectedbg_color = (status == B_NO_ERROR) ? *col : def_pinactiveselectedbg_color;
		status = prefs->FindData("pinactiveselecteddisplaycolor", B_RGB_COLOR_TYPE, (const void **)&col, &csize);
		active_prefs.pinactiveselecteddisplay_color = (status == B_NO_ERROR) ? *col : def_pinactiveselecteddisplay_color;

		if (prefs->FindRect("mainrect", &active_prefs.mainrect) != B_OK)
			active_prefs.mainrect.Set(50, 50, 650, 485);
		if (prefs->FindRect("propsrect", &active_prefs.propsrect) != B_OK)
			active_prefs.propsrect.Set(50, 50, 450, 450);
		if (prefs->FindRect("prefsrect", &active_prefs.prefsrect) != B_OK)
			active_prefs.prefsrect.Set(50, 50, 600, 374);

		if (prefs->FindInt32("splitpanepos", &active_prefs.splitpanepos) != B_OK)
			active_prefs.splitpanepos = 200;
		
		_CopyUserPrefs(revert_prefs, active_prefs);
	}
}

void Prefs::SavePrefs()
{
	if (prefs != NULL) {
		prefs->SetFloat("fontsize", active_prefs.fontsize);
		prefs->SetFloat("numcols", active_prefs.numcols);
		prefs->SetInt32("drawheights", active_prefs.drawheights);
		prefs->SetInt32("drawborder", active_prefs.drawborder);
		prefs->SetInt32("autoadjust", active_prefs.autoadjust);
		prefs->SetString("displaytext", GetDisplayText());
		prefs->SetData("mbgcolor", B_RGB_COLOR_TYPE, &active_prefs.mbg_color, sizeof(rgb_color));
		prefs->SetData("mdisplaycolor", B_RGB_COLOR_TYPE, &active_prefs.mdisplay_color, sizeof(rgb_color));
		prefs->SetData("minfocolor", B_RGB_COLOR_TYPE, &active_prefs.minfo_color, sizeof(rgb_color));
		prefs->SetData("mstrokecolor", B_RGB_COLOR_TYPE, &active_prefs.mstroke_color, sizeof(rgb_color));
		prefs->SetData("mheightscolor", B_RGB_COLOR_TYPE, &active_prefs.mheights_color, sizeof(rgb_color));
		prefs->SetData("mselectedbgcolor", B_RGB_COLOR_TYPE, &active_prefs.mselectedbg_color, sizeof(rgb_color));
		prefs->SetData("mselecteddisplaycolor", B_RGB_COLOR_TYPE, &active_prefs.mselecteddisplay_color, sizeof(rgb_color));
		prefs->SetData("mselectedinfocolor", B_RGB_COLOR_TYPE, &active_prefs.mselectedinfo_color, sizeof(rgb_color));
		prefs->SetData("mselectedheightscolor", B_RGB_COLOR_TYPE, &active_prefs.mselectedheights_color, sizeof(rgb_color));
		prefs->SetData("pbgcolor", B_RGB_COLOR_TYPE, &active_prefs.pbg_color, sizeof(rgb_color));
		prefs->SetData("pdisplaycolor", B_RGB_COLOR_TYPE, &active_prefs.pdisplay_color, sizeof(rgb_color));
		prefs->SetData("pstrokecolor", B_RGB_COLOR_TYPE, &active_prefs.pstroke_color, sizeof(rgb_color));		
		prefs->SetData("pselectedbgcolor", B_RGB_COLOR_TYPE, &active_prefs.pselectedbg_color, sizeof(rgb_color));
		prefs->SetData("pselecteddisplaycolor", B_RGB_COLOR_TYPE, &active_prefs.pselecteddisplay_color, sizeof(rgb_color));
		prefs->SetData("pinactiveselectedbgcolor", B_RGB_COLOR_TYPE, &active_prefs.pinactiveselectedbg_color, sizeof(rgb_color));
		prefs->SetData("pinactiveselecteddisplaycolor", B_RGB_COLOR_TYPE, &active_prefs.pinactiveselecteddisplay_color, sizeof(rgb_color));
		prefs->SetRect("mainrect", active_prefs.mainrect);
		prefs->SetRect("propsrect", active_prefs.propsrect);
		prefs->SetRect("prefsrect", active_prefs.prefsrect);

		prefs->SetInt32("splitpanepos", active_prefs.splitpanepos);
		
		_CopyUserPrefs(revert_prefs, active_prefs);
	}
}

float Prefs::GetFontSize()
{
	return active_prefs.fontsize;
}

void Prefs::SetFontSize(float fval)
{
	active_prefs.fontsize = fval;
}

float Prefs::GetRowHeight()
{
	return active_prefs.rowheight;
}

void Prefs::SetRowHeight(float fval)
{
	active_prefs.rowheight  = fval;
}

float Prefs::GetNumCols()
{
	return active_prefs.numcols;
}

void Prefs::SetNumCols(float fval)
{
	active_prefs.numcols  = fval;
}

int32 Prefs::GetDrawHeights()
{
	return active_prefs.drawheights;
}

void Prefs::SetDrawHeights(int32 ival)
{
	active_prefs.drawheights = ival;
}

int32 Prefs::GetAutoAdjust()
{
	return active_prefs.autoadjust;
}

void Prefs::SetAutoAdjust(int32 ival)
{
	active_prefs.autoadjust = ival;
}

int32 Prefs::GetDrawBorder()
{
	return active_prefs.drawborder;
}

void Prefs::SetDrawBorder(int32 ival)
{
	active_prefs.drawborder = ival;
}

BString	Prefs::GetDisplayText()
{
//_sPrintf("GetDisplayText %d %s\n", index, displaytext[index]->String());
	return active_prefs.displaytext;
}

void Prefs::SetDisplayText(BString astring)
{
	active_prefs.displaytext = astring;
}

void Prefs::SetDisplayText(const char *text)
{
	active_prefs.displaytext = text;
}

rgb_color Prefs::GetMBgColor(bool selected)
{
	return selected ? active_prefs.mselectedbg_color : active_prefs.mbg_color;
}

void Prefs::SetMBgColor(rgb_color *col)
{
	active_prefs.mbg_color = *col;
}

void Prefs::SetMSelectedBgColor(rgb_color *col)
{
	active_prefs.mselectedbg_color = *col;
}

rgb_color Prefs::GetMDisplayColor(bool selected)
{
	return selected ? active_prefs.mselecteddisplay_color : active_prefs.mdisplay_color;
}

void Prefs::SetMDisplayColor(rgb_color *col)
{
	active_prefs.mdisplay_color = *col;
}

void Prefs::SetMSelectedDisplayColor(rgb_color *col)
{
	active_prefs.mselecteddisplay_color = *col;
}

rgb_color Prefs::GetMInfoColor(bool selected)
{
	return selected ? active_prefs.mselectedinfo_color : active_prefs.minfo_color;
}

void Prefs::SetMInfoColor(rgb_color *col)
{
	active_prefs.minfo_color = *col;
}

void Prefs::SetMSelectedInfoColor(rgb_color *col)
{
	active_prefs.mselectedinfo_color = *col;
}

rgb_color Prefs::GetMStrokeColor()
{
	return active_prefs.mstroke_color;
}

void Prefs::SetMStrokeColor(rgb_color *col)
{
	active_prefs.mstroke_color = *col;
}

rgb_color Prefs::GetMHeightsColor(bool selected)
{
	return selected ? active_prefs.mselectedheights_color : active_prefs.mheights_color;
}

void Prefs::SetMHeightsColor(rgb_color *col)
{
	active_prefs.mheights_color = *col;
}

void Prefs::SetMSelectedHeightsColor(rgb_color *col)
{
	active_prefs.mselectedheights_color = *col;
}

rgb_color Prefs::GetPBgColor(bool selected)
{
	return selected ? active_prefs.pselectedbg_color : active_prefs.pbg_color;
}

rgb_color Prefs::GetPInactiveSelectedBgColor()
{
	return active_prefs.pinactiveselectedbg_color;
}

void Prefs::SetPInactiveSelectedBgColor(rgb_color *col)
{
	active_prefs.pinactiveselectedbg_color = *col;
}

rgb_color Prefs::GetPInactiveSelectedDisplayColor()
{
	return active_prefs.pinactiveselecteddisplay_color;
}

void Prefs::SetPInactiveSelectedDisplayColor(rgb_color *col)
{
	active_prefs.pinactiveselecteddisplay_color = *col;
}

void Prefs::SetPBgColor(rgb_color *col)
{
	active_prefs.pbg_color = *col;
}

void Prefs::SetPSelectedBgColor(rgb_color *col)
{
	active_prefs.pselectedbg_color = *col;
}

rgb_color Prefs::GetPDisplayColor(bool selected)
{
	return selected ? active_prefs.pselecteddisplay_color : active_prefs.pdisplay_color;
}

void Prefs::SetPDisplayColor(rgb_color *col)
{
	active_prefs.pdisplay_color = *col;
}

void Prefs::SetPSelectedDisplayColor(rgb_color *col)
{
	active_prefs.pselecteddisplay_color = *col;
}

rgb_color Prefs::GetPStrokeColor()
{
	return active_prefs.pstroke_color;
}

void Prefs::SetPStrokeColor(rgb_color *col)
{
	active_prefs.pstroke_color = *col;
}

void Prefs::SetListWindowForUpdate()
{
	update_listwindow = true;
}

bool Prefs::IsListWindowForUpdate()
{
	return	update_listwindow;
}

void Prefs::SetDetailWindowForUpdate()
{
	update_detailwindow = true;
}

bool Prefs::IsDetailWindowForUpdate()
{
	return update_detailwindow;
}

void Prefs::GetMainRect(BRect &arect)
{
	arect = active_prefs.mainrect;
}

void Prefs::SetMainRect(BRect *arect)
{
	active_prefs.mainrect.Set(arect->left, arect->top, arect->right, arect->bottom);
}

void Prefs::GetMViewRect(BRect &arect)
{
	arect = active_prefs.mviewrect;
}

void Prefs::SetMViewRect(BRect *arect)
{
	active_prefs.mviewrect.Set(arect->left, arect->top, arect->right, arect->bottom);
}

void Prefs::GetPropsRect(BRect &arect)
{
	arect = active_prefs.propsrect;
}

void Prefs::SetPropsRect(BRect *arect)
{
	active_prefs.propsrect.Set(arect->left, arect->top, arect->right, arect->bottom);
}

void Prefs::GetPrefsRect(BRect &arect)
{
	arect = active_prefs.prefsrect;
}

void Prefs::SetPrefsRect(BRect *arect)
{
	active_prefs.prefsrect.Set(arect->left, arect->top, arect->right, arect->bottom);
}

int Prefs::GetSplitPanePos(void)
{
	return int(active_prefs.splitpanepos);
}

void Prefs::SetSplitPanePos(int pos)
{
	active_prefs.splitpanepos = int32(pos);
}

