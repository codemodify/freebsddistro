
// Qt headers
#include <QtGui/QApplication>

// blocks
#include<Pluginer/PluginManager.h>

// local headers
#include "Architecture.Ui.h"


Ui::Ui( Shapes& model, IRendererFactory& view ) :
    QMdiSubWindow(),
    _model( model ),
    _view( view )
{
    setWidget( _view.getWidget() );
}
