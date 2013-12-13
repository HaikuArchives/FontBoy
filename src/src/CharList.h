#include <Font.h>
#include <List.h>
#include <String.h>
#include <Rect.h>
#include <ByteOrder.h>
#include <UTF8.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#ifndef CharList_H
#define CharList_H

enum {
	CL_BUFFERED	= 0x0001,
	CL_HASGLYPH	= 0x0002,
	CL_SELECTED	= 0x0004
};

struct cnode
{
	uint16	unicode;
	uint8	flag;
};

class CharList: public BList
{
public:
			CharList(const BFont&);
			~CharList();
	cnode	*GetNext(struct cnode*);
	cnode	*CharAt(uint16);
	cnode	*FirstChar();
	void	PrepareRange(uint16, uint16);

protected:
	bool	Init(bool = true);
	void	SetTargetFont(const BFont&);
	void	Empty();
	void	AddChar(struct cnode*);
	void	Build();

private:
	int32	selected;

	BFont	fFont;
};

#endif /* CharList_H */