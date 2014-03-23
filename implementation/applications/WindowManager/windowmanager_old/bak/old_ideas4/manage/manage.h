/* Portions Copyright (c) 1994 David Hogan, see README for licence details */

#include <X11/extensions/shape.h>

#define TRANSPARENT 0
#define OPAQUE      1

#define W95BUTTONS          0
#define REVW95BUTTONS       1
#define CROSSBUTTONS        2
#define TAJBUTTONS          3
#define XONLEFTBUTTONS      4

#define MARKOSPACING        0
#define NONMARKOSPACING     1

extern int WindowMoveType;
extern int buttonGroup;
extern int buttonSpacing;
extern int BUTTON_SEPARATION;
extern int TITLEBUTTON_SEPARATION;
extern int TITLEWINDOW_SEPARATION;





class Client;

void gravitate(Client* c, int invert);
void withdraw(Client *c);
void cleanup();
void cmapfocus(Client *c);
void setwindowlabel(Client *c);
void getwindowtrans(Client * c);
void getcmaps(Client *c);
void setshape(Client* c);

void active(Client *c);
void nofocus();
Client * getclient(Window w, int create);
void rmclient(Client *c);
void fatal(char *s);
int handler(Display *d, XErrorEvent *e);
int grab(Window w, int mask, Cursor curs, int t);
void ungrab(XButtonEvent *e);
void sweepcalc(Client *c, int x, int y);
void resizecalc(Client *c, int x, int y);
void dragcalc(Client* c, int x, int y);
void drawbound(Client* c);
int sweep(Client *c);
int drag(Client *c);
void resizedrag(Client *c);
void getmouse(int *x, int *y);
void setmouse(int x, int y);
void initkwm(int argc, char* argv[]);
void usage();
void sendcmessage(Window w, Atom a, long x);
Time timestamp();
void sendconfig(Client* c);
void scanwins();
void configurereq(XConfigureRequestEvent *e);
void mapreq(XMapRequestEvent *e);
void unmap(XUnmapEvent *e);
void circulatereq(XCirculateRequestEvent *e);
void newwindow(XCreateWindowEvent *e);
void destroywin(Window w);
void clientmesg(XClientMessageEvent *e);
void cmap(XColormapEvent *e);
void property(XPropertyEvent *e);
void reparent(XReparentEvent *e);
void shapenotify(XShapeEvent *e);
void enter(XCrossingEvent *e);
void sighandler(int);
void open();
void initdevel();
int manage(Client*  c, int mapped);
int _getprop(Window w, Atom a, Atom type, long len, unsigned char **p);
char * getprop(Window w, Atom a);
int getiprop(Window w, Atom a);
void setwindowstate(Client *c, int state);
int getwindowstate(Window w, int* state);
void getwindowproto(Client *c);
void move(Client *c);
void hidec(Client *c);
void unhide(int n, int map);
void unhidec(Client *c, int map);
void renamec(Client *c, char* name);
void reshape(Client *c);

int sweepdrag(Client* c, XButtonEvent *e0, void (*recalc)( Client *, int, int) );

#ifndef DAT_H
#define DAT_H

#define BORDER     4
#define BUTTON_SIZE    16
#define TITLEBAR_HEIGHT 20
#define CORNER          20
#define MAXHIDDEN   32
#define B3FIXED     5


#define AllButtonMask   (Button1Mask|Button2Mask|Button3Mask \
			|Button4Mask|Button5Mask)
#define ButtonMask  (ButtonPressMask|ButtonReleaseMask)
#define MenuMask    (ButtonMask|ButtonMotionMask|ExposureMask)
#define MenuGrabMask    (ButtonMask|ButtonMotionMask|StructureNotifyMask)


#define hidden(c)       ((c)->state == IconicState)
#define withdrawn(c)    ((c)->state == WithdrawnState)
#define normalstate(c)       ((c)->state == NormalState)

/* c->proto */
#define Pdelete     1
#define Ptakefocus  2

extern int          currentVD;

/* kwm.c */
extern Display      *dpy;
extern int          screen;
extern Window       root;
extern Colormap     def_cmap;
extern int          initting;
extern GC           gc;
extern XFontStruct  *font;
extern int          nostalgia;
extern Atom         wm_state;
extern Atom         _9wm_hold_mode;
extern Atom         wm_protocols;
extern Atom         wm_delete;
extern Atom         wm_take_focus;
extern Atom         wm_colormaps;
extern unsigned long    black9;
extern unsigned long    white9;
extern Bool         shape;
extern char         *termprog;
extern char         *shell;
extern char         *version[];
extern int          min_cmaps;
extern int          curtime;


/* cursor.c */
extern Cursor       target;
extern Cursor       sweep0;
extern Cursor       boxcurs;
extern Cursor       arrow;

/* error.c */
extern int          ignore_badwindow;

