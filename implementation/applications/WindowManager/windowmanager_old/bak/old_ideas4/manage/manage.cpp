
// Qt headers
#include <QtGui/QPushButton.h>
#include <QtGui/QCursor>

// X11 headers
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/shape.h>
#include <X11/Xos.h>

// local headers
#include "manage.h"
#include "dat.h"
#include "client/client.h"
#include "main.h"


int     ignore_badwindow;

void fatal(char *s){
fprintf(stderr, "kwm: ");
     perror(s);
     fprintf(stderr, "\n");
     exit(1);
 }
 
 int handler(Display *d, XErrorEvent *e){
     char msg[80], req[80], number[80];
 
     if (initting && (e->request_code == X_ChangeWindowAttributes) && (e->error_code == BadAccess)) {
         fprintf(stderr, "kwm: it looks like there's already a window manager running;  kwm not started\n");
         exit(1);
     }
 
     if (ignore_badwindow && (e->error_code == BadWindow || e->error_code == BadColor))
         return 0;
 
     XGetErrorText(d, e->error_code, msg, sizeof(msg));
     sprintf(number, "%d", e->request_code);
     XGetErrorDatabaseText(d, "XRequest", number, "<unknown>", req, sizeof(req));

     fprintf(stderr, "kwm: %s(0x%x): %s\n", req, e->resourceid, msg);
 
     if (initting) {
         fprintf(stderr, "kwm: failure during initialisation; aborting\n");
         exit(1);
     }
     return 0;
 }


extern wmMenuWidget *m;


int manage(Client*  c, int mapped){
    int fixsize, dohide, doreshape, state, n, order;
    long msize;
    XClassHint klass;
    XWMHints *hints;



#ifdef SHAPE
       /* fall out if window is non-rectangular */
       XShapeGetRectangles(dpy, c->window, ShapeBounding, &n, &order);
       if ( n > 1 ) return 1;
#endif

if (c->already_managed) {
 fprintf(stderr,"Redundant managing\n");
} 

    m->addMenuWin(c);           // add this to the windowlist

    c->already_managed = TRUE;
    
    XSelectInput(dpy, c->window, ColormapChangeMask | EnterWindowMask | PropertyChangeMask);

    /* Get loads of hints */

    if (XGetClassHint(dpy, c->window, &klass) != 0) {   /* ``Success'' */
        c->instance = klass.res_name;
        c->klass = klass.res_class;
    }
    else {
        c->instance = 0;
        c->klass = 0;
    }
    c->iconname = getprop(c->window, XA_WM_ICON_NAME);
    c->name = getprop(c->window, XA_WM_NAME);
    fprintf(stderr,"Managing %s\n",c->name);
    setwindowlabel(c);

    hints = XGetWMHints(dpy, c->window);
    if (XGetWMNormalHints(dpy, c->window, &c->size, &msize) == 0 || c->size.flags == 0)
        c->size.flags = PSize;      /* not specified - punt */

    getcmaps(c);
    getwindowproto(c);
    getwindowtrans(c);


    if(c->trans){
      if(c->buttonA) c->buttonA->hide();
      if(c->buttonB) c->buttonB->hide();
      if(c->buttonC) c->buttonC->hide();
      if(c->buttonD) c->buttonD->hide();
      if(c->buttonE) c->buttonE->hide();
      if(c->buttonF) c->buttonF->hide();
      if(c->rightResizeFrame) c->rightResizeFrame->hide();
    }

    // readjust dx and dy so they are Client sizes
    c->dx += 2*BORDER;
    if(!c->trans)
      c->dy += 2*BORDER + TITLEBAR_HEIGHT;
    else
      c->dy += 2*BORDER;
    c->setGeometry(c->x,c->y,c->dx,c->dy);

    if (!getwindowstate(c->window, &state))
        state = hints ? hints->initial_state : NormalState;
    dohide = (state == IconicState);

    fixsize = 0;
    if ((c->size.flags & (USSize|PSize)))
        fixsize = 1;
    if ((c->size.flags & (PMinSize|PMaxSize)) == PMinSize|PMaxSize && c->size.min_width == c->size.max_width && c->size.min_height == c->size.max_height)
        fixsize = 1;
    doreshape = !mapped;
    if (fixsize) {
        if (c->size.flags & USPosition)
            doreshape = 0;
        if (dohide && (c->size.flags & PPosition))
            doreshape = 0;
        if (c->trans != None)
            doreshape = 0;
    }
    if (c->size.flags & PBaseSize) {
        c->min_dx = c->size.base_width;
        c->min_dy = c->size.base_height;
    }
    else if (c->size.flags & PMinSize) {
        c->min_dx = c->size.min_width;
        c->min_dy = c->size.min_height;
    }
    else
        c->min_dx = c->min_dy = 0;

    if (hints)
        XFree(hints);

    /* Now do it!!! */

    if (doreshape) {
      cmapfocus(0);
      if (c->x == 0 && c->y == 0)
	drag(c);
      else{
	sendconfig( c );
      }
    }
    else {
      gravitate(c, 0);
    }
    
      c->setGeometry(c->x , c->y ,
		     c->dx , c->dy );

    if (mapped)
        c->reparenting = 1;
    if (doreshape && !fixsize){
       if(!c->trans)
         XResizeWindow(dpy, c->window, c->dx - 2*BORDER, c->dy - 2*BORDER - TITLEBAR_HEIGHT);
       else
         XResizeWindow(dpy, c->window, c->dx - 2*BORDER, c->dy - 2*BORDER);
    }
    XSetWindowBorderWidth(dpy, c->window, 0);
    if (c->trans)
      XReparentWindow(dpy, c->window, c->parent_window, (BORDER), (BORDER));
    else
      XReparentWindow(dpy, c->window, c->parent_window, (BORDER), (BORDER) + 
		      TITLEBAR_HEIGHT);


#ifdef  SHAPE
    if (shape) {
        XShapeSelectInput(dpy, c->window, ShapeNotifyMask);
        ignore_badwindow = 1;       /* magic */
        setshape(c);
        ignore_badwindow = 0;
    }
#endif
    XAddToSaveSet(dpy, c->window);
    if (dohide)
        hidec(c);
    else {
      XMapWindow(dpy, c->window);
      // XMapWindow(dpy, c->parent_window);
      c->show();
      // c->repaint();
       // The following lines may be important for click-to-focus (Matthias)
//         if (c->trans != None && current && current->window == c->trans)
// 	  active(c);
//         else
// 	  setactive(c, 0);
        setwindowstate(c, NormalState);
    }
    if (current != c)
      cmapfocus(current);
    
    c->init = 1; 
    
  XRaiseWindow(dpy, c->parent_window);
  XMapWindow(dpy, c->window);
	  
    return 1;
}

void getwindowtrans(Client * c)
{
    Window trans;

    trans = None;
    if (XGetTransientForHint(dpy, c->window, &trans) != 0)
        c->trans = trans;
    else
        c->trans = None;
}

void withdraw(Client *c)
{
fprintf(stderr,"withdraw\n");
// Doing all this is probably silly, since I'll probably just
// leave the rmclient(c) at the bottom.  This keeps blank frames
// from popping up, plus that it eliminates some annoying nutscrape
// behavior.

    XUnmapWindow(dpy, c->parent_window);
    gravitate(c, 1);
    if(!c->trans)
      XReparentWindow(dpy, c->window, root, c->x - 2*BORDER, c->y - 2*BORDER - TITLEBAR_HEIGHT);
    else
      XReparentWindow(dpy, c->window, root, c->x - 2*BORDER, c->y - 2*BORDER);
      
    gravitate(c, 0);
    XRemoveFromSaveSet(dpy, c->window);
    setwindowstate(c, WithdrawnState);

    /* flush any errors */
    ignore_badwindow = 1;
    XSync(dpy, False);
    ignore_badwindow = 0;
    
    // .... Hey Me! Look here!
    rmclient(c);
}

void gravitate(Client* c, int invert)
{

  // Do Nothing.  Who needs this crap?

}  /* end gravitate */

void cleanup()
{
    Client *c;
    XWindowChanges wc;

    for (c = clients; c; c = c->next) {
      gravitate(c, 1);
      XReparentWindow(dpy, c->window, root, c->x , c->y );
       
      wc.border_width = c->border;
      XConfigureWindow(dpy, c->window, CWBorderWidth, &wc);
    }
    XSetInputFocus(dpy, PointerRoot, RevertToPointerRoot, timestamp());
    cmapfocus(0);
    XCloseDisplay(dpy);
}

static void installcmap(Colormap cmap)
{
    XInstallColormap(dpy, (cmap == None) ? def_cmap : cmap);
}

void cmapfocus(Client *c)
{
    int i, found;
    Client *cc;

    if (c == 0)
        installcmap(None);
    else if (c->ncmapwins != 0) {
        found = 0;
        for (i = c->ncmapwins-1; i >= 0; i--) {
            installcmap(c->wmcmaps[i]);
            if (c->cmapwins[i] == c->window)
                found++;
        }
        if (!found)
            installcmap(c->cmap);
    }
    else if (c->trans != None && (cc = getclient(c->trans, 0)) != 0 && cc->ncmapwins != 0)
        cmapfocus(cc);
    else
        installcmap(c->cmap);
}

void getcmaps(Client *c)
{
    int n, i;
    Window *cw;
    XWindowAttributes attr;

    if (!c->init) {
        XGetWindowAttributes(dpy, c->window, &attr);
        c->cmap = attr.colormap;
    }

    n = _getprop(c->window, wm_colormaps, XA_WINDOW, 100L, (unsigned char **)&cw);
    if (c->ncmapwins != 0) {
        XFree((char *)c->cmapwins);
        free((char *)c->wmcmaps);
    }
    if (n <= 0) {
        c->ncmapwins = 0;
        return;
    }

    c->ncmapwins = n;
    c->cmapwins = cw;

    c->wmcmaps = (Colormap*)malloc(n*sizeof(Colormap));
    for (i = 0; i < n; i++) {
        if (cw[i] == c->window)
            c->wmcmaps[i] = c->cmap;
        else {
            XSelectInput(dpy, cw[i], ColormapChangeMask);
            XGetWindowAttributes(dpy, cw[i], &attr);
            c->wmcmaps[i] = attr.colormap;
        }
    }
}

void setwindowlabel(Client *c)
{

    if (getprop(c->window,XA_WM_NAME))
        c->setlabel(getprop(c->window,XA_WM_NAME));
    else if (c->iconname)
        c->setlabel( c->iconname);
    else if (c->name)
        c->setlabel  (c->name);
    else if (c->instance)
        c->setlabel (c->instance);
    else
        c->setlabel("???");
    
}

#ifdef  SHAPE
void setshape(Client* c)
{
    int n, order;
    XRectangle *rect;

    /* cheat: don't try to add a border if the window is non-rectangular */
    rect = XShapeGetRectangles(dpy, c->window, ShapeBounding, &n, &order);
    if (n > 1){
      if (!c->trans)
	XShapeCombineShape(dpy, c->parent_window, ShapeBounding, 
			   (BORDER), (BORDER) + TITLEBAR_HEIGHT,
			   c->window, ShapeBounding, ShapeSet);
      else
	XShapeCombineShape(dpy, c->parent_window, ShapeBounding, 
			   (BORDER), (BORDER),
			   c->window, ShapeBounding, ShapeSet);
    }
    XFree((void*)rect);
}
#endif

int _getprop(Window w, Atom a, Atom type, long len, unsigned char **p) {
    Atom real_type;
    int format;
    unsigned long n, extra;
    int status;

    status = XGetWindowProperty(dpy, w, a, 0L, len, False, type, &real_type, &format, &n, &extra, p);
    if (status != Success || *p == 0)
        return -1;
    if (n == 0)
        XFree((void*) *p);
    /* could check real_type, format, extra here... */
    return n;
}

char * getprop(Window w, Atom a) {
    unsigned char *p;
    int n;

    if (_getprop(w, a, XA_STRING, 100L, &p) <= 0)
        return 0;
    return (char *)p;
}

int
get1prop(Window w, Atom a, Atom type) {

    unsigned char *p, *x;

    if (_getprop(w, a, type, 1L, &p) <= 0)
        return 0;
    x = p;
    XFree((void*) p);
    return (int)x;
}

Window
getwprop(Window w, Atom a){
    return get1prop(w, a, XA_WINDOW);
}

int getiprop(Window w, Atom a){
    return get1prop(w, a, XA_INTEGER);
}

void setwindowstate(Client *c, int state){
    long data[2];

    data[0] = (long) state;
    data[1] = (long) None;

    c->state = state;
    XChangeProperty(dpy, c->window, wm_state, wm_state, 32,
        PropModeReplace, (unsigned char *)data, 2);
}

int getwindowstate(Window w, int* state) {
    long *p = 0;

    if (_getprop(w, wm_state, wm_state, 2L, (unsigned char**)&p) <= 0)
        return 0;

    *state = (int) *p;
    XFree((char *) p);
    return 1;
}

void getwindowproto(Client *c) {
    Atom *p;
    int i;
    long n;
    Window w;

    w = c->window;
    c->proto = 0;
    if ((n = _getprop(w, wm_protocols, XA_ATOM, 20L, (unsigned char**)&p)) <= 0)
        return;

    for (i = 0; i < n; i++)
        if (p[i] == wm_delete)
            c->proto |= Pdelete;
        else if (p[i] == wm_take_focus)
            c->proto |= Ptakefocus;

    XFree((char *) p);
}


void move(Client *c){
fprintf(stderr,"move()\n");
    if (c == 0)
        return;
    if (drag(c) == 0)
        return;
    active(c);
    XRaiseWindow(dpy, c->parent_window);
    XMoveWindow(dpy, c->parent_window, c->x, c->y);

    sendconfig(c);
}


void hidec(Client *c){
     XUnmapWindow(dpy, c->window);
     setwindowstate(c, IconicState);  /* Rethink this  (Brian) */
     if (c == current)
         nofocus();
}


void unhidec(Client *c, int map){

  // Do Nothing. Obsolete.
  
}

void renamec(Client *c, char* name){

  // Do Nothing.  It didn't do anything when I got it...

}

void reshape(Client *c){
  int odx, ody;
fprintf(stderr,"reshape\n");
    if (c == 0)
        return;
    odx = c->dx;
    ody = c->dy;
    if (sweep(c) == 0)
        return;
    active(c);
    XRaiseWindow(dpy, c->parent_window);
      c->setGeometry(c->x , c->y ,
		     c->dx , c->dy);
      
    
    if (c->dx == odx && c->dy == ody)
      sendconfig(c);
    else
      {
	if (c->trans != None) 
	  XMoveResizeWindow(dpy, c->window, (BORDER), (BORDER), c->dx - 2*BORDER, c->dy - 2*BORDER);
	else
	  XMoveResizeWindow(dpy, c->window, (BORDER), (BORDER) + TITLEBAR_HEIGHT,
			    c->dx - 2*BORDER, c->dy - 2*BORDER - TITLEBAR_HEIGHT);
      }
}


int
nobuttons(XButtonEvent *e){
	int state;

	state = (e->state & AllButtonMask);
	return (e->type == ButtonRelease) && (state & (state - 1)) == 0;
}


int grab(Window w, int mask, Cursor curs, int t){
     int status; 
      
      if (t == 0)
          t = timestamp();
      status = XGrabPointer(dpy, w, False, mask,
          GrabModeAsync, GrabModeAsync, None, curs, t);
      return status;
  }   
  
  void ungrab(XButtonEvent *e){
      XEvent ev;
      
      if (!nobuttons(e))
          for (;;) {
              XMaskEvent(dpy, ButtonMask | ButtonMotionMask, &ev);
              if (ev.type == MotionNotify) 
                  continue;
              e = &ev.xbutton;
              if (nobuttons(e))
                  break;
          }       
      XUngrabPointer(dpy, e->time);
      curtime = e->time;
  }
  void sweepcalc(Client *c, int x, int y){
      int dx, dy, min_dx, min_dy; //  sx, sy;
  
      dx = x - c->x;
      dy = y - c->y;
  //    sx = sy = 1;
      if (dx < 0) {
          x += dx;
          dx = -dx;
  //        sx = -1;
      }
      if (dy < 0) {
          y += dy;
          dy = -dy;
  //        sy = -1;
      }
  
      if (!c->trans){
       dx -= 2*BORDER;
        dy -= 2*BORDER+TITLEBAR_HEIGHT;
      }
      else{
        dx -= 2*BORDER;
        dy -= 2*BORDER;
      }
  
      if (dx < c->min_dx)
        dx = c->min_dx;
      if (dy < c->min_dy)
        dy = c->min_dy;
  
      if (c->size.flags & PResizeInc) {
          dx = c->min_dx + (dx-c->min_dx)/c->size.width_inc*c->size.width_inc;
         dy = c->min_dy + (dy-c->min_dy)/c->size.height_inc*c->size.height_inc;
      }
  
      if (c->size.flags & PMaxSize) {
          if (dx > c->size.max_width)
              dx = c->size.max_width;
          if (dy > c->size.max_height)
              dy = c->size.max_height;
      }
      if (!c->trans){
        c->dx = dx + 2*BORDER;
        c->dy = dy + 2*BORDER + TITLEBAR_HEIGHT;
      }
      else{
        c->dx = dx + 2*BORDER;
        c->dy = dx + 2*BORDER;
      }
  }  /* end sweepcalc */
  void resizecalc(Client *c, int x, int y){
      int dx, dy, min_dx, min_dy; //  sx, sy;
  
     dx = c->dx2 + x - c->x;
      dy = c->dy2 + y - c->y;
  //    sx = sy = 1;
      if (dx < 0) {
          x += dx;
          dx = -dx;
 //        sx = -1;
}
     if (dy < 0) {
         y += dy;
         dy = -dy;
//        sy = -1;
     }
 
     if (!c->trans){
       dx -= 2*BORDER;
       dy -= 2*BORDER+TITLEBAR_HEIGHT;
     }
     else{
       dx -= 2*BORDER;
       dy -= 2*BORDER;
     }
 
     if (dx < c->min_dx)
      dx = c->min_dx;
     if (dy < c->min_dy)
       dy = c->min_dy;
 
     if (c->size.flags & PResizeInc) {
         dx = c->min_dx + (dx-c->min_dx)/c->size.width_inc*c->size.width_inc;
         dy = c->min_dy + (dy-c->min_dy)/c->size.height_inc*c->size.height_inc;
     }
 
     if (c->size.flags & PMaxSize) {
         if (dx > c->size.max_width)
             dx = c->size.max_width;
         if (dy > c->size.max_height)
             dy = c->size.max_height;
     }
     if (!c->trans){
       c->dx = dx + 2*BORDER;
       c->dy = dy + 2*BORDER + TITLEBAR_HEIGHT;
     }
     else{
       c->dx = dx + 2*BORDER;
       c->dy = dx + 2*BORDER;
     }
 }  /* end resizecalc */

 void dragcalc(Client* c, int x, int y) { 
    c->x = x;
     c->y = y;
 }
 
 void drawbound(Client* c){
     int x, y, dx, dy;
 
     x = c->x;
     y = c->y;
    dx = c->dx;
    dy = c->dy;
     if (dx < 0) {
         x += dx;
         dx = -dx;
     }
     if (dy < 0) {
         y += dy;
         dy = -dy;
     }
     if (dx <= 2 || dy <= 2)
         return;
     XDrawRectangle(dpy, root, gc, x, y, dx-1, dy-1);
     XDrawRectangle(dpy, root, gc, x+1, y+1, dx-3, dy-3);
 }

 int sweepdrag(Client* c, XButtonEvent *e0,
               void (*recalc)( Client *, int, int) ){
               
     XEvent ev;
     int status;
     int cx, cy, rx, ry;
     int ox, oy, odx, ody;
     struct timeval t;
    XButtonEvent *e;
 
     getmouse(&cx, &cy);
     if (e0) {
         c->x = cx = e0->x;
         c->y = cy = e0->y;
         recalc(c, cx, cy);
     }
     ox = c->x;
     oy = c->y;
     odx = c->dx;
    ody = c->dy;
 //    if (!c->trans){
 //      c->x -= BORDER;
 //      c->y -= BORDER+TITLEBAR_HEIGHT;
 //      c->dx += 2*BORDER;
//      c->dy += 2*BORDER+TITLEBAR_HEIGHT;
 //    }
 //    else{
 //      c->x -= BORDER;
 //      c->y -= BORDER;
 //      c->dx += 2*BORDER;
 //      c->dy += 2*BORDER;
 //    }
     XGrabServer(dpy);
     drawbound(c);
     t.tv_sec = 0;
     t.tv_usec = 50*1000;
      for (;;) {
         if (XCheckMaskEvent(dpy, ButtonMask, &ev) == 0) {
             getmouse(&rx, &ry);
             if (rx == cx && ry == cy)
                 continue;
             drawbound(c);
             recalc(c, ox=ox+rx-cx, oy=oy+ry-cy);
             cx = rx;
            cy = ry;
             drawbound(c);
             XFlush(dpy);
             select(0, 0, 0, 0, &t);
             continue;
         }
         e = &ev.xbutton;
         switch (ev.type) {
         case ButtonPress:
         case ButtonRelease:
             drawbound(c);
             ungrab(e);
             XUngrabServer(dpy);
              if (e->button == Button3 && c->init)
                  goto bad;
             if (c->dx < 0) {
                 c->x += c->dx;
                 c->dx = -c->dx;
             }
             if (c->dy < 0) {
                 c->y += c->dy;
                 c->dy = -c->dy;
             }
             if (c->dx < 4 || c->dy < 4 || c->dx < c->min_dx || c->dy < c->min_dy)
                 goto bad;
             return 1;
         }
     }
 bad:
     c->x = ox;          // this section will probably fail miserably
     c->y = oy;
     c->dx = odx;
     c->dy = ody;
     return 0;
}


 int sweep(Client *c){
     XEvent ev;
     int status;
     XButtonEvent *e;
 
 
     status = grab(root, ButtonMask, crossCursor.handle(), 0);
     if (status != GrabSuccess) {
       fprintf(stderr, "sweep: Cannot grab >:-<, aborted. \n");
         return 0;
    }
 
     XMaskEvent(dpy, ButtonMask, &ev);
     e = &ev.xbutton;
     XChangeActivePointerGrab(dpy, ButtonMask, sizeAllCursor.handle(), e->time);
     return sweepdrag(c, e, sweepcalc);
 }
 
 
 int drag(Client *c){
     XEvent ev;
     int status;
 
     status = grab(root, ButtonMask, None, 0);
     if (status != GrabSuccess) {
         return 0;
     }
 
     if (c->init){
         setmouse(c->x, c->y);
  }
     else{
         getmouse(&c->x, &c->y);     /* start at current mouse pos */
     }

    return sweepdrag(c, 0, dragcalc);
 
 }  /* end drag */
 
 void resizedrag(Client *c){
   int odx, ody;
 
     if (c == 0)
         return;
     c->dx2 = c->dx;
     c->dy2 = c->dy;
     XChangeActivePointerGrab(dpy, ButtonMask, sizeFDiagCursor.handle(), 0);
     sweepdrag(c,0,resizecalc);
     active(c);
     XRaiseWindow(dpy, c->parent_window);
     c->setGeometry(c->x , c->y ,
                     c->dx , c->dy);
 
 
     if (c->dx == c->dx2 && c->dy == c->dy2)
      sendconfig(c);
     else
       {
        if (c->trans != None)
           XMoveResizeWindow(dpy, c->window, (BORDER), (BORDER), c->dx - 2*BORDER, c->dy - 2*BORDER);
         else
           XMoveResizeWindow(dpy, c->window, (BORDER), (BORDER) + TITLEBAR_HEIGHT,
                             c->dx - 2*BORDER, c->dy - 2*BORDER - TITLEBAR_HEIGHT);
       }
 }


 void getmouse(int *x, int *y){
     Window dw1, dw2;
     int t1, t2;
     unsigned int t3;
 
    XQueryPointer(dpy, root, &dw1, &dw2, x, y, &t1, &t2, &t3);
 }
 
 void setmouse(int x, int y){
     XWarpPointer(dpy, None, root, None, None, None, None, x, y);
 }

char    *version[] =
{
    "kwm: version 0.4 Copyright (c) 1996, 1997 Brian Cooper \n"
    "     based on version 0.1 Copyright (c) 1996 Matthias Ettrich \n"
    "     Parts derived from 9wm 1.1 Copyright (c) 1994 David Hogan\n"
    "     No warranty. Please see the README's\n",0
};


int             screen;
Display * dpy;
Window          root;
Colormap        def_cmap;
int             initting;
GC              gc;
unsigned long   black9;
unsigned long   white9;
XFontStruct     *font;
char            **myargv;
char            *termprog;
Bool            shape;
int             shape_event;
int             min_cmaps;
int             curtime;
int             signalled;

Atom        exit_kwm;
Atom        restart_kwm;
Atom        wm_state;
Atom        wm_change_state;
Atom        wm_protocols;
Atom        wm_delete;
Atom        wm_take_focus;
Atom        wm_colormaps;
Atom        _kwm_running;

void    usage(), sighandler(int);

char    *fontlist[] = {
    "lucm.latin1.9",
    "blit",
    "9x15bold",
    "lucidasanstypewriter-12",
    "fixed",
    0,
}

void initkwm(int argc, char* argv[]){
    int i, status, do_exit, do_restart, dummy;
    unsigned long mask;
    XEvent ev;
    XGCValues gv;
    XSetWindowAttributes attr;
    char *fname;

    myargv = argv;          /* for restart */

    do_exit = do_restart = 0;
    font = 0;
    fname = 0;
    for (i = 1; i < argc; i++)
        if (strcmp(argv[i], "-font") == 0 && i+1<argc) {
            i++;
            fname = argv[i];
        }
        else if (strcmp(argv[i], "-term") == 0 && i+1<argc)
            termprog = argv[++i];
        else if (strcmp(argv[i], "-version") == 0) {
            fprintf(stderr, "%s", version[0]);
            if (PATCHLEVEL > 0)
                fprintf(stderr, "patch level %d", PATCHLEVEL);
            putc('\n', stderr);
            exit(0);
        }
        else if (argv[i][0] == '-')
            usage();
        else
            break;
    for (; i < argc; i++)
        if (strcmp(argv[i], "exit") == 0)
            do_exit++;
        else if (strcmp(argv[i], "restart") == 0)
            do_restart++;
        else
            usage();

    if (do_exit && do_restart)
        usage();


    /*
      Matthias
    dpy = XOpenDisplay("");
    */
    if (dpy == 0)
        fatal("can't open display");
    screen = DefaultScreen(dpy);
    root = RootWindow(dpy, screen);
    def_cmap = DefaultColormap(dpy, screen);
    min_cmaps = MinCmapsOfScreen(ScreenOfDisplay(dpy, screen));

    initting = 1;
    XSetErrorHandler(handler);
    if (signal(SIGTERM, sighandler) == SIG_IGN)
        signal(SIGTERM, SIG_IGN);
    if (signal(SIGINT, sighandler) == SIG_IGN)
        signal(SIGINT, SIG_IGN);
    if (signal(SIGHUP, sighandler) == SIG_IGN)
        signal(SIGHUP, SIG_IGN);

    exit_kwm = XInternAtom(dpy, "KWM_EXIT", False);
    restart_kwm = XInternAtom(dpy, "KWM_RESTART", False);

    curtime = -1;       /* don't care */
    if (do_exit) {
        sendcmessage(root, exit_kwm, 0L);
        XSync(dpy, False);
        exit(0);
    }
    if (do_restart) {
        sendcmessage(root, restart_kwm, 0L);
        XSync(dpy, False);
        exit(0);
    }

    wm_state = XInternAtom(dpy, "WM_STATE", False);
    wm_change_state = XInternAtom(dpy, "WM_CHANGE_STATE", False);
    wm_protocols = XInternAtom(dpy, "WM_PROTOCOLS", False);
    wm_delete = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
    wm_take_focus = XInternAtom(dpy, "WM_TAKE_FOCUS", False);
    wm_colormaps = XInternAtom(dpy, "WM_COLORMAP_WINDOWS", False);
    _kwm_running = XInternAtom(dpy, "_KWM_RUNNING", False);

    black9 = BlackPixel(dpy, screen);
    white9 = WhitePixel(dpy, screen);

    if (fname != 0)
        if ((font = XLoadQueryFont(dpy, fname)) == 0)
            fprintf(stderr, "kwm: warning: can't load font %s\n", fname);

    if (font == 0) {
        i = 0;
        for (;;) {
            fname = fontlist[i++];
            if (fname == 0) {
                fprintf(stderr, "kwm: can't find a font\n");
                exit(1);
            }
            font = XLoadQueryFont(dpy, fname);
            if (font != 0)
                break;
        }
    }

    gv.foreground = black9^white9;
    gv.background = white9;
    gv.font = font->fid;
    gv.function = GXxor;
    gv.line_width = 0;
    gv.subwindow_mode = IncludeInferiors;
    mask = GCForeground | GCBackground | GCFunction | GCFont | GCLineWidth
        | GCSubwindowMode;
    gc = XCreateGC(dpy, root, mask, &gv);


#ifdef  SHAPE
    shape = XShapeQueryExtension(dpy, &shape_event, &dummy);
#endif


    XSelectInput(dpy, root,
                 ButtonPressMask | ButtonReleaseMask |
                 PropertyChangeMask |
                 ColormapChangeMask |
                 SubstructureRedirectMask |
                 SubstructureNotifyMask
                 );
    XSync(dpy, False);


    XSync(dpy, False);
    initting = 0;

    nofocus();
    scanwins();
}

void usage() {
    fprintf(stderr, "usage: kwm [-grey] [-version] [-font fname] [-term prog] [exit|restart]\n");
    exit(1);
}

void sendcmessage(Window w, Atom a, long x){
    XEvent ev;
    int status;
    long mask;

    memset(&ev, 0, sizeof(ev));
    ev.xclient.type = ClientMessage;
    ev.xclient.window = w;
    ev.xclient.message_type = a;
    ev.xclient.format = 32;
    ev.xclient.data.l[0] = x;
    ev.xclient.data.l[1] = timestamp();
    mask = 0L;
    if (w == root)
        mask = SubstructureRedirectMask;        /* magic! */
    status = XSendEvent(dpy, w, False, mask, &ev);
    if (status == 0)
        fprintf(stderr, "kwm: sendcmessage failed\n");
}

Time timestamp(){
    XEvent ev;

    if (curtime == CurrentTime) {
        XChangeProperty(dpy, root, _kwm_running, _kwm_running, 8,
                PropModeAppend, (unsigned char *)"", 0);
        XMaskEvent(dpy, PropertyChangeMask, &ev);
        curtime = ev.xproperty.time;
    }
    return curtime;
}

void sendconfig(Client* c) {
    XConfigureEvent ce;

fprintf(stderr,"friggin' sendconfig.\n");

    ce.type = ConfigureNotify;
    ce.event = c->window;
    ce.window = c->window;

if (!c->already_managed){
  fprintf(stderr,"Hey, this one's not managed!\n");
  return;
}

    if(!c->trans){
      ce.x = c->x + BORDER;
      ce.y = c->y + BORDER + TITLEBAR_HEIGHT;
      ce.width = c->dx - 2*BORDER;
      ce.height = c->dy - 2*BORDER - TITLEBAR_HEIGHT;
    }
    else{
      ce.x = c->x + BORDER;
      ce.y = c->y + BORDER;
      ce.width = c->dx - 2*BORDER;
      ce.height = c->dy - 2*BORDER;
    }
    ce.border_width = c->border;
    ce.above = None;
    ce.override_redirect = 0;
    XSendEvent(dpy, c->window, False, StructureNotifyMask, (XEvent*)&ce);
}

void scanwins() {
    unsigned int i, nwins;
    Client *c;
    Window dw1, dw2, *wins;
    XWindowAttributes attr;

    XQueryTree(dpy, root, &dw1, &dw2, &wins, &nwins);
    for (i = 0; i < nwins; i++) {
        XGetWindowAttributes(dpy, wins[i], &attr);
        if (attr.override_redirect )
            continue;
        c = getclient(wins[i], 1);
        if (c != 0 && c->window == wins[i]) {
            c->x = attr.x;
            c->y = attr.y;
            c->dx = attr.width;  // manage readjusts these.
            c->dy = attr.height; // probably this should also be in manage()
            c->border = attr.border_width;
            if (attr.map_state == IsViewable)
                manage(c, 1);
        }
    }
    XFree((void *) wins);   /* cast is to shut stoopid compiler up */
}

void configurereq(XConfigureRequestEvent *e) {
    XWindowChanges wc;
    XConfigureEvent ce;
    Client *c;
fprintf(stderr,"configurereq\n");
    /* we don't set curtime as nothing here uses it */
    c = getclient(e->window, 0);
    if (c) {                    // client already exists for this window
        gravitate(c, 1);
        if (e->value_mask & CWX)
            c->x = e->x;
        if (e->value_mask & CWY)
            c->y = e->y;
      if(!c->trans){
        if (e->value_mask & CWWidth)
            c->dx = e->width + 2*BORDER;
        if (e->value_mask & CWHeight)
            c->dy = e->height + 2*BORDER + TITLEBAR_HEIGHT;
        if (e->value_mask & CWBorderWidth)
            c->border = e->border_width;
       }
       else{
//      if (e->value_mask & CWX)
//            c->x = e->x - BORDER;
//        if (e->value_mask & CWY)
//            c->y = e->y - BORDER;
        if (e->value_mask & CWWidth)
            c->dx = e->width + 2*BORDER;
        if (e->value_mask & CWHeight)
            c->dy = e->height + 2*BORDER;
        if (e->value_mask & CWBorderWidth)
            c->border = e->border_width;
        }
        gravitate(c, 0);
        if (c->parent_window != root && c->window == e->window) {

            wc.x = c->x;
            wc.y = c->y;

            wc.width = c->dx;
            wc.height = c->dy;
            wc.border_width = 1;
            wc.sibling = e->above;
            wc.stack_mode = e->detail;
            XConfigureWindow(dpy, e->parent, e->value_mask, &wc);
            sendconfig(c);
        }
    }
else
  fprintf(stderr,"configurereq for window w/ no client\n");

    if (c && c->init) {
        wc.x = BORDER;
        if(!c->trans)
          wc.y = BORDER+TITLEBAR_HEIGHT;
        else
          wc.y = BORDER;
    }
    else {
        wc.x = e->x;
        wc.y = e->y;
    }

    wc.width = e->width;
    wc.height = e->height;
    wc.border_width = 0;

    wc.sibling = e->above;
    wc.stack_mode = e->detail;
    e->value_mask |= CWBorderWidth;

    XConfigureWindow(dpy, e->window, e->value_mask, &wc);
}

void mapreq(XMapRequestEvent *e){
    Client *c;
    XWindowAttributes attr;

fprintf(stderr,"mapreq\n");
    curtime = CurrentTime;
    c = getclient(e->window, 0);
    if (c == 0 || c->window != e->window) {
        fprintf(stderr, "kwm: bogus mapreq %x %x\n", c, e->window);
        return;
    }

    switch (c->state) {
    case WithdrawnState:
    printf("mapreq...WithdrawnState\n");
        if (!c->already_managed) {


        XGetWindowAttributes(dpy, e->window, &attr);
            c->x = attr.x;
            c->y = attr.y;
            c->dx = attr.width;  // manage readjusts these.
            c->dy = attr.height; // probably this should also be in manage()

            if (!manage(c, 0))
                return;
            break;
        }
        if (c->trans)
          XReparentWindow(dpy, c->window, c->parent_window, (BORDER), (BORDER));
        else
          XReparentWindow(dpy, c->window, c->parent_window, (BORDER), (BORDER) +
                          TITLEBAR_HEIGHT);
        XAddToSaveSet(dpy, c->window);

        /* fall through... */
    case NormalState:
        XMapRaised(dpy, c->parent_window);
        XMapWindow(dpy, c->window);
        setwindowstate(c, NormalState);
        break;
    case IconicState:
        unhidec(c, 1);
        break;
    }
}

void unmap(XUnmapEvent *e){
    Client *c;

    curtime = CurrentTime;
    c = getclient(e->window, 0);
    if (c) {
        switch (c->state) {
        case IconicState:
            if (e->send_event) {
                unhidec(c, 0);
                withdraw(c);
            }
            break;
        case NormalState:
            if (c == current)
                nofocus();
            if (!c->reparenting)
                withdraw(c);
            break;
        }
        c->reparenting = 0;
    }
}

void circulatereq(XCirculateRequestEvent *e){
    fprintf(stderr, "It must be the warlock Krill!\n");  /* :-) */
}

void newwindow(XCreateWindowEvent *e){
    Client *c;
    XWindowAttributes attr;
    int n=1, order;

    printf("newwindow\n");
    /* we don't set curtime as nothing here uses it ???  */
    if (e->override_redirect){
        return;
    }

    c = getclient(e->window, 1);

    if (c && e->window == c->parent_window){
      if (c->already_got_create_event){
        // this hack means that the window and the client are already destroyed (Matthias)
        rmclient(c);
        return;
      }
      c->already_got_create_event = TRUE;
    }

    XGetWindowAttributes(dpy, c->window, &attr);

    if (c && c->window == e->window) {
    printf("newwindow II\n");
        c->x = attr.x;
        c->y = attr.y;
        c->dx = attr.width;
        c->dy = attr.height;
        c->border = attr.border_width;
            if (attr.map_state == IsViewable){
#ifdef SHAPE
              /* don't try to manage if the window is non-rectangular */
              XShapeGetRectangles(dpy, c->window, ShapeBounding, &n, &order);
#endif
              if ( n==1 ) manage(c, 1);
            }
      if (!c->already_managed) fprintf(stderr,"Not Managing newwindow II\n");
    }
}  /* end newwindow */

void destroywin(Window w){
    Client *c, **l;

//fprintf(stderr,"destroy\n");
    curtime = CurrentTime;
    c = getclient(w, 0);
    if (c == 0 || c->window != w){
        return;
    }

    if (!c->already_got_create_event){
      c->already_got_create_event = TRUE;
      return;
    }

    rmclient(c);

    /* flush any errors generated by the window's sudden demise */
    ignore_badwindow = 1;
    XSync(dpy, False);
    ignore_badwindow = 0;

}

void clientmesg(XClientMessageEvent *e){
    Client *c;

    curtime = CurrentTime;
    if (e->window == root && e->message_type == exit_kwm) {
        cleanup();
        exit(0);
    }
    if (e->window == root && e->message_type == restart_kwm) {
        fprintf(stderr, "*** kwm restarting ***\n");
        cleanup();
        execvp(myargv[0], myargv);
        perror("kwm: exec failed");
        exit(1);
    }
    if (e->message_type == wm_change_state) {
        c = getclient(e->window, 0);
        if (e->format == 32 && e->data.l[0] == IconicState && c != 0) {
            if (normalstate(c))
                hidec(c);
        }
        else
            fprintf(stderr, "kwm: WM_CHANGE_STATE: format %d data %d w 0x%x\n",
                e->format, e->data.l[0], e->window);
        return;
    }
    fprintf(stderr, "kwm: strange ClientMessage, type 0x%x window 0x%x\n",
            e->message_type, e->window);
}

void cmap(XColormapEvent *e){
  Client *c;
  int i;

  /* we don't set curtime as nothing here uses it */
  if (e->c_new){
    c = getclient(e->window, 0);
    if (c) {
      c->cmap = e->colormap;
      if (c == current)
        cmapfocus(c);
    }
    else
      for (c = clients; c; c = c->next)
        for (i = 0; i < c->ncmapwins; i++)
          if (c->cmapwins[i] == e->window) {
            c->wmcmaps[i] = e->colormap;
            if (c == current)
              cmapfocus(c);
            return;
          }
  }
}

void property(XPropertyEvent *e){
    Atom a;
    int del;
    Client *c;

    /* we don't set curtime as nothing here uses it */
    a = e->atom;
    del = (e->state == PropertyDelete);
    c = getclient(e->window, 0);
    if (c == 0)
        return;

    switch (a) {
    case XA_WM_ICON_NAME:
        if (c->iconname != 0)
            XFree((char*) c->iconname);
        c->iconname = getprop(c->window, XA_WM_ICON_NAME);
        setwindowlabel(c);
        renamec(c, c->label);
        return;
    case XA_WM_NAME:
        if (c->name != 0)
            XFree((char*) c->name);
        c->name = getprop(c->window, XA_WM_NAME);
        setwindowlabel(c);
        renamec(c, c->label);
        return;
    case XA_WM_TRANSIENT_FOR:
        getwindowtrans(c);
        return;
    }
    if (a == wm_colormaps) {
        getcmaps(c);
        if (c == current)
            cmapfocus(c);
    }
}

void reparent(XReparentEvent *e){
    Client *c;
    XWindowAttributes attr;

    printf("reparent\n");
    if (e->event != root || e->override_redirect)
        return;
    if (e->parent == root) {
        c = getclient(e->window, 1);
        if (c != 0 && (c->dx == 0 || c->dy == 0)) {
            XGetWindowAttributes(dpy, c->window, &attr);
            c->x = attr.x;
            c->y = attr.y;
            c->dx = attr.width;
            c->dy = attr.height;
            c->border = attr.border_width;
            manage(c,1);
        }
    }
    else {
        c = getclient(e->window, 0);
        if (c != 0 && (!c->already_managed || withdrawn(c)))
            rmclient(c);
    }
}  /* end reparent */

#ifdef  SHAPE
void shapenotify(XShapeEvent *e){
    Client *c;

    c = getclient(e->window, 0);
    if (c == 0)
        return;

    setshape(c);
}
#endif

void enter(XCrossingEvent *e){
    Client *c;

    curtime = e->time;

    c = getclient(e->window, 0);

    if (c != 0 && c != current && c->state != WithdrawnState) {
      active(c);
    }
}

void sighandler(int) {
    cleanup();
    fprintf(stderr, "kwm: exiting on signal\n");
}


