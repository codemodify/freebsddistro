#define CONFDIR "/usr/local/qlwm/files/"
#define DVERSION "4.0\n"

// virtual desktops
#define MAXDESKS 10

// Qt headers
#include <QApplication>
#include <QPushButton>
#include <QLayout>
#include <QList>
#include <QStack>
#include <QMap>
#include <QTimer>
#include <QLabel>
#include <QDateTime>
#include <QPainter>
#include <QPalette>
#include <QCheckBox>
#include <QVariant>
#include <QCursor>
#include <QTextStream>
#include <QFile>
#include <QFileInfo>
#include <QColor>
#include <QRegion>
#include <QBitmap>
#include <QImage>
#include <QPixmap>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QHash>
#include <QSizePolicy>
#include <QAction>
#include <QVariant>
#include <QMenu>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QWidget>
#include <QByteArray>
#include <QBrush>
#include <QFrame>
#include <QX11Info>
#include <QDesktopWidget>

// system headers
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

// X11 headers
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/X.h>
#include <X11/Xutil.h>
#include <X11/extensions/shape.h>

// event names
static char *event_names[] = {
"",
"",
"KeyPress",
"KeyRelease",
"ButtonPress",
"ButtonRelease",
"MotionNotify",
"EnterNotify",
"LeaveNotify",
"FocusIn",
"FocusOut",
"KeymapNotify",
"Expose",
"GraphicsExpose",
"NoExpose",
"VisibilityNotify",
"CreateNotify",
"DestroyNotify",
"UnmapNotify",
"MapNotify",
"MapRequest",
"ReparentNotify",
"ConfigureNotify",
"ConfigureRequest",
"GravityNotify",
"ResizeRequest",
"CirculateNotify",
"CirculateRequest",
"PropertyNotify",
"SelectionClear",
"SelectionRequest",
"SelectionNotify",
"ColormapNotify",
"ClientMessage",
"MappingNotify"
};

