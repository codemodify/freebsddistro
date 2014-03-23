
// Qt headers
#include <QtGui/QPushButton.h>
#include <QtGui/QBitmap.h>
#include <QtGui/QFileDialog.h>
#include <QtGui/QMessageBox.h>
#include <QtGui/QDialog.h>
#include <QtCore/QString.h>
#include <QtGui/QWidget.h>
#include <QtGui/QMenuBar.h>

// X11 headers
#include <X11/X.h>
#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/shape.h>


extern void initkwm(int , char**);

#include "manage/manage.h"
#include "client/client.h"
#include "menuwidget/menuwidget.h"
#include "main.h"

// Global development/user options.
int WindowMoveType;
int buttonGroup;
int buttonSpacing;
int BUTTON_SEPARATION;
int TITLEBUTTON_SEPARATION;
int TITLEWINDOW_SEPARATION;

extern int ABUTTON;
extern int BBUTTON;
extern int CBUTTON;
extern int DBUTTON;
extern int EBUTTON;
extern int FBUTTON;

extern int shape_event;

QPalette *title_palette = NULL;
QPixmap *default_miniicon=NULL;
QBitmap *bm_minimize;
QBitmap *bm_maximize1;
QBitmap *bm_maximize2;
QBitmap *bm_maximize3;
QBitmap *bm_close1;
QBitmap *bm_close2;
QBitmap *bm_close3;
QBitmap *bm_close4;
QBitmap *bm_pinup;         
QBitmap *bm_pindown;         

QPopupMenu *operations;

int currentVD;

void open() {
  QFileDialog::getOpenFileName();
}


wmMenuWidget *m;


MyApp::MyApp(int &argc = 0, char **argv = 0, const QString& rAppName = 0):KApplication(argc, argv, rAppName ){

QString key;

  // Get KConfigs and set defaults if need be
  
  config = getKApplication()->getConfig();
  config->setGroup( "General" );
  key = config->readEntry("WindowMoveType");
  if( key == "Transparent")
    WindowMoveType = TRANSPARENT;
  else if( key == "Opaque")
    WindowMoveType = OPAQUE;
  else{
    config->writeEntry("WindowMoveType","Transparent");
    WindowMoveType = TRANSPARENT;
  }
  
  config->setGroup( "Titlebar");
  
  key = config->readEntry("ButtonA");
  if( key == "Off" )
    ABUTTON = NOFUNC;
  else if( key == "Maximize" )
    ABUTTON = MAXIMIZE;
  else if( key == "Minimize" )
    ABUTTON = MINIMIZE;
  else if( key == "Close" )
    ABUTTON = CLOSE;
  else if( key == "Sticky" )
    ABUTTON = STICKY;
  else{
    config->writeEntry("ButtonA","Close");
    ABUTTON = CLOSE;
  }
  key = config->readEntry("ButtonB");
  if( key == "Off" )
    BBUTTON = NOFUNC;
  else if( key == "Maximize" )
    BBUTTON = MAXIMIZE;
  else if( key == "Minimize" )
    BBUTTON = MINIMIZE;
  else if( key == "Close" )
    BBUTTON = CLOSE;
  else if( key == "Sticky" )
    BBUTTON = STICKY;
  else{
    config->writeEntry("ButtonB","Sticky");
    BBUTTON = STICKY;
  }
  key = config->readEntry("ButtonC");
  if( key == "Off" )
    CBUTTON = NOFUNC;
  else if( key == "Maximize" )
    CBUTTON = MAXIMIZE;
  else if( key == "Minimize" )
    CBUTTON = MINIMIZE;
  else if( key == "Close" )
    CBUTTON = CLOSE;
  else if( key == "Sticky" )
    CBUTTON = STICKY;
  else{
    config->writeEntry("ButtonC","Off");
    CBUTTON = NOFUNC;
  }
  key = config->readEntry("ButtonD");
  if( key == "Off" )
    DBUTTON = NOFUNC;
  else if( key == "Maximize" )
    DBUTTON = MAXIMIZE;
  else if( key == "Minimize" )
    DBUTTON = MINIMIZE;
  else if( key == "Close" )
    DBUTTON = CLOSE;
  else if( key == "Sticky" )
    DBUTTON = STICKY;
  else{
    config->writeEntry("ButtonD","Off");
    DBUTTON = NOFUNC;
  }
  key = config->readEntry("ButtonE");
  if( key == "Off" )
    EBUTTON = NOFUNC;
  else if( key == "Maximize" )
    EBUTTON = MAXIMIZE;
  else if( key == "Minimize" )
    EBUTTON = MINIMIZE;
  else if( key == "Close" )
    EBUTTON = CLOSE;
  else if( key == "Sticky" )
    EBUTTON = STICKY;
  else{
    config->writeEntry("ButtonE","Minimize");
    EBUTTON = MINIMIZE;
  }
  key = config->readEntry("ButtonF");
  if( key == "Off" )
    FBUTTON = NOFUNC;
  else if( key == "Maximize" )
    FBUTTON = MAXIMIZE;
  else if( key == "Minimize" )
    FBUTTON = MINIMIZE;
  else if( key == "Close" )
    FBUTTON = CLOSE;
  else if( key == "Sticky" )
    FBUTTON = STICKY;
  else{
    config->writeEntry("ButtonF","Maximize");
    FBUTTON = MAXIMIZE;
  }
  
  
  // Here's a (colored) desktop widget
  QWidget *d = QApplication::desktop();
  d->setBackgroundColor( QColor(43,110,128) );

  m = new wmMenuWidget();
  setMainWidget(m);
  m->show();

}

void MyApp::saveOptions(){
// This workaround is mainly because of the sync() bug
// But I may leave it in with modifications because it
// provides a little more control

  if(config)
    delete config;
  config = getKApplication()->getConfig();  
}

void MyApp::quit(){
printf("MyApp::quit\n");
exit(0);
}

bool MyApp::x11EventFilter( XEvent * ev){
  //  printf("Qt: got one event %d \n", ev->type);


  switch (ev->type) {
  case ButtonPress:
    break;
  case ButtonRelease:
    break;
  case MapRequest:
    mapreq(&ev->xmaprequest);
    break;
  case ConfigureRequest:
    configurereq(&ev->xconfigurerequest);
    break;
  case CirculateRequest:
    circulatereq(&ev->xcirculaterequest);
    break;
  case UnmapNotify:
     unmap(&ev->xunmap);
    break;
  case CreateNotify:
    newwindow(&ev->xcreatewindow);
    break;
  case DestroyNotify:
    destroywin(ev->xdestroywindow.window);
    break;
  case ClientMessage:
    clientmesg(&ev->xclient);
    break;
  case ColormapNotify:
    cmap(&ev->xcolormap);
    break;
  case PropertyNotify:
    property(&ev->xproperty);
    break;
  case SelectionClear:
    fprintf(stderr, "kwm: SelectionClear (this should not happen)\n");
    break;
  case SelectionNotify:
    fprintf(stderr, "kwm: SelectionNotify (this should not happen)\n");
    break;
  case SelectionRequest:
    fprintf(stderr, "kwm: SelectionRequest (this should not happen)\n");
    break;
  case EnterNotify:
    if (!current || current != current->mouseGrabber())
      enter(&ev->xcrossing); 
    break;
  case ReparentNotify:
     reparent(&ev->xreparent);
    break;
  case MotionNotify:
    break;
  case ConfigureNotify:
    // this is because Qt cannot handle (usually does not need to)
    // SubstructureNotify events. (Matthias)
    if (ev->xconfigure.window != ev->xconfigure.event){
      return TRUE;
    }
    break;
  case MapNotify:
  case Expose:
  case FocusIn:
  case FocusOut:
  case MappingNotify:
    /* not interested */
    break;
  default:
#ifdef  SHAPE
    if (shape && ev->type == shape_event)
      shapenotify((XShapeEvent *)ev);
    else
#endif
      //fprintf(stderr, "kwm: unknown ev->type %d\n", ev->type);
    break;
  }

  return FALSE;
}

extern Display *dpy;

#include "client.moc"
#include "main.moc"
#include "options.moc"

void initdevel(){
  BUTTON_SEPARATION = 0;
  TITLEBUTTON_SEPARATION = 2;
  TITLEWINDOW_SEPARATION = 1;

  currentVD=1;
  
}  /* end initdevel() */

