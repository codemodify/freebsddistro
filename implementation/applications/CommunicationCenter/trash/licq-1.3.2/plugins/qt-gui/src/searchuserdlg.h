#ifndef SEARCHUSERDLG_H
#define SEARCHUSERDLG_H

/*
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/

// Search user code base written by Alan Penner (apenner@andrew.cmu.edu)
// modified by Graham Roff && Dirk A. Mueller <dmuell@gmx.net>

#include <qwidget.h>
#include <qlistview.h>

#include "mainwin.h"

class QTabWidget;
class QCheckBox;
class QLabel;
class QLineEdit;
class QComboBox;
class QSpinBox;
class QCheckBox;

class CICQDaemon;
class CSignalManager;
class ICQEvent;
class ICQUser;
class CSearchAck;

//TODO for protocol plugin support

class SearchUserView : public QListView
{
  Q_OBJECT

public:
   SearchUserView(QWidget *parent = 0);
};


class SearchItem : public QListViewItem
{
public:
   SearchItem(CSearchAck *, const QString &, QListView *parent);
   unsigned long uin();
protected:
   unsigned long uinVal;
};



class SearchUserDlg : public QWidget
{
   Q_OBJECT
public:
   SearchUserDlg (CMainWindow *mainwin, CICQDaemon *s, CSignalManager *theSigMan, const QString &);
   ~SearchUserDlg();

private:
   bool uin_search;

protected:

   CMainWindow *mainwin;
   CICQDaemon *server;
   CSignalManager *sigman;
   QPushButton *btnSearch, *btnReset;
   QCheckBox* qcbAlertUser;
   QTabWidget* search_tab;

   QLabel *lblSearch;
   QLineEdit *edtEmail, *edtKeyword, *edtFirst, *edtLast, *edtNick, *edtUin,
     *edtCity, *edtState, *edtCoName, *edtCoDept, *edtCoPos;
   QComboBox *cmbAge, *cmbCountry, *cmbGender, *cmbLanguage;
   
   QWidget *whitepages_tab, *uin_tab;
   QCheckBox *chkOnlineOnly;

   unsigned long searchTag;
   QPushButton *btnDone, *btnInfo, *btnAdd;
   SearchUserView *foundView;
   QString m_Encoding;

   void searchFound(CSearchAck *);
   void searchDone(CSearchAck *);
   void searchFailed();

public slots:
   void startSearch();
   void viewInfo();
   void addUser();
   void resetSearch();
   void searchResult(ICQEvent *);
   void selectionChanged();
};


#endif
