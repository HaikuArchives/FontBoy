//FontList
#include "FontList.h"

FontList::FontList()
		  :BList((int32)count_font_families())
{
	Init(true);
	leader = 0;
	selected = -1;
}

FontList::~FontList() {
	Empty();
}

bool FontList::Init(bool force)
{
//	check for change in Systems fontlist
	if ((force == true) || (update_font_families(false))) {
		Empty();
		update_font_families(true);
		Build();
		return true;
	}
	return false;
}


void FontList::Empty() 
{
	void *item; 

	int i = CountItems()-1;
	while(i >= 0) {
		item = RemoveItem(i);
		if(item)
			delete (struct fnode *)item;
		i--;
	}
}


void FontList::Build() 
{
	BFont		font;
	font_family	family;
	font_style	style;

// build linked FontList
	int32 numFamilies = count_font_families();
	int32 cnt = 0;
	for (int32 i = 0; i < numFamilies; i++) {
		uint32 flags;
		if (get_font_family(i, &family, &flags) == B_OK) {
			int32 numStyles = count_font_styles(family);
			for (int32 j = 0; j < numStyles; j++) {
				if (get_font_style(family, j, &style, &flags) == B_OK) {
					struct fnode *t = new fnode;
					font.SetFamilyAndStyle(family, style);
					strcpy(t->family, family);
					strcpy(t->style, style);
					t->face = font.Face();
					t->fsysvalue = font.FamilyAndStyle();
					t->fileformat = font.FileFormat();
					t->encoding = font.Encoding();
					if (flags & B_HAS_TUNED_FONT)
						t->tuned = true;
					if (flags & B_IS_FIXED)
						t->fixed = true;
					t->selected = false;
					t->listpos = cnt++;
					AddFont(t);
				}
			}
		}
	}
	printf("Fonts found: %ld\n", CountItems());
}

int32 FontList::CountFonts() 
{
	return CountItems();
}

void FontList::AddFont(struct fnode *fnptr) 
{
	AddItem((void *)fnptr);
}

fnode* FontList::FontAt(int32 index) 
{
	if (index == -1)
		return NULL;

	fnode *node = (fnode *)ItemAt(index);
	return node;
}

fnode* FontList::FirstFont() 
{
	fnode *node = (fnode *)FirstItem();
	return node;
}

fnode* FontList::Get(font_family *family, font_style *style) 
{
	
	fnode* fn = (fnode *)FirstItem();
	int32 i = IndexOf(fn);

	while (fn != NULL) {
		if ((strncasecmp((char*)fn->family, (char*)family, 64) == 0)
		 && (strncasecmp((char*)fn->style,  (char*)style,  64) == 0))
			break;

		fn = (fnode *)ItemAt(++i);
	}

	return fn;
}

fnode* FontList::GetNext(fnode* node) 
{
	fnode *node2 = FontAt(IndexOf(node)+1);
	return node2;
}

int32 FontList::GetFirstInView() 
{
	return leader;
}

void FontList::SetFirstInView(int32 index) 
{
	leader = index;
	if (leader > CountFonts())
		leader = CountFonts();
}

int32 FontList::GetSelected() 
{
	return selected;
}

void FontList::SetSelected(int32 index) 
{
	fnode *node;
	
	if (selected != index) {
		node = FontAt(selected);

		if (node != NULL)
			node->selected = false;

		node = FontAt(index);
		if (node != NULL)
			node->selected = true;

		selected = index;
	}
}
