#include <Font.h>
#include <List.h>
#include <Rect.h>
#include <stdio.h>
#include <string.h>

#ifndef FONTLIST_H
#define FONTLIST_H

struct fnode
{
	uint32			fsysvalue;
	int32			fileformat, listpos;
	uint8			encoding;
	bool			fixed, tuned, selected;
	BRect			displayrect;
	font_family		family;
	font_style		style;
	uint16			face;
};

class FontList: public BList
{
public:
			FontList();
			~FontList();
	bool	Init(bool = true);
	void	Empty();
	int32	CountFonts();
	void	AddFont(struct fnode*);
	fnode	*Get(font_family*, font_style*);
	fnode	*GetNext(struct fnode*);
	fnode	*FontAt(int32);
	fnode	*FirstFont();
	int32	GetFirstInView();
	void	SetFirstInView(int32);
	int32	GetSelected();
	void	SetSelected(int32);

protected:
	void	Build();

private:
	int32	selected, leader;
};

#endif /* FONTLIST_H */