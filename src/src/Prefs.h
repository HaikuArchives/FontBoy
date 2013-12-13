//
// FontboyPrefs
// Save and synchronize Settings
//

#ifndef __GEOMETRIE_H__
#define __GEOMETRIE_H__

#include <Font.h>
#include <Path.h>
#include <Message.h>
#include <GraphicsDefs.h>
#include <stdio.h>

#include "ConstColors.h"

class Prefs
{
	public:
		enum
		{
			AREA_ALL = 1,
			AREA_GENERAL,
			AREA_LIST,
			AREA_DETAILS
		};

						Prefs(void);
						~Prefs(void);
		status_t		InitCheck(void);
	
				void	LoadPrefs(void);
				void	SavePrefs(void);
				void	Apply(void);
				void	Revert(void);
				void	Update(void);
				void	FinishUpdate(void);
				float	GetFontSize(void);
				void	SetFontSize(float fval);
				float	GetRowHeight();
				void	SetRowHeight(float fval);
				float	GetNumCols();
				void	SetNumCols(float fval);
				int32	GetDrawHeights();
				void	SetDrawHeights(int32 ival);
				int32	GetAutoAdjust();
				void	SetAutoAdjust(int32 ival);
				int32	GetDrawBorder();
				void	SetDrawBorder(int32 ival);
				int32	GetSplashScreen();
				void	SetSplashScreen(int32 ival);
				int32	GetToolTips();
				void	SetToolTips(int32 ival);
				int32	GetLiveupdate();
				void	SetLiveupdate(int32 ival);
				BString	*GetDisplayText();
				void	SetDisplayText(BString *astring);
				void	SetDisplayText(const char *text);
			rgb_color	GetMBgColor();
			rgb_color	GetMBgDefault();
				void	SetMBgColor(rgb_color *col);
			rgb_color	GetMDisplayColor();
			rgb_color	GetMDisplayDefault();
				void	SetMDisplayColor(rgb_color *col);
			rgb_color	GetMInfoColor();
			rgb_color	GetMInfoDefault();
				void	SetMInfoColor(rgb_color *col);
			rgb_color	GetMStrokeColor();
			rgb_color	GetMStrokeDefault();
				void	SetMStrokeColor(rgb_color *col);
			rgb_color	GetMHeightsColor();
			rgb_color	GetMHeightsDefault();
				void	SetMHeightsColor(rgb_color *col);
			rgb_color	GetMSelectColor();
			rgb_color	GetMSelectDefault();
				void	SetMSelectColor(rgb_color *col);
			rgb_color	GetPBgColor();
			rgb_color	GetPBgDefault();
				void	SetPBgColor(rgb_color *col);
			rgb_color	GetPDisplayColor();
			rgb_color	GetPDisplayDefault();
				void	SetPDisplayColor(rgb_color *col);
			rgb_color	GetPSelectColor();
			rgb_color	GetPSelectDefault();
				void	SetPSelectColor(rgb_color *col);
			rgb_color	GetPStrokeColor();
			rgb_color	GetPStrokeDefault();
				void	SetPStrokeColor(rgb_color *col);
				void	SetListWindowForUpdate(void);
				bool	IsListWindowForUpdate(void);
				void	SetDetailWindowForUpdate(void);
				bool	IsDetailWindowForUpdate(void);

				void	GetMainRect(BRect &arect);
				void	SetMainRect(BRect *arect);
				void	GetMViewRect(BRect &arect);
				void	SetMViewRect(BRect *arect);
				void	GetPropsRect(BRect &arect);
				void	SetPropsRect(BRect *arect);
				void	GetPrefsRect(BRect &arect);
				void	SetPrefsRect(BRect *arect);
				int		GetSplitPanePos(void);
				void	SetSplitPanePos(int pos);

	static const float	FM_ROWHEIGHT_BUFFER = 12;


	private:
		TPreferences	*prefs;
				bool	update_listwindow, update_detailwindow;

		struct prefs_data
		{
			BString		*displaytext;

			float		numcols,					// Numer of columns
						rowheight;					// calculated rowheight
			
			float		fontsize;					// Size of displayed fonts

			int32		drawheights,				// Draw Hieghtlines
						autoadjust,					// Automatic adjustment of columns
						drawborder,					// Draw Borders
						splashscreen,				// Show splashscreen
						tooltips,					// Show tooltips
						liveupdate,					// Use Liveupdate
						splitpanepos;				// Position of splitpane

			rgb_color	mbg_color,					// Backgroundcolor mainwindow
						mdisplay_color,				// Fontccolor mainwindow
						minfo_color,				// Infotextcolor mainwindow
						mstroke_color,				// Strokecolor mainwindow
						mheights_color,				// Heightbarcolor mainwindow
						mselect_color,				// Fontselectcolor mainwindow

						pbg_color,					// Backgroundcolor charsetwindow
						pdisplay_color,				// Fontcolor charsetwindow
						pselect_color,				// Fontselectcolor charsetwindow
						pstroke_color;				// Strokecolor charsetwindow

			BRect		mainrect,					// Position & size of mainwindow
						mviewrect,					// Position & size of mainview
						propsrect,					// Position & size of propertieswindow
						prefsrect;					// Position & size of preferenceswindow
		};

		prefs_data		active_prefs;
};


#endif
