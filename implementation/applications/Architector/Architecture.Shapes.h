#ifndef __Architecture_Shapes_H__
#define __Architecture_Shapes_H__

// Qt headers
#include <QtCore/QObject>
#include <QtCore/QStringList>


class Shapes : public QObject
{
    Q_OBJECT

    public:
        Shapes();

    public:
        static Shapes* loadFromFile( QString filePath );
        void saveToFile( QString filePath );

    // Global area
    public:
        QString getRenderer();

    private:
        QString _renderer;

    // Problem area
    // FIXME: ...

    // Analyze area
    // FIXME: ...

    // Implementation area
    signals:
        void classAdded( QString className );
        void classRemoved( QString className );

    private:
        QStringList _classes;
};

#endif
