
#include "main/main.cpp"

int main( int argc, char ** argv )
{

  initdevel();
  MyApp a(argc, argv,"kwm");
  a.setStyle(MotifStyle);

  OperationsHandler op; 

  QBitmap tmpbm;

//  default_miniicon = new QPixmap;
//  default_miniicon->load("default_miniicon.bmp");
//  tmpbm.load("default_miniicon.xbm");
//  default_miniicon->setMask(tmpbm);

   operations  = new QPopupMenu (); 
   CHECK_PTR( operations );
   operations->insertItem( "(Un-)Maximize", OP_MAXIMIZE);
   operations->insertItem( "Iconify", OP_ICONIFY);
   operations->insertItem( "Move", OP_MOVE);
   operations->insertItem( "Reshape", OP_RESHAPE);
   operations->insertSeparator();
   operations->insertItem( "Destroy", OP_DESTROY);
   QObject::connect(operations, SIGNAL(activated(int)), &op, SLOT(handle_operations_popup(int)));


  dpy = qt_xdisplay();
  initkwm(argc, argv);
  return a.exec();

}

