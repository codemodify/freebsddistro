
// local headers
#include "Architecture.Shapes.AbstractShape.h"

namespace Shape{


AbstractShape::AbstractShape() :
    IShape()
{}

AbstractShape::~AbstractShape()
{}

void AbstractShape::setText     ( QString text     ){ _text      = text;     }
void AbstractShape::setTextFont ( QFont   font     ){ _textFont  = font;     }
void AbstractShape::setTextColor( QColor  color    ){ _textColor = color;    }
void AbstractShape::setColor    ( QColor  color    ){ _color     = color;    }
void AbstractShape::setPosition ( QPoint  position ){ _position  = position; }
void AbstractShape::setSize     ( QSize   size     ){ _size      = size;     }

QString AbstractShape::getText     (){ return _text;      }
QFont   AbstractShape::getTextFont (){ return _textFont;  }
QColor  AbstractShape::getTextColor(){ return _textColor; }
QColor  AbstractShape::getColor    (){ return _color;     }
QPoint  AbstractShape::getPosition (){ return _position;  }
QSize   AbstractShape::getSize     (){ return _size;      }
 

}
