
#include "keyboard.h"
#include <X11/Xlib.h>
#include "windowmanager/windowmanager.h"

uint keyboard::NumLockMask;

void keyboard::init(void)
{
	int i;
	static KeySym keys[] = { PREV_WINDOW,NEXT_WINDOW,HIDDEN_WINDOW,HIDE_WINDOW,
	TOGGLE_SCREENMODE,TOGGLE_TILEDMODE,PREV_DESK,NEXT_DESK,WIN_POPUP,MENU_POPUP };
	static KeyCode mods[] = { PREV_WINDOW_MOD,NEXT_WINDOW_MOD,HIDDEN_WINDOW_MOD,HIDE_WINDOW_MOD,
	TOGGLE_SCREENMODE_MOD,TOGGLE_TILEDMODE_MOD,PREV_DESK_MOD,NEXT_DESK_MOD,WIN_POPUP_MOD,MENU_POPUP_MOD };
 	XModifierKeymap* xmk = XGetModifierMapping(QX11Info::display());
	
        for(i=0; i < 8; i++)
	{
		if(xmk->modifiermap[xmk->max_keypermod * i] == XKeysymToKeycode(QX11Info::display(), XK_Num_Lock))
		{
		         NumLockMask = (1<<i);
			 break;
		}	 
	}

	for(i=0; i < 10; i++)
	{
		XGrabKey(QX11Info::display(), XKeysymToKeycode(QX11Info::display(), keys[i]), mods[i], QX11Info::appRootWindow(), True, GrabModeAsync, GrabModeAsync);
		XGrabKey(QX11Info::display(), XKeysymToKeycode(QX11Info::display(), keys[i]), mods[i]|LockMask, QX11Info::appRootWindow(), True, GrabModeAsync, GrabModeAsync);
		XGrabKey(QX11Info::display(), XKeysymToKeycode(QX11Info::display(), keys[i]), mods[i]|NumLockMask, QX11Info::appRootWindow(), True, GrabModeAsync, GrabModeAsync);
		XGrabKey(QX11Info::display(), XKeysymToKeycode(QX11Info::display(), keys[i]), mods[i]|LockMask|NumLockMask, QX11Info::appRootWindow(), True, GrabModeAsync, GrabModeAsync);
	}
}

bool keyboard::keypress(XKeyEvent *kev)
{
	if(WindowManager::menu_open)
		return FALSE;
		
	KeySym sym = (int)XLookupKeysym(kev, 0);
	uint mod = kev->state & ~(NumLockMask|LockMask);
      
	if(sym == PREV_WINDOW && mod == PREV_WINDOW_MOD) {
	    prev_win();
	    return TRUE;
	} 

	if(sym == NEXT_WINDOW && mod == NEXT_WINDOW_MOD) {
	    next_win();
	    return TRUE;
	}

	if(sym == HIDDEN_WINDOW && mod == HIDDEN_WINDOW_MOD) {
	    tb_wl->hidden_win();
	    return TRUE;
	}

	if(sym == HIDE_WINDOW && mod == HIDE_WINDOW_MOD) {
	    hide_win();
	    return TRUE;
	}

	if(sym == TOGGLE_SCREENMODE && mod == TOGGLE_SCREENMODE_MOD) {
	    tscreen();
	    return TRUE;
	}

	if(sym == TOGGLE_TILEDMODE && mod == TOGGLE_TILEDMODE_MOD) {
	    WindowManager::toggle_tiled();
	    return TRUE;
	}

	if(sym == PREV_DESK && mod == PREV_DESK_MOD) {
	    pdesk();
	    return TRUE;
	}

	if(sym == NEXT_DESK && mod == NEXT_DESK_MOD) {
	    ndesk();
	    return TRUE;
	}

	if(sym == WIN_POPUP && mod == WIN_POPUP_MOD) {
	    tb_wl->start_popup();
	    return TRUE;
	}

	if(sym == MENU_POPUP && mod == MENU_POPUP_MOD) {	  
	    tb_mn->start_popup();
	    return TRUE;
	}

	return FALSE;
}

void keyboard::prev_win(void)
{
	xwindow *cur;
	int curcl,ccl;

	if(clients.size() < 2)
		return;
		
	if((curcl = clients.indexOf(WindowManager::focusclient)) == -1)
		curcl = clients.size()-1;

	ccl = curcl;
	while(--curcl != ccl)
	{
		if(curcl < 0)
			curcl = clients.size()-1;
			
		cur = clients.at(curcl);
			
		if(cur->isVisible() && ! (WindowManager::smode && (cur->isshaped() || (cur->get_pflags() & WindowManager::NoScreen))) &&
		! (cur->get_pflags() & WindowManager::NoKey) && WindowManager::is_curdesk(cur))
		{
			cur->focus_mouse();
			return;
		}
	} 
}

void keyboard::next_win(void)
{
	xwindow *cur;
	int curcl,ccl;

	if(clients.size() < 2)
		return;

	if((curcl = clients.indexOf(WindowManager::focusclient)) == -1) 
		curcl = 0;

	ccl = curcl;	
	while(++curcl != ccl)
	{
		if(curcl >= clients.size())
			curcl = 0;
			
		cur = clients.at(curcl);

		if(cur->isVisible() && ! (WindowManager::smode && (cur->isshaped() || (cur->get_pflags() & WindowManager::NoScreen))) &&
		! (cur->get_pflags() & WindowManager::NoKey) && WindowManager::is_curdesk(cur))
		{
			cur->focus_mouse();
			return;
		}	
	} 
}

void keyboard::hide_win(void)
{
	if(WindowManager::smode)
		return;

	if(clients.indexOf(WindowManager::focusclient) != -1 && WindowManager::focusclient->isVisible())
		WindowManager::focusclient->whide();
}

void keyboard::tscreen(void)
{
	xwindow *client,*cur;
	int i,curcl;
	
	if(WindowManager::smode)
	{
		WindowManager::smode = FALSE;
		WindowManager::stopautofocus();

		if(clients.size() == 0)
			return;

		for(i=0; i < clients.size(); i++)
			clients.at(i)->unscreen();

		if(WindowManager::is_tileddesk())
			WindowManager::tile_order(WindowManager::focusclient);
	}
	else
	{
		if(clients.size() == 0 || WindowManager::focusclient == NULL)
			return;

		if((curcl = clients.indexOf(WindowManager::focusclient)) == -1)
			curcl = 0;

		client = cur = clients.at(curcl);

		do
		{
			if(cur->isVisible() && ! (WindowManager::smode && (cur->isshaped() || (cur->get_pflags() & WindowManager::NoScreen))) &&
			! (cur->get_pflags() & WindowManager::NoKey) && WindowManager::is_curdesk(cur))
			{
				WindowManager::smode = TRUE;
				WindowManager::stopautofocus();
				cur->focus_mouse();
				return;
			}
			if(++curcl >= clients.size())
				curcl = 0;
			cur = clients.at(curcl);
		}
		while(cur != client);
	}
}

void keyboard::pdesk(void)
{
	if(WindowManager::smode)
		return;
		
	int desk = WindowManager::adesk;

	if(--desk < 0)
		desk = defaults::vdesks-1;

	tb_pg->change_desk(desk);
}

void keyboard::ndesk(void)
{
	if(WindowManager::smode)
		return;

	int desk = WindowManager::adesk;

	if(++desk >= defaults::vdesks)
		desk = 0;

	tb_pg->change_desk(desk);
}
