//CharList
#include "CharList.h"

CharList::CharList(const BFont& font)
		  :BList(256*256)
{
	SetTargetFont(font);
//	Init(true);
	selected = -1;
}

CharList::~CharList() {
	Empty();
}

bool CharList::Init(bool force)
{
//	check for change in Systems CharList
	if ((force == true) || (update_font_families(false))) {
		Empty();
		Build();
		return true;
	}
	return false;
}

void CharList::SetTargetFont(const BFont& font)
{
	fFont = font;
	Build();
}


void CharList::Empty() 
{
	void *item; 

	int i = CountItems()-1;
	while(i >= 0) {
		item = RemoveItem(i);
		if(item)
			delete (struct cnode *)item;
		i--;
	}
}


// Fills up the BList with the Unicode Characterset
void CharList::Build() 
{
	BString	str;
	cnode	*node;
	
	for (int32 i=0; i<256; i++) {
		for (int32 j=0; j<256; j+=1) {
			node = new cnode;
			node->unicode = 0;
			node->unicode = B_HOST_TO_BENDIAN_INT16((i+j == 0) ? 1 : uint16(i)*256+uint16(j));
			node->flag = 0;
			AddChar(node);
		}
	}
}

void CharList::PrepareRange(uint16 start, uint16 count)
{
	char	*utf8Chars = NULL;
	uint16	*uniChars = NULL;
	cnode	*node = NULL;

	// check if prepare is necessary
	bool buff = true;
	for (int32 i=0; i<count; i++) {
		if ((node = CharAt(start+i)) != NULL)
			buff &= (node->flag & CL_BUFFERED) ? true : false;
	}

	// buffer items in given range
	if (!buff) {
		int32 utf8Len = count*2;
		int32 uniLen = count*3+1;
	
		if ((utf8Chars = (char *) malloc(utf8Len)) != NULL) {
			if ((uniChars = (uint16 *) malloc(uniLen)) != NULL) {
	
				for (int32 i=0; i<count; i++) {
					node = CharAt(start+i);
					uniChars[i] = node->unicode;
				}
			
				int32 state = 0;
				convert_to_utf8(B_UNICODE_CONVERSION, (const char*)uniChars, &uniLen,
								utf8Chars, &utf8Len, &state);
				utf8Chars[utf8Len] = 0;
			
				bool hasGlyph[utf8Len];
				fFont.GetHasGlyphs(utf8Chars, utf8Len, hasGlyph);
			
				for (int32 i=0; i<count; i++) {
					node = CharAt(start+i);
					if (node != NULL) {
						node->flag |= CL_BUFFERED;
						if (hasGlyph[i]) {
							node->flag |= CL_HASGLYPH;
						}
					}
				}
				free(uniChars);
			}
			free(utf8Chars);
		}
	}
}

void CharList::AddChar(struct cnode *fnptr) 
{
	AddItem((void *)fnptr);
}

cnode* CharList::CharAt(uint16 index) 
{
	cnode *node = (cnode *)ItemAt(index);
	return node;
}

cnode* CharList::FirstChar() 
{
	cnode *node = (cnode *)FirstItem();
	return node;
}

cnode* CharList::GetNext(cnode* node) 
{
	cnode *node2 = CharAt(IndexOf(node)+1);
	return node2;
}
