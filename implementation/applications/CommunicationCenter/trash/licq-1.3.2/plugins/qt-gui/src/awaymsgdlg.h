#ifndef AWAYMSG_H
#define AWAYMSG_H

#include "licqdialog.h"

class QPushButton;
class QComboBox;
class MLEditWrap;

class AwayMsgDlg : public LicqDialog
{
  Q_OBJECT
public:
  AwayMsgDlg(QWidget *parent = 0);
  ~AwayMsgDlg();
  void SelectAutoResponse(unsigned short status, bool autoclose = false);

protected:
  MLEditWrap *mleAwayMsg;
  QPopupMenu* mnuSelect;
  QPushButton* btnSelect;
  QPushButton *btnOk, *btnCancel;
  unsigned short m_nStatus;
  static QPoint snPos;
  short m_nSAR;
  int m_autocloseCounter;

  void closeEvent(QCloseEvent *);
  bool eventFilter(QObject *, QEvent *);

signals:
  void popupOptions(int);
  void done();

protected slots:
  void ok();
  virtual void reject();
  void slot_selectMessage(int);
  void slot_hints();
  void slot_autocloseTick();
  void slot_autocloseStop();
};

class CustomAwayMsgDlg : public LicqDialog
{
  Q_OBJECT
public:
  CustomAwayMsgDlg(unsigned long, QWidget *parent = 0);
  CustomAwayMsgDlg(const char *, unsigned long, QWidget *parent = 0);

protected:
  MLEditWrap *mleAwayMsg;
  unsigned long m_nUin;
  char *m_szId;
  unsigned long m_nPPID;

protected slots:
  void slot_clear();
  void slot_ok();
  void slot_hints();
};


#endif

