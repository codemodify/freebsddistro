#ifndef __Architecture_Shapes_AbstractShape_H__
#define __Architecture_Shapes_AbstractShape_H__

// local headers
#include "Architecture.Shapes.IShape.h"

namespace Shape{


class AbstractShape : public IShape
{
    public:
        AbstractShape();
        virtual ~AbstractShape();

    public:
        virtual void setText     ( QString text     );
        virtual void setTextFont ( QFont   font     );
        virtual void setTextColor( QColor  color    );
        virtual void setColor    ( QColor  color    );
        virtual void setPosition ( QPoint  position );
        virtual void setSize     ( QSize   size     );

    public:
        QString getText     ();
        QFont   getTextFont ();
        QColor  getTextColor();
        QColor  getColor    ();
        QPoint  getPosition ();
        QSize   getSize     ();

    protected:
        QString _text;
        QFont   _textFont;
        QColor  _textColor;

        QColor  _color;

        QPoint  _position;
        QSize   _size;
};


}
#endif
