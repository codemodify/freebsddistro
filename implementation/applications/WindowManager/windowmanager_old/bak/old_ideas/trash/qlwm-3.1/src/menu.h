/* menu.h */

#ifndef MENU_H
#define MENU_H

class menu : public QPushButton
{
	Q_OBJECT
	
	QPopupMenu *basemenu;
	QIntDict <QString> mdict;     // menu item id's
	QList <QPopupMenu> mlist;     // submenue list for deletion
	bool killop;                  // window kill operation active
	QDateTime lmtime;             // last file modification time
	static uchar kcursor_bits[];  // cursor bitmap
	static uchar kcursor_mask[];  // cursor bitmap

	void recurse(QPopupMenu *, QFile &);
	void winkill_all();
	
private slots:	
	void run_cmd(int);          // start command with menu id
	void winkill(void);

protected:
	virtual void mousePressEvent(QMouseEvent *);

public:
	menu(QWidget *parent, const char *name);
	void start_popup(void);
	void readmenu(void);
};
#endif
