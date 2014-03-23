// -*- c-basic-offset: 2 -*-
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef USE_KDE
#include <kapp.h>
#include <kglobal.h>
#include <kglobalsettings.h>
#include <kwin.h>
#include <kiconloader.h>
#include <kurl.h>
#if KDE_VERSION >= 310
#include <kpassivepopup.h>
#endif
#else
#include <qapplication.h>
#endif
#include <qtextcodec.h>

#include <qaccel.h>
#include <qimage.h>
#include <qdatetime.h>
#include <qdir.h>
#include <qclipboard.h>
#include <qlayout.h>
#include <qtextview.h>
#include <qpainter.h>
#include <qtabwidget.h>
#if QT_VERSION >= 300
#include <qstylefactory.h>
#else
#include <qwindowsstyle.h>
#endif

#include "licqgui.h"
#include "mainwin.h"
#include "licq_icq.h"
#include "licq_sar.h"
#include "gui-defines.h"
#include "licq_log.h"
#include "licq_translate.h"
#include "licq_utility.h"
#include "adduserdlg.h"
#include "authuserdlg.h"
#include "editgrp.h"
#include "searchuserdlg.h"
#include "utilitydlg.h"
#include "ownermanagerdlg.h"
#include "skinbrowser.h"
#include "licq_icqd.h"
#include "awaymsgdlg.h"
#include "outputwin.h"
#include "ewidgets.h"
#include "sigman.h"
#include "showawaymsgdlg.h"
#include "optionsdlg.h"
#include "skin.h"
#include "securitydlg.h"
#include "userselectdlg.h"
#include "plugindlg.h"
#include "randomchatdlg.h"
#include "userinfodlg.h"
#include "usereventdlg.h"
#include "reqauthdlg.h"
#include "wharf.h"
#include "keyrequestdlg.h"
#include "usercodec.h"
#include "emoticon.h"
#include "ownermanagerdlg.h"

#ifdef USE_KDE
    #include "licqkimiface.h"
    #include "dcopclient.h"
#endif

#ifdef HAVE_LIBGPGME
#include "gpgkeyselect.h"
#include "gpgkeymanager.h"
#endif

#include "xpm/history.xpm"
#include "xpm/info.xpm"
#include "xpm/remove.xpm"
#include "xpm/search.xpm"
#include "xpm/secure_on.xpm"
#include "xpm/secure_off.xpm"
#include "xpm/charset.xpm"
#include "xpm/itemCollapsed.xpm"
#include "xpm/itemExpanded.xpm"
#include "xpm/pixCustomAR.xpm"
#include "xpm/pixPhone.xpm"
#include "xpm/pixCellular.xpm"
#include "xpm/pixBirthday.xpm"
#include "xpm/pixInvisible.xpm"
#include "xpm/pixKeyEnabled.xpm"
#include "xpm/pixKeyDisabled.xpm"
#include "xpm/pixTyping.xpm"
#include "xpm/pixICQphoneActive.xpm"
#include "xpm/pixICQphoneBusy.xpm"
#include "xpm/pixPhoneFollowMeActive.xpm"
#include "xpm/pixPhoneFollowMeBusy.xpm"
#include "xpm/pixSharedFiles.xpm"

#include "licq_qt-gui.conf.h"

extern "C" {

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#ifdef USE_SCRNSAVER
#include <X11/extensions/scrnsaver.h>
#endif

}

#include <cctype>

using std::isdigit;

#undef Bool
#undef None
#undef KeyPress
#undef KeyRelease
#undef FocusIn
#undef FocusOut
#undef Status

static QPixmap *ScaleWithBorder(const QPixmap &pm, int w, int h, struct Border border)
{
   QPainter p;

   // top left corner
   QPixmap pmTL(border.left, border.top);
   p.begin(&pmTL);
   p.drawPixmap(0, 0, pm, 0, 0, pmTL.width(), pmTL.height());
   p.end();

   // top border
   QPixmap pmT(pm.width() - border.left - border.right, border.top);
   p.begin(&pmT);
   p.drawPixmap(0, 0, pm, border.left, 0, pmT.width(), pmT.height());
   p.end();
   QImage imT( (pmT.convertToImage()).smoothScale(w - border.left - border.right, pmT.height()) );

   // top right corner
   QPixmap pmTR(border.right, border.top);
   p.begin(&pmTR);
   p.drawPixmap(0, 0, pm, pm.width() - border.right, 0, pmTR.width(), pmTR.height());
   p.end();

   // left border
   QPixmap pmL(border.left, pm.height() - border.top - border.bottom);
   p.begin(&pmL);
   p.drawPixmap(0, 0, pm, 0, border.top, pmL.width(), pmL.height());
   p.end();
   QImage imL( (pmL.convertToImage()).smoothScale(pmL.width(), h - border.top - border.bottom) );

   // center
   QPixmap pmC(pmT.width(), pmL.height());
   p.begin(&pmC);
   p.drawPixmap(0, 0, pm, border.left, border.top, pmC.width(), pmC.height());
   p.end();
   QImage imC( (pmC.convertToImage()).smoothScale(imT.width(), imL.height()) );

   // right border
   QPixmap pmR(border.right, pm.height() - border.top - border.bottom);
   p.begin(&pmR);
   p.drawPixmap(0, 0, pm, pm.width() - border.right, border.top, pmR.width(), pmR.height());
   p.end();
   QImage imR ( (pmR.convertToImage()).smoothScale(pmR.width(), h - border.top - border.bottom) );

   // bottom left border
   QPixmap pmBL(border.left, border.bottom);
   p.begin(&pmBL);
   p.drawPixmap(0, 0, pm, 0, pm.height() - border.bottom, pmBL.width(), pmBL.height());
   p.end();

   // bottom border
   QPixmap pmB(pm.width() - border.left - border.right, border.bottom);
   p.begin(&pmB);
   p.drawPixmap(0, 0, pm, border.left, pm.height() - border.bottom, pmB.width(), pmB.height());
   p.end();
   QImage imB( (pmB.convertToImage()).smoothScale(w - border.left - border.right, pmB.height()) );

   // bottom right border
   QPixmap pmBR(border.right, border.bottom);
   p.begin(&pmBR);
   p.drawPixmap(0, 0, pm, pm.width() - border.right, pm.height() - border.bottom, pmBR.width(), pmBR.height());
   p.end();

   // put the image together
   QPixmap *pmFinal = new QPixmap(w, h, pm.depth());
   p.begin(pmFinal);
   p.drawPixmap(0, 0, pmTL, 0, 0, -1, -1);
   p.drawImage(border.left, 0, imT, 0, 0, -1, -1);
   p.drawPixmap(pmFinal->width() - border.right, 0, pmTR, 0, 0, -1, -1);
   p.drawImage(0, border.top, imL, 0, 0, -1, -1);
   p.drawImage(pmFinal->width() - border.right, border.top, imR, 0, 0, -1, -1);
   p.drawPixmap(0, pmFinal->height() - border.bottom, pmBL, 0, 0, -1, -1);
   p.drawImage(border.left, pmFinal->height() - border.bottom, imB, 0, 0, -1, -1);
   p.drawPixmap(pmFinal->width() - border.right, pmFinal->height() - border.bottom, pmBR, 0, 0, -1, -1);
   p.drawImage(border.left, border.top, imC, 0, 0, -1, -1);
   p.end();

   return (pmFinal);
}


QPixmap& CMainWindow::iconForStatus(unsigned long Status, const char *szId,
  unsigned long nPPID)
{
  bool bAIM = (nPPID == LICQ_PPID) && (!isdigit(szId[0]));

  if((unsigned short) Status != ICQ_STATUS_OFFLINE && (Status & ICQ_STATUS_FxPRIVATE)
     && !gMainWindow->m_bShowExtendedIcons)
     if (nPPID == MSN_PPID)
       return gMainWindow->pmMSNPrivate;
     else
       return gMainWindow->pmPrivate;
       
  if ((unsigned short) Status == ICQ_STATUS_OFFLINE)
    if (nPPID == MSN_PPID)
      return gMainWindow->pmMSNOffline;
    else if (bAIM)
      return gMainWindow->pmAIMOffline;
    else
      return gMainWindow->pmOffline;

  if (Status & ICQ_STATUS_DND)
    if (nPPID == MSN_PPID)
      return gMainWindow->pmMSNOccupied;
    else if (bAIM)
      return gMainWindow->pmAIMAway;
    else
      return gMainWindow->pmDnd;

  if (Status & ICQ_STATUS_OCCUPIED)
    if (nPPID == MSN_PPID)
      return gMainWindow->pmMSNOccupied;
    else if (bAIM)
      return gMainWindow->pmAIMAway;
    else
      return gMainWindow->pmOccupied;

  if (Status & ICQ_STATUS_NA)
    if (nPPID == MSN_PPID)
      return gMainWindow->pmMSNAway;
    else if (bAIM)
      return gMainWindow->pmAIMAway;
    else
      return gMainWindow->pmNa;

  if (Status & ICQ_STATUS_AWAY)
    if (nPPID == MSN_PPID)
      return gMainWindow->pmMSNAway;
    else if (bAIM)
      return gMainWindow->pmAIMAway;
    else
      return gMainWindow->pmAway;

  if (Status & ICQ_STATUS_FREEFORCHAT) return gMainWindow->pmFFC;

  if (nPPID == MSN_PPID)
    return gMainWindow->pmMSNOnline;
  else if (bAIM)
    return gMainWindow->pmAIMOnline;
  else
    return gMainWindow->pmOnline;
}

QPixmap& CMainWindow::iconForEvent(unsigned short SubCommand)
{
  switch(SubCommand)
  {
  case ICQ_CMDxSUB_URL:
    return gMainWindow->pmUrl;
  case ICQ_CMDxSUB_CHAT:
    return gMainWindow->pmChat;
  case ICQ_CMDxSUB_FILE:
    return gMainWindow->pmFile;
  case ICQ_CMDxSUB_CONTACTxLIST:
    return gMainWindow->pmContact;
  case ICQ_CMDxSUB_AUTHxREQUEST:
  case ICQ_CMDxSUB_AUTHxREFUSED:
  case ICQ_CMDxSUB_AUTHxGRANTED:
    return gMainWindow->pmAuthorize;
  case ICQ_CMDxSUB_MSG:
  default:
    return gMainWindow->pmMessage;
  }
}


static XErrorHandler old_handler = 0;
static int licq_xerrhandler(Display* dpy, XErrorEvent* err)
{
  // XScreenSaverQueryInfo produces a BadDrawable error
  // if it cannot connect to the extension. This happens i.e. when
  // client runs on a 64bit machine and the server on a 32bit one.
  // We need to catch that here and tell the Xlib that we
  // ignore it, otherwise Qt's handler will terminate us. :-(
  if(err->error_code == BadDrawable)
    return 0;

  return (*old_handler)(dpy, err);
}

CMainWindow* gMainWindow = NULL;

//-----CMainWindow::constructor-------------------------------------------------
CMainWindow::CMainWindow(CICQDaemon *theDaemon, CSignalManager *theSigMan,
                         CQtLogWindow *theLogWindow, bool bStartHidden,
                         const char *skinName, const char *iconsName,
                         const char *extendedIconsName, bool bDisableDockIcon,
                         QWidget *parent)
  : QWidget(parent, "MainWindow")
{
  gMainWindow = this;
  licqDaemon = theDaemon;
  licqSigMan = theSigMan;
  licqLogWindow = theLogWindow;
  positionChanges = 0;
  m_szUserMenuId = 0;
  m_nProtoNum = 0;

  // Overwrite Qt's event handler
  old_handler = XSetErrorHandler(licq_xerrhandler);

  // allocating floating window vector
  if(CUserView::floaties == NULL)
      CUserView::floaties = new UserFloatyList;

  m_bDisableDockIcon = bDisableDockIcon;

  // set up appicon and docking, code supplied by Mark Deneed
  if (!m_bDisableDockIcon)
  {
    WId win = winId();     // get the window
    XWMHints *hints;  // hints
    Display *dsp = x11Display();  // get the display
    hints = XGetWMHints(dsp, win);  // init hints
    hints->window_group = win;  // set set the window hint
    hints->flags = WindowGroupHint;  // set the window group hint
    XSetWMHints(dsp, win, hints);  // set the window hints for WM to use.
    XFree( hints );
  }

  connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(slot_aboutToQuit()));

  // read in info from file
  char szTemp[MAX_FILENAME_LEN];

#ifdef USE_KDE
  gLog.Info("%sKDE GUI configuration.\n", L_INITxSTR);
#else
  gLog.Info("%sQt GUI configuration.\n", L_INITxSTR);
#endif
  snprintf(szTemp, MAX_FILENAME_LEN, "%s/licq_qt-gui.conf", BASE_DIR);
  szTemp[MAX_FILENAME_LEN - 1] = '\0';
  CIniFile licqConf;
  if (! licqConf.LoadFile(szTemp) )
  {
    FILE *f = fopen(szTemp, "w");
    fprintf(f, "%s", QTGUI_CONF);
    fclose(f);
    licqConf.LoadFile(szTemp);
  }

  licqConf.SetSection("appearance");
  QFont f;
  licqConf.ReadStr("Font", szTemp, "default");
#ifdef USE_KDE
  defaultFont = KGlobalSettings::generalFont();
#else
  defaultFont = qApp->font();
#endif
  if (strcmp(szTemp, "default") != 0)
  {
#if QT_VERSION >= 300
    f.fromString(szTemp);
#else
    f.setRawName(szTemp);
#endif
    qApp->setFont(f, true);
  }
  licqConf.ReadStr("EditFont", szTemp, "default");
  if(!strcmp(szTemp, "default"))
    f = qApp->font();
  else
#if QT_VERSION >= 300
    f.fromString(szTemp);
#else
    f.setRawName(szTemp);
#endif
  delete MLEditWrap::editFont;
  MLEditWrap::editFont = new QFont(f);

  licqConf.ReadBool("GridLines", m_bGridLines, false);
  licqConf.ReadBool("FontStyles", m_bFontStyles, true);
  licqConf.ReadBool("ShowHeader", m_bShowHeader, true);
  licqConf.ReadBool("ShowOfflineUsers", m_bShowOffline, true);
  licqConf.ReadBool("AlwaysShowONU", m_bAlwaysShowONU, true);
  licqConf.ReadBool("ShowDividers", m_bShowDividers, true);
  licqConf.ReadNum("SortByStatus", m_nSortByStatus, 1);
  licqConf.ReadNum("SortColumn", m_nSortColumn, 0);
  licqConf.ReadBool("SortColumnAscending", m_bSortColumnAscending, true);
  licqConf.ReadBool("ShowGroupIfNoMsg", m_bShowGroupIfNoMsg, true);
  licqConf.ReadBool("BoldOnMsg", m_bBoldOnMsg, true);
  licqConf.ReadBool("ManualNewUser", m_bManualNewUser, false);
  licqConf.ReadBool("UseThreadView", m_bThreadView, false);
  licqConf.ReadNum("TVGroupStates", m_nGroupStates, 0xFFFFFFFE);
  licqConf.ReadBool("ShowExtIcons", m_bShowExtendedIcons, true);
  licqConf.ReadBool("SystemBackground", m_bSystemBackground, false);
  licqConf.ReadBool("SendFromClipboard", m_bSendFromClipboard, true);
  licqConf.ReadBool("MsgChatView", m_bMsgChatView, true );
#if QT_VERSION < 300
  m_bTabbedChatting = false;
#else
  licqConf.ReadBool("TabbedChatting", m_bTabbedChatting, true);
#endif
  licqConf.ReadBool("ShowHistory", m_bShowHistory, true);
  licqConf.ReadBool("AutoPosReplyWin", m_bAutoPosReplyWin, true);
  licqConf.ReadBool("AutoSendThroughServer", m_bAutoSendThroughServer, false);
  licqConf.ReadBool("EnableMainwinMouseMovement", m_bEnableMainwinMouseMovement, true);
  licqConf.ReadNum("ChatMessageStyle", m_nMsgStyle, 0);
  licqConf.ReadBool("ChatAppendLinebreak", m_bAppendLineBreak, false);
  licqConf.ReadBool("FlashTaskbar", m_bFlashTaskbar, true);
  
  licqConf.ReadStr("ReceiveMessageColor", szTemp, "red");
  m_colorRcv = QColor(szTemp);
  licqConf.ReadStr("ReceiveHistoryColor", szTemp, "light pink");
  m_colorRcvHistory = QColor(szTemp);
  licqConf.ReadStr("SentMessageColor", szTemp, "blue");
  m_colorSnt = QColor(szTemp);
  licqConf.ReadStr("SentHistoryColor", szTemp, "light blue");
  m_colorSntHistory = QColor(szTemp);
  licqConf.ReadStr("TabFontColor", szTemp, 
      QApplication::palette(this).active().text().name());
  m_colorTab = QColor(szTemp);
  licqConf.ReadStr("TabOnTypingColor", szTemp, "yellow");
  m_colorTabTyping = QColor(szTemp);
  licqConf.ReadStr("ChatBackground", szTemp, "white");
  m_colorChatBkg = QColor(szTemp);

  licqConf.ReadBool("showPopEmail",m_bPopEmail, false);
  licqConf.ReadBool("showPopPhone",m_bPopPhone, true);
  licqConf.ReadBool("showPopFax",m_bPopFax, false);
  licqConf.ReadBool("showPopCellular",m_bPopCellular, true);
  licqConf.ReadBool("showPopIP",m_bPopIP, false);
  licqConf.ReadBool("showPopLastOnelin",m_bPopLastOnline, false);
  licqConf.ReadBool("showPopOnlineSince", m_bPopOnlineSince, false);
  licqConf.ReadBool("showPopIdleTime", m_bPopIdleTime, true);
  licqConf.ReadBool("showPopID", m_bPopID, true);
  
  unsigned short nFlash;
  licqConf.ReadNum("Flash", nFlash, FLASH_URGENT);
  m_nFlash = (FlashType)nFlash;
  licqConf.ReadBool("ScrollBar", m_bScrollBar, true);
  bool bFrameTransparent;
  licqConf.ReadBool("Transparent", bFrameTransparent, false);
  unsigned short nFrameStyle;
  licqConf.ReadNum("FrameStyle", nFrameStyle, 51);
  bool bDockIcon48;
  unsigned short nDockMode;
#ifdef USE_KDE
  licqConf.ReadNum("UseDock", nDockMode, (unsigned short)DockSmall);
#else
  licqConf.ReadNum("UseDock", nDockMode, (unsigned short)DockNone);
#endif
  m_nDockMode = (DockMode)nDockMode;
  licqConf.ReadBool("Dock64x48", bDockIcon48, false);
  char szDockTheme[64];
  licqConf.ReadStr("DockTheme", szDockTheme, "");
  licqConf.ReadBool("Hidden", m_bHidden, false);
  if (!nDockMode) m_bHidden = 0;
  licqConf.ReadBool("AutoRaise", m_bAutoRaise, true);

  licqConf.SetSection("startup");
  licqConf.ReadNum("Logon", m_nAutoLogon, 0);
  if (m_nAutoLogon > 16) m_nAutoLogon = 0;
  licqConf.ReadNum("AutoAway", autoAwayTime, 0);
  licqConf.ReadNum("AutoNA", autoNATime, 0);
  licqConf.ReadNum("AutoOffline", autoOfflineTime, 0);
  licqConf.ReadNum("AutoAwayMess", autoAwayMess, 0);
  licqConf.ReadNum("AutoNAMess", autoNAMess, 0);

  licqConf.SetSection("functions");
  licqConf.ReadBool("AutoClose", m_bAutoClose, true);
  licqConf.ReadBool("AutoPopup", m_bAutoPopup, false);
  licqConf.ReadStr("MsgPopupKey", szTemp, "none");

  m_MsgAutopopupKey = QString::fromLatin1(szTemp);
  if(!(!szTemp || !strcmp(szTemp, "none"))) {
    if(!static_cast<CLicqGui*>(qApp)->grabKey(m_MsgAutopopupKey))
       gLog.Error("%sUnknown popup key: %s\n", L_INITxSTR, szTemp);
  }

  m_nCurrentGroup = gUserManager.DefaultGroup();
  m_nGroupType = GROUPS_USER;

  // load up position and size from file
  licqConf.SetSection("geometry");
  unsigned short xPos, yPos, hVal, wVal;
  licqConf.ReadNum("x", xPos, 100);
  licqConf.ReadNum("y", yPos, 100);
  licqConf.ReadNum("h", hVal, 400);
  licqConf.ReadNum("w", wVal, 150);
  gLog.Info("%sGeometry configuration (%d, %d) (%d x %d)\n", L_INITxSTR,
            xPos, yPos, wVal, hVal);
  if (yPos > QApplication::desktop()->height() - 16) yPos = 0;
  if (xPos > QApplication::desktop()->width() - 16) xPos = 0;

  // Check for qt-gui directory in current base dir
  if (!QDir(QString("%1/%2").arg(BASE_DIR).arg(QTGUI_DIR)).exists())
  {
    QDir d;
    d.mkdir(QString("%1/%2").arg(BASE_DIR).arg(QTGUI_DIR));
  }

  // Load the icons
  licqConf.SetSection("appearance");
  licqConf.SetFlags(0);
  char szIcons[MAX_FILENAME_LEN];
  if (strlen(iconsName) == 0)
    licqConf.ReadStr("Icons", szIcons);
  else
    strcpy(szIcons, iconsName);
  m_szIconSet = NULL;

  // Load the extended icons
  char szExtendedIcons[MAX_FILENAME_LEN];
  if (strlen(extendedIconsName) == 0)
    licqConf.ReadStr("ExtendedIcons", szExtendedIcons, "basic");
  else
    strcpy(szExtendedIcons, extendedIconsName);
  m_szExtendedIconSet = NULL;

  // Load the Emoticons
  char szEmoticons[MAX_FILENAME_LEN];
  licqConf.ReadStr("Emoticons", szEmoticons, "Default" );
  QString s =  QString::fromLatin1(SHARE_DIR) + QTGUI_DIR + EMOTICONS_DIR;
  QString alt = QString::fromLatin1(BASE_DIR) + QTGUI_DIR + EMOTICONS_DIR;
  emoticons = new CEmoticons(s.latin1(), alt.latin1());
  if (*szEmoticons)
     if (emoticons->SetTheme(szEmoticons) < 0)
       gLog.Error("%s Loading emoticons theme `%s'\n", L_ERRORxSTR,
                  szEmoticons);

  // Load the skin
  char szSkin[MAX_FILENAME_LEN] = "basic";
  if (strlen(skinName) == 0)
    licqConf.ReadStr("Skin", szSkin);
  else
    strcpy(szSkin, skinName);

#if QT_VERSION >= 300
  style = QStyleFactory::create("windows");
#else
  style = new QWindowsStyle;
#endif
  awayMsgDlg = NULL;
  optionsDlg = NULL;
  ownerManagerDlg = NULL;
  pluginDlg = NULL;
#if QT_VERSION >= 300
  userEventTabDlg = NULL;
#endif
  m_nRealHeight = 0;

  ICQOwner *o = gUserManager.FetchOwner(LOCK_R);
  if (o != NULL)
  {
    m_szCaption = tr("Licq (%1)").arg(QString::fromLocal8Bit(o->GetAlias()));
    gUserManager.DropOwner();
  }
  else
    m_szCaption = QString("Licq");
  setCaption(m_szCaption);

  // Group Combo Box
  cmbUserGroups = new CEComboBox(false, this);
  connect(cmbUserGroups, SIGNAL(activated(int)), this, SLOT(setCurrentGroup(int)));

  // Widgets controlled completely by the skin
  btnSystem = NULL;
  lblMsg = NULL;
  lblStatus = NULL;
  pmBorder = NULL;
  pmMask = NULL;
  skin = NULL;
  menu = NULL;

  pmSecureOn = QPixmap(secure_on_xpm);
  pmSecureOff = QPixmap(secure_off_xpm);
  pmHistory = QPixmap(history_xpm);
  pmInfo = QPixmap(info_xpm);
  pmRemove = QPixmap(remove_xpm);
  pmSearch = QPixmap(search_xpm);
  #ifdef USE_KDE
  pmEncoding = KGlobal::iconLoader()->loadIcon("charset", KIcon::Small, 0, KIcon::DefaultState, 0L, true);
  if (pmEncoding.isNull())
    pmEncoding = QPixmap(charset_xpm);
  #else
  pmEncoding = QPixmap(charset_xpm);
  #endif
  ApplyIcons(szIcons, true);
  ApplyExtendedIcons(szExtendedIcons, true);
  initMenu();
  ApplySkin(szSkin, true);
  skin->frame.frameStyle = nFrameStyle;
  skin->frame.transparent = bFrameTransparent;

  // set the icon
  setIconText("Licq");

  // User List
  char key[16], colTitle[32], colFormat[32];
  unsigned short colWidth, colAlign, numColumns;
  licqConf.ReadNum("NumColumns", numColumns, 1);
  for (unsigned short i = 1; i <= numColumns; i++)
  {
     sprintf(key, "Column%d.Title", i);
     licqConf.ReadStr(key, colTitle, "Alias");
     sprintf(key, "Column%d.Format", i);
     licqConf.ReadStr(key, colFormat, "%a");
     sprintf(key, "Column%d.Width", i);
     licqConf.ReadNum(key, colWidth, 100);
     sprintf(key, "Column%d.Align", i);
     licqConf.ReadNum(key, colAlign, 0);
     colInfo.push_back(new CColumnInfo(QString::fromLocal8Bit(colTitle), colFormat, colWidth, colAlign));
  }
  CreateUserView();

  unsigned short nFloaties = 0, xPosF, yPosF, wValF;
  unsigned long nUin;
  licqConf.SetSection("floaties");
  licqConf.ReadNum("Num", nFloaties, 0);
  for (unsigned short i = 0; i < nFloaties; i++)
  {
    //TODO for protocol plugin
    sprintf(key, "Floaty%d.Uin", i);
    licqConf.ReadNum(key, nUin, 0);
    sprintf(key, "Floaty%d.X", i);
    licqConf.ReadNum(key, xPosF, 0);
    sprintf(key, "Floaty%d.Y", i);
    licqConf.ReadNum(key, yPosF, 0);
    sprintf(key, "Floaty%d.W", i);
    licqConf.ReadNum(key, wValF, 80);

    if (nUin != 0)
      CreateUserFloaty(nUin, xPosF, yPosF, wValF);
  }

  usprintfHelp = tr("<ul>"
                    "<li><tt>%a - </tt>user alias</li>"
                    "<li><tt>%e - </tt>email</li>"
                    "<li><tt>%f - </tt>first name</li>"
                    "<li><tt>%h - </tt>phone number</li>"
                    "<li><tt>%i - </tt>user ip</li>"
                    "<li><tt>%l - </tt>last name</li>"
                    "<li><tt>%m - </tt># pending messages</li>"
                    "<li><tt>%n - </tt>full name</li>"
                    "<li><tt>%o - </tt>last seen online</li>"
		    "<li><tt>%O - </tt>online since</li>"
                    "<li><tt>%p - </tt>user port</li>"
                    "<li><tt>%s - </tt>full status</li>"
                    "<li><tt>%S - </tt>abbreviated status</li>"
                    "<li><tt>%u - </tt>uin</li>"
                    "<li><tt>%w - </tt>webpage</li></ul>");
  licqIcon = NULL;
  if (!m_bDisableDockIcon)
  {
#ifdef USE_KDE
	if (m_nDockMode != DockNone)
      licqIcon = new IconManager_KDEStyle(this, mnuSystem);
#else
	switch (m_nDockMode)
	{
      case DockDefault:
    	licqIcon = new IconManager_Default(this, mnuSystem, bDockIcon48);
    	break;
      case DockThemed:
    	licqIcon = new IconManager_Themed(this, mnuSystem, szDockTheme);
    	break;
      case DockSmall:
    	licqIcon = new IconManager_KDEStyle(this, mnuSystem);
      case DockNone:
    	break;
	}
#endif
  }

   // all settings relating to localization
   licqConf.SetSection("locale");
   licqConf.ReadStr("DefaultEncoding", szTemp, "");
   m_DefaultEncoding = QString::fromLatin1(szTemp);
   uc_DefaultEncoding = &m_DefaultEncoding;
   licqConf.ReadBool("ShowAllEncodings", m_bShowAllEncodings, false);

   autoAwayTimer.start(10000);  // start the inactivity timer for auto away

   connect (&autoAwayTimer, SIGNAL(timeout()), this, SLOT(autoAway()));
   connect (licqSigMan, SIGNAL(signal_updatedList(CICQSignal *)),
            this, SLOT(slot_updatedList(CICQSignal *)));
   connect (licqSigMan, SIGNAL(signal_updatedUser(CICQSignal *)),
            this, SLOT(slot_updatedUser(CICQSignal *)));
   connect (licqSigMan, SIGNAL(signal_updatedStatus(CICQSignal *)), this, SLOT(updateStatus(CICQSignal *)));
   connect (licqSigMan, SIGNAL(signal_doneOwnerFcn(ICQEvent *)),
            this, SLOT(slot_doneOwnerFcn(ICQEvent *)));
   connect (licqSigMan, SIGNAL(signal_logon()),
            this, SLOT(slot_logon()));
//   connect (licqSigMan, SIGNAL(signal_ui_viewevent(unsigned long)),
//            this, SLOT(slot_ui_viewevent(unsigned long)));
   connect (licqSigMan, SIGNAL(signal_ui_message(const char *,unsigned long)),
            this, SLOT(slot_ui_message(const char *, unsigned long)));
   connect (licqSigMan, SIGNAL(signal_ui_viewevent(const char *)),
            this, SLOT(slot_ui_viewevent(const char *)));
   connect (licqSigMan, SIGNAL(signal_protocolPlugin(unsigned long)),
            this, SLOT(slot_protocolPlugin(unsigned long)));
   connect (licqSigMan, SIGNAL(signal_eventTag(const char *, unsigned long, unsigned long)),
            this, SLOT(slot_eventTag(const char *, unsigned long, unsigned long)));
   connect (licqSigMan, SIGNAL(signal_socket(const char *, unsigned long, unsigned long)),
            this, SLOT(slot_socket(const char *, unsigned long, unsigned long)));
   connect (licqSigMan, SIGNAL(signal_convoJoin(const char *, unsigned long, unsigned long)),
            this, SLOT(slot_convoJoin(const char *, unsigned long, unsigned long)));
   connect (licqSigMan, SIGNAL(signal_convoLeave(const char *, unsigned long, unsigned long)),
            this, SLOT(slot_convoLeave(const char *, unsigned long, unsigned long)));
                        
   m_bInMiniMode = false;
   updateStatus();
   updateEvents();
   updateGroups();
   manualAway = 0;

   resize(wVal, hVal);
   move(xPos, yPos);
   if (!m_bHidden && !bStartHidden) show();

   // verify we exist
   if (gUserManager.NumOwners() == 0)
     showOwnerManagerDlg();
   else
   {
     // Do we need to get a password
     o = gUserManager.FetchOwner(LOCK_R);
     if (o != NULL)
     {
      if(o->Password()[0] == '\0')
      {
        gUserManager.DropOwner();
        (void) new UserSelectDlg(licqDaemon);
      }
      else
        gUserManager.DropOwner();
     }
   }

   o = gUserManager.FetchOwner(LICQ_PPID, LOCK_R);
   if (o != NULL)
   {
    mnuPFM->setItemChecked(o->PhoneFollowMeStatus(), true);
    gUserManager.DropOwner();
   }

#if QT_VERSION > 3
  XClassHint ClassHint;
  ClassHint.res_class = (char *)qAppName();
  ClassHint.res_name = (char *)name();
  XSetClassHint(x11Display(), winId(), &ClassHint);
#endif

#ifdef USE_KDE
    kdeIMInterface = new LicqKIMIface(KApplication::dcopClient()->appId(), this);
    connect( kdeIMInterface,
             SIGNAL(sendMessage(const char*, unsigned long, const QString&)),
             this, SLOT(sendMsg(const char*, unsigned long, const QString&)) );
    connect( kdeIMInterface,
             SIGNAL(sendFileTransfer(const char*, unsigned long,
                                     const QString&, const QString&)),
             this, SLOT(sendFileTransfer(const char*, unsigned long,
                        const QString&, const QString&)) );
    connect( kdeIMInterface,
             SIGNAL(sendChatRequest(const char*, unsigned long)),
             this, SLOT(sendChatRequest(const char*, unsigned long)));
    connect( kdeIMInterface,
             SIGNAL(addUser(const char*, unsigned long)),
             this, SLOT(addUser(const char*, unsigned long)));

#endif

    ProtoPluginsList pl;
    ProtoPluginsListIter it;
    licqDaemon->ProtoPluginList(pl);
    for (it = pl.begin(); it != pl.end(); it++)
    {
#ifdef USE_KDE
        kdeIMInterface->addProtocol((*it)->Name(), (*it)->PPID());
#endif
        if ((*it)->PPID() != LICQ_PPID) // XXX To be removed later
        slot_protocolPlugin((*it)->PPID());
    }
   
   // automatically logon if requested in conf file
   if (m_nAutoLogon > 0)
   {
      if (m_nAutoLogon >= 10)
      {
        mnuStatus->setItemChecked(ICQ_STATUS_FxPRIVATE, true);

        // Check the protocol status menu items as well
        std::vector<unsigned long>::iterator iter;
        unsigned long nAt = 0;
        for (iter = m_lnProtMenu.begin(); iter != m_lnProtMenu.end(); ++iter, nAt++)
          mnuProtocolStatus[nAt]->setItemChecked(CHANGE_STATUS_PRV, true);
      }

      switch (m_nAutoLogon % 10)
      {
      case 0: break;
      case 1: changeStatus(ICQ_STATUS_ONLINE, 0xFFFFFFFF, true); break;
      case 2: changeStatus(ICQ_STATUS_AWAY, 0xFFFFFFFF, true); break;
      case 3: changeStatus(ICQ_STATUS_NA, 0xFFFFFFFF, true); break;
      case 4: changeStatus(ICQ_STATUS_OCCUPIED, 0xFFFFFFFF, true); break;
      case 5: changeStatus(ICQ_STATUS_DND, 0xFFFFFFFF, true); break;
      case 6: changeStatus(ICQ_STATUS_FREEFORCHAT, 0xFFFFFFFF, true); break;
      default: gLog.Warn("%sInvalid auto online id: %d.\n", L_WARNxSTR, m_nAutoLogon);
      }
   }
}

//-----ApplySkin----------------------------------------------------------------
void CMainWindow::ApplySkin(const char *_szSkin, bool _bInitial)
{
  gLog.Info("%sApplying %s skin.\n", L_INITxSTR, _szSkin);

  if (skin != NULL) delete skin;
  skin = new CSkin(_szSkin);

  // Set the background pixmap and mask
  if (skin->frame.pixmap != NULL)
  {
     if (pmBorder != NULL) delete pmBorder;
     pmBorder = new QPixmap(skin->frame.pixmap);
     if (pmBorder->isNull())
     {
       gLog.Error("%sError loading background pixmap (%s).\n", L_ERRORxSTR, skin->frame.pixmap);
       delete[] skin->frame.pixmap;
       skin->frame.pixmap = NULL;
     }
  }
  else
  {
    setBackgroundMode(PaletteBackground);
#if QT_VERSION >= 300
    unsetPalette();
#endif
  }

  if (skin->frame.mask != NULL)
  {
     if (pmMask != NULL) delete pmMask;
     pmMask = new QPixmap(skin->frame.mask);
     if (pmMask->isNull())
     {
       gLog.Error("%sError loading background mask (%s).\n", L_ERRORxSTR, skin->frame.mask);
       delete[] skin->frame.mask;
       skin->frame.mask = NULL;
     }
  }
  else
  {
    clearMask();
  }

  // Group Combo Box
  cmbUserGroups->setNamedBgColor(skin->cmbGroups.color.bg);
  cmbUserGroups->setNamedFgColor(skin->cmbGroups.color.fg);

  // System Button
  if (btnSystem != NULL) delete btnSystem;
  if (menu != NULL)
  {
    menu->removeItemAt(0);
    delete menu;
  }
  if (!skin->frame.hasMenuBar)
  {
     if (skin->btnSys.pixmapUpNoFocus == NULL)
     {
        btnSystem = new CEButton(skin->btnSys.caption == NULL ?
                                 tr("System") : QString::fromLocal8Bit(skin->btnSys.caption), this);
     }
     else
     {
       btnSystem = new CEButton(new QPixmap(skin->btnSys.pixmapUpFocus),
                                new QPixmap(skin->btnSys.pixmapUpNoFocus),
                                new QPixmap(skin->btnSys.pixmapDown),
                                this);
       btnSystem->setStyle(style);
     }
     connect(btnSystem, SIGNAL(clicked()), this, SLOT(popupSystemMenu()));
     btnSystem->setNamedFgColor(skin->btnSys.color.fg);
     btnSystem->setNamedBgColor(skin->btnSys.color.bg);
     menu = NULL;
  }
  else
  {
#ifdef USE_KDE
    menu = new KMenuBar(this);
#else
    menu = new QMenuBar(this);
#endif
    menu->insertItem(skin->btnSys.caption == NULL ?
                     tr("&System") : QString::fromLocal8Bit(skin->btnSys.caption),
                     mnuSystem);
    btnSystem = NULL;
    skin->AdjustForMenuBar(menu->height());
  }

  
  // Message Label
  delete lblMsg;
  lblMsg = new CELabel(skin->lblMsg.transparent, mnuUserGroups, this);
  if (skin->lblMsg.pixmap != NULL || skin->lblMsg.transparent)
    lblMsg->setStyle(style);
  lblMsg->setFrameStyle(skin->lblMsg.frameStyle);
  lblMsg->setIndent(skin->lblMsg.margin);
  lblMsg->setNamedFgColor(skin->lblMsg.color.fg);
  lblMsg->setNamedBgColor(skin->lblMsg.color.bg);
  if (skin->lblMsg.pixmap != NULL)
  {
#if QT_VERSION < 300
    lblMsg->setBackgroundPixmap(QPixmap(skin->lblMsg.pixmap));
  }
#else
    lblMsg->setBackgroundOrigin(WidgetOrigin);
    lblMsg->setPaletteBackgroundPixmap(QPixmap(skin->lblMsg.pixmap));
  }
  else if (skin->lblMsg.transparent && skin->frame.pixmap)
  {
    lblMsg->setBackgroundOrigin(ParentOrigin);
    lblMsg->setPaletteBackgroundPixmap(QPixmap(skin->frame.pixmap));
  }
#endif

  connect(lblMsg, SIGNAL(doubleClicked()), this, SLOT(callMsgFunction()));
  QToolTip::add(lblMsg, tr("Right click - User groups\n"
                           "Double click - Show next message"));

  // Status Label
  delete lblStatus;
  lblStatus = new CELabel(skin->lblStatus.transparent, mnuStatus, this);
  if (skin->lblStatus.pixmap != NULL || skin->lblStatus.transparent)
    lblStatus->setStyle(style);
  lblStatus->setFrameStyle(skin->lblStatus.frameStyle);
  lblStatus->setIndent(skin->lblStatus.margin);
  lblStatus->setNamedFgColor(skin->lblStatus.color.fg);
  lblStatus->setNamedBgColor(skin->lblStatus.color.bg);
  if (skin->lblStatus.pixmap != NULL)
  {
#if QT_VERSION < 300
    lblStatus->setBackgroundPixmap(QPixmap(skin->lblStatus.pixmap));
  }
#else
    lblStatus->setBackgroundOrigin(WidgetOrigin);
    lblStatus->setPaletteBackgroundPixmap(QPixmap(skin->lblStatus.pixmap));
  }
  else if (skin->lblStatus.transparent && skin->frame.pixmap)
  {
    lblStatus->setBackgroundOrigin(ParentOrigin);
    lblStatus->setPaletteBackgroundPixmap(QPixmap(skin->frame.pixmap));
  }
#endif

  connect(lblStatus, SIGNAL(doubleClicked()), this, SLOT(slot_AwayMsgDlg()));
  QToolTip::add(lblStatus, tr("Right click - Status menu\n"
                              "Double click - Set auto response"));

  if (!_bInitial)
  {
    resizeEvent(NULL);
    userView->setFrameStyle(skin->frame.frameStyle);
    userView->QListView::setPalette(skin->palette(this));
    userView->setColors(skin->colors.online, skin->colors.away,
                        skin->colors.offline, skin->colors.newuser,
                        skin->colors.background, skin->colors.gridlines);
    // Update all the floaties
    CUserView::UpdateFloaties();

    // btnSystem
    if (btnSystem != NULL)
      if ((skin->btnSys.rect.x1 == skin->btnSys.rect.x2) && (skin->btnSys.rect.y1 == skin->btnSys.rect.y2))
        btnSystem->hide();
      else
        btnSystem->show();
    // lblStatus
    if (lblStatus != NULL)
      if ((skin->lblStatus.rect.x1 == skin->lblStatus.rect.x2) && (skin->lblStatus.rect.y1 == skin->lblStatus.rect.y2))
        lblStatus->hide();
      else
        lblStatus->show();
    // lblMsg
    if (lblMsg != NULL)
      if ((skin->lblMsg.rect.x1 == skin->lblMsg.rect.x2) && (skin->lblMsg.rect.y1 == skin->lblMsg.rect.y2))
        lblMsg->hide();
      else
        lblMsg->show();
    // cmbGroups
    if (cmbUserGroups != NULL)
      if ((skin->cmbGroups.rect.x1 == skin->cmbGroups.rect.x2) && (skin->cmbGroups.rect.y1 == skin->cmbGroups.rect.y2))
        cmbUserGroups->hide();
      else
        cmbUserGroups->show();
    if (menu != NULL) menu->show();
    updateUserWin();
    updateEvents();
    updateStatus();
  }
}


//-----CMainWindow::CreateUserView---------------------------------------------
void CMainWindow::CreateUserView()
{
  userView = new CUserView(mnuUser, this);
  userView->QListView::setPalette(skin->palette(this));
  userView->setColors(skin->colors.online, skin->colors.away, skin->colors.offline,
                      skin->colors.newuser, skin->colors.background, skin->colors.gridlines);
  connect (userView, SIGNAL(doubleClicked(QListViewItem *)), SLOT(callDefaultFunction(QListViewItem *)));
}


//-----CMainWindow::CreateUserFloaty---------------------------------------------
void CMainWindow::CreateUserFloaty(const char *szId, unsigned long nPPID,
   unsigned short x, unsigned short y, unsigned short w)
{
  if (szId == 0 || nPPID == 0) return;
  ICQUser *u = gUserManager.FetchUser(szId, nPPID, LOCK_R);
  if (u == NULL) return;

  CUserView *f = new CUserView(mnuUser);
  connect (f, SIGNAL(doubleClicked(QListViewItem *)), SLOT(callDefaultFunction(QListViewItem *)));

  CUserViewItem *i = new CUserViewItem(u, f);

  gUserManager.DropUser(u);

  // not so good, we should allow for multiple guys in one box...
  // perhaps use the viewport sizeHint
  f->setFixedHeight(i->height() + f->frameWidth() * 2);

  if (w != 0)
  {
    if (y > QApplication::desktop()->height() - 16) y = 0;
    if (x > QApplication::desktop()->width() - 16) x = 0;
    f->setGeometry(x, y, w, f->height());
  }

  f->show();
}

void CMainWindow::CreateUserFloaty(unsigned long nUin, unsigned short x,
   unsigned short y, unsigned short w)
{
  if (nUin == 0) return;
  ICQUser *u = gUserManager.FetchUser(nUin, LOCK_R);
  if (u == NULL) return;

  CUserView *f = new CUserView(mnuUser);
  connect (f, SIGNAL(doubleClicked(QListViewItem *)), SLOT(callDefaultFunction(QListViewItem *)));

  CUserViewItem *i = new CUserViewItem(u, f);

  gUserManager.DropUser(u);

  // not so good, we should allow for multiple guys in one box...
  // perhaps use the viewport sizeHint
  f->setFixedHeight(i->height() + f->frameWidth() * 2);

  if (w != 0)
  {
    if (y > QApplication::desktop()->height() - 16) y = 0;
    if (x > QApplication::desktop()->width() - 16) x = 0;
    f->setGeometry(x, y, w, f->height());
  }

  f->show();
}


//-----CMainWindow::destructor--------------------------------------------------
CMainWindow::~CMainWindow()
{
  delete licqIcon;
  if (emoticons) delete emoticons;
  if (skin) delete skin;
  if (m_szExtendedIconSet) free(m_szExtendedIconSet);
  if (pmBorder) delete pmBorder;
  if (pmMask) delete pmMask;
  gMainWindow = NULL;
  if (m_szUserMenuId)
    free(m_szUserMenuId);
}


//-----CMainWindow::resizeEvent------------------------------------------------
void CMainWindow::resizeEvent (QResizeEvent *)
{
  userView->setGeometry(skin->frame.border.left, skin->frame.border.top,
                        width() - skin->frameWidth(), height() - skin->frameHeight());

  if (!skin->frame.hasMenuBar)
    btnSystem->setGeometry(skin->borderToRect(&skin->btnSys, this));

  // Do this to save the new geometry
  positionChanges++;

  // Resize the background pixmap and mask
  QPixmap *p;
  if (skin->frame.pixmap != NULL)
  {
    p = ScaleWithBorder(*pmBorder, width(), height(), skin->frame.border);
#if QT_VERSION >= 300
    setPaletteBackgroundPixmap(*p);

    // set the palette for the labels here as well
    if (skin->lblMsg.transparent)
      lblMsg->setPaletteBackgroundPixmap(*p);

    if (skin->lblStatus.transparent)
      lblStatus->setPaletteBackgroundPixmap(*p);
#else
    setBackgroundPixmap(*p);
#endif
    delete p;
  }
  if (skin->frame.mask != NULL)
  {
    p = ScaleWithBorder(*pmMask, width(), height(), skin->frame.border);
    bmMask = *p;
    setMask(bmMask);
    delete p;
  }

/*  Set geometry of our widgets
  Hide the widget if all position values are equal because this would result
  in a unusable 1-pixel widget*/
  // cmbUserGroups
  if (cmbUserGroups != NULL)
    if ((skin->cmbGroups.rect.x1 == skin->cmbGroups.rect.x2) && (skin->cmbGroups.rect.y1 == skin->cmbGroups.rect.y2))
      cmbUserGroups->hide();
    else
      cmbUserGroups->setGeometry(skin->borderToRect(&skin->cmbGroups, this));
  // lblMsg
  if (lblMsg != NULL)
    if ((skin->lblMsg.rect.x1 == skin->lblMsg.rect.x2) && (skin->lblMsg.rect.y1 == skin->lblMsg.rect.y2))
      lblMsg->hide();
    else
      lblMsg->setGeometry(skin->borderToRect(&skin->lblMsg, this));
  // lblStatus
  if (lblStatus != NULL)
    if ((skin->lblStatus.rect.x1 == skin->lblStatus.rect.x2) && (skin->lblStatus.rect.y1 == skin->lblStatus.rect.y2))
      lblStatus->hide();
    else
      lblStatus->setGeometry(skin->borderToRect(&skin->lblStatus, this));
  // btnSystem
  if (btnSystem != NULL)
    if ((skin->btnSys.rect.x1 == skin->btnSys.rect.x2) && (skin->btnSys.rect.y1 == skin->btnSys.rect.y2))
      btnSystem->hide();
    else
      btnSystem->setGeometry(skin->borderToRect(&skin->btnSys, this));
}

void CMainWindow::moveEvent(QMoveEvent* e)
{
  if(isVisible())  positionChanges++;

  QWidget::moveEvent(e);
}

void CMainWindow::closeEvent( QCloseEvent *e )
{
#if 0
    qDebug("closeEvent() visible %d", isVisible());
    qDebug("geometry: x: %d y: %d, w: %d, h: %d", geometry().x(), geometry().y(), geometry().width(), geometry().height());
    qDebug("x: %d, y: %d, w: %d, h: %d", x(), y(), size().width(), size().height());
    qDebug(" toGlobal: x %d, y %d", mapToGlobal(QPoint(0,0)).x(), mapToGlobal(QPoint(0,0)).y());
#endif

  if(isVisible() && positionChanges > 1)
  {
    // save window position and size
    char buf[MAX_FILENAME_LEN];
    snprintf(buf, MAX_FILENAME_LEN, "%s/licq_qt-gui.conf", BASE_DIR);
    buf[MAX_FILENAME_LEN - 1] = '\0';
    CIniFile licqConf(INI_FxALLOWxCREATE | INI_FxWARN);
    // need some more error checking here...
    licqConf.LoadFile(buf);

    int x, y;
    if(pos().x() < 2 || pos().y() < 2) {
      // WMaker bug.  will investigate...
      QPoint p  = mapToGlobal(QPoint(0, 0));

//      qDebug("wmaker workaround enabled");

      x = p.x() - 1 - ( geometry().x() < p.x() ? geometry().x() : 0);
      y = p.y() - 1 - ( geometry().y() < p.y() ? geometry().y() : 0);
    }
    else
    {
      x = pos().x();
      y = pos().y();
    }

    x = x < 0 ? 0 : x;
    y = y < 0 ? 0 : y;

    licqConf.SetSection("geometry");
    // I'm not sure if we should really test for negative values...
    licqConf.WriteNum("x", (unsigned short)x);
    licqConf.WriteNum("y", (unsigned short)y);
    licqConf.WriteNum("h", (unsigned short)(size().height() < 0 ? 0 : (m_bInMiniMode ? m_nRealHeight : size().height())));
    licqConf.WriteNum("w", (unsigned short)(size().width() < 0 ? 0 : size().width()));

#if 0
    licqConf.SetSection("floaties");
    licqConf.WriteNum("Num", (unsigned short)CUserView::floaties->size());
    unsigned short i = 0;
    char key[32];
    for (; i < CUserView::floaties->size(); )
    {
      CUserView* iter = CUserView::floaties->at(i);
      sprintf(key, "Floaty%d.Uin", i);
      licqConf.WriteNum(key, iter->firstChild()->ItemUin());
      sprintf(key, "Floaty%d.X", i);
      licqConf.WriteNum(key, (unsigned short)(iter->x() > 0 ? iter->x() : 0));
      sprintf(key, "Floaty%d.Y", i);
      licqConf.WriteNum(key, (unsigned short)(iter->y() > 0 ? iter->y() : 0));
      sprintf(key, "Floaty%d.W", i);
      licqConf.WriteNum(key, (unsigned short)iter->width());
      i++;
    }
#endif

    licqConf.FlushFile();
    licqConf.CloseFile();
  }

  if (licqIcon != NULL)
  {
    e->ignore();
    hide();
  }
  else
  {
    e->ignore();
    slot_shutdown();
  }
}


// Receive key press events for the main widget
// Ctrl-M : toggle mini mode
// Ctrl-H : hide window
// Ctrl-I : next message
// Ctrl-X : exit
// Ctrl-V : View message
// Ctrl-S : Send message
// Ctrl-U : Send Url
// Ctrl-C : Send chat request
// Ctrl-F : Send File
// Ctrl-A : Check Auto response
// Ctrl-P : Popup all messages
// Ctrl-O : Toggle offline users
// Ctrl-L : Redraw user window
// Delete : Remove the user from the group
// Ctrl-Delete : Remove the user from the list

void CMainWindow::keyPressEvent(QKeyEvent *e)
{
  char *szId = 0;  //must be free()'d
  unsigned long nPPID = 0;
  userView->MainWindowSelectedItemUser(szId, nPPID);

  if (e->key() == Qt::Key_Delete)
  {
    if (szId == 0 || nPPID == 0)
    {
      if (szId) free(szId);
      return;
    }
    if (e->state() & ControlButton)
      RemoveUserFromList(szId, nPPID, this);
    else
      RemoveUserFromGroup(m_nGroupType, m_nCurrentGroup, szId, nPPID, this);

    if (szId)
      free(szId);

    //e->accept();
    return;
  }
  else if (! (e->state() & ControlButton))
  {
    e->ignore();
    QWidget::keyPressEvent(e);
    return;
  }

  switch(e->key())
  {
    case Qt::Key_M:
      ToggleMiniMode();
      break;

    case Qt::Key_H:
      if (licqIcon != NULL) hide();
      break;

    case Qt::Key_I:
      callMsgFunction();
      break;

    case Qt::Key_X:
      slot_shutdown();
      break;

    case Qt::Key_V:
      callFunction(mnuUserView, szId, nPPID);
      break;

    case Qt::Key_S:
      callFunction(mnuUserSendMsg, szId, nPPID);
      break;

    case Qt::Key_U:
      callFunction(mnuUserSendUrl, szId, nPPID);
      break;

    case Qt::Key_C:
      callFunction(mnuUserSendChat, szId, nPPID);
      break;

    case Qt::Key_F:
      callFunction(mnuUserSendFile, szId, nPPID);
      break;

    case Qt::Key_A:
      if (szId && nPPID)
        (void) new ShowAwayMsgDlg(licqDaemon, licqSigMan, szId, nPPID);
      break;

    case Qt::Key_P:
      slot_popupall();
      break;

    case Qt::Key_O:
      ToggleShowOffline();
      break;

    case Qt::Key_L:
      updateUserWin();
      break;

    default:
      e->ignore();
      QWidget::keyPressEvent(e);
      break;
  }
  if (szId)
    free(szId);
}


//-----CMainWindow::mouseEvent----------------------------------------------
void CMainWindow::mousePressEvent(QMouseEvent *m)
{
   mouseX = m->x();
   mouseY = m->y();
}

/*! \brief Drags the mainwindow around
 *
 * If the appropriate option is set (EnableMainwinMouseMovement = 1) 
 * this drags the mainwindow around when moving the mouse and left 
 * button is pressed.
 */
void CMainWindow::mouseMoveEvent(QMouseEvent *m)
{
  if (m_bEnableMainwinMouseMovement && (m->state() == Qt::LeftButton))
  {
    int deltaX = m->x() - mouseX;
    int deltaY = m->y() - mouseY;
    move(x() + deltaX, y() + deltaY);
  }
}


// ---------------------------------------------------------------------------
inline bool CMainWindow::show_user(ICQUser *u)
{
  return (m_bShowOffline || !u->StatusOffline() || u->NewMessages() > 0 ||
          (m_bAlwaysShowONU && u->OnlineNotify()) || u->NotInList());
}


void CMainWindow::slot_updatedUser(CICQSignal *sig)
{
  char *szId = sig->Id();
  unsigned long nPPID = sig->PPID();

  switch(sig->SubSignal())
  {
    case USER_EVENTS:
    {
      // Skip all this if it was just an away message check
      if (sig->Argument() == 0) {
        userView->AnimationAutoResponseCheck(szId, nPPID);
        break;
      }
      // Otherwise an event was added or removed
      updateEvents();
      // autoRaise if needed
      if(m_bAutoRaise && sig->Argument() > 0)  raise();

      if (m_bAutoPopup && sig->Argument() > 0)
      {
        ICQUser *u = gUserManager.FetchUser(szId, nPPID, LOCK_R);
        if (u != NULL && u->NewMessages() > 0)
        {
          ICQOwner *o = gUserManager.FetchOwner(nPPID, LOCK_R);
          unsigned short s;
          if (o == 0)
          {
            s = ICQ_STATUS_OFFLINE; // if we have no owner we're very likely offline ;)
          }
          else
          {  
            s = o->Status();
            gUserManager.DropOwner(nPPID);
          }
          if (s == ICQ_STATUS_ONLINE || s == ICQ_STATUS_FREEFORCHAT)
          {
            bool bCallUserView = false, bCallSendMsg = false;

            for (unsigned short i = 0; i < u->NewMessages(); i++)
            {
              if (m_bMsgChatView &&
                  u->EventPeek(i)->SubCommand() == ICQ_CMDxSUB_MSG)
              {
                bCallSendMsg = true;
                if (bCallUserView)
                  break;
              }
              else
              {
                bCallUserView = true;
                if (!m_bMsgChatView || bCallSendMsg)
                  break;
              }
            }
            
            gUserManager.DropUser(u);

            if (bCallUserView)
              callFunction(mnuUserView, szId, nPPID, sig->CID());
            if (bCallSendMsg)
              callFunction(mnuUserSendMsg, szId, nPPID, sig->CID());
          }
          else
            gUserManager.DropUser(u);
        }
        else
        {
          gUserManager.DropUser(u);
        }
      }
      // Fall through
    }
    case USER_STATUS:
#ifdef USE_KDE
        kdeIMInterface->userStatusChanged(szId, nPPID);
#endif
    case USER_BASIC:
    case USER_GENERAL:
    case USER_EXT:
    case USER_SECURITY:
    case USER_TYPING:
    {
      if (gUserManager.FindOwner(szId, nPPID) != NULL)
      {
        if (sig->SubSignal() == USER_STATUS || sig->SubSignal() == USER_EXT) break;
        ICQOwner *o = gUserManager.FetchOwner(nPPID, LOCK_R);
        if (o != 0)
        {
          m_szCaption = tr("Licq (%1)").arg(QString::fromLocal8Bit(o->GetAlias()));
          gUserManager.DropOwner(nPPID);
        }
        else
        {
          m_szCaption = tr("Licq (%1)").arg(QString(tr("Error! No owner set")));
        }
        if (caption()[0] == '*')
          setCaption(QString("* ") + m_szCaption);
        else
          setCaption(m_szCaption);
        break;
      }

      ICQUser *u = gUserManager.FetchUser(szId, nPPID, LOCK_R);
      if (u == NULL)
      {
        gLog.Warn("%sCMainWindow::slot_updatedUser(): Invalid user received: %s\n",
          L_ERRORxSTR, szId);
        break;
      }
      if (m_bThreadView && m_nGroupType == GROUPS_USER && m_nCurrentGroup == 0)
      {
        CUserViewItem* i = userView->firstChild();

        while (i)
        {
          if (u->GetInGroup(GROUPS_USER, i->GroupId()))
          {
            CUserViewItem* it = i->firstChild();

            while (it)
            {
              char *szRealId = 0;
              ICQUser::MakeRealId(it->ItemId(), it->ItemPPID(), szRealId);
              if(strcasecmp(szRealId, szId) == 0 && it->ItemPPID() == nPPID)
              {
                delete it;
                delete [] szRealId;
                if (show_user(u))
                  (void) new CUserViewItem(u, i);
                break;
              }
              delete [] szRealId;
              it = it->nextSibling();
            }
            if (it == NULL)
            {
              if ( show_user(u) &
                   ((i->GroupId() != 0 && u->GetInGroup(GROUPS_USER, i->GroupId())) ||
                    (i->GroupId() == 0 && u->GetGroups(GROUPS_USER) == 0 && !u->IgnoreList())))
                (void) new CUserViewItem(u, i);
            }
          }
          i = i->nextSibling();
        }
      }
      else if (u->GetInGroup(m_nGroupType, m_nCurrentGroup))
      {
        // Update this user if they are in the current group
        CUserViewItem *i = (CUserViewItem *)userView->firstChild();
        char *szItemId = 0, *szRealId = 0;
        if (i)
          szItemId = ICQUser::MakeRealId(i->ItemId(), i->ItemPPID(), szItemId);
        ICQUser::MakeRealId(szId, nPPID, szRealId);
        while (i && !(i->ItemPPID() == nPPID && strcasecmp(szItemId, szRealId) == 0))
        {
          i = (CUserViewItem *)i->nextSibling();
          if (i)
          {
            delete [] szItemId; szItemId = 0;
            ICQUser::MakeRealId(i->ItemId(), i->ItemPPID(), szItemId);
          }
        }
        if (szItemId) delete [] szItemId;
        delete [] szRealId;

        if (i != NULL)
        {
          delete i;
          if (show_user(u))
            (void) new CUserViewItem(u, userView);
        }
        else
        {
          if ( show_user(u) &&
               (!u->IgnoreList() || (m_nGroupType == GROUPS_SYSTEM && m_nCurrentGroup == GROUP_IGNORE_LIST)) )
            (void) new CUserViewItem(u, userView);
        }
      }

      if(sig->SubSignal() == USER_STATUS && sig->Argument() == 1)
      {
        userView->AnimationOnline(szId, nPPID);
#if defined(USE_KDE) && (KDE_VERSION >= 310)
        // User on notify list went online -> show popup at systray icon
        if (licqIcon && u->OnlineNotify())
        {
          QString alias = QString::fromUtf8(u->GetAlias());
          // Escape HTML
          alias.replace(QChar('&'), "&amp;");
          alias.replace(QChar('<'), "&lt;");
          alias.replace(QChar('>'), "&gt;");

          QString msg(tr("<b>%1</b> is online").arg(alias));
          QPixmap px = iconForStatus(u->StatusFull(), u->IdString(), u->PPID());
          KPassivePopup::message("Licq", msg, px, licqIcon, NULL, 4000);
        }
#endif
      }

      // Update their floaty
      CUserView *v = CUserView::FindFloaty(szId, nPPID);

      if (v != NULL )
      {
        static_cast<CUserViewItem*>(v->firstChild())->setGraphics(u);
        v->triggerUpdate();
      }

#if QT_VERSION >= 300
      // update the tab icon of this user
      if (m_bTabbedChatting && userEventTabDlg)
      {
        if (sig->SubSignal() == USER_TYPING)
          userEventTabDlg->gotTyping(u, sig->Argument());
        userEventTabDlg->updateTabLabel(u);
      }
      else
#endif
      if (sig->SubSignal() == USER_TYPING)
      {
        // First, update the window if available
#if QT_VERSION < 300
        QListIterator<UserSendCommon> it(licqUserSend );
#else
        QPtrListIterator<UserSendCommon> it(licqUserSend);
#endif
        UserEventCommon *e = 0;

        for (; it.current(); ++it)
        {
          if ((*it)->PPID() == MSN_PPID)
          {
            // For protocols that use the convo id
            if ((*it)->ConvoId() == sig->Argument() && (*it)->PPID() == nPPID)
            {
              e = static_cast<UserSendCommon *>(*it);
              e->gotTyping(u->GetTyping());
            }
          }
          else
          {
            // For protocols that don't use a convo id
            if (strcasecmp((*it)->Id(), szId) == 0 && (*it)->PPID() == nPPID)
            {
              e = static_cast<UserSendCommon*>(*it);
              e->gotTyping(u->GetTyping());
            }
          }
        }
      }

      gUserManager.DropUser(u);

      break;
    }
  }
}


// ---------------------------------------------------------------------------

void CMainWindow::slot_updatedList(CICQSignal *sig)
{
  switch(sig->SubSignal())
  {
    case LIST_ALL:
    {
      updateUserWin();
      break;
    }
    case LIST_ADD:
    {
      ICQUser *u = gUserManager.FetchUser(sig->Id(), sig->PPID(), LOCK_W);
      if (u == NULL)
      {
        gLog.Warn("%sCMainWindow::slot_updatedList(): Invalid user received: %s\n",
          L_ERRORxSTR, sig->Id());
        break;
      }

      if(m_bThreadView && m_nGroupType == GROUPS_USER && m_nCurrentGroup == 0)
      {
        CUserViewItem* i = userView->firstChild();

        while(i)
        {
          if(u->GetInGroup(GROUPS_USER, i->GroupId()) && show_user(u))
            (void) new CUserViewItem(u, i);

          i = i->nextSibling();
        }
      }
      else
      {

        if (u->GetInGroup(m_nGroupType, m_nCurrentGroup) && show_user(u) )
          (void) new CUserViewItem(u, userView);
      }
      // as we intercept the user's addition, we set it our default codec
      if (!m_DefaultEncoding.isEmpty())
      {
        u->SetUserEncoding(m_DefaultEncoding.latin1());
      }
      gUserManager.DropUser(u);
      break;
    }

    case LIST_REMOVE:
    {
      // delete their entries in the user list
      QListViewItemIterator it(userView);
      while(it.current())
      {
        CUserViewItem* item = static_cast<CUserViewItem*>(it.current());

        if(sig->Id() && item->ItemId() &&
           strcmp(sig->Id(), item->ItemId()) == 0 &&
           sig->PPID() == item->ItemPPID())
        {
          ++it;
          delete item;
          item = 0;
        }
        else
          ++it;
      }

      updateEvents();
      // If their box is open, kill it
      {
#if QT_VERSION < 300
        QListIterator<UserViewEvent> it(licqUserView);
#else
        QPtrListIterator<UserViewEvent> it(licqUserView);
#endif
        for (; it.current() != NULL; ++it)
        {
          if (strcmp((*it)->Id(), sig->Id()) == 0 && (*it)->PPID() == sig->PPID())
          {
            it.current()->close();
            licqUserView.remove(it.current());
            break;
          }
        }
      }
      {
        // if their info box is open, kill it
#if QT_VERSION < 300
        QListIterator<UserInfoDlg> it(licqUserInfo);
#else
        QPtrListIterator<UserInfoDlg> it(licqUserInfo);
#endif
        for(; it.current() != NULL; ++it)
        {
          if (strcmp((*it)->Id(), sig->Id()) == 0 && (*it)->PPID() == sig->PPID())
          {
            it.current()->close();
            licqUserInfo.remove(it.current());
            break;
          }
        }
      }
      {
        // if their send box is open, kill it
#if QT_VERSION < 300
        QListIterator<UserSendCommon> it(licqUserSend);
#else
        QPtrListIterator<UserSendCommon> it(licqUserSend);
#endif
        for(; it.current() != NULL; ++it)
        {
          if (strcmp((*it)->Id(), sig->Id()) == 0 && (*it)->PPID() == sig->PPID())
          {
            it.current()->close();
            licqUserSend.remove(it.current());
            break;
          }
        }
      }

      break;
    }

  }  // Switch
}

void CMainWindow::slot_socket(const char *szId, unsigned long nPPID, unsigned long nConvoId)
{
  // Add the user to an ongoing conversation
#if QT_VERSION < 300
  QListIterator<UserSendCommon> it(licqUserSend);
#else
  QPtrListIterator<UserSendCommon> it(licqUserSend);
#endif
  for (; it.current(); ++it)
  {
    if (strcmp((*it)->Id(), szId) == 0 && (*it)->PPID() == nPPID)
    {
      (*it)->SetConvoId(nConvoId);
      break;
    }
  }
}

void CMainWindow::slot_convoJoin(const char *szId, unsigned long nPPID, unsigned long nConvoId)
{
  // Add the user to an ongoing conversation
#if QT_VERSION < 300
  QListIterator<UserSendCommon> it(licqUserSend);
#else
  QPtrListIterator<UserSendCommon> it(licqUserSend);
#endif
  for (; it.current(); ++it)
  {
    if ((*it)->ConvoId() == nConvoId)
    {
      (*it)->convoJoin(szId, nConvoId);
      return;
    }
  }
  
  // No conversation shown, so make one
  
}

void CMainWindow::slot_convoLeave(const char *szId, unsigned long nPPID, unsigned long nConvoId)
{
  // Add the user to an ongoing conversation
#if QT_VERSION < 300
  QListIterator<UserSendCommon> it(licqUserSend);
#else
  QPtrListIterator<UserSendCommon> it(licqUserSend);
#endif
  for (; it.current(); ++it)
  {
    if ((*it)->ConvoId() == nConvoId)
    {
      (*it)->convoLeave(szId, nConvoId);
      return;
    }
  }
}

//-----CMainWindow::updateUserWin-----------------------------------------------
void CMainWindow::updateUserWin()
{
  // set the pixmap and color for each user and add them to the view
  userView->setUpdatesEnabled(false);
  userView->clear();

  bool doGroupView = m_bThreadView &&
    m_nGroupType == GROUPS_USER && m_nCurrentGroup == 0;

  if (doGroupView)
  {
    CUserViewItem* gi = new CUserViewItem(0, tr("Other Users").local8Bit(), userView);
    gi->setOpen(m_nGroupStates & 1);
    GroupList *g = gUserManager.LockGroupList(LOCK_R);
    for (unsigned short i = 0; i < g->size(); i++)
    {
      gi = new CUserViewItem(i+1, (*g)[i], userView);
      gi->setOpen(m_nGroupStates & (1<<QMIN(i+1, 31)));
    }
    gUserManager.UnlockGroupList();
  }
  FOR_EACH_USER_START(LOCK_R)
  {
    // Only show users on the current group and not on the ignore list
    if (!doGroupView && (!pUser->GetInGroup(m_nGroupType, m_nCurrentGroup) ||
      (pUser->IgnoreList() && m_nGroupType != GROUPS_SYSTEM && m_nCurrentGroup != GROUP_IGNORE_LIST) ))
      FOR_EACH_USER_CONTINUE

    // Ignore offline users if necessary
    if (!show_user(pUser))
      FOR_EACH_USER_CONTINUE;

    if (doGroupView)
    {
      for(CUserViewItem* gi = userView->firstChild(); gi; gi = gi->nextSibling())
      {
        if((gi->GroupId() != 0 && pUser->GetInGroup(GROUPS_USER, gi->GroupId())) ||
           (gi->GroupId() == 0 && pUser->GetGroups(GROUPS_USER) == 0 && !pUser->IgnoreList()))
          (void) new CUserViewItem(pUser, gi);
      }
    }
    else
      // Add the user to the list
      (void) new CUserViewItem(pUser, userView);
  }
  FOR_EACH_USER_END
  userView->setUpdatesEnabled(true);
  userView->triggerUpdate();
}


void CMainWindow::updateEvents()
{
  QString szCaption;
  unsigned short nNumOwnerEvents = 0;

  FOR_EACH_OWNER_START(LOCK_R)
  {
    nNumOwnerEvents += pOwner->NewMessages();
  }
  FOR_EACH_OWNER_END

  unsigned short nNumUserEvents = ICQUser::getNumUserEvents() - nNumOwnerEvents;

  lblMsg->setBold(false);
  QString s, l;

  if (nNumOwnerEvents > 0)
  {
    s = tr("SysMsg");
    l = tr("System Message");
    if (m_bBoldOnMsg) lblMsg->setBold(true);
    szCaption = "* " + m_szCaption;
//    lblMsg->setPrependPixmap(CMainWindow::iconForEvent(ICQ_CMDxSUB_MSG));
  }
  else if (nNumUserEvents > 0)
  {
    s = tr("%1 msg%2").arg(nNumUserEvents).arg(nNumUserEvents == 1 ? tr(" ") : tr("s"));
    l = tr("%1 message%2").arg(nNumUserEvents).arg(nNumUserEvents == 1 ? tr(" ") : tr("s"));
    if (m_bBoldOnMsg) lblMsg->setBold(true);
    szCaption = "* " + m_szCaption;
//    lblMsg->setPrependPixmap(QPixmap());
  }
  else
  {
    // Update the msg label if necessary
    if (m_bShowGroupIfNoMsg && ICQUser::getNumUserEvents() == 0)
    {
      //lblMsg->setText(cmbUserGroups->currentText());
      s = cmbUserGroups->currentText();
      l = cmbUserGroups->currentText();
    }
    else
    {
      //lblMsg->setText(tr("No msgs"));
      s = tr("No msgs");
      l = tr("No messages");
    }
    szCaption = m_szCaption;
//    lblMsg->setPrependPixmap(QPixmap());
  }
  if (lblMsg->fontMetrics().width(l)+lblMsg->margin() > lblMsg->width())
    lblMsg->setText(s);
  else
    lblMsg->setText(l);
  lblMsg->update();
  setCaption(szCaption);

  if (licqIcon) licqIcon->SetDockIconMsg(nNumUserEvents, nNumOwnerEvents);
}


//-----CMainWindow::setCurrentGroup---------------------------------------------
void CMainWindow::setCurrentGroupMenu(int id)
{
  int index = mnuUserGroups->indexOf(id);
  if (index > gUserManager.NumGroups() + 2)
    index -= 2;
  else if (index > 1)
    index -= 1;

  setCurrentGroup(index);
}

void CMainWindow::setCurrentGroup(int index)
{
  m_nCurrentGroup = index;
  m_nGroupType = GROUPS_USER;
  unsigned short nNumGroups = gUserManager.NumGroups();
  if (m_nCurrentGroup > nNumGroups)
  {
    m_nCurrentGroup -= nNumGroups;
    m_nGroupType = GROUPS_SYSTEM;
  }
  // Update the combo box
  cmbUserGroups->setCurrentItem(index);
  // Update the msg label if necessary
  if (m_bShowGroupIfNoMsg && ICQUser::getNumUserEvents() == 0)
    lblMsg->setText(cmbUserGroups->currentText());

  // Update the group menu
  for (unsigned short i = 0; i < mnuUserGroups->count(); i++)
    mnuUserGroups->setItemChecked(mnuUserGroups->idAt(i), false);
  if (index > gUserManager.NumGroups())
    index += 2;
  else if (index >= 1)
    index += 1;
  mnuUserGroups->setItemChecked(mnuUserGroups->idAt(index), true);

  // Update the user window
  updateUserWin();
}

//-----CMainWindow::updateGroups------------------------------------------------
void CMainWindow::updateGroups()
{
  // update the combo box
  cmbUserGroups->clear();
  mnuUserGroups->clear();
  mnuGroup->clear();
  mnuServerGroup->clear();
  cmbUserGroups->insertItem(tr("All Users"));
  mnuUserGroups->insertItem(tr("All Users"));
  mnuUserGroups->insertSeparator();

  // take care of this first
  mnuGroup->insertItem(tr("Server Group"), mnuServerGroup);
  mnuGroup->insertSeparator();

  GroupList *g = gUserManager.LockGroupList(LOCK_R);
  for (unsigned short i = 0; i < g->size(); i++)
  {
    cmbUserGroups->insertItem(QString::fromLocal8Bit((*g)[i]));
    mnuUserGroups->insertItem(QString::fromLocal8Bit((*g)[i]));
    mnuGroup->insertItem(QString::fromLocal8Bit((*g)[i]), i+1);
    mnuServerGroup->insertItem(QString::fromLocal8Bit((*g)[i]), i+1);
  }
  gUserManager.UnlockGroupList();
  mnuUserGroups->insertSeparator();
  mnuGroup->insertSeparator();

  cmbUserGroups->insertItem(tr("Online Notify"));
  mnuUserGroups->insertItem(tr("Online Notify"));
  mnuGroup->insertItem(tr("Online Notify"), 1000+GROUP_ONLINE_NOTIFY);
  cmbUserGroups->insertItem(tr("Visible List"));
  mnuUserGroups->insertItem(tr("Visible List"));
  mnuGroup->insertItem(tr("Visible List"), 1000+GROUP_VISIBLE_LIST);
  cmbUserGroups->insertItem(tr("Invisible List"));
  mnuUserGroups->insertItem(tr("Invisible List"));
  mnuGroup->insertItem(tr("Invisible List"), 1000+GROUP_INVISIBLE_LIST);
  cmbUserGroups->insertItem(tr("Ignore List"));
  mnuUserGroups->insertItem(tr("Ignore List"));
  mnuGroup->insertItem(tr("Ignore List"), 1000+GROUP_IGNORE_LIST);
  cmbUserGroups->insertItem(tr("New Users"));
  mnuUserGroups->insertItem(tr("New Users"));
  mnuGroup->insertItem(tr("New Users"), 1000+GROUP_NEW_USERS);

  int index = m_nCurrentGroup;
  if (m_nGroupType == GROUPS_SYSTEM)
    index += gUserManager.NumGroups();
  setCurrentGroup(index);
}


//-----CMainWindow::updateStatus------------------------------------------------
void CMainWindow::updateStatus(CICQSignal *s)
{
   char *theColor = skin->colors.offline;
   unsigned long nPPID = LICQ_PPID;
   if (s)
     nPPID = s->PPID();
   ICQOwner *o = gUserManager.FetchOwner(nPPID, LOCK_R);
   if (o != NULL)
   {
    unsigned long status = o->Status();
    switch (status)
    {
    case ICQ_STATUS_OFFLINE:
      theColor = skin->colors.offline;
      break;
    case ICQ_STATUS_ONLINE:
    case ICQ_STATUS_FREEFORCHAT:
      theColor = skin->colors.online;
      break;
    case ICQ_STATUS_AWAY:
    case ICQ_STATUS_NA:
    case ICQ_STATUS_OCCUPIED:
    case ICQ_STATUS_DND:
    default:
      theColor = skin->colors.away;
      break;
    }
  
    // Update the protocol menu if there is one
    int nAt = 0;
    if (m_lnProtMenu.size() ==  0)
      nAt = -1;
    else
    {
      std::vector<unsigned long>::iterator iter;
      for (iter = m_lnProtMenu.begin(); iter != m_lnProtMenu.end(); ++iter, nAt++)
        if (*iter == nPPID)
          break;
    }   

    if (nAt != -1)
    {
      if (status != ICQ_STATUS_OFFLINE)
        mnuProtocolStatus[nAt]->setItemChecked(CHANGE_STATUS_PRV,
          o->StatusInvisible());
          
      mnuProtocolStatus[nAt]->setItemChecked(CHANGE_STATUS_ONLINE | (nAt << 8),
        o->Status() == ICQ_STATUS_ONLINE); 
      mnuProtocolStatus[nAt]->setItemChecked(CHANGE_STATUS_AWAY | (nAt << 8),
        o->Status() == ICQ_STATUS_AWAY);
      mnuProtocolStatus[nAt]->setItemChecked(CHANGE_STATUS_NA | (nAt << 8),
        o->Status() == ICQ_STATUS_NA);
      mnuProtocolStatus[nAt]->setItemChecked(CHANGE_STATUS_OCC | (nAt << 8),
        o->Status() == ICQ_STATUS_OCCUPIED);
      mnuProtocolStatus[nAt]->setItemChecked(CHANGE_STATUS_DND | (nAt << 8),
        o->Status() == ICQ_STATUS_DND);
      mnuProtocolStatus[nAt]->setItemChecked(CHANGE_STATUS_FFC | (nAt << 8),
        o->Status() == ICQ_STATUS_FREEFORCHAT);
      mnuProtocolStatus[nAt]->setItemChecked(CHANGE_STATUS_OFFLINE | (nAt << 8),
        o->Status() == ICQ_STATUS_OFFLINE);
    }

    if (m_nProtoNum < 2)
    {
      // Only one protocol is loaded, show some text too
      lblStatus->clearPixmaps();
      lblStatus->setText(o->StatusStr());
      lblStatus->setPrependPixmap(CMainWindow::iconForStatus(o->StatusFull()));
      lblStatus->update();
    }
    else
    {
      gUserManager.DropOwner(nPPID);
      
      // Show multiple icons for each protocol
      lblStatus->clearPrependPixmap();
      lblStatus->setText("");
      lblStatus->clearPixmaps();
      
      vector<unsigned long>::iterator it;
      for (it = m_lnProtMenu.begin(); it != m_lnProtMenu.end(); it++)
      {
        o = gUserManager.FetchOwner(*it, LOCK_R);
        if (o)
        {
          lblStatus->addPixmap(CMainWindow::iconForStatus(o->StatusFull(),
            o->IdString(), *it));
          gUserManager.DropOwner(*it);
        }
      }
      
      lblStatus->update();
      
      o = gUserManager.FetchOwner(nPPID, LOCK_R);
    }
    
    // set icon of the licq main widget for window manager
    if (o)
    {
#ifdef USE_KDE
#if KDE_VERSION >= 320
    /* KDE 3.2 handles app-icon updates differently, since KDE 3.2 a simple setIcon() call
        does no longer update the icon in kicker anymore :(
        So we do it the "kde-way" here */
      KWin::setIcons(winId(), CMainWindow::iconForStatus(o->StatusFull()),
        CMainWindow::iconForStatus(o->StatusFull()));
#else    // KDE_VERSION
      setIcon(CMainWindow::iconForStatus(o->StatusFull()));
#endif   // KDE_VERSION
#else   // USE_KDE
      setIcon(CMainWindow::iconForStatus(o->StatusFull()));
#endif   // USE_KDE
  
      gUserManager.DropOwner(nPPID);
    }
   }
   else
   {
     lblStatus->clearPrependPixmap();
     lblStatus->setText("");
     lblStatus->clearPixmaps();
     
     vector<unsigned long>::iterator it;
     for (it = m_lnProtMenu.begin(); it != m_lnProtMenu.end(); it++)
     {
       o = gUserManager.FetchOwner(*it, LOCK_R);
       if (o)
       {
         lblStatus->addPixmap(CMainWindow::iconForStatus(o->StatusFull(),
           o->IdString(), *it));
         gUserManager.DropOwner(*it);
       }
     }
     
     lblStatus->update();
   }
       
   // set the color if it isn't set by the skin
   if (skin->lblStatus.color.fg == NULL) lblStatus->setNamedFgColor(theColor);

  if (licqIcon) licqIcon->SetDockIconStatus();
}


void CMainWindow::slot_AwayMsgDlg()
{
  //TODO iterate all owners that support fetching away message
  ICQOwner *o = gUserManager.FetchOwner(LOCK_R);
  if(o != NULL) {
    showAwayMsgDlg(o->Status());
    gUserManager.DropOwner();
  }
}


void CMainWindow::slot_doneAwayMsgDlg()
{
  awayMsgDlg = NULL;
}


void CMainWindow::showAwayMsgDlg(unsigned short nStatus, bool autoclose)
{
  if(awayMsgDlg == NULL) {
    awayMsgDlg = new AwayMsgDlg();
    connect(awayMsgDlg, SIGNAL(popupOptions(int)), this, SLOT(showOptionsDlg(int)));
    connect(awayMsgDlg, SIGNAL(done()), this, SLOT(slot_doneAwayMsgDlg()));
  }
  else
    awayMsgDlg->raise();

  awayMsgDlg->SelectAutoResponse(nStatus, autoclose);
}


//----CMainWindow::changeStatusManual-------------------------------------------
void CMainWindow::changeStatusManualProtocol(int id)
{
  int nAt = (id & 0xFF00) >> 8;
  int nPPID = m_lnProtMenu[nAt];
  int nRealID = 0;
  
  if (id & CHANGE_STATUS_PRV)
    nRealID |= ICQ_STATUS_FxPRIVATE;
    
  if (id & CHANGE_STATUS_ONLINE)
    nRealID |= ICQ_STATUS_ONLINE;
  else if (id & CHANGE_STATUS_OFFLINE)
    nRealID |= ICQ_STATUS_OFFLINE;
  else if (id & CHANGE_STATUS_FFC)
    nRealID |= ICQ_STATUS_FREEFORCHAT;
  else if (id & CHANGE_STATUS_DND)
    nRealID |= ICQ_STATUS_DND;
  else if (id & CHANGE_STATUS_OCC)
    nRealID |= ICQ_STATUS_OCCUPIED;
  else if (id & CHANGE_STATUS_AWAY)
    nRealID |= ICQ_STATUS_AWAY;
  else if (id & CHANGE_STATUS_NA)
    nRealID |= ICQ_STATUS_NA;
  
  if (nRealID != ICQ_STATUS_OFFLINE && (nRealID & 0xFF) != ICQ_STATUS_ONLINE)
    showAwayMsgDlg(nRealID, true);

  changeStatus(nRealID, nPPID);
}

//----CMainWindow::changeStatusManual-------------------------------------------
void CMainWindow::changeStatusManual(int id)
{
  if (id != ICQ_STATUS_OFFLINE && (id & 0xFF) != ICQ_STATUS_ONLINE)
    showAwayMsgDlg(id, true);

  changeStatus(id);
}


//----CMainWindow::changeStatus-------------------------------------------------
void CMainWindow::changeStatus(int id, unsigned long _nPPID, bool _bAutoLogon)
{
  unsigned long newStatus = ICQ_STATUS_OFFLINE;
  bool bLicqOnly = false;
  
  // we may have been offline and gone online with invisible toggled
  // so we will check the main invisiblity toggle for all plugins to obey.
  bool bAllInvis = false;
  if (_bAutoLogon)
  {
    if (_nPPID == 0xFFFFFFFF && mnuStatus->isItemChecked(ICQ_STATUS_FxPRIVATE)) 
    {
      newStatus |= ICQ_STATUS_FxPRIVATE;
      bAllInvis = true;
    }
  }
  else if (_nPPID == 0xFFFFFFFF && id == ICQ_STATUS_FxPRIVATE)
  {
    bAllInvis = !mnuStatus->isItemChecked(ICQ_STATUS_FxPRIVATE);
    mnuStatus->setItemChecked(ICQ_STATUS_FxPRIVATE, bAllInvis);
  }

  ProtoPluginsList pl;
  ProtoPluginsListIter it;
  licqDaemon->ProtoPluginList(pl);
  if (pl.size() == 1)
    bLicqOnly = true;
    
  for (it = pl.begin(); it != pl.end(); it++)
  {
    unsigned long nPPID = (*it)->PPID();
    if (_nPPID != 0xFFFFFFFF && nPPID != _nPPID)
      continue;
      
    // Find the menu to work with
    std::vector<unsigned long>::iterator itMenu;
    int nAt = -1;
    for (itMenu = m_lnProtMenu.begin(); itMenu != m_lnProtMenu.end(); ++itMenu)
    {
      nAt++;
      if ((*itMenu) == nPPID)
        break;
    }

    QPopupMenu *pMenu;
    int nInvisibleLocation;
    if (bLicqOnly)
    {
      nInvisibleLocation = ICQ_STATUS_FxPRIVATE;
      pMenu = mnuStatus;
    }
    else
    {
      nInvisibleLocation = (CHANGE_STATUS_PRV | nAt << 8);
      pMenu = mnuProtocolStatus[nAt];
    }
        
    ICQOwner *o = gUserManager.FetchOwner(nPPID, LOCK_R);
    if (o == NULL) continue;
              
    if (id == ICQ_STATUS_OFFLINE)
    {
      gUserManager.DropOwner(nPPID);
      licqDaemon->ProtoLogoff(nPPID);
      continue;
    }
    else if (id == (int)ICQ_STATUS_FxPRIVATE) // toggle invisible status
    {    
      if (_nPPID == 0xFFFFFFFF)
       pMenu->setItemChecked(nInvisibleLocation, bAllInvis);
      else
        pMenu->setItemChecked(nInvisibleLocation,
          !pMenu->isItemChecked(nInvisibleLocation));
          
      if (o->StatusOffline())
      {
        gUserManager.DropOwner(nPPID);
        continue;
      }

      if (pMenu->isItemChecked(nInvisibleLocation))
        newStatus = o->StatusFull() | ICQ_STATUS_FxPRIVATE;
      else
        newStatus = o->StatusFull() & (~ICQ_STATUS_FxPRIVATE);
    }
    else
    {
      newStatus = id;
      if (pMenu->isItemChecked(nInvisibleLocation))
        newStatus |= ICQ_STATUS_FxPRIVATE;
    }

    // Just to be safe
    if (bAllInvis)
    {
      newStatus |= ICQ_STATUS_FxPRIVATE;
      if (nAt != -1)
        mnuProtocolStatus[nAt]->setItemChecked(nInvisibleLocation, true);
    }
      
    // disable combo box, flip pixmap...
    //lblStatus->setEnabled(false);

    // call the right function
    bool b = o->StatusOffline();
    gUserManager.DropOwner(nPPID);
    if (b)
      licqDaemon->ProtoLogon(nPPID, newStatus);
    else
      licqDaemon->ProtoSetStatus(nPPID, newStatus);
  }
}

//----CMainWindow::changeStatus-------------------------------------------------
void CMainWindow::changePFMStatus(int id)
{
  licqDaemon->icqSetPhoneFollowMeStatus(id);
  for (unsigned int i = 0; i < mnuPFM->count(); i++)
    mnuPFM->setItemChecked(mnuPFM->idAt(i), false);
  mnuPFM->setItemChecked(id, true);
}

// -----------------------------------------------------------------------------

void CMainWindow::callDefaultFunction(const char *_szId, unsigned long _nPPID)
{
  if (_szId == 0 || _nPPID == 0) return;

  ICQUser *u = gUserManager.FetchUser(_szId, _nPPID, LOCK_R);

  if (u == 0) return;

  // For multi user conversations (i.e. in MSN)
  int nConvoId = -1;
  
  // set default function to read or send depending on whether or not
  // there are new messages
  int fcn = (u->NewMessages() == 0 ? mnuUserSendMsg : mnuUserView);
  if (fcn == mnuUserView && m_bMsgChatView)
  {
    // if one of the new events is a msg in chatview mode,
    // change def function to send
    for (unsigned short i = 0; i < u->NewMessages(); i++)
      if (u->EventPeek(i)->SubCommand() == ICQ_CMDxSUB_MSG)
      {
        nConvoId = u->EventPeek(i)->ConvoId();
        fcn = mnuUserSendMsg;
        break;
      }
  }
  gUserManager.DropUser(u);

  // See if the clipboard contains a url
  if (fcn == mnuUserSendMsg && m_bSendFromClipboard)
  {
    QString c = QApplication::clipboard()->text();
    if (c.left(5) == "http:" || c.left(4) == "ftp:" || c.left(6) == "https:")
    {
      UserEventCommon *ec = callFunction(mnuUserSendUrl, _szId, _nPPID);
      if (!ec || !ec->inherits("UserSendUrlEvent")) return;
      UserSendUrlEvent* e = static_cast<UserSendUrlEvent*>(ec);
      // Set the url
      e->setUrl(c, "");
      // Clear the buffer now
      QApplication::clipboard()->clear();
      return;
    }
    else if (c.left(5) == "file:" || c.left(1) == "/")
    {
      UserEventCommon *ec = callFunction(mnuUserSendFile, _szId, _nPPID);
      if (!ec || !ec->inherits("UserSendFileEvent")) return;
      UserSendFileEvent* e = static_cast<UserSendFileEvent*>(ec);
      // Set the file
      if(c.left(5) == "file:")
        c.remove(0, 5);
      while (c[0] == '/') c.remove(0, 1);
      c.prepend('/');
      e->setFile(c, "");
      // Clear the buffer now
      QApplication::clipboard()->clear();
      return;
    }
  }

  callFunction(fcn, _szId, _nPPID, nConvoId);
}

void CMainWindow::callDefaultFunction(QListViewItem *i)
{
  if(i == NULL)
    return;

  callDefaultFunction(((CUserViewItem *)i)->ItemId(),
    ((CUserViewItem *)i)->ItemPPID());
}

void CMainWindow::callOwnerFunction(int index, unsigned long nPPID)
{
  int nAt = (index & 0x00FF0000) >> 16;
  unsigned long nThisPPID = nAt > 0 ? m_lnProtMenu[nAt] : LICQ_PPID;
  index &= 0x0000FFFF;
  
  if (index == OwnerMenuView)
  {
    ProtoPluginsList pl; 
    ProtoPluginsListIter it;
    licqDaemon->ProtoPluginList(pl);
    for (it = pl.begin(); it != pl.end(); it++)
    {
      char *szId = 0;
      ICQOwner *o = gUserManager.FetchOwner((*it)->PPID(), LOCK_R);
      if (o == 0) continue;
      szId = strdup(o->IdString());
      unsigned short nNumMsg = o->NewMessages();
      gUserManager.DropOwner((*it)->PPID());
      
      if (nNumMsg > 0)
        callFunction(index, szId, (*it)->PPID());

      free(szId);
    } 
  }

  else if (index == OwnerMenuGeneral || index == OwnerMenuHistory)
  {
    char *szId;
    ProtoPluginsList pl;
    ProtoPluginsListIter it;
    licqDaemon->ProtoPluginList(pl);
    for (it = pl.begin(); it != pl.end(); it++)
    {
      if ((*it)->PPID() == nThisPPID)
      {
        ICQOwner *o = gUserManager.FetchOwner((*it)->PPID(), LOCK_R);
        if (o == 0) continue;
        szId = strdup(o->IdString());
        gUserManager.DropOwner((*it)->PPID());
        callInfoTab(index, szId, (*it)->PPID());
        free(szId);
      }
    }
  }

  else if (index == OwnerMenuSecurity)
    (void) new SecurityDlg(licqDaemon, licqSigMan);

  else if (index == OwnerMenuRandomChat)
    (void) new CSetRandomChatGroupDlg(licqDaemon, licqSigMan);

  else if (index == OwnerMenuManager)
    showOwnerManagerDlg();
    
  else
    gLog.Warn("%sInternal Error: CMainWindow::callOwnerFunction(): Unknown index (%d).\n",
              L_WARNxSTR, index);
}


void CMainWindow::callUrlFunction (const char *_szUrl)
{
//  ICQFunctions *f = callFunction(-1, true);
//  if (f != NULL) f->SendUrl(_szUrl, "");
}

void CMainWindow::callFileFunction (const char *_szFile)
{
//  ICQFunctions *f = callFunction(-1, true);
//  if (f != NULL) f->SendFile(_szFile, "");
}


void CMainWindow::callMsgFunction()
{
  // No need for code duplication
  slot_ui_viewevent("0");
}

//-----CMainWindow::callUserFunction-------------------------------------------
void CMainWindow::callUserFunction(int index)
{
  if (m_szUserMenuId == 0 || m_nUserMenuPPID == 0) return;

  char *szId = strdup(m_szUserMenuId); //must free
  unsigned long nPPID = m_nUserMenuPPID;

  switch(index)
  {
    case mnuUserAuthorize:
    {
      (void) new AuthUserDlg(licqDaemon, szId, nPPID, true);
      break;
    }
    case mnuUserAuthorizeRequest:
    {
      (void) new ReqAuthDlg(licqDaemon, szId, nPPID);
      break;
    }
    case mnuUserCheckIfInvisible:
    {
      licqDaemon->icqCheckInvisible(szId);
      break;
    }
    case mnuUserCheckResponse:
    {
      (void) new ShowAwayMsgDlg(licqDaemon, licqSigMan, szId, nPPID);
      break;
    }
    case mnuUserCustomAutoResponse:
    {
      (void) new CustomAwayMsgDlg(szId, nPPID);
      break;
    }
    case mnuUserFloaty:
    {
      // Check that the floaty does not already exist
      CUserView *v = CUserView::FindFloaty(szId, nPPID);
      if (v == NULL)
      {
        CreateUserFloaty(szId, nPPID);
      }
      else
      {
        delete v->firstChild();
        if (v->childCount() == 0) delete v;
      }
      break;
    }
#ifdef HAVE_LIBGPGME
    case mnuUserSelectGPGKey:
       new GPGKeySelect( szId, nPPID );
      break;
#endif
    case mnuUserHistory:
    case mnuUserGeneral:
      callInfoTab(index, szId, nPPID);
      break;
    case mnuUserRemoveFromList:
      RemoveUserFromList(szId, nPPID, this);
      break;

    case mnuUserSendKey:
    {
      (void) new KeyRequestDlg(licqSigMan, szId, nPPID);
      break;
    }

    case mnuUserSendInfoPluginListRequest:
    {
      if (nPPID == LICQ_PPID)
        licqDaemon->icqRequestInfoPluginList(szId, true);
      break;
    }

    case mnuUserSendStatusPluginListRequest:
    {
      if (nPPID == LICQ_PPID)
        licqDaemon->icqRequestStatusPluginList(szId, true);
      break;
    }

    case mnuUserSendPhoneFollowMeRequest:
    {
      if (nPPID == LICQ_PPID)
        licqDaemon->icqRequestPhoneFollowMe(szId, true);
      break;
    }

    case mnuUserSendICQphoneRequest:
    {
      if (nPPID == LICQ_PPID)
        licqDaemon->icqRequestICQphone(szId, true);
      break;
    }

    case mnuUserSendFileServerRequest:
    {
      if (nPPID == LICQ_PPID)
        licqDaemon->icqRequestSharedFiles(szId, true);
      break;
    }

    default:
      callFunction(index, szId, nPPID);
  }

  if (szId)
    free(szId);
}

void CMainWindow::callInfoTab(int fcn, const char *szId, unsigned long nPPID,
  bool toggle, bool bUpdateNow)
{
  if(szId == 0 || nPPID == 0) return;

  UserInfoDlg *f = NULL;
#if QT_VERSION < 300
  QListIterator<UserInfoDlg> it(licqUserInfo);
#else
  QPtrListIterator<UserInfoDlg> it(licqUserInfo);
#endif

  for(; it.current(); ++it)
  {
    if(strcmp((*it)->Id(), szId) == 0 && (*it)->PPID() == nPPID)
    {
      f = *it;
      break;
    }
  }

  if (f)
  {
    int tab = UserInfoDlg::WorkInfo;
    switch(fcn) {
    case mnuUserHistory:
      tab = UserInfoDlg::HistoryInfo;
      break;
    case mnuUserGeneral:
      if (isalpha(szId[0]))
        tab = UserInfoDlg::AboutInfo;
      else
        tab = UserInfoDlg::GeneralInfo;
      break;
    }
    if(toggle && f->isTabShown(tab))
    {
      delete f; // will notify us about deletion
      return;
    }
    else {
      f->show();
      f->raise();
    }
  }
  else
  {
    f = new UserInfoDlg(licqDaemon, licqSigMan, this, szId, nPPID);
    connect(f, SIGNAL(finished(const char *, unsigned long)), this, SLOT(UserInfoDlg_finished(const char *, unsigned long)));
    f->show();
    licqUserInfo.append(f);
  }

  switch(fcn)
  {
    case mnuUserHistory:
      f->showTab(UserInfoDlg::HistoryInfo);
      break;
    case mnuUserGeneral:
      if (isalpha(szId[0]))
        f->showTab(UserInfoDlg::AboutInfo);
      else
        f->showTab(UserInfoDlg::GeneralInfo);
      break;
  }
  f->show();
  f->raise();
  if (bUpdateNow)
    f->retrieveSettings();
}


//-----CMainWindow::callICQFunction-------------------------------------------
UserEventCommon *CMainWindow::callFunction(int fcn, const char *szId,
                                           unsigned long nPPID, int nConvoId)
{
  if (szId == 0 || nPPID == 0) return NULL;

  UserEventCommon *e = NULL;

  switch (fcn)
  {
    case mnuUserView:
    {
#if QT_VERSION < 300
      QListIterator<UserViewEvent> it(licqUserView);
#else
      QPtrListIterator<UserViewEvent> it(licqUserView);
#endif
      for (; it.current(); ++it)
        if ((*it)->Id() && strcasecmp((*it)->Id(), szId) == 0 &&
            (*it)->PPID() == nPPID)
        {
          e = *it;
          e->show();
          if(!qApp->activeWindow() || !qApp->activeWindow()->inherits("UserEventCommon"))
          {
            e->raise();
#ifdef USE_KDE
            KWin::setActiveWindow(e->winId());
#endif
          }
          return e;
        }
    }
    break;
    case mnuUserSendMsg:
    case mnuUserSendUrl:
    case mnuUserSendChat:
    case mnuUserSendFile:
    case mnuUserSendContact:
    case mnuUserSendSms:
    {
#if QT_VERSION < 300
        QListIterator<UserSendCommon> it(licqUserSend );
#else
        QPtrListIterator<UserSendCommon> it(licqUserSend);
#endif

        if (!m_bMsgChatView) break;

        for (; it.current(); ++it)
          // Protocols (MSN only atm) that support convo ids are differentiated from
          // the icq protocol because the convo id will be the server socket.. which does
          // not meet the requirement that convo ids must be unique for each conversation.
          if ( ((nPPID == MSN_PPID && (*it)->PPID() == MSN_PPID) && ((*it)->FindUserInConvo(const_cast<char *>(szId)) ||
                 ((*it)->ConvoId() == nConvoId && (*it)->ConvoId() != -1))) ||
               ((*it)->FindUserInConvo(const_cast<char *>(szId)) && (*it)->PPID() == nPPID))
          {
            e = static_cast<UserSendCommon*>(*it);
            //if (!e->FindUserInConvo(const_cast<char *>(szId)))
            //  e->convoJoin(szId);
#if QT_VERSION >= 300
            if (userEventTabDlg && userEventTabDlg->tabExists(e))
            {
              userEventTabDlg->show();
              userEventTabDlg->selectTab(e);
              userEventTabDlg->raise();
#ifdef USE_KDE
              KWin::setActiveWindow(userEventTabDlg->winId());
#endif
            }
            else
#endif
            {
              e->show();
              if (!qApp->activeWindow() || !qApp->activeWindow()->inherits("UserEventCommon"))
              {
                e->raise();
#ifdef USE_KDE
                KWin::setActiveWindow(e->winId());
#endif
              }
            }
            return e;
          }
    }
  default:
    break;
  }

  QWidget *parent = NULL;
#if QT_VERSION >= 300
  if (m_bTabbedChatting)
  {
    if (userEventTabDlg != NULL)
      userEventTabDlg->raise();
    else
    {
      // create the tab dialog if it does not exist
      userEventTabDlg = new UserEventTabDlg(this);
      connect(userEventTabDlg, SIGNAL(signal_done()), this, SLOT(slot_doneUserEventTabDlg()));
    }
    parent = userEventTabDlg;
  }
#endif

  switch (fcn)
  {
    case mnuUserView:
    {
      e = new UserViewEvent(licqDaemon, licqSigMan, this, szId, nPPID);
      break;
    }
    case mnuUserSendMsg:
    {
      e = new UserSendMsgEvent(licqDaemon, licqSigMan, this, szId, nPPID, parent);
      break;
    }
    case mnuUserSendUrl:
    {
      e = new UserSendUrlEvent(licqDaemon, licqSigMan, this, szId, nPPID, parent);
      break;
    }
    case mnuUserSendChat:
    {
      e = new UserSendChatEvent(licqDaemon, licqSigMan, this, szId, nPPID, parent);
      break;
    }
    case mnuUserSendFile:
    {
      e = new UserSendFileEvent(licqDaemon, licqSigMan, this, szId, nPPID, parent);
      break;
    }
    case mnuUserSendContact:
    {
      e = new UserSendContactEvent(licqDaemon, licqSigMan, this, szId, nPPID, parent);
      break;
    }
    case mnuUserSendSms:
    {
      e = new UserSendSmsEvent(licqDaemon, licqSigMan, this, szId, nPPID, parent);
      break;
    }
    default:
      gLog.Warn("%sunknown callFunction() fcn: %d\n", L_WARNxSTR, fcn);
  }
  if (e == NULL) return NULL;

  connect(e, SIGNAL(viewurl(QWidget*, QString)), this, SLOT(slot_viewurl(QWidget *, QString)));
#if QT_VERSION >= 300
  if (m_bTabbedChatting && fcn != mnuUserView)
  {
    userEventTabDlg->addTab(e);
    userEventTabDlg->show();
  }
  else
#endif
    e->show();

  // there might be more than one send window open
  // make sure we only remember one, or it will get complicated
  if (fcn == mnuUserView)
  {
    slot_userfinished(szId, nPPID);
    connect(e, SIGNAL(finished(const char *, unsigned long)),
      SLOT(slot_userfinished(const char *, unsigned long)));
    licqUserView.append(static_cast<UserViewEvent*>(e));
  }
  else
  {
    slot_sendfinished(szId, nPPID);
    connect(e, SIGNAL(finished(const char *, unsigned long)),
      SLOT(slot_sendfinished(const char *, unsigned long)));
    licqUserSend.append(static_cast<UserSendCommon*>(e));
  }
  return e;
}

// -----------------------------------------------------------------------------
void CMainWindow::UserInfoDlg_finished(const char *szId, unsigned long nPPID)
{
#if QT_VERSION < 300
  QListIterator<UserInfoDlg> it(licqUserInfo);
#else
  QPtrListIterator<UserInfoDlg> it(licqUserInfo);
#endif

  for ( ; it.current(); ++it)
  {
    if ((*it)->PPID() == nPPID && strcmp((*it)->Id(), szId) == 0)
    {
      licqUserInfo.remove(*it);
      return;
    }
  }

  gLog.Warn("%sUser Info finished signal for user with no window (%s)!\n",
            L_WARNxSTR, szId);
}


// -----------------------------------------------------------------------------
void CMainWindow::slot_doneUserEventTabDlg()
{
#if QT_VERSION >= 300
  userEventTabDlg = NULL;
#endif
}

void CMainWindow::slot_userfinished(const char *szId, unsigned long nPPID)
{
#if QT_VERSION < 300
  QListIterator<UserViewEvent> it(licqUserView);
#else
  QPtrListIterator<UserViewEvent> it(licqUserView);
#endif

  for ( ; it.current(); ++it)
  {
    if ((*it)->PPID() == nPPID && (*it)->Id() &&
       strcmp((*it)->Id(), szId) == 0)
    {
      licqUserView.remove(*it);
      return;
    }
  }
}

void CMainWindow::slot_sendfinished(const char *szId, unsigned long nPPID)
{
#if QT_VERSION < 300
  QListIterator<UserSendCommon> it(licqUserSend);
#else
  QPtrListIterator<UserSendCommon> it(licqUserSend);
#endif
  // go through the whole list, there might be more than
  // one hit
  for ( ; it.current(); ++it)
    if ((*it)->PPID() == nPPID && (*it)->Id() &&
        strcmp((*it)->Id(), szId) == 0)
      licqUserSend.remove(*it);
}

void CMainWindow::slot_shutdown()
{
  licqDaemon->Shutdown();
}

void CMainWindow::slot_aboutToQuit()
{
  // nothing to do
//  qDebug("aboutToQuit!");
}

//-----CMainWindow::slot_logon------------------------------------------------
void CMainWindow::slot_logon()
{
  updateStatus();
  //lblStatus->setEnabled(true);
}


//-----CMainWindow::slot_ui_viewevent-------------------------------------------
void CMainWindow::slot_ui_viewevent(const char *szId)
{
  // Do nothing if there are no events pending
  if (ICQUser::getNumUserEvents() == 0 || szId == 0) return;

  unsigned long nPPID = 0;

  if (strcmp(szId, "0") == 0)
  {
    // Do system messages first
    ProtoPluginsList pl;
    ProtoPluginsListIter it;
    licqDaemon->ProtoPluginList(pl);
    for (it = pl.begin(); it != pl.end(); it++)
    {
      ICQOwner *o = gUserManager.FetchOwner((*it)->PPID(), LOCK_R);
      if (o == 0) continue; // just in case
      unsigned short nNumMsg = o->NewMessages();
      gUserManager.DropOwner((*it)->PPID());
      if (nNumMsg > 0)
      {
        callOwnerFunction(OwnerMenuView, (*it)->PPID());
        return;
      }
    }
    
    time_t t = time(NULL);
    FOR_EACH_USER_START(LOCK_R)
    {
      if (pUser->NewMessages() > 0 && pUser->Touched() <= t)
      {
        szId = pUser->IdString();
        nPPID = pUser->PPID();
        t = pUser->Touched();
      }
    }
    FOR_EACH_USER_END
  }

  if (szId)
  {
    if (m_bMsgChatView)
    {
      ICQUser *u = 0;
      if (nPPID == 0)
      {
        ProtoPluginsList pl;
        ProtoPluginsListIter it;
        licqDaemon->ProtoPluginList(pl);
        for (it = pl.begin(); it != pl.end(); it++)
        {
          u = gUserManager.FetchUser(szId, (*it)->PPID(), LOCK_R);
          if (u)
          {
            if (u->NewMessages())
            {
              nPPID = (*it)->PPID();
              break;
            }
            else
            {
              gUserManager.DropUser(u);
              u = 0;
            }
          }
        }  
      }
      else
        u = gUserManager.FetchUser(szId, nPPID, LOCK_R);
        
      if (u == 0) return;
      
      for (unsigned short i = 0; i < u->NewMessages(); i++)
      {
        if (u->EventPeek(i)->SubCommand() == ICQ_CMDxSUB_MSG)
        {
          gUserManager.DropUser(u);
          callFunction(mnuUserSendMsg, szId, nPPID, u->EventPeek(i)->ConvoId());
          return;
        }
      }
      gUserManager.DropUser(u);
      callFunction(mnuUserView, szId, nPPID);
    }
    else
      callFunction(mnuUserView, szId, nPPID);
  }
}

//-----CMainWindow::slot_ui_message---------------------------------------------
void CMainWindow::slot_ui_message(const char *szId, unsigned long nPPID)
{
  callFunction(mnuUserSendMsg, szId, nPPID);
}

//-----slot_protocolPlugin------------------------------------------------------
void CMainWindow::slot_protocolPlugin(unsigned long nPPID)
{
  bool bMSN = (nPPID == MSN_PPID);
   
  // We can now add the users of this protocol
  FOR_EACH_PROTO_USER_START(nPPID, LOCK_R)
  {
    //FIXME
    // Add the user to the list
    //(void) new CUserViewItem(pUser, userView);
  }
  FOR_EACH_PROTO_USER_END

  // Menu item for the status of this protocol
  if (m_nProtoNum == 0)
  {
    // Add ICQ status menu
    mnuProtocolStatus[m_nProtoNum] = new QPopupMenu(NULL);
    mnuProtocolStatus[m_nProtoNum]->insertItem(pmOnline, tr("&Online"),
      CHANGE_STATUS_ONLINE);
    mnuProtocolStatus[m_nProtoNum]->insertItem(pmAway, tr("&Away"),
      CHANGE_STATUS_AWAY);
    mnuProtocolStatus[m_nProtoNum]->insertItem(pmNa, tr("&Not Available"),
      CHANGE_STATUS_NA);
    mnuProtocolStatus[m_nProtoNum]->insertItem(pmOccupied, tr("O&ccupied"),
      CHANGE_STATUS_OCC);
    mnuProtocolStatus[m_nProtoNum]->insertItem(pmDnd, tr("&Do Not Disturb"),
      CHANGE_STATUS_DND);
    mnuProtocolStatus[m_nProtoNum]->insertItem(pmFFC, tr("Free for C&hat"),
      CHANGE_STATUS_FFC);
    mnuProtocolStatus[m_nProtoNum]->insertItem(pmOffline, tr("O&ffline"),
      CHANGE_STATUS_OFFLINE);
    mnuProtocolStatus[m_nProtoNum]->insertSeparator();
    mnuProtocolStatus[m_nProtoNum]->insertItem(pmPrivate, tr("&Invisible"),
      CHANGE_STATUS_PRV);
    mnuStatus->insertItem("ICQ", mnuProtocolStatus[m_nProtoNum], -1, m_nProtoNum);
    connect(mnuProtocolStatus[m_nProtoNum], SIGNAL(activated(int)),
      this, SLOT(changeStatusManualProtocol(int)));
    m_lnProtMenu.push_back(LICQ_PPID);
    
    // System functions menu
    mnuOwnerAdm->removeItemAt(2);
    mnuOwnerAdm->removeItemAt(2);
      
    mnuProtocolOwnerAdm[m_nProtoNum] = new QPopupMenu(NULL);
    mnuProtocolOwnerAdm[m_nProtoNum]->insertItem(pmInfo, tr("&Info"), OwnerMenuGeneral);
    mnuProtocolOwnerAdm[m_nProtoNum]->insertItem(pmHistory, tr("View &History"), OwnerMenuHistory);
    connect(mnuProtocolOwnerAdm[m_nProtoNum], SIGNAL(activated(int)),
    this, SLOT(callOwnerFunction(int)));
    mnuOwnerAdm->insertItem("ICQ", mnuProtocolOwnerAdm[m_nProtoNum], -1, 2);
      
    m_nProtoNum++;
  }
  else
      mnuStatus->removeItemAt(m_nProtoNum+1); // Move separator

   
  // Temp pointers that if they are not null, will be in the menu     
  QPixmap *pOnline, *pAway, *pNA, *pOcc,*pDND, *pFFC, *pOffline, *pPrivate;
  pOnline = pAway = pNA = pOcc = pDND = pFFC = pOffline = pPrivate = 0;
  
  // Set icons per protocol here
  if (bMSN)
  {
    pOnline = &pmMSNOnline;
    pAway = &pmMSNAway;
    pOcc = &pmMSNOccupied;
    pOffline = &pmMSNOffline;
    pPrivate = &pmMSNOffline;
  }
  
  char *pName = licqDaemon->ProtoPluginName(nPPID);
  
  // Create the menu items now
  mnuProtocolStatus[m_nProtoNum] = new QPopupMenu(NULL);
  if (pOnline)
    mnuProtocolStatus[m_nProtoNum]->insertItem(*pOnline, tr("&Online"),
      CHANGE_STATUS_ONLINE | (m_nProtoNum << 8));
  if (pAway)
    mnuProtocolStatus[m_nProtoNum]->insertItem(*pAway, tr("&Away"),
      CHANGE_STATUS_AWAY | (m_nProtoNum << 8));
  if (pNA)
    mnuProtocolStatus[m_nProtoNum]->insertItem(*pNA, tr("&Not Available"),
      CHANGE_STATUS_NA | (m_nProtoNum << 8));
  if (pOcc)
    mnuProtocolStatus[m_nProtoNum]->insertItem(*pOcc, tr("O&ccupied"),
      CHANGE_STATUS_OCC | (m_nProtoNum << 8));
  if (pDND)
    mnuProtocolStatus[m_nProtoNum]->insertItem(*pDND, tr("&Do Not Disturb"),
      CHANGE_STATUS_DND | (m_nProtoNum << 8));
  if (pFFC)
    mnuProtocolStatus[m_nProtoNum]->insertItem(*pFFC, tr("Free for C&hat"),
      CHANGE_STATUS_FFC | (m_nProtoNum << 8));
  if (pOffline)
    mnuProtocolStatus[m_nProtoNum]->insertItem(*pOffline, tr("O&ffline"),
      CHANGE_STATUS_OFFLINE | (m_nProtoNum << 8));
  if (pPrivate)
  {
    mnuProtocolStatus[m_nProtoNum]->insertSeparator();
    mnuProtocolStatus[m_nProtoNum]->insertItem(*pPrivate, tr("&Invisible"),
      CHANGE_STATUS_PRV | (m_nProtoNum << 8));
  }
  mnuStatus->insertItem(pName ? pName : "(No Name)",
    mnuProtocolStatus[m_nProtoNum], -1, m_nProtoNum);
  mnuStatus->insertSeparator(m_nProtoNum + 1);
  //Connect to emit a different signal.  One that will give the PPID and
  //status to change to one slot.
  connect(mnuProtocolStatus[m_nProtoNum], SIGNAL(activated(int)),
    this, SLOT(changeStatusManualProtocol(int)));
  m_lnProtMenu.push_back(nPPID);
  
  // Create the system functions now
  mnuProtocolOwnerAdm[m_nProtoNum] = new QPopupMenu(NULL);
  mnuProtocolOwnerAdm[m_nProtoNum]->insertItem(pmInfo, tr("&Info"),
    OwnerMenuGeneral | (m_nProtoNum << 16));
  mnuProtocolOwnerAdm[m_nProtoNum]->insertItem(pmHistory, tr("View &History"),
    OwnerMenuHistory | (m_nProtoNum << 16));
  connect(mnuProtocolOwnerAdm[m_nProtoNum], SIGNAL(activated(int)),
    this, SLOT(callOwnerFunction(int)));
  mnuOwnerAdm->insertItem(pName ? pName : "(No Name)", mnuProtocolOwnerAdm[m_nProtoNum], -1,
    m_nProtoNum + 2);
  
  m_nProtoNum++;
  
  updateStatus();

#ifdef USE_KDE
    // let KDE IM interface know about the new protocol
    kdeIMInterface->addProtocol(QString(pName), nPPID);
#endif
}

//-----slot_eventTag------------------------------------------------------------
void CMainWindow::slot_eventTag(const char *_szId, unsigned long _nPPID,
                                unsigned long _nEventTag)
{
  if (!_szId || !_nPPID || !_nEventTag)
    return;

#if QT_VERSION < 300
  QListIterator<UserSendCommon> it(licqUserSend);
#else
  QPtrListIterator<UserSendCommon> it(licqUserSend);
#endif
  for(; it.current() != NULL; ++it)
  {
    if (strcmp((*it)->Id(), _szId) == 0 && (*it)->PPID() == _nPPID)
    {
      (*it)->AddEventTag(_nEventTag);
      break;
    }
  }
}

//-----slot_doneOwnerFcn--------------------------------------------------------
void CMainWindow::slot_doneOwnerFcn(ICQEvent *e)
{
  updateStatus();
  switch (e->SNAC())
  {
    case MAKESNAC(ICQ_SNACxFAM_SERVICE, ICQ_SNACxSRV_SETxSTATUS):
      if (e->Result() != EVENT_SUCCESS)
        WarnUser(this, tr("Logon failed.\nSee network window for details."));
      break;
//  Deprecated
//    case MAKESNAC(ICQ_SNACxFAM_NEWUIN, ICQ_SNACxREGISTER_USER):
//      if (ownerManagerDlg)
//        ownerManagerDlg->slot_doneRegisterUser(e);
//      break;

/*
    case ICQ_CMDxSND_AUTHORIZE:
       if (e->Result() != EVENT_ACKED)
         WarnUser(this, tr("Error sending authorization."));
       else
         InformUser(this, tr("Authorization granted."));
       break;
*/
    default:
       break;
  }
}

bool CMainWindow::RemoveUserFromList(const char *szId, unsigned long nPPID, QWidget *p)
{
  ICQUser *u = gUserManager.FetchUser(szId, nPPID, LOCK_R);
  if (u == NULL) return true;
  QString warning(tr("Are you sure you want to remove\n%1 (%2)\nfrom your contact list?")
                     .arg(QString::fromUtf8(u->GetAlias()))
                     .arg(u->IdString()) );
  gUserManager.DropUser(u);
  if (QueryUser(p, warning, tr("Ok"), tr("Cancel")))
  {
    licqDaemon->RemoveUserFromList(szId, nPPID);
    return true;
  }
  return false;
}

bool CMainWindow::RemoveUserFromList(unsigned long nUin, QWidget *p)
{
  ICQUser *u = gUserManager.FetchUser(nUin, LOCK_R);
  if (u == NULL) return true;
  QString warning(tr("Are you sure you want to remove\n%1 (%2)\nfrom your contact list?")
                     .arg(QString::fromUtf8(u->GetAlias()))
                     .arg(nUin) );
  gUserManager.DropUser(u);
  if (QueryUser(p, warning, tr("Ok"), tr("Cancel")))
  {
    licqDaemon->RemoveUserFromList(nUin);
    return true;
  }
  return false;
}

void CMainWindow::FillUserGroup()
{
  ICQUser *u = gUserManager.FetchUser(m_szUserMenuId, m_nUserMenuPPID, LOCK_R);
  if(u == NULL) return;

  mnuGroup->setItemChecked(1000+GROUP_ONLINE_NOTIFY, u->OnlineNotify());
  mnuGroup->setItemChecked(1000+GROUP_INVISIBLE_LIST, u->InvisibleList());
  mnuGroup->setItemChecked(1000+GROUP_VISIBLE_LIST, u->VisibleList());
  mnuGroup->setItemChecked(1000+GROUP_IGNORE_LIST, u->IgnoreList());
  mnuGroup->setItemChecked(1000+GROUP_NEW_USERS, u->NewUser());

  GroupList *g = gUserManager.LockGroupList(LOCK_R);
  for (unsigned short i = 0; i < g->size(); i++)
    mnuGroup->setItemChecked(i+1, u->GetInGroup(GROUPS_USER, i+1));
  gUserManager.UnlockGroupList();
  gUserManager.DropUser(u);
}


void CMainWindow::UserGroupToggled(int id)
{
  if(id >= 0 && id < 1000)
  {
    // User groups
    if(mnuGroup->isItemChecked(id))
      RemoveUserFromGroup(GROUPS_USER, id, m_szUserMenuId, m_nUserMenuPPID, this);
    else {
      gUserManager.AddUserToGroup(m_szUserMenuId, m_nUserMenuPPID, id);
      updateUserWin();
    }
  }
  else if(id >= 1000)
  {
    switch(id-1000) {
    case GROUP_NEW_USERS:
    {
      ICQUser *u = gUserManager.FetchUser(m_szUserMenuId, m_nUserMenuPPID, LOCK_W);
      if (!u) return;
      u->SetNewUser(!u->NewUser());
      gUserManager.DropUser(u);
      updateUserWin();
      break;
    }
    case GROUP_ONLINE_NOTIFY:
    {
      ICQUser *u = gUserManager.FetchUser(m_szUserMenuId, m_nUserMenuPPID, LOCK_W);
      if (!u) return;
      u->SetOnlineNotify(!u->OnlineNotify());
      gUserManager.DropUser(u);
      if (m_bFontStyles) updateUserWin();
      break;
    }
    case GROUP_VISIBLE_LIST:
    {
      licqDaemon->ProtoToggleVisibleList(m_szUserMenuId, m_nUserMenuPPID);
      if (m_bFontStyles)
        updateUserWin();
      break;
    }
    case GROUP_INVISIBLE_LIST:
    {
      licqDaemon->ProtoToggleInvisibleList(m_szUserMenuId, m_nUserMenuPPID);
      if (m_bFontStyles)
        updateUserWin();
      break;
    }
    case GROUP_IGNORE_LIST:
    {
      ICQUser *u = gUserManager.FetchUser(m_szUserMenuId, m_nUserMenuPPID, LOCK_W);
      if (!u) return;
      if(!u->IgnoreList() && !QueryUser(this,
          tr("Do you really want to add\n%1 (%2)\nto your ignore list?")
          .arg(QString::fromUtf8(u->GetAlias())).arg(m_szUserMenuId), tr("&Yes"), tr("&No")))
      {
        gUserManager.DropUser(u);
        break;
      }
      u->SetIgnoreList(!u->IgnoreList());
      gUserManager.DropUser(u);
      licqDaemon->icqToggleIgnoreList(m_szUserMenuId, m_nUserMenuPPID); // network only
      updateUserWin();
      break;
    }
    }
  }
}

bool CMainWindow::RemoveUserFromGroup(GroupType gtype, unsigned long group,
  const char *szId, unsigned long nPPID, QWidget *p)
{
  if (gtype == GROUPS_USER)
  {
    if (group == 0)
      return RemoveUserFromList(szId, nPPID, p);
    else
    {
      ICQUser *u = gUserManager.FetchUser(szId, nPPID, LOCK_R);
      if (u == NULL) return true;
      GroupList *g = gUserManager.LockGroupList(LOCK_R);
      QString warning(tr("Are you sure you want to remove\n%1 (%2)\nfrom the '%3' group?")
                         .arg(QString::fromUtf8(u->GetAlias()))
                         .arg(u->IdString()).arg(QString::fromLocal8Bit( (*g)[group - 1] )) );
      gUserManager.UnlockGroupList();
      gUserManager.DropUser(u);
      if (QueryUser(p, warning, tr("Ok"), tr("Cancel")))
      {
         gUserManager.RemoveUserFromGroup(szId, nPPID, group);
         updateUserWin();
         return true;
      }
    }
  }
  else if (gtype == GROUPS_SYSTEM)
  {
    if (group == 0) return true;
    ICQUser *u = gUserManager.FetchUser(szId, nPPID, LOCK_W);
    if (u == NULL) return true;
    u->RemoveFromGroup(GROUPS_SYSTEM, group);
    gUserManager.DropUser(u);
    updateUserWin();
    return true;
  }

  return false;
}


bool CMainWindow::RemoveUserFromGroup(GroupType gtype, unsigned long group, unsigned long nUin, QWidget *p)
{
  if (gtype == GROUPS_USER)
  {
    if (group == 0)
      return RemoveUserFromList(nUin, p);
    else
    {
      ICQUser *u = gUserManager.FetchUser(nUin, LOCK_R);
      if (u == NULL) return true;
      unsigned long nUin = u->Uin();
      GroupList *g = gUserManager.LockGroupList(LOCK_R);
      QString warning(tr("Are you sure you want to remove\n%1 (%2)\nfrom the '%3' group?")
                         .arg(QString::fromUtf8(u->GetAlias()))
                         .arg(nUin).arg(QString::fromLocal8Bit( (*g)[group - 1] )) );
      gUserManager.UnlockGroupList();
      gUserManager.DropUser(u);
      if (QueryUser(p, warning, tr("Ok"), tr("Cancel")))
      {
         gUserManager.RemoveUserFromGroup(nUin, group);
         updateUserWin();
         return true;
      }
    }
  }
  else if (gtype == GROUPS_SYSTEM)
  {
    if (group == 0) return true;
    ICQUser *u = gUserManager.FetchUser(nUin, LOCK_W);
    if (u == NULL) return true;
    u->RemoveFromGroup(GROUPS_SYSTEM, group);
    gUserManager.DropUser(u);
    updateUserWin();
    return true;
  }

  return false;
}

void CMainWindow::FillServerGroup()
{
  ICQUser *u = gUserManager.FetchUser(m_szUserMenuId, m_nUserMenuPPID, LOCK_R);
  if (u == NULL) return;

  GroupList *g = gUserManager.LockGroupList(LOCK_R);
  for (unsigned short i = 0; i < g->size(); i++)
    mnuServerGroup->setItemChecked(i+1, false);

  for (unsigned short i = 0; i < g->size(); i++)
  {
    if (u->GetSID() && (u->GetGSID() == gUserManager.GetIDFromGroup((*g)[i])))
    {
      mnuServerGroup->setItemChecked(i+1, true);
      break;
    }
  }
  gUserManager.UnlockGroupList();
  gUserManager.DropUser(u);
}

void CMainWindow::ServerGroupChanged(int n)
{
  if (mnuServerGroup->isItemChecked(n)) return;

  ICQUser *u = gUserManager.FetchUser(m_szUserMenuId, m_nUserMenuPPID, LOCK_R);
  if (u == NULL) return;

  GroupList *g = gUserManager.LockGroupList(LOCK_R);
  for (unsigned int i = 0; i < g->size(); i++)
    mnuServerGroup->setItemChecked(i+1, (int)(i+1) == n);

  gUserManager.UnlockGroupList();
  gUserManager.DropUser(u);

  gUserManager.AddUserToGroup(m_szUserMenuId, m_nUserMenuPPID, n);
  updateUserWin();
}

//-----CMainWindow::saveAllUsers-----------------------------------------------
void CMainWindow::saveAllUsers()
{
   gUserManager.SaveAllUsers();
}

void CMainWindow::slot_updateContactList()
{
  licqDaemon->icqUpdateContactList();
}

void CMainWindow::slot_updateAllUsers()
{
  licqDaemon->UpdateAllUsers();
}


void CMainWindow::slot_updateAllUsersInGroup()
{
  licqDaemon->UpdateAllUsersInGroup(m_nGroupType, m_nCurrentGroup);
}



//-----CMainWindow::saveOptions-----------------------------------------------
void CMainWindow::saveOptions()
{
  // Tell the daemon to save it's options
  licqDaemon->SaveConf();

  // Save all our options
  char filename[MAX_FILENAME_LEN];
  snprintf(filename, MAX_FILENAME_LEN, "%s/licq_qt-gui.conf", BASE_DIR);
  filename[MAX_FILENAME_LEN - 1] = '\0';
  CIniFile licqConf(INI_FxERROR | INI_FxALLOWxCREATE);
  if (!licqConf.LoadFile(filename)) return;

  licqConf.SetSection("startup");
  licqConf.WriteNum("Logon", m_nAutoLogon);
  licqConf.WriteNum("AutoAway", autoAwayTime);
  licqConf.WriteNum("AutoNA", autoNATime);
  licqConf.WriteNum("AutoOffline", autoOfflineTime);
  licqConf.WriteNum("AutoAwayMess", autoAwayMess);
  licqConf.WriteNum("AutoNAMess", autoNAMess);

  licqConf.SetSection("functions");
  licqConf.WriteBool("AutoClose", m_bAutoClose);
  licqConf.WriteBool("AutoPopup", m_bAutoPopup);
  licqConf.WriteStr("MsgPopupKey", m_MsgAutopopupKey.isEmpty() ? "none" : m_MsgAutopopupKey.latin1());

  licqConf.SetSection("appearance");
  licqConf.WriteStr("Skin", skin->szSkinName);
  licqConf.WriteStr("Icons", m_szIconSet);
  licqConf.WriteStr("ExtendedIcons", m_szExtendedIconSet);
  licqConf.WriteStr("Emoticons", emoticons->Theme() ? emoticons->Theme() : "None");

#if QT_VERSION >= 300
  licqConf.WriteStr("Font", qApp->font() == defaultFont ?
                    "default" : qApp->font().toString().latin1());
  licqConf.WriteStr("EditFont",
                    (MLEditWrap::editFont == NULL ||
                     *MLEditWrap::editFont == defaultFont) ?
                     "default" : MLEditWrap::editFont->toString().latin1());
#else
  licqConf.WriteStr("Font", qApp->font() == defaultFont ?
                    "default" : qApp->font().rawName().latin1());
  licqConf.WriteStr("EditFont",
                    (MLEditWrap::editFont == NULL ||
                     *MLEditWrap::editFont == defaultFont) ?
                     "default" : MLEditWrap::editFont->rawName().latin1());
#endif
  licqConf.WriteBool("GridLines", m_bGridLines);
  licqConf.WriteBool("FontStyles", m_bFontStyles);
  licqConf.WriteNum("Flash", (unsigned short)m_nFlash);
  licqConf.WriteBool("ShowHeader", m_bShowHeader);
  licqConf.WriteBool("ShowDividers", m_bShowDividers);
  licqConf.WriteNum("SortByStatus", m_nSortByStatus);
  licqConf.WriteNum("SortColumn", m_nSortColumn);
  licqConf.WriteBool("SortColumnAscending", m_bSortColumnAscending);
  licqConf.WriteBool("ShowGroupIfNoMsg", m_bShowGroupIfNoMsg);
  licqConf.WriteBool("UseThreadView", m_bThreadView);
  licqConf.WriteNum("TVGroupStates", m_nGroupStates);
  licqConf.WriteBool("BoldOnMsg", m_bBoldOnMsg);
  licqConf.WriteBool("ManualNewUser", m_bManualNewUser);
  licqConf.WriteBool("Transparent", skin->frame.transparent);
  licqConf.WriteBool("ScrollBar", m_bScrollBar);
  licqConf.WriteNum("FrameStyle", skin->frame.frameStyle);
  licqConf.WriteBool("ShowOfflineUsers", m_bShowOffline);
  licqConf.WriteBool("AlwaysShowONU", m_bAlwaysShowONU);
  licqConf.WriteBool("AutoRaise", m_bAutoRaise);
  licqConf.WriteBool("Hidden", m_bHidden);
  licqConf.WriteBool("ShowExtIcons", m_bShowExtendedIcons);
  licqConf.WriteBool("SystemBackground", m_bSystemBackground);
  licqConf.WriteBool("SendFromClipboard", m_bSendFromClipboard);
  licqConf.WriteBool("MsgChatView", m_bMsgChatView);
  licqConf.WriteBool("TabbedChatting", m_bTabbedChatting);
  licqConf.WriteBool("ShowHistory", m_bShowHistory);
  licqConf.WriteBool("AutoPosReplyWin", m_bAutoPosReplyWin);
  licqConf.WriteBool("AutoSendThroughServer", m_bAutoSendThroughServer);
  licqConf.WriteBool("EnableMainwinMouseMovement", m_bEnableMainwinMouseMovement);
  licqConf.WriteBool("FlashTaskbar", m_bFlashTaskbar);
  
  licqConf.WriteNum("ChatMessageStyle", m_nMsgStyle);
  licqConf.WriteBool("ChatAppendLinebreak", m_bAppendLineBreak);
  licqConf.WriteStr("ReceiveMessageColor", m_colorRcv.name());
  licqConf.WriteStr("ReceiveHistoryColor", m_colorRcvHistory.name());
  licqConf.WriteStr("SentMessageColor", m_colorSnt.name());
  licqConf.WriteStr("SentHistoryColor", m_colorSntHistory.name());
  licqConf.WriteStr("TabFontColor", m_colorTab.name());
  licqConf.WriteStr("TabOnTypingColor", m_colorTabTyping.name());
  licqConf.WriteStr("ChatBackground", m_colorChatBkg.name());
  
  licqConf.WriteBool("showPopEmail",m_bPopEmail);
  licqConf.WriteBool("showPopPhone",m_bPopPhone);
  licqConf.WriteBool("showPopFax",m_bPopFax);
  licqConf.WriteBool("showPopCellular",m_bPopCellular);
  licqConf.WriteBool("showPopIP",m_bPopIP);
  licqConf.WriteBool("showPopLastOnelin",m_bPopLastOnline);
  licqConf.WriteBool("showPopOnlineSince", m_bPopOnlineSince);
  licqConf.WriteBool("showPopIdleTime", m_bPopIdleTime);
  licqConf.WriteBool("showPopID", m_bPopID);

  licqConf.WriteNum("UseDock", (unsigned short)m_nDockMode);
  switch(m_nDockMode)
  {
    case DockDefault:
      licqConf.WriteBool("Dock64x48", ((IconManager_Default *)licqIcon)->FortyEight());
      break;
    case DockThemed:
      licqConf.WriteStr("DockTheme", ((IconManager_Themed *)licqIcon)->Theme().latin1());
      break;
    case DockSmall:
    case DockNone:
      break;
  }

  // save the column info
  licqConf.WriteNum("NumColumns", (unsigned short)colInfo.size());
  char key[32];
  for (unsigned short i = 1; i <= colInfo.size(); i++)
  {
     sprintf(key, "Column%d.Title", i);
     licqConf.WriteStr(key, colInfo[i - 1]->m_sTitle.local8Bit());
     sprintf(key, "Column%d.Format", i);
     licqConf.WriteStr(key, colInfo[i - 1]->m_szFormat);
     sprintf(key, "Column%d.Width", i);
     licqConf.WriteNum(key, colInfo[i - 1]->m_nWidth);
     sprintf(key, "Column%d.Align", i);
     licqConf.WriteNum(key, colInfo[i - 1]->m_nAlign);
  }

  licqConf.SetSection("floaties");
  licqConf.WriteNum("Num", (unsigned short)CUserView::floaties->size());
  for (unsigned short i = 0; i < CUserView::floaties->size(); i++)
  {
    CUserView* iter = CUserView::floaties->at(i);
    sprintf(key, "Floaty%d.Uin", i);
    licqConf.WriteNum(key, static_cast<CUserViewItem*>(iter->firstChild())->ItemUin());
    sprintf(key, "Floaty%d.X", i);
    licqConf.WriteNum(key, (unsigned short)(iter->x() > 0 ? iter->x() : 0));
    sprintf(key, "Floaty%d.Y", i);
    licqConf.WriteNum(key, (unsigned short)(iter->y() > 0 ? iter->y() : 0));
    sprintf(key, "Floaty%d.W", i);
    licqConf.WriteNum(key, (unsigned short)iter->width());
  }

  // save settings relating to localization
  licqConf.SetSection("locale");
  licqConf.WriteStr("DefaultEncoding", m_DefaultEncoding.latin1());
  licqConf.WriteBool("ShowAllEncodings", m_bShowAllEncodings);

  licqConf.FlushFile();
}


//-----CMainWindow::aboutBox--------------------------------------------------
void CMainWindow::aboutBox()
{
  ICQOwner *o = gUserManager.FetchOwner(LOCK_R);
  
  // We might have no owner
  QString m_Alias = (o == 0) ? QString(tr("(Error! No owner set)")) : QString::fromLocal8Bit(o->GetAlias());
  unsigned long m_Uin = (o == 0) ? 0 : o->Uin();
  
  QString about(tr("Licq version %1%8.\n"
                   "Qt GUI plugin version %2.\n"
                   "Compiled on: %7\n"
                   "%6\n"
                   "Maintainer: Jon Keating\n"
                   "Contributions by Dirk A. Mueller\n"
                   "Original Author: Graham Roff\n\n"
                   "http://www.licq.org\n"
                   "#licq on irc.freenode.net\n\n"
                   "%3 (%4)\n"
                   "%5 contacts.").arg(licqDaemon->Version())
                   .arg(VERSION).arg(m_Alias)
                   .arg(m_Uin).arg(gUserManager.NumUsers())
#ifdef USE_KDE
                   .arg(tr("(with KDE support)\n"))
#else
                   .arg("\n")
#endif
                   .arg(__DATE__).arg(CICQDaemon::CryptoEnabled() ? "/SSL" : ""));
  gUserManager.DropOwner();
  InformUser(this, about);
}


//-----CMainWindow::changeDebug-----------------------------------------------
void CMainWindow::changeDebug(int _nId)
{
  int nLevel = mnuDebug->indexOf(_nId);
  if (nLevel == MNUxITEM_DEBUGxALL)
  {
    gLog.ModifyService(S_STDERR, L_ALL);
    for (int i = 0; i < 5; i++)
      mnuDebug->setItemChecked(mnuDebug->idAt(i), true);
    return;
  }

  if (nLevel == MNUxITEM_DEBUGxNONE)
  {
    gLog.ModifyService(S_STDERR, L_NONE);
    for (int i = 0; i < 5; i++)
      mnuDebug->setItemChecked(mnuDebug->idAt(i), false);
    return;
  }

  // First see if we are setting on or off the value
  if (mnuDebug->isItemChecked(_nId))
  {
    gLog.RemoveLogTypeFromService(S_STDERR, 1 << nLevel);
    mnuDebug->setItemChecked(_nId, false);
  }
  else
  {
    gLog.AddLogTypeToService(S_STDERR, 1 << nLevel);
    mnuDebug->setItemChecked(_nId, true);
  }
}


//-----CMainWindow::slot_utility----------------------------------------------
void CMainWindow::slot_utility(int _nId)
{
  CUtility *u = gUtilityManager.Utility(mnuUtilities->indexOf(_nId));
  
  if ( u && m_szUserMenuId )
  	(void) new CUtilityDlg(u, m_szUserMenuId, m_nUserMenuPPID, licqDaemon);
}


//-----CMainWindow::slot_miscmodes--------------------------------------------
void CMainWindow::slot_miscmodes(int _nId)
{
  int nAwayModes = mnuMiscModes->indexOf(_nId);
  //ICQUser *u = gUserManager.FetchUser(userView->SelectedItemUin(), LOCK_W);
  ICQUser *u = gUserManager.FetchUser(m_szUserMenuId, m_nUserMenuPPID, LOCK_W);
  if (u == NULL) return;

  switch(nAwayModes)
  {
  case 0:
    u->SetAcceptInAway(!u->AcceptInAway());
    break;
  case 1:
    u->SetAcceptInNA(!u->AcceptInNA());
    break;
  case 2:
    u->SetAcceptInOccupied(!u->AcceptInOccupied());
    break;
  case 3:
    u->SetAcceptInDND(!u->AcceptInDND());
    break;
  case 4:
    u->SetAutoFileAccept(!u->AutoFileAccept() );
    break;
  case 5:
    u->SetAutoChatAccept( !u->AutoChatAccept() );
    break;
  case 6:
    u->SetAutoSecure( !u->AutoSecure() );
    break;
  case 7:
#ifdef HAVE_LIBGPGME
    if ( strcmp(u->GPGKey(),"")!=0 )
      u->SetUseGPG( !u->UseGPG() );
    else
    {
      gUserManager.DropUser(u);
      u = NULL;
      new GPGKeySelect( m_szUserMenuId, m_nUserMenuPPID );
    }
    break;
#endif
  case 8:
    u->SetSendRealIp( !u->SendRealIp() );
    break;

  case 10:
    if (u->StatusToUser() == ICQ_STATUS_ONLINE)
      u->SetStatusToUser(ICQ_STATUS_OFFLINE);
    else
      u->SetStatusToUser(ICQ_STATUS_ONLINE);
    break;
  case 11:
    if (u->StatusToUser() == ICQ_STATUS_AWAY)
      u->SetStatusToUser(ICQ_STATUS_OFFLINE);
    else
      u->SetStatusToUser(ICQ_STATUS_AWAY);
    break;
  case 12:
    if (u->StatusToUser() == ICQ_STATUS_NA)
      u->SetStatusToUser(ICQ_STATUS_OFFLINE);
    else
      u->SetStatusToUser(ICQ_STATUS_NA);
    break;
  case 13:
    if (u->StatusToUser() == ICQ_STATUS_OCCUPIED)
      u->SetStatusToUser(ICQ_STATUS_OFFLINE);
    else
      u->SetStatusToUser(ICQ_STATUS_OCCUPIED);
    break;
  case 14:
    if (u->StatusToUser() == ICQ_STATUS_DND)
      u->SetStatusToUser(ICQ_STATUS_OFFLINE);
    else
      u->SetStatusToUser(ICQ_STATUS_DND);
    break;
  }
  if ( u ) gUserManager.DropUser(u);

  // update icon
  if ( nAwayModes==7 )
  {
    CICQSignal s(SIGNAL_UPDATExUSER, USER_GENERAL, m_szUserMenuId, m_nUserMenuPPID);
    slot_updatedUser(&s);
  }
}


//-----ToggleShowOffline------------------------------------------------------
void CMainWindow::ToggleShowOffline()
{
  m_bShowOffline = !m_bShowOffline;
  mnuSystem->setItemChecked(mnuSystem->idAt(MNUxITEM_SHOWxOFFLINE), m_bShowOffline);
  updateUserWin();
}


//-----ToggleThreadedView------------------------------------------------------
void CMainWindow::ToggleThreadView()
{
  m_bThreadView = !m_bThreadView;
  mnuSystem->setItemChecked(mnuSystem->idAt(MNUxITEM_THREADxVIEW), m_bThreadView);
  updateUserWin();
}


//-----CMainWindow::ToggleMiniMode--------------------------------------------------
void CMainWindow::ToggleMiniMode()
{

   if (m_bInMiniMode)
   {
      userView->show();
      setMaximumHeight(4096);
      resize(width(), m_nRealHeight);
      setMinimumHeight(100);
   }
   else
   {
      userView->QWidget::hide();
      m_nRealHeight = height();
      unsigned short newH = skin->frame.border.top + skin->frame.border.bottom;
      setMinimumHeight(newH);
      resize(width(), newH);
      setMaximumHeight(newH);
   }
   m_bInMiniMode = !m_bInMiniMode;
   mnuSystem->setItemChecked(mnuSystem->idAt(MNUxITEM_MINIxMODE), m_bInMiniMode);
}


//-----CMainWindow::autoAway--------------------------------------------------
void CMainWindow::autoAway()
{
#ifdef USE_SCRNSAVER
  static XScreenSaverInfo *mit_info = NULL;
  static bool bAutoAway = false;
  static bool bAutoNA = false;
  static bool bAutoOffline = false;

  // Fetch current status
  unsigned short status = ICQ_STATUS_OFFLINE;
  ICQOwner *o = gUserManager.FetchOwner(LOCK_R);
  if (o != NULL)
  {
    status = o->Status();
    gUserManager.DropOwner();
  }

  if (mit_info == NULL) {
    int event_base, error_base;
    if(XScreenSaverQueryExtension(x11Display(), &event_base, &error_base)) {
      mit_info = XScreenSaverAllocInfo ();
    }
    else {
      gLog.Warn("%sNo XScreenSaver extension found on current XServer, disabling auto-away.\n",
                L_WARNxSTR);
      autoAwayTimer.stop();
      return;
    }
  }

  if (!XScreenSaverQueryInfo(x11Display(), qt_xrootwin(), mit_info)) {
    gLog.Warn("%sXScreenSaverQueryInfo failed, disabling auto-away.\n",
              L_WARNxSTR);
    autoAwayTimer.stop();
    return;
  }
  Time idleTime = mit_info->idle;

  // Check no one changed the status behind our back
  if ( (bAutoOffline && status != ICQ_STATUS_OFFLINE) ||
       (bAutoNA && status != ICQ_STATUS_NA && !bAutoOffline) ||
       (bAutoAway && status != ICQ_STATUS_AWAY && !bAutoNA && !bAutoOffline) )
  {
    bAutoOffline = false;
    bAutoNA = false;
    bAutoAway = false;
    return;
  }

//  gLog.Info("offl %d, n/a %d, away %d idlt %d\n",
//            bAutoOffline, bAutoNA, bAutoAway, idleTime);

  if ( (autoOfflineTime > 0) &&
       (unsigned long)idleTime > (unsigned long)(autoOfflineTime * 60000))
  {
    if (status == ICQ_STATUS_ONLINE || status == ICQ_STATUS_AWAY || status == ICQ_STATUS_NA)
    {
      changeStatus(ICQ_STATUS_OFFLINE);
      bAutoOffline = true;
      bAutoAway = (status == ICQ_STATUS_ONLINE || bAutoAway);
      bAutoNA = ((status == ICQ_STATUS_AWAY && bAutoAway) || bAutoNA);
    }
  }
  else if ( (autoNATime > 0) &&
       (unsigned long)idleTime > (unsigned long)(autoNATime * 60000))
  {
    if (status == ICQ_STATUS_ONLINE || status == ICQ_STATUS_AWAY)
    {
      if (autoNAMess) {
       SARList &sar = gSARManager.Fetch(SAR_NA);
       ICQOwner *o = gUserManager.FetchOwner(LOCK_W);
       if (o != 0)
       {
        o->SetAutoResponse(QString(sar[autoNAMess-1]->AutoResponse()).local8Bit());
        gUserManager.DropOwner();
       }
       gSARManager.Drop();
      }

      changeStatus(ICQ_STATUS_NA);
      bAutoNA = true;
      bAutoAway = (status == ICQ_STATUS_ONLINE || bAutoAway);
    }
  }
  else if ( (autoAwayTime > 0) &&
            (unsigned long)idleTime > (unsigned long)(autoAwayTime * 60000))
  {
    if (status == ICQ_STATUS_ONLINE)
    {
      if (autoAwayMess) {
       SARList &sar = gSARManager.Fetch(SAR_AWAY);
       ICQOwner *o = gUserManager.FetchOwner(LOCK_W);
       if (o != 0)
       {
        o->SetAutoResponse(QString(sar[autoAwayMess-1]->AutoResponse()).local8Bit());
        gUserManager.DropOwner();
       }
       gSARManager.Drop();
      }

      changeStatus(ICQ_STATUS_AWAY);
      bAutoAway = true;
    }
  }
  else
  {
    if (bAutoOffline)
    {
      if (bAutoNA && bAutoAway)
      {
        changeStatus(ICQ_STATUS_ONLINE);
        bAutoOffline = bAutoNA = bAutoAway = false;
      }
      else if (bAutoNA)
      {
        changeStatus(ICQ_STATUS_AWAY);
        bAutoNA = bAutoOffline = false;
      }
      else
      {
        changeStatus(ICQ_STATUS_NA);
        bAutoOffline = false;
      }
    }
    else if (bAutoNA)
    {
      if (bAutoAway)
      {
        changeStatus(ICQ_STATUS_ONLINE);
        bAutoNA = bAutoAway = false;
      }
      else
      {
        changeStatus(ICQ_STATUS_AWAY);
        bAutoNA = false;
      }
    }
    else if (bAutoAway)
    {
      changeStatus(ICQ_STATUS_ONLINE);
      bAutoAway = false;
    }
  }

#endif // USE_SCRNSAVER
}


void CMainWindow::popupSystemMenu()
{
   mnuSystem->popup(mapToGlobal(QPoint(btnSystem->x(), btnSystem->y())));
}


//-----CMainWindow::loadExtendedIcons-----------------------------------------
void CMainWindow::ApplyExtendedIcons(const char *_sIconSet, bool _bInitial)
{
   char sFilename[MAX_FILENAME_LEN],
        sFilepath[MAX_FILENAME_LEN],
        sIconPath[MAX_FILENAME_LEN];

   if (m_szExtendedIconSet != NULL) free (m_szExtendedIconSet);
   m_szExtendedIconSet = strdup(_sIconSet);
   if (_sIconSet[0] == '/')
   {
     strcpy(sIconPath, _sIconSet);
     if (sIconPath[strlen(sIconPath) - 1] != '/')
       strcat(sIconPath, "/");
   }
   else
   {
     snprintf(sIconPath, MAX_FILENAME_LEN, "%s/%sextended.icons.%s/", BASE_DIR, QTGUI_DIR, _sIconSet);
     sIconPath[MAX_FILENAME_LEN - 1] = '\0';
   }
   snprintf(sFilename, MAX_FILENAME_LEN, "%s%s.icons", sIconPath, _sIconSet);
   sFilename[MAX_FILENAME_LEN - 1] = '\0';
   CIniFile fIconsConf;
   if (!fIconsConf.LoadFile(sFilename))
   {
     snprintf(sIconPath, MAX_FILENAME_LEN, "%s%sextended.icons.%s/", SHARE_DIR, QTGUI_DIR, _sIconSet);
     sIconPath[MAX_FILENAME_LEN - 1] = '\0';
     snprintf(sFilename, MAX_FILENAME_LEN, "%s%s.icons", sIconPath, _sIconSet);
     sFilename[MAX_FILENAME_LEN - 1] = '\0';
     if (!fIconsConf.LoadFile(sFilename))
     {
       if (_bInitial)
         gLog.Warn("%sUnable to open extended icons file %s.\n", L_WARNxSTR, sFilename);
       else
         WarnUser(this, tr("Unable to open extended icons file\n%1.").arg(sFilename));
       return;
     }
   }

   fIconsConf.SetSection("icons");
   fIconsConf.ReadStr("Collapsed", sFilename, "");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmCollapsed.load(sFilepath);
   if (pmCollapsed.isNull())
     pmCollapsed = QPixmap(itemCollapsed_xpm);
     
   fIconsConf.ReadStr("Expanded", sFilename, "");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmExpanded.load(sFilepath);
   if (pmExpanded.isNull())
     pmExpanded = QPixmap(itemExpanded_xpm);
    
   fIconsConf.ReadStr("Phone", sFilename, "");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmPhone.load(sFilepath);
   if (pmPhone.isNull())
     pmPhone = QPixmap(pixPhone_xpm);
   
   fIconsConf.ReadStr("Cellular", sFilename, "");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmCellular.load(sFilepath);
   if (pmCellular.isNull())
     pmCellular = QPixmap(pixCellular_xpm);
   
   fIconsConf.ReadStr("Birthday", sFilename, "");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmBirthday.load(sFilepath);
   if (pmBirthday.isNull())
     pmBirthday = QPixmap(pixBirthday_xpm);

   
   fIconsConf.ReadStr("CustomAR", sFilename, "");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmCustomAR.load(sFilepath);
   if (pmCustomAR.isNull())
     pmCustomAR = QPixmap(pixCustomAR_xpm);

   fIconsConf.ReadStr("Invisible", sFilename, "");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmInvisible.load(sFilepath);
   if (pmInvisible.isNull())
     pmInvisible = QPixmap(pixInvisible_xpm);
 
   fIconsConf.ReadStr("Typing", sFilename, "");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmTyping.load(sFilepath);
   if (pmTyping.isNull())
     pmTyping = QPixmap(pixTyping_xpm);
     
   fIconsConf.ReadStr("ICQphoneActive", sFilename, "");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmICQphoneActive.load(sFilepath);
   if (pmICQphoneActive.isNull())
     pmICQphoneActive = QPixmap(pixICQphoneActive_xpm);

   fIconsConf.ReadStr("ICQphoneBusy", sFilename, "");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmICQphoneBusy.load(sFilepath);
   if (pmICQphoneBusy.isNull())
     pmICQphoneBusy = QPixmap(pixICQphoneBusy_xpm);

   fIconsConf.ReadStr("PhoneFollowMeActive", sFilename, "");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmPhoneFollowMeActive.load(sFilepath);
   if (pmPhoneFollowMeActive.isNull())
     pmPhoneFollowMeActive = QPixmap(pixPhoneFollowMeActive_xpm);

   fIconsConf.ReadStr("PhoneFollowMeBusy", sFilename, "");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmPhoneFollowMeBusy.load(sFilepath);
   if (pmPhoneFollowMeBusy.isNull())
     pmPhoneFollowMeBusy = QPixmap(pixPhoneFollowMeBusy_xpm);

   fIconsConf.ReadStr("SharedFiles", sFilename, "");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmSharedFiles.load(sFilepath);
   if (pmSharedFiles.isNull())
     pmSharedFiles = QPixmap(pixSharedFiles_xpm);

#ifdef HAVE_LIBGPGME
   fIconsConf.ReadStr("GPGKeyEnabled", sFilename, "");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmGPGKeyEnabled.load(sFilepath);
   if (pmGPGKeyEnabled.isNull())
     pmGPGKeyEnabled = QPixmap(pixKeyEnabled_xpm);
   
   fIconsConf.ReadStr("GPGKeyDisabled", sFilename, "");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmGPGKeyDisabled.load(sFilepath);
   if (pmGPGKeyDisabled.isNull())
     pmGPGKeyDisabled = QPixmap(pixKeyDisabled_xpm);
#endif

   if (!_bInitial)
   {
     mnuUser->changeItem(pmCustomAR, tr("Custom Auto Response..."), mnuUserCustomAutoResponse);
     updateUserWin();
   }
}

//-----CMainWindow::loadIcons-------------------------------------------------
void CMainWindow::ApplyIcons(const char *_sIconSet, bool _bInitial)
// load the pixmaps
{
   char sFilename[MAX_FILENAME_LEN],
        sFilepath[MAX_FILENAME_LEN],
        sIconPath[MAX_FILENAME_LEN];

   if (m_szIconSet != NULL) free (m_szIconSet);
   m_szIconSet = strdup(_sIconSet);
   if (_sIconSet[0] == '/')
   {
     strcpy(sIconPath, _sIconSet);
     if (sIconPath[strlen(sIconPath) - 1] != '/')
       strcat(sIconPath, "/");
   }
   else
   {
     snprintf(sIconPath, MAX_FILENAME_LEN, "%s/%sicons.%s/", BASE_DIR, QTGUI_DIR, _sIconSet);
     sIconPath[MAX_FILENAME_LEN - 1] = '\0';
   }
   snprintf(sFilename, MAX_FILENAME_LEN, "%s%s.icons", sIconPath, _sIconSet);
   sFilename[MAX_FILENAME_LEN - 1] = '\0';
   CIniFile fIconsConf;
   if (!fIconsConf.LoadFile(sFilename))
   {
     snprintf(sIconPath, MAX_FILENAME_LEN, "%s%sicons.%s/", SHARE_DIR, QTGUI_DIR, _sIconSet);
     sIconPath[MAX_FILENAME_LEN - 1] = '\0';
     snprintf(sFilename, MAX_FILENAME_LEN, "%s%s.icons", sIconPath, _sIconSet);
     sFilename[MAX_FILENAME_LEN - 1] = '\0';
     if (!fIconsConf.LoadFile(sFilename))
     {
       if (_bInitial)
         gLog.Warn("%sUnable to open icons file %s.\n", L_WARNxSTR, sFilename);
        else
         WarnUser(this, tr("Unable to open icons file\n%1.").arg(sFilename));
       return;
     }
   }

   fIconsConf.SetSection("icons");
   fIconsConf.ReadStr("Online", sFilename, "");
   sFilepath[MAX_FILENAME_LEN - 1] = '\0';
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmOnline.load(sFilepath);

   fIconsConf.ReadStr("FFC", sFilename, "");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmFFC.load(sFilepath);
   if (pmFFC.isNull()) pmFFC = pmOnline;

   fIconsConf.ReadStr("Offline", sFilename, "");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmOffline.load(sFilepath);

   fIconsConf.ReadStr("Away", sFilename, "");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmAway.load(sFilepath);

   fIconsConf.ReadStr("NA", sFilename, "");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmNa.load(sFilepath);

   fIconsConf.ReadStr("Occupied", sFilename, "");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmOccupied.load(sFilepath);

   fIconsConf.ReadStr("DND", sFilename, "");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmDnd.load(sFilepath);

   fIconsConf.ReadStr("Private", sFilename, "");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmPrivate.load(sFilepath);

   fIconsConf.ReadStr("Message", sFilename, "none");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmMessage.load(sFilepath);

   fIconsConf.ReadStr("Url", sFilename, "");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmUrl.load(sFilepath);

   fIconsConf.ReadStr("Chat", sFilename, "");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmChat.load(sFilepath);

   fIconsConf.ReadStr("File", sFilename, "");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmFile.load(sFilepath);

   fIconsConf.ReadStr("AIMOnline", sFilename, "");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmAIMOnline.load(sFilepath);
   if (pmAIMOnline.isNull()) pmMSNOnline = pmOnline;

   fIconsConf.ReadStr("AIMOffline", sFilename, "");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmAIMOffline.load(sFilepath);
   if (pmAIMOffline.isNull()) pmAIMOffline = pmOffline;

   fIconsConf.ReadStr("AIMAway", sFilename, "");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmAIMAway.load(sFilepath);
   if (pmAIMAway.isNull()) pmAIMAway = pmAway;

   fIconsConf.ReadStr("MSNOnline", sFilename, "");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmMSNOnline.load(sFilepath);
   if (pmMSNOnline.isNull()) pmMSNOnline = pmOnline;

   fIconsConf.ReadStr("MSNOffline", sFilename, "");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmMSNOffline.load(sFilepath);
   if (pmMSNOffline.isNull()) pmMSNOffline = pmOffline;
      
   fIconsConf.ReadStr("MSNAway", sFilename, "");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmMSNAway.load(sFilepath);
   if (pmMSNAway.isNull()) pmMSNAway = pmAway;
   
   fIconsConf.ReadStr("MSNOccupied", sFilename, "");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmMSNOccupied.load(sFilepath);
   if (pmMSNOccupied.isNull()) pmMSNOccupied = pmOccupied;
   
   fIconsConf.ReadStr("MSNPrivate", sFilename, "");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmMSNPrivate.load(sFilepath);
   if (pmMSNPrivate.isNull()) pmMSNPrivate = pmPrivate;
   
   fIconsConf.ReadStr("Contact", sFilename, "");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmContact.load(sFilepath);
   if(pmContact.isNull()) pmContact = pmMessage;

   fIconsConf.ReadStr("Authorize", sFilename, "");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmAuthorize.load(sFilepath);
   if(pmAuthorize.isNull()) pmAuthorize = pmMessage;

   fIconsConf.ReadStr("SMS", sFilename, "");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmSMS.load(sFilepath);
   if(pmSMS.isNull()) pmSMS = pmMessage;

   fIconsConf.ReadStr("SecureOff", sFilename, "");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmSecureOff.load(sFilepath);

   fIconsConf.ReadStr("SecureOn", sFilename, "");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmSecureOn.load(sFilepath);

   fIconsConf.ReadStr("History", sFilename, "");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmHistory.load(sFilepath);

   fIconsConf.ReadStr("Info", sFilename, "");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmInfo.load(sFilepath);
   
   fIconsConf.ReadStr("Remove", sFilename, "");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmRemove.load(sFilepath);

   fIconsConf.ReadStr("Search", sFilename, "");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmSearch.load(sFilepath);

#ifdef HAVE_LIBGPGME
   fIconsConf.ReadStr("GPGKey", sFilename, "");
   snprintf(sFilepath, MAX_FILENAME_LEN - 1, "%s%s", sIconPath, sFilename);
   pmGPGKey.load(sFilepath);
   if(pmGPGKey.isNull()) pmGPGKey = QPixmap(pixKeyEnabled_xpm);
#endif

   if (!_bInitial)
   {
     mnuStatus->changeItem(pmOnline, tr("&Online"), ICQ_STATUS_ONLINE);
     mnuStatus->changeItem(pmAway, tr("&Away"), ICQ_STATUS_AWAY);
     mnuStatus->changeItem(pmNa, tr("&Not Available"), ICQ_STATUS_NA);
     mnuStatus->changeItem(pmOccupied, tr("O&ccupied"), ICQ_STATUS_OCCUPIED);
     mnuStatus->changeItem(pmDnd, tr("&Do Not Disturb"), ICQ_STATUS_DND);
     mnuStatus->changeItem(pmFFC, tr("Free for C&hat"), ICQ_STATUS_FREEFORCHAT);
     mnuStatus->changeItem(pmOffline, tr("O&ffline"), ICQ_STATUS_OFFLINE);
     mnuStatus->changeItem(pmPrivate, tr("&Invisible"), ICQ_STATUS_FxPRIVATE);
     mnuUser->changeItem(pmMessage, tr("Send &Message"), mnuUserSendMsg);
     mnuUser->changeItem(pmUrl, tr("Send &URL"), mnuUserSendUrl);
     mnuUser->changeItem(pmChat, tr("Send &Chat Request"), mnuUserSendChat);
     mnuUser->changeItem(pmFile, tr("Send &File Transfer"), mnuUserSendFile);
     mnuUser->changeItem(pmContact, tr("Send Contact &List"), mnuUserSendContact);
     mnuUser->changeItem(pmAuthorize, tr("Send &Authorization"), mnuUserAuthorize);
     mnuUser->changeItem(pmAuthorize, tr("Send Authorization Re&quest"), mnuUserAuthorizeRequest);
     mnuUser->changeItem(pmSMS, tr("Send &SMS"), mnuUserSendSms);
     mnuUser->changeItem(pmSecureOff, tr("Request &Secure Channel"), mnuUserSendKey);
     mnuUser->changeItem(pmRemove, tr("Remove From List"), mnuUserRemoveFromList);
     mnuUser->changeItem(pmHistory, tr("View &History"), mnuUserHistory);
     mnuUser->changeItem(pmInfo, tr("&Info"), mnuUserGeneral);
     mnuOwnerAdm->changeItem(pmInfo, tr("&Info"), OwnerMenuGeneral);
     mnuOwnerAdm->changeItem(pmHistory, tr("View &History"), OwnerMenuHistory);
     mnuUserAdm->changeItem(MNU_USER_ADM_SEARCH_USER, pmSearch, tr("S&earch for User"));
     mnuUserAdm->changeItem(MNU_USER_ADM_AUTHORIZE_USER, pmAuthorize, tr("A&uthorize User"));
     mnuUserAdm->changeItem(MNU_USER_ADM_REQUEST_AUTH, pmAuthorize, tr("Re&quest Authorization"));
#ifdef HAVE_LIBGPGME
     mnuSystem->changeItem(pmGPGKey, tr("&GPG Key Manager..."), MNU_SYS_GPG);
#endif
     CUserView::UpdateFloaties();
     updateUserWin();
     updateEvents();
     updateStatus(0); //This shows ICQ status for the status label and dock icon

   }
}


//-----CMainWindow::initMenu--------------------------------------------------
void CMainWindow::initMenu()
{
   // Skins without a menubar (frame.hasMenuBar = 0) cannot use
   // QMenuData accelerators, so we need a accel that is available
   // even without a menubar:
   QAccel *a = new QAccel(this, "CMainWindow change Status Accel");
   a->insertItem(ALT + Key_O, ICQ_STATUS_ONLINE);
   a->insertItem(ALT + Key_A, ICQ_STATUS_AWAY);
   a->insertItem(ALT + Key_N, ICQ_STATUS_NA);
   a->insertItem(ALT + Key_C, ICQ_STATUS_OCCUPIED);
   a->insertItem(ALT + Key_D, ICQ_STATUS_DND);
   a->insertItem(ALT + Key_H, ICQ_STATUS_FREEFORCHAT);
   a->insertItem(ALT + Key_F, ICQ_STATUS_OFFLINE);
   a->insertItem(ALT + Key_I, ICQ_STATUS_FxPRIVATE);
   connect(a, SIGNAL(activated(int)), this, SLOT(changeStatusManual(int)));
#if QT_VERSION >= 0x030100
   connect(a, SIGNAL(activatedAmbiguously(int)), this, SLOT(changeStatusManual(int)));
#endif

   mnuStatus = new QPopupMenu(NULL);
   mnuPFM = new QPopupMenu(NULL);
   mnuPFM->insertItem(tr("Don't Show"), (int)ICQ_PLUGIN_STATUSxINACTIVE);
   mnuPFM->insertItem(tr("Available"), ICQ_PLUGIN_STATUSxACTIVE);
   mnuPFM->insertItem(tr("Busy"), ICQ_PLUGIN_STATUSxBUSY);
   connect(mnuPFM, SIGNAL(activated(int)), this, SLOT(changePFMStatus(int)));
   mnuStatus->insertItem(tr("Phone \"Follow Me\""), mnuPFM);
   mnuStatus->insertSeparator();
   mnuStatus->insertItem(pmOnline, tr("&Online"), ICQ_STATUS_ONLINE);
   mnuStatus->insertItem(pmAway, tr("&Away"), ICQ_STATUS_AWAY);
   mnuStatus->insertItem(pmNa, tr("&Not Available"), ICQ_STATUS_NA);
   mnuStatus->insertItem(pmOccupied, tr("O&ccupied"), ICQ_STATUS_OCCUPIED);
   mnuStatus->insertItem(pmDnd, tr("&Do Not Disturb"), ICQ_STATUS_DND);
   mnuStatus->insertItem(pmFFC, tr("Free for C&hat"), ICQ_STATUS_FREEFORCHAT);
   mnuStatus->insertItem(pmOffline, tr("O&ffline"), ICQ_STATUS_OFFLINE);
   mnuStatus->insertSeparator();
   mnuStatus->insertItem(pmPrivate, tr("&Invisible"), ICQ_STATUS_FxPRIVATE);
   mnuStatus->setAccel(a->key(ICQ_STATUS_ONLINE), ICQ_STATUS_ONLINE);
   mnuStatus->setAccel(a->key(ICQ_STATUS_AWAY), ICQ_STATUS_AWAY);
   mnuStatus->setAccel(a->key(ICQ_STATUS_NA), ICQ_STATUS_NA);
   mnuStatus->setAccel(a->key(ICQ_STATUS_OCCUPIED), ICQ_STATUS_OCCUPIED);
   mnuStatus->setAccel(a->key(ICQ_STATUS_DND), ICQ_STATUS_DND);
   mnuStatus->setAccel(a->key(ICQ_STATUS_FREEFORCHAT), ICQ_STATUS_FREEFORCHAT);
   mnuStatus->setAccel(a->key(ICQ_STATUS_OFFLINE), ICQ_STATUS_OFFLINE);
   mnuStatus->setAccel(a->key(ICQ_STATUS_FxPRIVATE), ICQ_STATUS_FxPRIVATE);

   connect(mnuStatus, SIGNAL(activated(int)), this, SLOT(changeStatusManual(int)));

   mnuUserGroups = new QPopupMenu(NULL);
   connect(mnuUserGroups, SIGNAL(activated(int)), this, SLOT(setCurrentGroupMenu(int)));

   mnuDebug = new QPopupMenu(NULL);
   mnuDebug->insertItem(tr("Status Info"));
   mnuDebug->insertItem(tr("Unknown Packets"));
   mnuDebug->insertItem(tr("Errors"));
   mnuDebug->insertItem(tr("Warnings"));
   mnuDebug->insertItem(tr("Packets"));
   mnuDebug->insertSeparator();
   mnuDebug->insertItem(tr("Set All"));
   mnuDebug->insertItem(tr("Clear All"));
   mnuDebug->setCheckable(true);
   for (int i = 0; i < 5; i++)
     mnuDebug->setItemChecked(mnuDebug->idAt(i), DEBUG_LEVEL & (1 << i));
   connect(mnuDebug, SIGNAL(activated(int)), this, SLOT(changeDebug(int)));

   mnuOwnerAdm = new QPopupMenu(NULL);
   mnuOwnerAdm->insertItem(tr("&View System Messages"), OwnerMenuView);
   mnuOwnerAdm->insertSeparator();
   mnuOwnerAdm->insertItem(pmInfo, tr("&Info"), OwnerMenuGeneral);
   mnuOwnerAdm->insertItem(pmHistory, tr("View &History"), OwnerMenuHistory);
   mnuOwnerAdm->insertSeparator();
   mnuOwnerAdm->insertItem(tr("&Owner Manager"), OwnerMenuManager);
   mnuOwnerAdm->insertItem(tr("&Security/Password Options"), OwnerMenuSecurity);
   mnuOwnerAdm->insertItem(tr("&Random Chat Group"), OwnerMenuRandomChat);
   mnuOwnerAdm->insertSeparator();
   mnuOwnerAdm->insertItem(tr("Debug Level"), mnuDebug);
   connect (mnuOwnerAdm, SIGNAL(activated(int)), this, SLOT(callOwnerFunction(int)));

   mnuUserAdm = new QPopupMenu(NULL);
   mnuUserAdm->insertItem(tr("&Add User"), this, SLOT(showAddUserDlg()), 0, MNU_USER_ADM_ADD_USER);
   mnuUserAdm->insertItem(pmSearch, tr("S&earch for User"), this, SLOT(showSearchUserDlg()), 0, MNU_USER_ADM_SEARCH_USER);
   mnuUserAdm->insertItem(pmAuthorize, tr("A&uthorize User"), this, SLOT(showAuthUserDlg()), 0, MNU_USER_ADM_AUTHORIZE_USER);
   mnuUserAdm->insertItem(pmAuthorize, tr("Re&quest Authorization"), this, SLOT(showReqAuthDlg(int)), 0, MNU_USER_ADM_REQUEST_AUTH);
   mnuUserAdm->insertItem(tr("R&andom Chat"), this, SLOT(slot_randomchatsearch()), 0, MNU_USER_ADM_RANDOM_CHAT);
   mnuUserAdm->insertSeparator();
   mnuUserAdm->insertItem(tr("&Popup All Messages"), this, SLOT(slot_popupall()), 0, MNU_USER_ADM_POPUP_ALL_MSG);
   mnuUserAdm->insertItem(tr("Edit &Groups"), this, SLOT(showEditGrpDlg()), 0, MNU_USER_ADM_EDIT_GROUPS);
   mnuUserAdm->insertSeparator();
   mnuUserAdm->insertItem(tr("Update All Users"), this, SLOT(slot_updateAllUsers()), 0, MNU_USER_ADM_UPDATE_ALL_USERS);
   mnuUserAdm->insertItem(tr("Update Current Group"), this, SLOT(slot_updateAllUsersInGroup()), 0, MNU_USER_ADM_UPDATE_CURRENT_GROUP);
   mnuUserAdm->insertItem(tr("&Redraw User Window"), this, SLOT(updateUserWin()), 0, MNU_USER_ADM_REDRAW_USER_WIN);
   mnuUserAdm->insertItem(tr("&Save All Users"), this, SLOT(saveAllUsers()), 0, MNU_USER_ADM_SAVE_ALL_USERS);

   QPopupMenu *mnuHelp = new QPopupMenu(NULL);
   mnuHelp->insertItem(tr("&Hints"), this, SLOT(slot_hints()));
   mnuHelp->insertItem(tr("&About"), this, SLOT(aboutBox()));
   mnuHelp->insertItem(tr("&Statistics"), this, SLOT(slot_stats()));

   mnuSystem = new QPopupMenu(NULL);
   mnuSystem->setCheckable(true);
   mnuSystem->insertItem(tr("System Functions"), mnuOwnerAdm, 0, MNU_SYS_SYSTEM_FUNCTIONS);
   mnuSystem->insertItem(tr("User Functions"), mnuUserAdm, 0, MNU_SYS_USER_FUNCTIONS);
   mnuSystem->insertItem(tr("&Status"), mnuStatus, 0, MNU_SYS_STATUS);
   mnuSystem->insertItem(tr("&Group"), mnuUserGroups, 0, MNU_SYS_GROUP);
   mnuSystem->insertItem(tr("Set &Auto Response..."), this, SLOT(slot_AwayMsgDlg()), 0, MNU_SYS_AUTO_RESPONSE);
   mnuSystem->insertSeparator();
   mnuSystem->insertItem(tr("&Network Window"), licqLogWindow, SLOT(show()), 0, MNU_SYS_NETWORKWIN);
   mnuSystem->insertItem(tr("&Mini Mode"), this, SLOT(ToggleMiniMode()), 0, MNU_SYS_MINI_MODE);
   mnuSystem->insertItem(tr("Show Offline &Users"), this, SLOT(ToggleShowOffline()), 0, MNU_SYS_SHOW_OFFLINE);
   mnuSystem->insertItem(tr("&Thread Group View"), this, SLOT(ToggleThreadView()), 0, MNU_SYS_THREAD_VIEW);
   mnuSystem->insertItem(tr("&Options..."), this, SLOT(popupOptionsDlg()), 0, MNU_SYS_OPTIONS);
   mnuSystem->insertItem(tr("S&kin Browser..."), this, SLOT(showSkinBrowser()), 0, MNU_SYS_SKINBROWSER);
   mnuSystem->insertItem(tr("&Plugin Manager..."), this, SLOT(showPluginDlg()), 0, MNU_SYS_PLUGINS);
#ifdef HAVE_LIBGPGME
   mnuSystem->insertItem(pmGPGKey, tr("&GPG Key Manager..."), this, SLOT(showGPGKeyManager()), 0, MNU_SYS_GPG);
#endif
   mnuSystem->insertSeparator();
   mnuSystem->insertItem(tr("Sa&ve Settings"), this, SLOT(saveOptions()));
   mnuSystem->insertItem(tr("&Help"), mnuHelp);
   mnuSystem->insertItem(tr("E&xit"), this, SLOT(slot_shutdown()));
   mnuSystem->setItemChecked(mnuSystem->idAt(MNUxITEM_SHOWxOFFLINE), m_bShowOffline);
   mnuSystem->setItemChecked(mnuSystem->idAt(MNUxITEM_THREADxVIEW), m_bThreadView);

   mnuGroup = new QPopupMenu(NULL);
   mnuGroup->setCheckable(true);
   connect(mnuGroup, SIGNAL(activated(int)), this, SLOT(UserGroupToggled(int)));
   connect(mnuGroup, SIGNAL(aboutToShow()), this, SLOT(FillUserGroup()));

   mnuServerGroup = new QPopupMenu(NULL);
   mnuServerGroup->setCheckable(true);
   connect(mnuServerGroup, SIGNAL(activated(int)), this, SLOT(ServerGroupChanged(int)));
   connect(mnuServerGroup, SIGNAL(aboutToShow()), this, SLOT(FillServerGroup()));

   mnuUtilities = new QPopupMenu(NULL);
   for (unsigned short i = 0; i < gUtilityManager.NumUtilities(); i++)
   {
     mnuUtilities->insertItem(gUtilityManager.Utility(i)->Name());
   }
   connect(mnuUtilities, SIGNAL(activated(int)), this, SLOT(slot_utility(int)));

   mnuMiscModes = new QPopupMenu(NULL);
   mnuMiscModes->setCheckable(true);
   mnuMiscModes->insertItem(tr("Accept in Away"));
   mnuMiscModes->insertItem(tr("Accept in Not Available"));
   mnuMiscModes->insertItem(tr("Accept in Occupied"));
   mnuMiscModes->insertItem(tr("Accept in Do Not Disturb"));
   mnuMiscModes->insertItem(tr("Auto Accept Files" ) );
   mnuMiscModes->insertItem(tr("Auto Accept Chats" ) );
   mnuMiscModes->insertItem(tr("Auto Request Secure" ) );
   mnuMiscModes->insertItem(tr("Use GPG Encryption" ) );
   mnuMiscModes->insertItem(tr("Use Real Ip (LAN)" ) );
   mnuMiscModes->insertSeparator();
   mnuMiscModes->insertItem(tr("Online to User"));
   mnuMiscModes->insertItem(tr("Away to User"));
   mnuMiscModes->insertItem(tr("Not Available to User"));
   mnuMiscModes->insertItem(tr("Occupied to User"));
   mnuMiscModes->insertItem(tr("Do Not Disturb to User"));
   connect(mnuMiscModes, SIGNAL(activated(int)), this, SLOT(slot_miscmodes(int)));

   mnuUser = new QPopupMenu(NULL);
   mnuUser->insertItem(tr("&View Event"), mnuUserView);
   mnuSend = new QPopupMenu(this);
   mnuSend->insertItem(pmMessage, tr("Send &Message"), mnuUserSendMsg);
   mnuSend->insertItem(pmUrl, tr("Send &URL"), mnuUserSendUrl);
   mnuSend->insertItem(pmChat, tr("Send &Chat Request"), mnuUserSendChat);
   mnuSend->insertItem(pmFile, tr("Send &File Transfer"), mnuUserSendFile);
   mnuSend->insertItem(pmContact, tr("Send Contact &List"), mnuUserSendContact);
   mnuSend->insertItem(pmAuthorize, tr("Send &Authorization"), mnuUserAuthorize);
   mnuSend->insertItem(pmAuthorize, tr("Send Authorization Re&quest"), mnuUserAuthorizeRequest);
   mnuSend->insertItem(pmSMS, tr("Send &SMS"), mnuUserSendSms);
   mnuSend->insertItem(tr("Update Info Plugin List"),
                       mnuUserSendInfoPluginListRequest);
   mnuSend->insertItem(tr("Update Status Plugin List"),
                       mnuUserSendStatusPluginListRequest);
   mnuSend->insertItem(tr("Update Phone \"Follow Me\" Status"),
                       mnuUserSendPhoneFollowMeRequest);
   mnuSend->insertItem(tr("Update ICQphone Status"),
                       mnuUserSendICQphoneRequest);
   mnuSend->insertItem(tr("Update File Server Status"),
                       mnuUserSendFileServerRequest);
   mnuSend->insertSeparator();
   mnuSend->insertItem(pmSecureOff, tr("Request &Secure Channel"), mnuUserSendKey);
   connect (mnuSend, SIGNAL(activated(int)), this, SLOT(callUserFunction(int)));
   mnuUser->insertItem(tr("Send"), mnuSend);
   mnuUser->insertItem(tr("Misc Modes"), mnuMiscModes);
   mnuUser->insertItem(tr("U&tilities"), mnuUtilities);
   //mnuUser->insertItem(tr("Check If Invisible"), mnuUserCheckIfInvisible);
   mnuUser->insertItem(tr("Check Auto Response"), mnuUserCheckResponse);
   mnuUser->insertItem(pmCustomAR, tr("Custom Auto Response..."), mnuUserCustomAutoResponse);
   mnuUser->insertSeparator();
   mnuUser->insertItem(tr("Toggle &Floaty"), mnuUserFloaty);
   mnuUser->insertItem(tr("Edit User Group"), mnuGroup);
   mnuUser->insertItem(pmRemove, tr("Remove From List"), mnuUserRemoveFromList);
   mnuUser->insertSeparator();
#ifdef HAVE_LIBGPGME
   mnuUser->insertItem(pmGPGKey, tr("Set GPG key"), mnuUserSelectGPGKey );
#endif
   mnuUser->insertItem(pmHistory, tr("View &History"), mnuUserHistory);
   mnuUser->insertItem(pmInfo, tr("&Info"), mnuUserGeneral);
   connect (mnuUser, SIGNAL(activated(int)), this, SLOT(callUserFunction(int)));
   connect (mnuUser, SIGNAL(aboutToShow()), this, SLOT(slot_usermenu()));
}


void CMainWindow::slot_usermenu()
{
  //ICQUser *u = gUserManager.FetchUser(userView->SelectedItemUin(), LOCK_R);
  ICQUser *u = gUserManager.FetchUser(m_szUserMenuId, m_nUserMenuPPID, LOCK_R);

  if (u == NULL)
  {
    mnuUser->changeItem(mnuUserCheckResponse, tr("Check Auto Response"));
    mnuUser->setItemEnabled(mnuUserCheckResponse, false);
    return;
  }

  const char *szStatus = u->StatusStrShort();
  unsigned short status = u->Status();
  
  if (status == ICQ_STATUS_OFFLINE)
  {
    mnuUser->changeItem(mnuUserCheckResponse, tr("Check Auto Response"));
    mnuUser->setItemEnabled(mnuUserCheckResponse, false);
  }
  else if (status == ICQ_STATUS_ONLINE)
  {
    mnuUser->changeItem(mnuUserCheckResponse, tr("Check Auto Response"));
    mnuUser->setItemEnabled(mnuUserCheckResponse, true);
  }
  else
  {
    mnuUser->changeItem(mnuUserCheckResponse, QString(tr("Check %1 Response")).arg(szStatus));
    mnuUser->setItemEnabled(mnuUserCheckResponse, true);
  }

  mnuMiscModes->setItemChecked(mnuMiscModes->idAt(0), u->AcceptInAway());
  mnuMiscModes->setItemChecked(mnuMiscModes->idAt(1), u->AcceptInNA());
  mnuMiscModes->setItemChecked(mnuMiscModes->idAt(2), u->AcceptInOccupied());
  mnuMiscModes->setItemChecked(mnuMiscModes->idAt(3), u->AcceptInDND());
  mnuMiscModes->setItemChecked(mnuMiscModes->idAt(4), u->AutoFileAccept());
  mnuMiscModes->setItemChecked(mnuMiscModes->idAt(5), u->AutoChatAccept());
  mnuMiscModes->setItemChecked(mnuMiscModes->idAt(6), u->AutoSecure());
  mnuMiscModes->setItemChecked(mnuMiscModes->idAt(7), u->UseGPG() );
  mnuMiscModes->setItemChecked(mnuMiscModes->idAt(8), u->SendRealIp());
  mnuMiscModes->setItemChecked(mnuMiscModes->idAt(10), u->StatusToUser() == ICQ_STATUS_ONLINE);
  mnuMiscModes->setItemChecked(mnuMiscModes->idAt(11), u->StatusToUser() == ICQ_STATUS_AWAY);
  mnuMiscModes->setItemChecked(mnuMiscModes->idAt(12), u->StatusToUser() == ICQ_STATUS_NA);
  mnuMiscModes->setItemChecked(mnuMiscModes->idAt(13), u->StatusToUser() == ICQ_STATUS_OCCUPIED);
  mnuMiscModes->setItemChecked(mnuMiscModes->idAt(14), u->StatusToUser() == ICQ_STATUS_DND);
  mnuMiscModes->setItemEnabled(6, gLicqDaemon->CryptoEnabled());
  mnuUser->setItemChecked(mnuUserCustomAutoResponse, u->CustomAutoResponse()[0] != '\0');
  // Send modes
  mnuSend->setItemEnabled(mnuUserSendChat, !u->StatusOffline());
  mnuSend->setItemEnabled(mnuUserSendFile, !u->StatusOffline());

  if (strlen(u->GetCellularNumber()))
    mnuSend->setItemEnabled(mnuUserSendSms, true);
  else
    mnuSend->setItemEnabled(mnuUserSendSms, false);

  if (u->Secure())
    mnuSend->changeItem(pmSecureOn, tr("Close &Secure Channel"), mnuUserSendKey);
  else
    mnuSend->changeItem(pmSecureOff, tr("Request &Secure Channel"), mnuUserSendKey);

  ProtoPluginsList pl;
  ProtoPluginsListIter it;
  licqDaemon->ProtoPluginList(pl);
  unsigned long nSendFuncs = 0xFFFFFFFF;
  bool bIsLicq = m_nUserMenuPPID == LICQ_PPID;
  for (it = pl.begin(); it != pl.end(); it++)
  {
    if ((*it)->PPID() != LICQ_PPID && (*it)->PPID() == m_nUserMenuPPID)
    {
      nSendFuncs = (*it)->SendFunctions();
      break;
    }
  }
  
  // The send submenu and misc modes submenu that depend on sending capabilities
  mnuUser->setItemVisible(mnuUserSendMsg, nSendFuncs & PP_SEND_MSG);
  mnuUser->setItemVisible(mnuUserSendUrl, nSendFuncs & PP_SEND_URL);
  mnuUser->setItemVisible(mnuUserSendFile, nSendFuncs & PP_SEND_FILE);
  mnuMiscModes->setItemVisible(mnuMiscModes->idAt(4), nSendFuncs & PP_SEND_FILE);
  mnuUser->setItemVisible(mnuUserSendChat, nSendFuncs & PP_SEND_CHAT);
  mnuMiscModes->setItemVisible(mnuMiscModes->idAt(5), nSendFuncs & PP_SEND_CHAT);
  mnuUser->setItemVisible(mnuUserSendContact, nSendFuncs & PP_SEND_CONTACT);
  mnuUser->setItemVisible(mnuUserAuthorize, nSendFuncs & PP_SEND_AUTH);
  mnuUser->setItemVisible(mnuUserAuthorizeRequest, nSendFuncs & PP_SEND_AUTHxREQ);
  mnuUser->setItemVisible(mnuUserSendSms, nSendFuncs & PP_SEND_SMS);
  mnuUser->setItemVisible(mnuUserSendKey, nSendFuncs & PP_SEND_SECURE);
  mnuMiscModes->setItemVisible(mnuMiscModes->idAt(6), nSendFuncs & PP_SEND_SECURE);
  
  // ICQ Protocol only
  mnuUser->setItemVisible(mnuUserSendInfoPluginListRequest, bIsLicq);
  mnuUser->setItemVisible(mnuUserSendStatusPluginListRequest, bIsLicq);
  mnuUser->setItemVisible(mnuUserSendPhoneFollowMeRequest, bIsLicq);
  mnuUser->setItemVisible(mnuUserSendICQphoneRequest, bIsLicq);
  mnuUser->setItemVisible(mnuUserSendFileServerRequest, bIsLicq);
  mnuUser->setItemVisible(mnuUserCheckIfInvisible, bIsLicq);
  mnuUser->setItemVisible(mnuUserCheckResponse, bIsLicq);
  mnuUser->setItemVisible(mnuUserCustomAutoResponse, bIsLicq);
  
  // ICQ Protocol only
  mnuMiscModes->setItemVisible(mnuMiscModes->idAt(8), bIsLicq);
  mnuMiscModes->setItemVisible(mnuMiscModes->idAt(10), bIsLicq);
  mnuMiscModes->setItemVisible(mnuMiscModes->idAt(11), bIsLicq);
  mnuMiscModes->setItemVisible(mnuMiscModes->idAt(12), bIsLicq);
  mnuMiscModes->setItemVisible(mnuMiscModes->idAt(13), bIsLicq);
  mnuMiscModes->setItemVisible(mnuMiscModes->idAt(14), bIsLicq);  
  
  // FIXME: Groups! Show only what is for that protocol plugin in the submenu
  // to properly manage users
  
  gUserManager.DropUser(u);
}


void CMainWindow::slot_stats()
{
#ifdef SAVE_STATS
  QString s = tr("Daemon Statistics\n(Today/Total)\n");
  QDateTime t_total, t_today;
  t_today.setTime_t(licqDaemon->StartTime());
  t_total.setTime_t(licqDaemon->ResetTime());
  s += tr("Up since %1\n").arg(t_today.toString());
  s += tr("Last reset %1\n\n").arg(t_total.toString());
  DaemonStatsList::iterator iter;
  for (iter = licqDaemon->AllStats().begin(); iter != licqDaemon->AllStats().end(); iter++)
  {
    s += tr("%1: %2 / %3\n").arg(iter->Name()).arg(iter->Today()).arg(iter->Total());
  }
  
  if (QueryUser(this, s, tr("&Reset"), tr("&Ok"), true, tr("Do you really want to reset your stats?"), false, NULL))
    licqDaemon->ResetStats();
#else
  QString s = tr("Daemon Statistics\n\n");
  QDateTime t_today;
  t_today.setTime_t(licqDaemon->StartTime());
  s += tr("Up since %1\n\n").arg(t_today.toString());
  DaemonStatsList::iterator iter;
  for (iter = licqDaemon->AllStats().begin(); iter != licqDaemon->AllStats().end(); iter++)
  {
    s += tr("%1: %2\n")
       .arg(iter->Name())
       .arg(iter->Today());
  }
  InformUser(this, s);
#endif
}


void CMainWindow::showOwnerManagerDlg()
{
  if (ownerManagerDlg)
    ownerManagerDlg->raise();
  else
  {
    ownerManagerDlg = new OwnerManagerDlg(this, licqDaemon, licqSigMan);
    connect(ownerManagerDlg, SIGNAL(signal_done()), this, SLOT(slot_doneOwnerManager())); 
    ownerManagerDlg->show();
  }
}

void CMainWindow::showSearchUserDlg()
{
  SearchUserDlg *searchUserDlg = new SearchUserDlg(this, licqDaemon, licqSigMan, m_DefaultEncoding);
  searchUserDlg->show();
}


void CMainWindow::showAddUserDlg()
{
  AddUserDlg *addUserDlg = new AddUserDlg(licqDaemon);
  addUserDlg->show();
}


void CMainWindow::showAuthUserDlg()
{
  (void) new AuthUserDlg(licqDaemon, 0, true);
}

// Wrapper for the true function, necessary to kill a Qt2 warning
void CMainWindow::showReqAuthDlg(int nId)
{
  showReqAuthDlg((char *)0, (unsigned long)0);
}

void CMainWindow::showReqAuthDlg(const char *szId, unsigned long nPPID)
{
  ReqAuthDlg *reqAuthDlg = new ReqAuthDlg(licqDaemon, szId, nPPID);
  reqAuthDlg->show();
}

void CMainWindow::showEditGrpDlg()
{
  EditGrpDlg *d = new EditGrpDlg;
  connect (d, SIGNAL(signal_updateGroups()), this, SLOT(updateGroups()));
  d->show();
}

void CMainWindow::showOptionsDlg(int tab)
{
  if (optionsDlg) {
    optionsDlg->raise();
//    optionsDlg->showTab(tab);
  }
  else {
    optionsDlg = new OptionsDlg(this, (OptionsDlg::tabs) tab);
    connect(optionsDlg, SIGNAL(signal_done()), this, SLOT(slot_doneOptions()));
  }
}

void CMainWindow::showSkinBrowser()
{
  SkinBrowserDlg *d = new SkinBrowserDlg(this);
  d->show();
}

void CMainWindow::showGPGKeyManager()
{
#ifdef HAVE_LIBGPGME
  ( new GPGKeyManager() )->show();
#endif
}

void CMainWindow::showPluginDlg()
{
  if (pluginDlg != NULL)
    pluginDlg->raise();
  else
  {
    pluginDlg = new PluginDlg();
    connect(pluginDlg, SIGNAL(signal_done()), this, SLOT(slot_doneplugindlg()));

    connect(pluginDlg, SIGNAL(pluginUnloaded(unsigned long)), this, SLOT(slot_pluginUnloaded(unsigned long)));
  }
}

void CMainWindow::slot_pluginUnloaded(unsigned long _nPPID)
{
  if (m_lnProtMenu.size() == 2)
  {
    mnuStatus->removeItemAt(2);
    mnuStatus->removeItemAt(1);
    mnuStatus->removeItemAt(0);
    m_nProtoNum = 0;
    m_lnProtMenu.clear();
  }
  else
  {
    std::vector<unsigned long>::iterator iter;
    int n = 0;
    for (iter = m_lnProtMenu.begin(); iter != m_lnProtMenu.end(); ++iter)
    {
      if (*iter == _nPPID)
      {
        m_lnProtMenu.erase(iter);
        mnuStatus->removeItemAt(n);
        m_nProtoNum--;
        break;
      }
      n++;
    }
  }

#ifdef USE_KDE
    kdeIMInterface->removeProtocol(_nPPID);
#endif
}

void CMainWindow::slot_doneplugindlg()
{
  pluginDlg = NULL;
}

void CMainWindow::slot_randomchatsearch()
{
  (void) new CRandomChatDlg(this, licqDaemon, licqSigMan);
}


void CMainWindow::slot_popupall()
{
  // Do nothing if there are no events pending
  if (ICQUser::getNumUserEvents() == 0) return;

  // Do system messages first
  ICQOwner *o = gUserManager.FetchOwner(LOCK_R);
  unsigned short nNumMsg = 0;
  if (o)
  {
    nNumMsg = o->NewMessages();
    gUserManager.DropOwner();
  }
  
  if (nNumMsg > 0)
  {
    callOwnerFunction(OwnerMenuView);
  }

  UserStringList users;
  std::list<unsigned long> ppids;
  FOR_EACH_USER_START(LOCK_R)
  {
    if (pUser->NewMessages() > 0)
    {
      users.push_back(pUser->IdString());
      ppids.push_back(pUser->PPID());
    }
  }
  FOR_EACH_USER_END

  for (UserStringList::iterator iter = users.begin(); iter != users.end(); iter++)
  {
    callDefaultFunction(*iter, ppids.front());
    ppids.pop_front();
  }
}


void CMainWindow::slot_doneOwnerManager()
{
  ownerManagerDlg = 0;
}


void CMainWindow::slot_doneOptions()
{
  optionsDlg = NULL;
}

// -----------------------------------------------------------------------------

void CMainWindow::slot_hints()
{
  QString hints = tr(
    "<h2>Hints for Using<br>the Licq Qt-GUI Plugin</h2><br><hr><br>"
    "<ul>"
    "<li>Change your status by right clicking on the status label.</li>"
    "<li>Change your auto response by double-clicking on the status label.</li>"
    "<li>View system messages by double clicking on the message label.</li>"
    "<li>Change groups by right clicking on the message label.</li>"
    "<li>Use the following shortcuts from the contact list:<ul>"
    "<li><tt>Ctrl-M : </tt>Toggle mini-mode</li>"
    "<li><tt>Ctrl-O : </tt>Toggle show offline users</li>"
    "<li><tt>Ctrl-X : </tt>Exit</li>"
    "<li><tt>Ctrl-H : </tt>Hide</li>"
    "<li><tt>Ctrl-I : </tt>View the next message</li>"
    "<li><tt>Ctrl-V : </tt>View message</li>"
    "<li><tt>Ctrl-S : </tt>Send message</li>"
    "<li><tt>Ctrl-U : </tt>Send Url</li>"
    "<li><tt>Ctrl-C : </tt>Send chat request</li>"
    "<li><tt>Ctrl-F : </tt>Send File</li>"
    "<li><tt>Ctrl-A : </tt>Check Auto response</li>"
    "<li><tt>Ctrl-P : </tt>Popup all messages</li>"
    "<li><tt>Ctrl-L : </tt>Redraw user window</li>"
    "<li><tt>Delete : </tt>Delete user from current group</li>"
    "<li><tt>Ctrl-Delete : </tt>Delete user from contact list</li></ul>"
    "<li>Hold control while clicking on close in the function window to remove"
    "   the user from your contact list.</li>"
    "<li>Hit Ctrl-Enter from most text entry fields to select \"Ok\" or \"Accept\"."
    "   For example in the send tab of the user function window.</li>"
    "<li>Here is the complete list of user % options, which can be used in <b>OnEvent</b>"
    "   parameters, <b>auto responses</b>, and <b>utilities</b>:\n") + gMainWindow->usprintfHelp +
                   "</li></ul>" + tr(
    "<hr><p> For more information, see the Licq webpage (<tt>http://www.licq.org</tt>).</p>");

  (void) new HintsDlg(hints);
}


// -----------------------------------------------------------------------------
HintsDlg::HintsDlg(QString &hint)
  : LicqDialog(0, "HintsDlg", false, WDestructiveClose)
{
  setCaption(tr("Licq - Hints"));

  QBoxLayout* topLay = new QVBoxLayout(this, 5);

  txtView = new QTextView(this);
  txtView->setMinimumWidth(370);
  txtView->setMinimumHeight(450);
  txtView->setText(hint);
  txtView->setFocus();
  topLay->addWidget(txtView);

  QBoxLayout* lay = new QHBoxLayout(topLay);
  lay->addStretch(2);
  btnClose = new QPushButton(tr("&Close"), this);
  btnClose->setDefault(true);
  connect(btnClose, SIGNAL(clicked()), this, SLOT(close()));
  lay->addWidget(btnClose);

  show();
}

// -----------------------------------------------------------------------------

void CMainWindow::slot_viewurl(QWidget *q, QString url)
{
#ifdef USE_KDE
  KApplication* app = static_cast<KApplication*>(qApp);
  if (url.startsWith("mailto:"))
    app->invokeMailer( KURL(url) );
  else
  // If no URL viewer is set, use KDE default
  if (licqDaemon && (!licqDaemon->getUrlViewer()))
    app->invokeBrowser( url );
  else
#endif
  {
    if (licqDaemon == NULL)
      WarnUser(q, tr("Licq is unable to find a browser application due to an internal error."));
    else if (!licqDaemon->ViewUrl(url.local8Bit().data()))
      WarnUser(q, tr("Licq is unable to start your browser and open the URL.\nYou will need to start the browser and open the URL manually."));
  }
}

// -----------------------------------------------------------------------------

void CMainWindow::sendMsg(const char* szId, unsigned long nPPID, const QString& message)
{
    UserSendCommon* event =
            static_cast<UserSendCommon*>(callFunction(mnuUserSendMsg, szId, nPPID));
    if (event == 0) return;

    event->setText(message);
}

// -----------------------------------------------------------------------------

void CMainWindow::sendFileTransfer(const char* szId, unsigned long nPPID,
                                   const QString& filename, const QString& description)
{
    UserSendFileEvent* event =
            static_cast<UserSendFileEvent*>(callFunction(mnuUserSendFile, szId, nPPID));
    if (event == 0) return;

    event->setFile(filename, description);
}

// -----------------------------------------------------------------------------

void CMainWindow::sendChatRequest(const char* szId, unsigned long nPPID)
{
    UserSendCommon* event =
            static_cast<UserSendCommon*>(callFunction(mnuUserSendChat, szId, nPPID));
    if (event == 0) return;
}

// -----------------------------------------------------------------------------

void CMainWindow::addUser(const char* szId, unsigned long nPPID)
{
    if (szId == 0 || nPPID == 0) return;

    AddUserDlg* addUserDlg = new AddUserDlg(licqDaemon, szId, nPPID);
    addUserDlg->show();
}

// -----------------------------------------------------------------------------

#include "mainwin.moc"
