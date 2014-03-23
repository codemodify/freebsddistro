
// Qt headers
#include <QtGui/QApplication>
#include <QtGui/QColor.h>
#include <QtGui/QBitmap.h>
#include <QtGui/QPalette.h>
#include <QtGui/QPainter.h>
#include <QtGui/QCursor.h>
#include <QtGui/QFileDialog.h>
#include <QtGui/QMenu.h>

// X11 headers
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

// local headers
#include "dat.h"
#include "fns.h"
#include "devel.h"
#include "client.h"
#include "main.h"
#include "options.h"

Client      *clients;
Client      *current;

extern int ABUTTON;
extern int BBUTTON;
extern int CBUTTON;
extern int DBUTTON;
extern int EBUTTON;
extern int FBUTTON;

extern QMenu *operations;
extern wmMenuWidget *m;

void active(Client *c){
    Client *cc;

    if (c == 0) {
        fprintf(stderr, "kwm: active(c==0)\n");
        return;
    }
    if (c == current){
      return;
    }
    if (current)
        current->setactive( FALSE );
    c->setactive( TRUE );
    for (cc = clients; cc; cc = cc->next)
        if (cc->revert == c)
            cc->revert = c->revert;
    
    c->revert = current;
    while (c->revert && !normalstate(c->revert))
        c->revert = c->revert->revert;
    current = c;
}

void nofocus() {
fprintf(stderr,"nofocus\n");
    static Window w = 0;
    int mask;
    XSetWindowAttributes attr;
    Client *c;
    if (current) {
        current->setactive( FALSE );
        for (c = current->revert; c; c = c->revert)
            if (normalstate(c)) {
                active(c);
                return;
            }
        /* if no candidates to revert to, fall through */
    }
    current = 0;
    if (w == 0) {
        mask = CWOverrideRedirect;
        attr.override_redirect = 1;
        w = XCreateWindow(dpy, root, 0, 0, 1, 1, 0, CopyFromParent,
            InputOnly, CopyFromParent, mask, &attr);
        XMapWindow(dpy, w);
    }
    XSetInputFocus(dpy, w, RevertToPointerRoot, timestamp());
    cmapfocus(0);
}


Client * getclient(Window w, int create) {
    Client *c;

    if (w == 0 || w == root)
        return 0;

    for (c = clients; c; c = c->next)
        if (c->window == w || c->parent_window == w)
            return c;

    if (!create)
        return 0;

    c = new Client;
    c->window = w;
    c->parent_window = c->winId(); 
//    m->addMenuWin(c);    // I'm moving this to manage, to remove BS entries

    // overwrite Qt-defaults because we need  SubstructureNotifyMask (Matthias)
    XSelectInput(dpy, c->parent_window, 
  		 KeyPressMask | KeyReleaseMask |
  		 ButtonPressMask | ButtonReleaseMask |
  		 KeymapStateMask |
  		 ButtonMotionMask |
  		 EnterWindowMask | LeaveWindowMask |
  		 FocusChangeMask |
  		 ExposureMask |
  		 StructureNotifyMask | 
  		 SubstructureRedirectMask |
		 SubstructureNotifyMask 
  		 );


    c->next = clients;
    clients = c;
    return c;
}

void rmclient(Client *c){
    Client *cc;
    int i;

    if (!c) return;
    
    c->craphack(c->id);
    for (cc = current; cc && cc->revert; cc = cc->revert)
        if (cc->revert == c)
            cc->revert = cc->revert->revert;

    if (c == clients)
        clients = c->next;
    for (cc = clients; cc && cc->next; cc = cc->next)
        if (cc->next == c)
            cc->next = cc->next->next;

    if (hidden(c)){
        unhidec(c, 0);
    }

    c->parent_window = c->window = None;       /* paranoia */

    if (current == c) {
        current = c->revert;
        if (current == 0){
	  nofocus();
	}
        else{
          current->setactive( TRUE );
	}
    }
    if (c->ncmapwins != 0) {
        XFree((char *)c->cmapwins);
        free((char *)c->wmcmaps);
    }
    if (c->iconname != 0)
        XFree((char*) c->iconname);
    if (c->name != 0)
        XFree((char*) c->name);
    if (c->instance != 0)
        XFree((char*) c->instance);
    if (c->klass != 0)
        XFree((char*) c->klass);
     delete (c);
}


void Client::craphack( int i){
   fprintf(stderr,"Got remove client\n");
   emit clientClosed( i );
}

Client::Client( QWidget *parent, const char *name_for_qt )
  : QFrame( parent, name_for_qt){
    parent_window = root;
    already_managed = FALSE;
    already_got_create_event = FALSE;
    reparenting = 0;
    state = WithdrawnState;
    init = 0;
    cmap = None;
    label = klass = instance = name = iconname = 0;
    revert = 0;
    ncmapwins = 0;
    cmapwins = 0;
    wmcmaps = 0;
    is_active = FALSE;

    vd = currentVD;
    id = 0;
    sticky = 0;
    
    setFrameStyle( QFrame::WinPanel | QFrame::Raised );

    buttonA = getNewButton( ABUTTON );
    buttonB = getNewButton( BBUTTON );
    buttonC = getNewButton( CBUTTON );
    buttonD = getNewButton( DBUTTON );
    buttonE = getNewButton( EBUTTON );
    buttonF = getNewButton( FBUTTON );
    
    rightResizeFrame = new ResizeFrame(this,"rightresize"); 
    rightResizeFrame->setCursor(sizeFDiagCursor);
    
    connect (m, SIGNAL(switchVD()), SLOT(switchVD()));
    
}  /* end Client::Client */

QPushButton * Client::getNewButton(int buttonFunction){

  QString s = getenv("KDEDIR");
  s += "/lib/pics/";
  
  if(buttonFunction == NOFUNC)
    return 0;
  else{
    QPushButton *button = new QPushButton(this);
    switch(buttonFunction){
      case MAXIMIZE:
        s += "maximize.xbm";
        button->setPixmap(QBitmap::QBitmap(s));
        button->setToggleButton(TRUE);
        connect( button, SIGNAL(toggled(bool)), SLOT(maximize_toggled(bool)));
	break;
      case MINIMIZE:
        s += "minimize.xbm";
        button->setPixmap(QBitmap::QBitmap(s));
        connect( button, SIGNAL(clicked()), SLOT(minimize_clicked()));
	break;
      case CLOSE:
        s += "close.xbm";
        button->setPixmap(QBitmap::QBitmap(s));
        connect( button, SIGNAL(clicked()), SLOT(close_clicked()));
	break;
      case STICKY:
        s += "pinup.xbm";
        button->setPixmap(QBitmap::QBitmap(s));
        button->setToggleButton(TRUE);
        connect( button, SIGNAL(toggled(bool)), SLOT(pushpin_toggled(bool)));
	break;
      default:
        fprintf(stderr,"Bad Button Function %d\n", buttonFunction);
	break;
    }  /* end switch */
    
    return button;
    
  }  /* end else */
  
}  /* end Client::getNewButton */

void Client::setlabel(char* new_label){
  label = new_label;
  if (isVisible())                 
    setactive(is_active);
}


void Client::setactive(Bool on){
  static QColorGroup active_grp( black, black,
				white, lightGray.dark(), gray,
				white, white );
//  static QColorGroup active_grp( black, QColor(0, 0, 128),
//				white, lightGray.dark(), gray,
//				white, white );
  static QColorGroup inactive_grp( black, gray,
				white, lightGray.dark(), gray,
				black, white );
  is_active=on;
 
  if (is_active) {
    XSetInputFocus(dpy, window, RevertToPointerRoot, timestamp());
    if (proto & Ptakefocus)
      sendcmessage(window, wm_protocols, wm_take_focus);
    cmapfocus(this);
  }

 if (!trans){
 
   QRect r = title_rect;
   
   QPainter p;
   p.begin( this );
   if (is_active){
     p.setBackgroundColor(active_grp.background());
     p.eraseRect(r);
     qDrawWinPanel( &p, r, active_grp, TRUE );
     p.setPen(active_grp.text());
   }
   else {
     p.setBackgroundColor(inactive_grp.background());
     p.eraseRect(r);
     //qDrawShadeRect( &p, r, inactive_grp, TRUE, 1, 0);
     p.setPen(inactive_grp.text());
   }
   if (label){
     p.setFont(QFont("Helvetica", 12, QFont::Bold));
//     r.setLeft(r.left()+5);
     p.drawText(r, AlignCenter, label);
   }
   p.end();
 }
}

void Client::paintEvent( QPaintEvent *ev ){
  QFrame::paintEvent(ev);
  setactive(is_active);
}

void Client::mouseReleaseEvent( QMouseEvent *ev ){
  releaseMouse();
}

void Client::mousePressEvent( QMouseEvent *ev ){
  raise();
  old_cursor_pos = ev->pos();

  if (dragging_state == dragging_wait_for_click){
    if (ev->button() == LeftButton || ev->button() == MidButton){
      dragging_state = dragging_enabled;
      grabMouse(sizeAllCursor);
    }
    else {
      dragging_state = dragging_nope;
    }
    
    return;
  }

  if (ev->button() ==RightButton){
    operations->move(QCursor::pos());
    if (operations->pos().x() + 
	operations->width() > QApplication::desktop()->width())
      operations->move(operations->pos().x() - operations->width(),
		       operations->pos().y());
    if (operations->pos().y() + 
	operations->height() > QApplication::desktop()->height())
      operations->move(operations->pos().x(),
		       operations->pos().y() - operations->height());
    
  
    operations->show();
    return;
  }

  dragging_state = dragging_nope;

  if (ev->button() != LeftButton){     // i.e. the middle button
    XLowerWindow(dpy, this->parent_window);                                   
    sendconfig(this);
    XMapWindow(dpy, this->window);
    nofocus();
    return;
  }
  
  if (!trans){

// check wether the mini-icon was hit
//  if(buttonGroup != XONLEFTBUTTONS){
//    if (ev->pos().x() >= miniicon_rect.x() && ev->pos().x() <=miniicon_rect.x()+miniicon_rect.width() &&
//	ev->pos().y() >= miniicon_rect.y() && ev->pos().y() <= miniicon_rect.y()+miniicon_rect.height()){
//      operations->move( QCursor::pos().x(),QCursor::pos().y() );
//      operations->show();
//      return;
//    }
//  }  
    
    // check wether the titlebar was hit
    
    // This is buggy.... why???      (Matthias)
    //  if (title_rect.contains(ev->pos()))  
    
    if (ev->pos().x() >= title_rect.x() && ev->pos().x() <= title_rect.x()+title_rect.width() &&
	ev->pos().y() >= title_rect.y() && ev->pos().y() <= title_rect.y()+title_rect.height()){
      dragging_state = dragging_smooth_wait;
      return;
    }  /* end if titlebar hit */

//     fprintf(stderr,"frame dx %d dy %d, ev x %d y %d\n",dx,dy,ev->pos().x(),ev->pos().y());
     
  }  /* end if !trans */
  
}  /* end Client::mousePressEvent */


void Client::mouseMoveEvent( QMouseEvent *ev ){


if (dragging_state != dragging_smooth_wait
      && dragging_state != dragging_enabled)
    return;


  // this is for a smoother start of the  drag
  if (dragging_state == dragging_smooth_wait){
    if (
	(ev->pos().x() - old_cursor_pos.x())
	*
	(ev->pos().x() - old_cursor_pos.x())
	+
	(ev->pos().y() - old_cursor_pos.y())
	*
	(ev->pos().y() - old_cursor_pos.y())
	< 9)
      return;
    else{
      if(WindowMoveType==TRANSPARENT){
       dragging_state = dragging_nope;
       sweepdrag(this,0,dragcalc);
       move(x,y);
       sendconfig(this);
       return;
      }
      else{
       dragging_state = dragging_enabled;
       grabMouse( sizeAllCursor );
      }
    }
  }
 

  if (!trans){
    x = QCursor::pos().x() - old_cursor_pos.x();
    y = QCursor::pos().y() - old_cursor_pos.y();


    move(x , y);           // these arguments blow my mind
    sendconfig( this );

  }
  else{
    x = QCursor::pos().x() - old_cursor_pos.x();
    y = QCursor::pos().y() - old_cursor_pos.y();

    move(x , y);
    sendconfig( this );

  }
}


void Client::resizeEvent( QResizeEvent * ){

  if(!trans){
    int trX = BORDER;
    int trY = BORDER;
    int trW = width() - 2 * BORDER;
    int trH = TITLEBAR_HEIGHT - TITLEWINDOW_SEPARATION;
    
    if( ABUTTON > -1 ){
      trX += BUTTON_SIZE;
      trW -= BUTTON_SIZE;
      buttonA->setGeometry(BORDER, 
	BORDER + (TITLEBAR_HEIGHT - TITLEWINDOW_SEPARATION - BUTTON_SIZE)/ 2, 
	BUTTON_SIZE, BUTTON_SIZE);
    }
    if( BBUTTON > -1 ){
      trX += BUTTON_SIZE;
      trW -= BUTTON_SIZE;
      buttonB->setGeometry(BORDER + BUTTON_SIZE, 
	BORDER + (TITLEBAR_HEIGHT - TITLEWINDOW_SEPARATION - BUTTON_SIZE)/ 2, 
	BUTTON_SIZE, BUTTON_SIZE);
    }
    if( CBUTTON > -1 ){
      trX += BUTTON_SIZE;
      trW -= BUTTON_SIZE;
      buttonC->setGeometry(BORDER + 2*BUTTON_SIZE, 
	BORDER + (TITLEBAR_HEIGHT - TITLEWINDOW_SEPARATION - BUTTON_SIZE)/ 2, 
	BUTTON_SIZE, BUTTON_SIZE);
    }
    if( DBUTTON > -1 ){
      trW -= BUTTON_SIZE;
      buttonD->setGeometry(width() - BORDER - 3*BUTTON_SIZE, 
	BORDER + (TITLEBAR_HEIGHT - TITLEWINDOW_SEPARATION - BUTTON_SIZE)/ 2, 
	BUTTON_SIZE, BUTTON_SIZE);
    }
    if( EBUTTON > -1 ){
      trW -= BUTTON_SIZE;
      buttonE->setGeometry(width() - BORDER - 2*BUTTON_SIZE, 
	BORDER + (TITLEBAR_HEIGHT - TITLEWINDOW_SEPARATION - BUTTON_SIZE)/ 2, 
	BUTTON_SIZE, BUTTON_SIZE);
    }
    if( FBUTTON > -1 ){
      trW -= BUTTON_SIZE;
      buttonF->setGeometry(width() - BORDER - BUTTON_SIZE, 
	BORDER + (TITLEBAR_HEIGHT - TITLEWINDOW_SEPARATION - BUTTON_SIZE)/ 2, 
	BUTTON_SIZE, BUTTON_SIZE);
    }
    
    title_rect.setRect(trX,trY,trW,trH);
    
    rightResizeFrame->setGeometry(width()-20,height()-20,20,20);

  }  /* end if !trans */
  
}  /* end Client::resizeEvent */

void Client::minimize_clicked(){
  hidec(this);      // hide the window
  hide();           // hide the frame
  setwindowstate(this, IconicState);
  nofocus();
  vd = -1;                  // untranslate Virtual Desktop         
}

void Client::unMinimize(){

  if ((vd != -1) && (vd != currentVD) && (!sticky)) { 
    // It's really on another VD, not minimized
    m->vdRequest(vd);
  }
  show();                       // unhide the window
  setwindowstate(this, NormalState);
  XRaiseWindow(dpy, this->parent_window);                                   
  sendconfig(this);
  //      XMapRaised(dpy, this->parent);
  XMapWindow(dpy, this->window);
  vd = currentVD;               // unMinimizing translates Virtual Desktop
}


void Client::maximize_toggled(bool depressed){
  raise();
  // maximize / unmaximize 
  if ( depressed){          // Maximize
    x2 = x;
    y2 = y;
    dx2 = dx;
    dy2 = dy;
    
    if (!trans){
      x = 0;
      y = 0;
      dx = QApplication::desktop()->width();
      dy = QApplication::desktop()->height();
    }else{
      x = 0;
      y = 0;
      dx = QApplication::desktop()->width();
      dy = QApplication::desktop()->height();
    }
  }
  else {                       // unMaximize
    x = x2;
    y = y2;
    dx = dx2;
    dy = dy2;
  }
  if (!trans){
    setGeometry(x , y ,
		dx , dy );
    XMoveResizeWindow(dpy, window, BORDER, BORDER + TITLEBAR_HEIGHT,
		      dx - 2*BORDER, dy - 2*BORDER - TITLEBAR_HEIGHT );
  }
  else{
    setGeometry(x , y ,
		dx , dy );
    XMoveResizeWindow(dpy, window, BORDER, BORDER,
		      dx - 2*BORDER, dy - 2*BORDER );
  }
}

void Client::close_clicked(){
    XEvent ev;
    int status;
    long mask;

    memset(&ev, 0, sizeof(ev));
    ev.xclient.type = ClientMessage;
    ev.xclient.window = window;
    ev.xclient.message_type = XInternAtom(dpy, "WM_PROTOCOLS", False);
    ev.xclient.format = 32;
    ev.xclient.data.l[0] = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
    ev.xclient.data.l[1] = timestamp();
    mask = 0L;
    if (window == root)
        mask = SubstructureRedirectMask;        /* magic! */
    status = XSendEvent(dpy, window, False, mask, &ev);
    if (status == 0)
        fprintf(stderr, "kwm: send delete window failed\n");
//    rmclient(this);  // Hey Me, take  another look here
    nofocus();
} 

void Client::pushpin_toggled(bool depressed){

  QString pu = getenv("KDEDIR");
  QString pd = pu + "/lib/pics/pindown.xbm";
  pu += "/lib/pics/pinup.xbm";
  
  if (depressed){
    vd = 0; sticky = 1;
    if( ABUTTON == STICKY )
      buttonA->setPixmap(QBitmap::QBitmap(pd));
    if( BBUTTON == STICKY )
      buttonB->setPixmap(QBitmap::QBitmap(pd));
    if( CBUTTON == STICKY )
      buttonC->setPixmap(QBitmap::QBitmap(pd));
    if( DBUTTON == STICKY )
      buttonD->setPixmap(QBitmap::QBitmap(pd));
    if( EBUTTON == STICKY )
      buttonE->setPixmap(QBitmap::QBitmap(pd));
    if( FBUTTON == STICKY )
      buttonF->setPixmap(QBitmap::QBitmap(pd));
  }
  else{
    vd = currentVD;
    sticky = 0;
    if( ABUTTON == STICKY )
      buttonA->setPixmap(QBitmap::QBitmap(pu));
    if( BBUTTON == STICKY )
      buttonB->setPixmap(QBitmap::QBitmap(pu));
    if( CBUTTON == STICKY )
      buttonC->setPixmap(QBitmap::QBitmap(pu));
    if( DBUTTON == STICKY )
      buttonD->setPixmap(QBitmap::QBitmap(pu));
    if( EBUTTON == STICKY )
      buttonE->setPixmap(QBitmap::QBitmap(pu));
    if( FBUTTON == STICKY )
      buttonF->setPixmap(QBitmap::QBitmap(pu));
  }

}  /* end Client:: pushpin_toggled() */

void Client::sorry(){
  printf("Sorry, not yet implemented.\n");
}

void Client::switchVD(){
  
  if ( !vd || sticky ) return;  /* sticky windows don't switch */
  if ( !id ) return;  /* unmanaged, unidentified windows don't switch */

  if (vd == currentVD){
//    unhide();           // unhide the frame
    show();           // unhide the window
    sendconfig(this);
    XMapWindow(dpy, this->window);
  }
  else {
    hidec(this);
    hide();
  }
  
}  /* end Client::switchVD */

void Client::  handle_operations_popup(int i){
  switch (i){
  case OP_MOVE:
    grabMouse( crossCursor );
    dragging_state = dragging_wait_for_click;
    break;
  case OP_RESHAPE:
    reshape( this );
    break;
  case OP_MAXIMIZE:
    break;
  case OP_ICONIFY:
    minimize_clicked();
    break;
  case OP_DESTROY:
    XDestroyWindow(dpy, window);
    rmclient( this );
    break;
  }
}

