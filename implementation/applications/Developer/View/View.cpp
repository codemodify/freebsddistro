
// local headers
#include "View.h"

View::View() :
    CustomDockWidget()
{
    // preapre the titlebar
    setTitleStyleSheet
    (
        "QWidget {                                                                                              "\
        "   background:     url(:/images/titlebar-background.png);                                              "\
        "   max-height:     25px;                                                                               "\
        "                                                                                                       "\
        "   border-style:   none;                                                                               "\
        "   border-width:   0px;                                                                                "\
        "                                                                                                       "\
        "   margin-top:     0px;                                                                                "\
        "   margin-bottom:  0px;                                                                                "\
        "   margin-left:    0px;                                                                                "\
        "   margin-right:   0px;                                                                                "\
        "                                                                                                       "\
        "   spacing:        0;                                                                                  "\
        "}                                                                                                      "\
        "QToolButton {                                                                                          "\
        "   border-style:   none;                                                                               "\
        "   border-width:   0px;                                                                                "\
        "   border-radius:  0px;                                                                                "\
        "}                                                                                                      "\
        "QToolButton:hover {                                                                                    "\
        "   background:     url(:/images/titlebar-button-hover-background.png);                                 "\
        "}                                                                                                      "\
        "QToolButton:checked {                                                                                  "\
        "   background:     url(:/images/titlebar-button-checked-background.png);                               "\
        "}                                                                                                      "\
        "QToolButton:pressed {                                                                                  "\
        "   background:     url(:/images/titlebar-button-pressed-background.png);                               "\
        "}                                                                                                      "\
        "QLabel {                                                                                               "\
        "   font-family:    Verdana;                                                                            "\
        "   color:          rgb(255,255,255);                                                                   "\
        "   font-size:      12px;                                                                               "\
        "}"
    );
}

View::~View()
{}

