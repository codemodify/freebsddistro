// -*- c-basic-offset: 2; -*-
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
#if KDE_VERSION < 290
#include <kthemestyle.h>
#endif
#endif

// In Qt 3.x and above, we should use QStyleFactory, since the styles
// can come as plugins.
#if QT_VERSION < 300
#include <qwindowsstyle.h>
#include <qmotifstyle.h>
#include <qplatinumstyle.h>
#include <qcdestyle.h>
#if QT_VERSION >= 230
#if HAVE_QINTERLACESTYLE_H
#include <qinterlacestyle.h>
#endif
#endif
#if QT_VERSION >= 220
#include <qsgistyle.h>
#include <qmotifplusstyle.h>
#endif
#endif // end of QT_VERSION < 300

#include <qsessionmanager.h>
#include <qaccel.h>
#include "licqgui.h"
#include <qtextcodec.h>
#ifdef HAVE_LOCALE_H
#include <locale.h>
#endif
#include <unistd.h> // for getopt

#if QT_VERSION < 300
#include "jfcstyle.h"
#endif
#include "mainwin.h"
#include "sigman.h"
#include "outputwin.h"
#include "licq_log.h"
#include "licq_plugin.h"

#include "licq_icqd.h"

#if QT_VERSION >= 300
#include <qstylefactory.h>
#endif

CLicqGui *licqQtGui;

const char *LP_Usage(void)
{
#ifdef USE_KDE
  static const char usage[] =
    "Usage:  Licq [options] -p kde-gui -- [-h] [-s skinname] [-i iconpack] [-e extendediconpack] [-g gui style]\n"
    " -h : this help screen\n"
    " -s : set the skin to use (must be in {base dir}/qt-gui/skin.skinname)\n"
    " -i : set the icons to use (must be in {base dir}/qt-gui/icons.iconpack)\n"
    " -e : set the extended icons to use (must be in [base dir]/qt-gui/extended.icons.iconpack)\n"
    " -g : set the gui style (MOTIF / WINDOWS / MAC / CDE /"
#if QT_VERSION < 300
    " JFC /"
#endif
    " GTK / SGI / LCD), ignored by KDE support\n"
    " -d : start hidden (dock icon only)\n"
    " -D : disable dock icon for this session (does not affect dock icon settings)";
#else
  static const char usage[] =
    "Usage:  Licq [options] -p qt-gui -- [-h] [-s skinname] [-i iconpack] [-e extendediconpack] [-g gui style]\n"
    " -h : this help screen\n"
    " -s : set the skin to use (must be in {base dir}/qt-gui/skin.skinname)\n"
    " -i : set the icons to use (must be in {base dir}/qt-gui/icons.iconpack)\n"
    " -e : set the extended icons to use (must be in [base dir]/qt-gui/extended.icons.iconpack)\n"
    " -g : set the gui style (MOTIF / WINDOWS / MAC / CDE /"
#if QT_VERSION < 300
    " JFC /"
#endif
    " GTK / SGI / LCD), ignored by KDE support\n"
    " -d : start hidden (dock icon only)\n"
    " -D : disable dock icon for this session (does not affect dock icon settings)";
#endif

  return usage;
}

const char *LP_Name(void)
{
#ifdef USE_KDE
  static const char name[] = "KDE GUI";
#else
  static const char name[] = "Qt GUI";
#endif
  return name;
}


const char *LP_ConfigFile(void)
{
  return "licq_qt-gui.conf";
}


const char *LP_Version(void)
{
  static const char version[] = VERSION;
  return version;
}

const char *LP_Status(void)
{
  static const char status[] = "running";
  return status;
}

const char *LP_Description(void)
{
  static const char desc[] = "Qt based GUI";
  return desc;
}

static int gui_argc = 0;
static char** gui_argv = NULL;

bool LP_Init(int argc, char **argv)
{
  if (qApp != NULL)
  {
    gLog.Error("%sA Qt application is already loaded.\n%sRemove the plugin from the command line.\n", L_ERRORxSTR, L_BLANKxSTR);
    return false;
  }
  int i = argc-1;
  while(i >= 0) {
    if(strcmp(argv[i], "-h") == 0) {
      printf("%s\n", LP_Usage());
      return false;
    }
    i--;
  }

  // save for LP_Main (below)
  gui_argc=argc;
  gui_argv=argv;
  return true;
}


int LP_Main(CICQDaemon *_licqDaemon)
{
  licqQtGui = new CLicqGui(gui_argc, gui_argv);

  int nResult = licqQtGui->Run(_licqDaemon);
  licqQtGui->Shutdown();

  gui_argc = 0;
  gui_argv = NULL;

  return nResult;
}

QStyle *CLicqGui::SetStyle(const char *_szStyle)
{
  QStyle *s = NULL;

#if QT_VERSION < 300
  if (strncmp(_szStyle, "MOTIF", 3) == 0)
    s = new QMotifStyle;
  else if (strncmp(_szStyle, "WINDOWS", 3) == 0)
    s = new QWindowsStyle;
  else if (strncmp(_szStyle, "MAC", 3) == 0)
    s = new QPlatinumStyle;
  else if (strncmp(_szStyle, "CDE", 3) == 0)
    s = new QCDEStyle;
#if QT_VERSION >= 220
  else if (strncmp( _szStyle, "SGI", 3 ) == 0 )
    s = new QSGIStyle;
#endif
#if (QT_VERSION >= 230) && defined(QINTERLACESTYLE_H)
  else if (strncmp( _szStyle, "LCD", 3 ) == 0 )
    s = new QInterlaceStyle;
#endif
#if QT_VERSION < 300
  else if (strncmp(_szStyle, "JFC", 3) == 0)
    s = new JFCStyle;
#endif
#if QT_VERSION >= 220
  else if (strncmp(_szStyle, "GTK", 3) == 0)
    s = new QMotifPlusStyle(true);
#endif
#else  // QT_VERSION >= 300
  if (strncmp(_szStyle, "MOTIF", 3) == 0)
    s = QStyleFactory::create("motif");
  else if (strncmp(_szStyle, "WINDOWS", 3) == 0)
    s = QStyleFactory::create("windows");
  else if (strncmp(_szStyle, "MAC", 3) == 0)
    s = QStyleFactory::create("platinum");
  else if (strncmp(_szStyle, "CDE", 3) == 0)
    s = QStyleFactory::create("cde");
  else if (strncmp( _szStyle, "SGI", 3 ) == 0 )
    s = QStyleFactory::create("sgi");
#endif

  return s;
}

//-----------------------------------------------------------------------------

const char *GetLocale()
{
  char *szLang;
  szLang = getenv("LC_ALL");
  if (szLang == 0)
    szLang = getenv("LC_MESSAGES");
  if (szLang == 0)
    szLang = getenv("LANG");

  return szLang;
}

// -----------------------------------------------------------------------------

void CLicqGui::commitData(QSessionManager& sm)
{
  if(sm.allowsInteraction()) {
    gMainWindow->saveOptions();
    sm.release();
  }

  QStringList restartCmd;
  restartCmd  = cmdLineParams;
  restartCmd += QString("-session");
  restartCmd += sm.sessionId();
  sm.setRestartCommand(restartCmd);
}

void CLicqGui::saveState(QSessionManager& sm)
{
  sm.setRestartHint(QSessionManager::RestartIfRunning);
}

// -----------------------------------------------------------------------------

CLicqGui::CLicqGui(int argc, char **argv)
#ifdef USE_KDE
  : KApplication(argc, argv, "licq")
#else
  : QApplication(argc, argv)
#endif
{
  char skinName[32] = "";
  char iconsName[32] = "";
  char extendedIconsName[32] = "";
  char styleName[32] = "";
  bool bStartHidden = false;

  // initialize Members
  grabKeysym = 0;
  m_bDisableDockIcon = false;

  // store command line arguments for session management
  cmdLineParams += QString(argv[0]);
  cmdLineParams += QString("-p");
#ifdef USE_KDE
  cmdLineParams += QString("kde-gui");
#else
  cmdLineParams += QString("qt-gui");
#endif
  cmdLineParams += QString("--");
  for(int i=1;i<argc;i++)
    cmdLineParams += QString(argv[i]);

  // parse command line for arguments
  int i = 0;
  while( (i = getopt(argc, argv, "hs:i:e:g:dD")) > 0)
  {
    switch (i)
    {
    case 'h':  // help
      // ignore here
      break;
    case 's':  // skin name
      snprintf(skinName, sizeof(skinName), "%s", optarg);
      skinName[sizeof(skinName) - 1] = '\0';
      break;
    case 'i':  // icons name
      snprintf(iconsName, sizeof(iconsName), "%s", optarg);
      iconsName[sizeof(iconsName) - 1] = '\0';
      break;
    case 'e': // extended icons name
      snprintf(extendedIconsName, sizeof(extendedIconsName), "%s", optarg);
      extendedIconsName[sizeof(extendedIconsName) - 1] = '\0';
      break;
    case 'g': // gui style
      strncpy(styleName, optarg, sizeof(styleName));
      styleName[sizeof(styleName) - 1] = '\0';
      break;
    case 'd': // dock icon
      if (!m_bDisableDockIcon)
        bStartHidden = true;
      break;
    case 'D': // disable dock icon
      bStartHidden = false; // discard any -d
      m_bDisableDockIcon = true;
    }
  }

#ifndef USE_KDE
  char buf[500];
  snprintf(buf, sizeof(buf), "%s/licq_qt-gui.style", BASE_DIR);
  buf[sizeof(buf) - 1] = '\0';

  QStyle *style = SetStyle(styleName);

  // Write out the style if not NULL
  if (style != NULL)
  {
    FILE *f = fopen(buf, "w");
    if (f != NULL)
    {
      fprintf(f, "%s\n", styleName);
      fclose(f);
    }
  }
  // Otherwise try and load it from the file
  else
  {
    FILE *f = fopen(buf, "r");
    if (f != NULL)
    {
      if (fgets(buf, 64, f) != NULL)
        style = SetStyle(buf);
      fclose(f);
    }
  }

  if ( style )
    setStyle(style);
#endif
  m_szSkin = strdup(skinName);
  m_szIcons = strdup(iconsName);
  m_szExtendedIcons = strdup(extendedIconsName);
  m_bStartHidden = bStartHidden;

  // Try and load a translation
  gLog.Info("%sAttempting to load %s Qt-GUI translation.\n", L_INITxSTR,
            GetLocale());
  QString str;
  str.sprintf("%sqt-gui/locale/%s", SHARE_DIR, GetLocale());
  QTranslator *trans = new QTranslator(this);
  trans->load(str);
  installTranslator(trans);
}


CLicqGui::~CLicqGui(void)
{
  delete licqSignalManager;
  delete licqLogWindow;
}

void CLicqGui::Shutdown(void)
{
  gLog.Info("%sShutting down gui.\n", L_ENDxSTR);
  gLog.ModifyService(S_PLUGIN, 0);
  if(licqMainWindow) {
    licqMainWindow->close();
    delete licqMainWindow;
    licqMainWindow = 0;
  }

  delete qApp;
  qApp = 0;
}


int CLicqGui::Run(CICQDaemon *_licqDaemon)
{
  // Register with the daemon, we want to receive all signals
  int nPipe = _licqDaemon->RegisterPlugin(SIGNAL_ALL);

  // Create the main widgets
  licqSignalManager = new CSignalManager(_licqDaemon, nPipe);
  licqLogWindow = new CQtLogWindow;
  gLog.AddService(new CLogService_Plugin(licqLogWindow, L_MOST));
  licqMainWindow = new CMainWindow(_licqDaemon, licqSignalManager, licqLogWindow,
     m_bStartHidden, m_szSkin, m_szIcons, m_szExtendedIcons, m_bDisableDockIcon);

  setMainWidget(licqMainWindow);
  int r = exec();
  _licqDaemon->UnregisterPlugin();

  return r;
}


// -----------------------------------------------------------------------------
// global accelerator stuff

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

static unsigned int keyToXMod( int keyCode )
{
	uint mod = 0;

	if ( keyCode == 0 ) return mod;

	if ( keyCode & Qt::SHIFT )
		 mod |= ShiftMask;
	if ( keyCode & Qt::CTRL )
		 mod |= ControlMask;
	if ( keyCode & Qt::ALT )
		 mod |= Mod1Mask;

	return mod;
}

static uint keyToXSym( int keyCode )
{
  char *toks[4], *next_tok;
  char sKey[100];
  int nb_toks = 0;

  uint keysym = 0;
  QString s = QAccel::keyToString( keyCode );

  if ( s.isEmpty() ) return keysym;

  qstrncpy(sKey, s.ascii(), sizeof(sKey));
  next_tok = strtok( sKey, "+" );

  if ( next_tok == 0L ) return 0;

  do {
    toks[nb_toks] = next_tok;
    nb_toks++;
    if ( nb_toks == 5 ) return 0;
    next_tok = strtok( 0L, "+" );
  } while ( next_tok != 0L );

  // Test for exactly one key (other tokens are accelerators)
  // Fill the keycode with infos
  bool  keyFound = false;
  for ( int i=0; i<nb_toks; i++ ) {
    if (qstricmp(toks[i], "SHIFT") != 0 &&
        qstricmp(toks[i], "CTRL")  != 0 &&
        qstricmp(toks[i], "ALT")   != 0) {
      if ( keyFound ) return 0;
      keyFound = true;
      QCString l = toks[i];
      l = l.lower();
      keysym = XStringToKeysym(l.data());
      if (keysym == NoSymbol){
        keysym = XStringToKeysym( toks[i] );
      }
      if ( keysym == NoSymbol ) {
        return 0;
      }
    }
  }

  return keysym;
}

bool CLicqGui::grabKey(QString key)
{
  int keyCode = QAccel::stringToKey(key);

  if(!keyCode) return false;

  grabKeysym = keyCode;

  XGrabKey(qt_xdisplay(),
           XKeysymToKeycode(qt_xdisplay(), keyToXSym(keyCode)), keyToXMod(keyCode),
           qt_xrootwin(), True,
           GrabModeAsync, GrabModeSync);

  return true;
}


bool CLicqGui::x11EventFilter(XEvent *e)
{
//   QETWidget *widget = (QETWidget*)QWidget::find( (WId)e->xany.window );
//   printf("event %d %d %d\n", e->type, widget, e->xany.window);

  if ( e->type != KeyPress || !grabKeysym ) 
#ifdef USE_KDE
      return KApplication::x11EventFilter(e);
#else
      return QApplication::x11EventFilter(e);
#endif

  unsigned int mod = e->xkey.state & (ControlMask | ShiftMask | Mod1Mask);
  unsigned int keysym = XKeycodeToKeysym(qt_xdisplay(), e->xkey.keycode, 0);

  if(keysym == keyToXSym(grabKeysym) && mod == keyToXMod(grabKeysym))
    licqMainWindow->callMsgFunction();

  if ( !QWidget::keyboardGrabber() ) {
    XAllowEvents(qt_xdisplay(), AsyncKeyboard, CurrentTime);
    XUngrabKeyboard(qt_xdisplay(), CurrentTime);
    XSync(qt_xdisplay(), false);
  }

  return QApplication::x11EventFilter(e);
}
