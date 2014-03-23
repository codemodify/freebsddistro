#ifndef __Architecture_Shapes_IShape_H__
#define __Architecture_Shapes_IShape_H__

// Qt headers
#include <QtCore/QString>
#include <QtCore/QPoint>
#include <QtCore/QSize>
#include <QtGui/QFont>
#include <QtGui/QColor>

namespace Shape{


class IShape
{
    public:
        virtual void setText     ( QString text     ) = 0;
        virtual void setTextFont ( QFont   font     ) = 0;
        virtual void setTextColor( QColor  color    ) = 0;
        virtual void setColor    ( QColor  color    ) = 0;
        virtual void setPosition ( QPoint  position ) = 0;
        virtual void setSize     ( QSize   size     ) = 0;

    public:
        virtual QString getText     () = 0;
        virtual QFont   getTextFont () = 0;
        virtual QColor  getTextColor() = 0;
        virtual QColor  getColor    () = 0;
        virtual QPoint  getPosition () = 0;
        virtual QSize   getSize     () = 0;
};


}
#endif

