#ifndef TOOLBAR_H
#define TOOLBAR_H

#include "pager/pager.h"
#include "winlist/winlist.h"
#include "menu/menu.h"
#include "procbar/procbar.h"
#include "appbar/apbar.h"

extern pager   *tb_pg;     // pager
extern winlist *tb_wl;     // winlist
extern menu    *tb_mn;     // menu
extern procbar *tb_pb;     // procbar
extern apbar   *tb_ap;     // apbox

#include <QtGui/QFrame>

class Toolbar : public QFrame
{
	QBoxLayout *layout;
	void addsep(void);              // add separator

public:
	Toolbar(QWidget *parent=0);

};


#endif
