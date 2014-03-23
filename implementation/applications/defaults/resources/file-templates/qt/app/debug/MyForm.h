#ifndef __MyForm_H__
#define __MyForm_H__

// Qt headers
#include <QtGui/QWidget>

class MyForm : public QWidget
{
    Q_OBJECT

    // default stuff
    public:
        MyForm();
        ~MyForm();
        
    private slots:
        void showAbout();

};

#endif

