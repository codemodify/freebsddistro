/* procbar.h */

#ifndef PROCBAR_H
#define PROCBAR_H

#include "xwindow.h"

class procbar : public QWidget
{
	int bwidth;                   // button width  
	typedef struct 
	{
		xwindow *win;
		QPixmap *pixmap;      // max sized button image
		QPushButton *button;  // button
	} WINLIST;
	QList <WINLIST> wlist;        // list of visible buttons

	QPixmap *make_pixmap(const QString &txt, QPixmap *pix);  // create max sized button image  
	bool remove_fg(void);         // remove buttons for windows in foreground
	void draw_button(WINLIST *);
	void draw_buttons(void);      // draw buttons on bar

protected:
	virtual void resizeEvent(QResizeEvent *);
	virtual void paletteChange(const QPalette &);
	
public:
	procbar(QWidget *parent=NULL, const char *name=NULL);
	void add(xwindow *win);       // add button
	void set_on(xwindow *win);    // set button to on state
	void remove(xwindow *win);    // remove from bar
	void remove_all(void);        // remove all buttons
	void change_palette(const QPalette &, xwindow *); // change button palette
	void change_text(QString *icname, xwindow *win);  // change icon text
	~procbar(void);
};
#endif
