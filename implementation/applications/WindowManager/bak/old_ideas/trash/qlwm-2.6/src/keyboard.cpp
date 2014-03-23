/* keyboard.cpp */

#include "keyboard.h"
#include "qapp.h"

uint keyboard::NumLockMask;

void keyboard::init(void)
{
	int i;
	static KeySym keys[] = { PREV_WINDOW,NEXT_WINDOW,HIDDEN_WINDOW,HIDE_WINDOW,
	TOGGLE_SCREENMODE,TOGGLE_TILEDMODE,PREV_DESK,NEXT_DESK,WIN_POPUP,MENU_POPUP };
	static KeyCode mods[] = { PREV_WINDOW_MOD,NEXT_WINDOW_MOD,HIDDEN_WINDOW_MOD,HIDE_WINDOW_MOD,
	TOGGLE_SCREENMODE_MOD,TOGGLE_TILEDMODE_MOD,PREV_DESK_MOD,NEXT_DESK_MOD,WIN_POPUP_MOD,MENU_POPUP_MOD };
 	XModifierKeymap* xmk = XGetModifierMapping(qt_xdisplay());
	
        for(i=0; i < 8; i++)
	{
		if(xmk->modifiermap[xmk->max_keypermod * i] == XKeysymToKeycode(qt_xdisplay(), XK_Num_Lock))
		{
		         NumLockMask = (1<<i);
			 break;
		}	 
	}

	for(i=0; i < 10; i++)
	{
		XGrabKey(qt_xdisplay(), XKeysymToKeycode(qt_xdisplay(), keys[i]), mods[i], qt_xrootwin(), True, GrabModeAsync, GrabModeAsync);
		XGrabKey(qt_xdisplay(), XKeysymToKeycode(qt_xdisplay(), keys[i]), mods[i]|LockMask, qt_xrootwin(), True, GrabModeAsync, GrabModeAsync);
		XGrabKey(qt_xdisplay(), XKeysymToKeycode(qt_xdisplay(), keys[i]), mods[i]|NumLockMask, qt_xrootwin(), True, GrabModeAsync, GrabModeAsync);
		XGrabKey(qt_xdisplay(), XKeysymToKeycode(qt_xdisplay(), keys[i]), mods[i]|LockMask|NumLockMask, qt_xrootwin(), True, GrabModeAsync, GrabModeAsync);
	}
}

bool keyboard::keypress(XKeyEvent *kev)
{
	if(qapp::menu_open)
		return(FALSE);
		
	KeySym sym = (int)XLookupKeysym(kev, 0);
	uint mod = kev->state & ~(NumLockMask|LockMask);
      
	if(sym == PREV_WINDOW && mod == PREV_WINDOW_MOD) {
	    prev_win();
	    goto succ_exit;
	} 

	if(sym == NEXT_WINDOW && mod == NEXT_WINDOW_MOD) {
	    next_win();
	    goto succ_exit;
	}

	if(sym == HIDDEN_WINDOW && mod == HIDDEN_WINDOW_MOD) {
	    tb_wl->hidden_win();
	    goto succ_exit;
	}

	if(sym == HIDE_WINDOW && mod == HIDE_WINDOW_MOD) {
	    hide_win();
	    goto succ_exit;
	}

	if(sym == TOGGLE_SCREENMODE && mod == TOGGLE_SCREENMODE_MOD) {
	    tscreen();
	    goto succ_exit;
	}

	if(sym == TOGGLE_TILEDMODE && mod == TOGGLE_TILEDMODE_MOD) {
	    qapp::toggle_tiled();
	    goto succ_exit;
	}

	if(sym == PREV_DESK && mod == PREV_DESK_MOD) {
	    pdesk();
	    goto succ_exit;
	}

	if(sym == NEXT_DESK && mod == NEXT_DESK_MOD) {
	    ndesk();
	    goto succ_exit;
	}

	if(sym == WIN_POPUP && mod == WIN_POPUP_MOD) {
	    tb_wl->start_popup();
	    goto succ_exit;
	}

	if(sym == MENU_POPUP && mod == MENU_POPUP_MOD) {	  
	    tb_mn->start_popup();
	    goto succ_exit;
	}

	return false;

succ_exit:
	return true;
}

void keyboard::prev_win(void)
{
	xwindow *client,*cur;

	if(qapp::focusclient == NULL)
		return;
		
	clients.find(qapp::focusclient);

	if((client = clients.current()) == NULL)
		client = clients.last();

	while(1)
	{
		if((cur = clients.prev()) == NULL)
			cur = clients.last();

		if(cur == NULL || cur == client)
			break;

		if(cur->isVisible() && ! (qapp::smode && (cur->isshaped() || (cur->get_pflags() & qapp::NoScreen))) &&
		! (cur->get_pflags() & qapp::NoKey) && qapp::is_curdesk(cur))
		{
			cur->focus_mouse();
			return;
		}
	}
}

void keyboard::next_win(void)
{
	xwindow *client,*cur;

	if(qapp::focusclient == NULL)
		return;

	clients.find(qapp::focusclient);

	if((client = clients.current()) == NULL)
		client = clients.first();

	while(1)
	{
		if((cur = clients.next()) == NULL)
			cur = clients.first();

		if(cur == NULL || cur == client)
			break;

		if(cur->isVisible() && ! (qapp::smode && (cur->isshaped() || (cur->get_pflags() & qapp::NoScreen))) &&
		! (cur->get_pflags() & qapp::NoKey) && qapp::is_curdesk(cur))
		{
			cur->focus_mouse();
			return;
		}
	}
}

void keyboard::hide_win(void)
{
	if(qapp::smode)
		return;

	if(clients.find(qapp::focusclient) != -1 && qapp::focusclient->isVisible())
		qapp::focusclient->whide();
}

void keyboard::tscreen(void)
{
	xwindow *client,*cur;
	
	if(qapp::smode)
	{
		qapp::smode = FALSE;
		qapp::stopautofocus();

		for(client = clients.first(); client != NULL; client = clients.next())
			client->unscreen();

		if(qapp::is_tileddesk())
			qapp::tile_order(qapp::focusclient);
	}
	else
	{
		if(qapp::focusclient == NULL)
			return;

		clients.find(qapp::focusclient);

		if((client = clients.current()) == NULL)
			client = clients.first();

		if(client == NULL)
			return;
			
		cur = client;
		
		do
		{
			if(cur->isVisible() && ! (qapp::smode && (cur->isshaped() || (cur->get_pflags() & qapp::NoScreen))) &&
			! (cur->get_pflags() & qapp::NoKey) && qapp::is_curdesk(cur))
			{
				qapp::smode = TRUE;
				qapp::stopautofocus();
				cur->focus_mouse();
				return;
			}
			if((cur = clients.next()) == NULL)
				cur = clients.first();
		}
		while(cur != NULL && cur != client);
	}
}

void keyboard::pdesk(void)
{
	if(qapp::smode)
		return;
		
	int desk = qapp::adesk;

	if(--desk < 0)
		desk = defaults::vdesks-1;

	tb_pg->change_desk(desk);
}

void keyboard::ndesk(void)
{
	if(qapp::smode)
		return;

	int desk = qapp::adesk;

	if(++desk >= defaults::vdesks)
		desk = 0;

	tb_pg->change_desk(desk);
}
