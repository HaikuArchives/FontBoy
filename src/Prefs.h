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
				bool	CanRevert(void);
				void	Default(void);
				bool	IsDefault();
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
				BString	GetDisplayText();
				void	SetDisplayText(BString astring);
				void	SetDisplayText(const char *text);
			rgb_color	GetMBgColor(bool selected = false);
				void	SetMBgColor(rgb_color *col);
			rgb_color	GetMDisplayColor(bool selected = false);
				void	SetMDisplayColor(rgb_color *col);
			rgb_color	GetMInfoColor(bool selected = false);
				void	SetMInfoColor(rgb_color *col);
			rgb_color	GetMStrokeColor();
				void	SetMStrokeColor(rgb_color *col);
			rgb_color	GetMHeightsColor(bool selected = false);
				void	SetMHeightsColor(rgb_color *col);
				void	SetMSelectedBgColor(rgb_color *col);
				void	SetMSelectedDisplayColor(rgb_color *col);
				void	SetMSelectedInfoColor(rgb_color *col);
				void	SetMSelectedHeightsColor(rgb_color *col);
			rgb_color	GetPBgColor(bool selected = false);
				void	SetPBgColor(rgb_color *col);
			rgb_color	GetPDisplayColor(bool selected = false);
				void	SetPDisplayColor(rgb_color *col);
			rgb_color	GetPStrokeColor();
				void	SetPStrokeColor(rgb_color *col);
				void	SetPSelectedBgColor(rgb_color *col);
				void	SetPSelectedDisplayColor(rgb_color *col);
			rgb_color	GetPInactiveSelectedBgColor();
			rgb_color	GetPInactiveSelectedDisplayColor();
				void	SetPInactiveSelectedBgColor(rgb_color *col);
				void	SetPInactiveSelectedDisplayColor(rgb_color *col);
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
			BString		displaytext;

			float		numcols,					// Numer of columns
						rowheight;					// calculated rowheight
			
			float		fontsize;					// Size of displayed fonts

			int32		drawheights,				// Draw Hieghtlines
						autoadjust,					// Automatic adjustment of columns
						drawborder,					// Draw Borders
						splitpanepos;				// Position of splitpane

			rgb_color	mbg_color,					// Backgroundcolor mainwindow
						mdisplay_color,				// Fontccolor mainwindow
						minfo_color,				// Infotextcolor mainwindow
						mstroke_color,				// Strokecolor mainwindow
						mheights_color,				// Heightbarcolor mainwindow
						
						mselectedbg_color,			// Selected backgroundcolor mainwindow
						mselecteddisplay_color,		// Selected fontccolor mainwindow
						mselectedinfo_color,		// Selected infotextcolor mainwindow
						mselectedheights_color,		// Selected heightbarcolor mainwindow
						
						pbg_color,					// Backgroundcolor charsetwindow
						pdisplay_color,				// Fontcolor charsetwindow
						pselect_color,				// Fontselectcolor charsetwindow
						pstroke_color,				// Strokecolor charsetwindow
						pselectedbg_color,			// Selected backgroundcolor charsetwindow
						pselecteddisplay_color,		// Selected fontcolor charsetwindow
						pinactiveselectedbg_color,	// Inactive selected bgcolor 
						pinactiveselecteddisplay_color;	// Inactive selected fontcolor
													

			BRect		mainrect,					// Position & size of mainwindow
						mviewrect,					// Position & size of mainview
						propsrect,					// Position & size of propertieswindow
						prefsrect;					// Position & size of preferenceswindow
		};
		bool			_CompareUserPrefs(prefs_data& prefs1, prefs_data& prefs2);
		void			_CopyUserPrefs(prefs_data& prefs1, prefs_data& prefs2);
		
		prefs_data		active_prefs;
		prefs_data		default_prefs;
		prefs_data		revert_prefs;
};


#endif
