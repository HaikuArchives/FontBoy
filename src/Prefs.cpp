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
	active_prefs.displaytext = NULL;

	// create Preferences-Object
	prefs = new TPreferences("Fontboy_prefs");			
	LoadPrefs();
}

// Write the preferences to disk.
Prefs::~Prefs()
{
	SavePrefs();

	delete prefs;
	delete active_prefs.displaytext;
}

void Prefs::Apply()
{
}

void Prefs::Revert()
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

void Prefs::FinishUpdate()
{
	update_listwindow = false;
	update_detailwindow = false;
}

void Prefs::LoadPrefs()
{
	if (prefs != NULL) {
		if (prefs->FindFloat("fontsize", &active_prefs.fontsize) != B_OK)
			active_prefs.fontsize = 25;
		if (prefs->FindFloat("numcols", &active_prefs.numcols) != B_OK)
			active_prefs.numcols = 3;
		if (prefs->FindInt32("drawheights", &active_prefs.drawheights) != B_OK)
			active_prefs.drawheights = false;
		if (prefs->FindInt32("drawborder", &active_prefs.drawborder) != B_OK)
			active_prefs.drawborder = true;
		if (prefs->FindInt32("autoadjust", &active_prefs.autoadjust) != B_OK)
			active_prefs.autoadjust = false;
		if (prefs->FindInt32("splashscreen", &active_prefs.splashscreen) != B_OK)
			active_prefs.splashscreen = true;
		if (prefs->FindInt32("liveupdate", &active_prefs.liveupdate) != B_OK)
			active_prefs.liveupdate = false;

		const char *dtext;
		if (prefs->FindString("displaytext", &dtext) == B_OK)
			active_prefs.displaytext = new BString(dtext);
		else
			active_prefs.displaytext = new BString("Love is all around!\0");
		
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
		status = prefs->FindData("mselectcolor", B_RGB_COLOR_TYPE, (const void **)&col, &csize);
		active_prefs.mselect_color = (status == B_NO_ERROR) ? *col : def_mselect_color;
		status = prefs->FindData("pbgcolor", B_RGB_COLOR_TYPE, (const void **)&col, &csize);
		active_prefs.pbg_color = (status == B_NO_ERROR) ? *col : def_pbg_color;
		status = prefs->FindData("pdisplaycolor", B_RGB_COLOR_TYPE, (const void **)&col, &csize);
		active_prefs.pdisplay_color = (status == B_NO_ERROR) ? *col : def_pdisplay_color;
		status = prefs->FindData("pselectcolor", B_RGB_COLOR_TYPE, (const void **)&col, &csize);
		active_prefs.pselect_color = (status == B_NO_ERROR) ? *col : def_pselect_color;
		status = prefs->FindData("pstrokecolor", B_RGB_COLOR_TYPE, (const void **)&col, &csize);
		active_prefs.pstroke_color = (status == B_NO_ERROR) ? *col : def_pstroke_color;

		if (prefs->FindRect("mainrect", &active_prefs.mainrect) != B_OK)
			active_prefs.mainrect.Set(50, 50, 650, 485);
		if (prefs->FindRect("propsrect", &active_prefs.propsrect) != B_OK)
			active_prefs.propsrect.Set(50, 50, 450, 450);
		if (prefs->FindRect("prefsrect", &active_prefs.prefsrect) != B_OK)
			active_prefs.prefsrect.Set(50, 50, 480, 374);

		if (prefs->FindInt32("splitpanepos", &active_prefs.splitpanepos) != B_OK)
			active_prefs.splitpanepos = 200;
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
		prefs->SetInt32("splashscreen", active_prefs.splashscreen);
		prefs->SetInt32("liveupdate", active_prefs.liveupdate);
		prefs->SetString("displaytext", GetDisplayText()->String());
		prefs->SetData("mbgcolor", B_RGB_COLOR_TYPE, &active_prefs.mbg_color, sizeof(rgb_color));
		prefs->SetData("mdisplaycolor", B_RGB_COLOR_TYPE, &active_prefs.mdisplay_color, sizeof(rgb_color));
		prefs->SetData("minfocolor", B_RGB_COLOR_TYPE, &active_prefs.minfo_color, sizeof(rgb_color));
		prefs->SetData("mstrokecolor", B_RGB_COLOR_TYPE, &active_prefs.mstroke_color, sizeof(rgb_color));
		prefs->SetData("mheightscolor", B_RGB_COLOR_TYPE, &active_prefs.mheights_color, sizeof(rgb_color));
		prefs->SetData("mselectcolor", B_RGB_COLOR_TYPE, &active_prefs.mselect_color, sizeof(rgb_color));
		prefs->SetData("pbgcolor", B_RGB_COLOR_TYPE, &active_prefs.pbg_color, sizeof(rgb_color));
		prefs->SetData("pdisplaycolor", B_RGB_COLOR_TYPE, &active_prefs.pdisplay_color, sizeof(rgb_color));
		prefs->SetData("pselectcolor", B_RGB_COLOR_TYPE, &active_prefs.pselect_color, sizeof(rgb_color));
		prefs->SetData("pstrokecolor", B_RGB_COLOR_TYPE, &active_prefs.pstroke_color, sizeof(rgb_color));

		prefs->SetRect("mainrect", active_prefs.mainrect);
		prefs->SetRect("propsrect", active_prefs.propsrect);
		prefs->SetRect("prefsrect", active_prefs.prefsrect);

		prefs->SetInt32("splitpanepos", active_prefs.splitpanepos);
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

int32 Prefs::GetSplashScreen()
{
	return active_prefs.splashscreen;
}

void Prefs::SetSplashScreen(int32 ival)
{
	active_prefs.splashscreen = ival;
}

int32 Prefs::GetLiveupdate()
{
	return active_prefs.liveupdate;
}

void Prefs::SetLiveupdate(int32 ival)
{
	active_prefs.liveupdate = ival;
}

BString	*Prefs::GetDisplayText()
{
//_sPrintf("GetDisplayText %d %s\n", index, displaytext[index]->String());
	return active_prefs.displaytext;
}

void Prefs::SetDisplayText(BString *astring)
{
	active_prefs.displaytext->SetTo(astring->String());
}

void Prefs::SetDisplayText(const char *text)
{
	active_prefs.displaytext->SetTo(text);
}

rgb_color Prefs::GetMBgColor()
{
	return active_prefs.mbg_color;
}

rgb_color Prefs::GetMBgDefault()
{
	return def_mbg_color;
}

void Prefs::SetMBgColor(rgb_color *col)
{
	active_prefs.mbg_color = *col;
}

rgb_color Prefs::GetMDisplayColor()
{
	return active_prefs.mdisplay_color;
}

rgb_color Prefs::GetMDisplayDefault()
{
	return def_mdisplay_color;
}

void Prefs::SetMDisplayColor(rgb_color *col)
{
	active_prefs.mdisplay_color = *col;
}

rgb_color Prefs::GetMInfoColor()
{
	return active_prefs.minfo_color;
}

rgb_color Prefs::GetMInfoDefault()
{
	return def_minfo_color;
}

void Prefs::SetMInfoColor(rgb_color *col)
{
	active_prefs.minfo_color = *col;
}

rgb_color Prefs::GetMStrokeColor()
{
	return active_prefs.mstroke_color;
}

rgb_color Prefs::GetMStrokeDefault()
{
	return def_mstroke_color;
}

void Prefs::SetMStrokeColor(rgb_color *col)
{
	active_prefs.mstroke_color = *col;
}

rgb_color Prefs::GetMHeightsColor()
{
	return active_prefs.mheights_color;
}

rgb_color Prefs::GetMHeightsDefault()
{
	return def_mheights_color;
}

void Prefs::SetMHeightsColor(rgb_color *col)
{
	active_prefs.mheights_color = *col;
}

rgb_color Prefs::GetMSelectColor()
{
	return active_prefs.mselect_color;
}

rgb_color Prefs::GetMSelectDefault()
{
	return def_mselect_color;
}

void Prefs::SetMSelectColor(rgb_color *col)
{
	active_prefs.mselect_color = *col;
}

rgb_color Prefs::GetPBgColor()
{
	return active_prefs.pbg_color;
}

rgb_color Prefs::GetPBgDefault()
{
	return def_pbg_color;
}

void Prefs::SetPBgColor(rgb_color *col)
{
	active_prefs.pbg_color = *col;
}

rgb_color Prefs::GetPDisplayColor()
{
	return active_prefs.pdisplay_color;
}

rgb_color Prefs::GetPDisplayDefault()
{
	return def_pdisplay_color;
}

void Prefs::SetPDisplayColor(rgb_color *col)
{
	active_prefs.pdisplay_color = *col;
}

rgb_color Prefs::GetPSelectColor()
{
	return active_prefs.pselect_color;
}

rgb_color Prefs::GetPSelectDefault()
{
	return def_pselect_color;
}

void Prefs::SetPSelectColor(rgb_color *col)
{
	active_prefs.pselect_color = *col;
}

rgb_color Prefs::GetPStrokeColor()
{
	return active_prefs.pstroke_color;
}

rgb_color Prefs::GetPStrokeDefault()
{
	return def_pstroke_color;
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

