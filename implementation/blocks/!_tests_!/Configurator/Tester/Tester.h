#ifndef __Tester_H__
#define __Tester_H__

// Qt headers
#include <QtGui/QWidget>

// blocks
#include <Configurator/Configurator.h>


class Tester : public QWidget
{
    Q_OBJECT

    public:
        Tester();
        ~Tester();

    private slots:
        void slot_chooseConfigurationFile();

};

#endif
