
#include "menuwidget.h"

wmMenuWidget::wmMenuWidget( QWidget *parent, const char *name )
    : QWidget( parent, name ){

  QPopupMenu *file = new QPopupMenu;
  CHECK_PTR( file );
  file->insertItem( "Run",  this, SLOT(run()) );
  file->insertItem( "Restart",  this, SLOT(restart()) );
  file->insertItem( "Exit",  kapp , SLOT(quit()) );

  view = new QPopupMenu;
  CHECK_PTR( view );
  view->insertItem( "Page 1",  this, SLOT(page1()) );
  view->insertItem( "Page 2",  this, SLOT(page2()) );
  view->insertItem( "Page 3",  this, SLOT(page3()) );
  view->insertItem( "Page 4",  this, SLOT(page4()) );

  QPopupMenu *help = new QPopupMenu;
  CHECK_PTR( help );
  help->insertItem( "KWM Help",  this, SLOT(kwmhelp()) );
  help->insertSeparator();
  help->insertItem( "About",  this, SLOT(about()) );

  windows = new QPopupMenu;
  CHECK_PTR( help );

  QPopupMenu *options = new QPopupMenu;
  CHECK_PTR( help );
  int checkT = options->insertItem( "Transparent Move",  this, SLOT(transpMove()) );
  options->setItemChecked(checkT,TRUE);
  int checkO = options->insertItem( "Opaque Move",  this, SLOT(opaqueMove()) );
  options->insertItem( "Titlebar",  this, SLOT(changeTitlebar()) );
//  options->insertSeparator();
//  int enableSO = options->insertItem( "Save Options", kapp, SLOT(saveOptions()) );
//  options->setItemEnabled( enableSO, FALSE);  

  menu = new QMenuBar( this );
  CHECK_PTR( menu );
  menu->insertItem( "File", file );
  menu->insertItem( "View", view );
  menu->insertItem( "Windows", windows );
  menu->insertItem( "Options", options );
  menu->insertSeparator();
  menu->insertItem( "Help", help );
  menu->setGeometry( 0, 0, 300, 22);

  setGeometry(0,0,308,28);

}  /* end wmMenuWidget::wmMenuWidget */

void wmMenuWidget::run(){
  system("acli &");
}  /* end wmMenuWidget::run */

void wmMenuWidget::restart(){
  system("kwm restart &");
}  /* end wmMenuWidget::run */

void wmMenuWidget::vdRequest(int newvd){
  // for now, no reason to deny requests
  currentVD = newvd;
  emit switchVD();
}  /* end wmMenuWidget::run */

void wmMenuWidget::page1(){
  if (currentVD == 1)
    return;
  fprintf(stderr,"Switching to Virtual Desktop 1\n");
  vdRequest(1);
}  /* end wmMenuWidget::page1 */

void wmMenuWidget::page2(){
  if (currentVD == 2)
    return;
  fprintf(stderr,"Switching to Virtual Desktop 2\n");
  vdRequest(2);
}  /* end wmMenuWidget::page2 */

void wmMenuWidget::page3(){
  if (currentVD == 3)
    return;
  fprintf(stderr,"Switching to Virtual Desktop 3\n");
  vdRequest(3);
} 

void wmMenuWidget::page4(){
  if (currentVD == 4)
    return;
  fprintf(stderr,"Switching to Virtual Desktop 4\n");
  vdRequest(4);
}  /* end wmMenuWidget::page4 */

void wmMenuWidget::transpMove(){

  config->setGroup("General");
  config->writeEntry("WindowMoveType","Transparent");

  WindowMoveType=TRANSPARENT;

}  /* end wmMenuWidget::transpMove */

void wmMenuWidget::opaqueMove(){

  config->setGroup("General");
  config->writeEntry("WindowMoveType","Opaque");

  WindowMoveType=OPAQUE;

}  /* end wmMenuWidget::opaqueMove */

void wmMenuWidget::changeTitlebar(){

   ot   = new TitleOptions;
   ot->show();

   //  buttonGroup=W95BUTTONS;
}  /* end wmMenuWidget::changeTitlebar */

void wmMenuWidget::kwmhelp(){
  char c[150];
  strcpy(c,"kdehelp ");
  strcat(c,getenv("KDEDIR"));
  strcat(c,"/doc/HTML/kwmhelp.html &\n");

  system(c);

}  /* end wmMenuWidget::kwmhelp */

void wmMenuWidget::about(){
    QString str = "kwm version 0.4\n";
    str += "\nBrian H. Cooper 1997\n";
    str += "cooperb@er4.eng.ohio-state.edu\n";
    str += "\nFor the KDE project\n";
    str += "http://www-pu.informatik.uni-tuebingen.de/users/ettrich/kde.html\n";
    str += "\nkwm 0.1 copyright Matthias Ettrich 1996\n";
    str += "Parts based on 9wm copyright David Hogan 1994\n";
    str += "Please read the README's\n";
    QMessageBox::message( "About kwm", str, 0 );
}  /* end wmMenuWidget::about */

void wmMenuWidget::addMenuWin(Client *c){

  // ICCCM calls for XA_WM_ICON_NAME here
  // but that doesn't quite cut it IMHO (Brian)

  // No Name, No listing
  if( !getprop(c->window,XA_WM_NAME) ) return;

  c->id = windows->insertItem(getprop(c->window,XA_WM_NAME),c,SLOT(unMinimize()));
  fprintf(stderr,"adding window with unique id %d and name %s\n",c->id,getprop(c->window,XA_WM_NAME));
  connect(c,SIGNAL(clientClosed( int )), m,SLOT(removeMenuWin( int)));

}  /* end wmMenuWidget::addMenuWin */

void wmMenuWidget::removeMenuWin(int id){

  windows->removeItem( id );
  fprintf(stderr,"removing window with unique id %d\n",id);

}  /* end wmMenuWidget::removeMenuWin */

void wmMenuWidget::nabWindow(){
}  /* end wmMenuWidget::nabWindow */

