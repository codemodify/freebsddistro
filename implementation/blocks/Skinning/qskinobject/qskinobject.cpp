/********************************************************************************************************
 * PROGRAM      : childform
 * DATE - TIME  : Samstag 30 Dezember 2006 - 12h04
 * AUTHOR       :  (  )
 * FILENAME     : QSkinObject.cpp
 * LICENSE      : 
 * COMMENTARY   : 
 ********************************************************************************************************/
#include <QtGui>
#include <cmath>

#include "qskinobject.h"



QSkinObject::QSkinObject( QWidget* wgtParent)
    : QObject()
   
{
	// Widget which will be skinned
	skinWidget = wgtParent;
	contentsRect = skinWidget->rect();
	// Save flags for later unsset the skin
	flags = skinWidget->windowFlags();
#ifdef WIN32
	wlong = GetWindowLong(skinWidget->winId(), GWL_EXSTYLE);
#endif
	// do some initialisation
	skinTimer = new QBasicTimer();
	widgetMask = QPixmap(skinWidget->width()+12, skinWidget->height()+12);
	mousePress = true;

	
}

void QSkinObject::startSkinning()
{
	
	loadSkinIni();
	QPoint pos = skinWidget->pos();
	skinWidget->setAttribute(Qt::WA_PaintOnScreen);
	skinWidget->setAttribute(Qt::WA_NoSystemBackground);
	//no windows frame
	skinWidget->setWindowFlags(Qt::FramelessWindowHint);
	//loadSkinIni();
	
	
	//mouse tracking on, because the close-button should glow if the cursor comes over it
	skinWidget->setMouseTracking(true);
	
	skinWidget->setContentsMargins ( QPixmap(skinPath + "sl.png").width(), QPixmap(skinPath + "so.png").height(), QPixmap(skinPath + "sr.png").width(), QPixmap(skinPath + "su.png").height());
	
	skinWidget->installEventFilter(this);
	resizeHandler =  new QSkinWidgetResizeHandler(this, skinWidget);
#ifdef WIN32
	alpha = 255;
	setLayered();
#endif
	skinTimer->start(100, this);
	skinWidget->show();
	skinWidget->move(pos);
}

void QSkinObject::stopSkinning()
{
	skinWidget->setContentsMargins ( 5, 5, 5, 5);
	skinWidget->clearMask();
	skinWidget->setAttribute(Qt::WA_PaintOnScreen, false);
	skinWidget->setAttribute(Qt::WA_NoSystemBackground, false);
#ifdef WIN32
	SetWindowLong(skinWidget->winId(), 
              GWL_EXSTYLE, 
              wlong);
#endif
	skinWidget->setWindowFlags(flags);
	skinWidget->removeEventFilter(this);
	skinWidget->removeEventFilter(resizeHandler);
	skinTimer->stop();
	skinWidget->setMouseTracking(false);
	skinWidget->show();
}
bool QSkinObject::eventFilter(QObject *o, QEvent *e)
{
	
	if(e->type() == QEvent::MouseButtonPress)
	{
		mousePressEvent(static_cast<QMouseEvent*>(e));
		return false;
	}
	else if(e->type() == QEvent::Resize)
	{
		resizeEvent(static_cast<QResizeEvent*>(e));
		return false;
	}
	else if(e->type() == QEvent::Paint)
	{
		paintEvent(static_cast<QPaintEvent*>(e));
		return false;
	}
	/*else if(e->type() == QEvent::Close)
	{
		closeEvent(static_cast<QCloseEvent*>(e));
		return false;
	}*/
	else
	{
		 //resizeHandler->eventFilter(o, e);
		 return skinWidget->eventFilter(o, e);
	}
}

void QSkinObject::manageRegions()
{
	const int quiWidth = QPixmap(skinPath + "quiButton.png").width();
	const int maxWidth = QPixmap(skinPath + "maxButton.png").width();
	const int minWidth = QPixmap(skinPath + "minButton.png").width();
	const int quiHeight = QPixmap(skinPath + "quiButton.png").height();
	const int maxHeight = QPixmap(skinPath + "maxButton.png").height();
	const int minHeight = QPixmap(skinPath + "minButton.png").height();
	const int rightWidth = QPixmap(skinPath + "sr.png").width();
	quitButton 			= QRegion( QRect(skinWidget->width()-8-quiWidth - rightWidth, 2, quiWidth, quiHeight) );
	maxButton 			= QRegion( QRect(skinWidget->width()-8-quiWidth-maxWidth - rightWidth, 2, maxWidth, maxHeight) );
	minButton 			= QRegion( QRect(skinWidget->width()-8-quiWidth-maxWidth-minWidth - rightWidth, 2, minWidth, minHeight) );
	
		
}

/*void QSkinObject::closeEvent(QCloseEvent *e)
{

}*/

void QSkinObject::resizeEvent(QResizeEvent *e)
{
	manageRegions();
	
	e->ignore();
	//skinWidget->resizeEvent(e);
}

void QSkinObject::mousePressEvent(QMouseEvent *e)
 {
	// winwow Mover
	QPoint pos = e->pos();
	
	//QRect rect = skinWidget->rect();
    if (e->button() == Qt::LeftButton) 
    {
	mousePress = true;
       
		// buttons
		int button = -1;
		int butPos = (QPixmap(skinPath + "so.png").height() - QPixmap(skinPath + "quiButton.png").height()+6);
		if(quitButton.contains(e->pos()))
		 button = 0 ;
		else if(maxButton.contains(e->pos()))
		 button = 1 ;
		else if(minButton.contains(e->pos()))
		 button = 2 ;
		if(pos.x() < skinWidget->width() - 5)
			if(pos.y() < QPixmap(skinPath + "so.png").height() + butPos && pos.y() > 6)
			{
				switch (button) {
					case 0:
					skinWidget->close();
					 e->accept();
					 break;
					case 1:
					 if(skinWidget->isMaximized())
						skinWidget->showNormal();
					 else
						skinWidget->showMaximized();
					 e->accept();
					 break;
					case 2: 
					 skinWidget->showMinimized();
					 e->accept();
					 break;
				}
			}
			else
				e->ignore();
		
    }
    else
	mousePress = false;

 }
/*void QSkinObject::mouseReleaseEvent(QMouseEvent *mouseEvent)
{
	
}
void QSkinObject::mouseMoveEvent(QMouseEvent *e)
{
	
}*/

void QSkinObject::setSkinPath(const QString & skinpath)
{
	skinPath = skinpath;
	skinWidget->update();
	
}
QString QSkinObject::getSkinPath()
{
	return skinPath;
}
void QSkinObject::updateButtons()
{
	QPainter p(&widgetMask);
	p.setRenderHint(QPainter::Antialiasing);
	QPixmap so = QPixmap(skinPath + "so.png");
	QPixmap sl = QPixmap(skinPath + "sl.png");
	QPixmap sr = QPixmap(skinPath + "sr.png");
	QPixmap su = QPixmap(skinPath + "su.png");
	QPixmap min;
	QPixmap max;
	QPixmap qui;
	if(minButton.contains(skinWidget->mapFromGlobal(QCursor::pos())))
	{
		if(!mousePress)
			min = QPixmap(skinPath + "minButton1.png");
		else
			min = QPixmap(skinPath + "minButton2.png");
	}
	else
	{
	 min = QPixmap(skinPath + "minButton.png");
	}

	if(quitButton.contains(skinWidget->mapFromGlobal(QCursor::pos())))
	{
		if(!mousePress)
			qui = QPixmap(skinPath + "quiButton1.png");
		else
			qui = QPixmap(skinPath + "quiButton2.png");
	}
	else
	{
	 qui = QPixmap(skinPath + "quiButton.png");
	}

	if(maxButton.contains(skinWidget->mapFromGlobal(QCursor::pos())))
	{
		if(!mousePress)
			max = QPixmap(skinPath + "maxButton1.png");
		else
			max = QPixmap(skinPath + "maxButton2.png");
	}
	else
	{
	 max = QPixmap(skinPath + "maxButton.png");
	}
	p.drawPixmap(QRect(skinWidget->width() - qui.width() - sr.width() -8, 2, qui.width(), qui.height()),
	qui,
	QRect(0,0, qui.width(), qui.height()));

	p.drawPixmap(QRect(skinWidget->width() - qui.width() - max.width()-8 - sr.width(), 2, max.width(), max.height()),
	max,
	QRect(0,0, max.width(), max.height()));

	p.drawPixmap(QRect(skinWidget->width() - qui.width()-max.width() - min.width()-8 - sr.width(), 2, min.width(), min.height()),
	min,
	QRect(0,0, min.width(), min.height()));
	p.end();
}
void QSkinObject::updateStyle()
{
	
	//this makes the alpha blending work. just gdi funktions and a conversation from QPixmap to HBITMAP. not really interresting
	QPixmap so = QPixmap(skinPath + "so.png");
	QPixmap sl = QPixmap(skinPath + "sl.png");
	QPixmap sr = QPixmap(skinPath + "sr.png");
	QPixmap su = QPixmap(skinPath + "su.png");
	QPixmap qui = QPixmap(skinPath + "quiButton.png");
	
	skinWidget->setMinimumSize (sr.width() + sl.width() + 128, su.height() + so.height());
	
	widgetMask = QPixmap(skinWidget->width(), skinWidget->height());
	widgetMask.fill(Qt::transparent);

	#ifdef WIN32
		if(milchglas)
		{
			QImage tmp = QPixmap::QPixmap::grabWindow(QApplication::desktop()->winId(), 
						 skinWidget->geometry().x(), skinWidget->geometry().y(), skinWidget->rect().width(), skinWidget->rect().height()).toImage();
			fastbluralpha(tmp, 4);
			widgetMask = QPixmap::fromImage(tmp);
		}
	#endif
	QPainter p(&widgetMask);
	
	int lo = skinWidget->width()-(so.width()-68);
	if(lo < 68)
		lo = 68;
	p.drawPixmap(QRect(skinWidget->width()-64, 0,64, so.height()), so, QRect(so.width()-64, 0 , 64, so.height()));
	p.drawPixmap(QRect(0, 0, 64, so.height()), so, QRect(0, 0 , 64, so.height()));
	p.drawPixmap(QRect(64, 0, skinWidget->width()-128, so.height()), so, QRect(64, 0, so.width()-128, so.height()));
	
	//p.drawPixmap(QRect(sl.width(), height()-su.height(), width(), su.height()), su);
	
	p.drawPixmap(QRect(sl.width(), skinWidget->height() - su.height(), skinWidget->width()-sl.width()-sr.width(), su.height()), 
				su, 
				QRect(sl.width(), 0, su.width()-sl.width()-sr.width(), su.height()));
	p.drawPixmap(QRect(0, skinWidget->height() - su.height(), sl.width(), su.height()),
				su, 
				QRect(0, 0 , sl.width(), su.height()));
				
	
	p.drawPixmap(QRect(skinWidget->width()-sr.width(), skinWidget->height() - su.height(), sr.width(), su.height()), 
				su, 
				QRect(su.width()-sr.width(), 0 , sr.width(), su.height()));			
	
	
	
	
	
	p.drawPixmap(QRect(skinWidget->width()-sr.width(), so.height(), sr.width(), skinWidget->height()-so.height()-su.height()), sr);
	
	
	p.drawPixmap(QRect(0, so.height(), sl.width(), skinWidget->height()-su.height()-so.height()), sl);
	
	p.setFont(titleFont);
	p.setPen(QPen(titleColor, 2));
	
	contentsRect = QRect(sl.width(), so.height(), skinWidget->width() - sr.width() - sl.width(), skinWidget->height() - so.height() - su.height());
	p.setBrush(backgroundColor);
	p.setPen(QPen(Qt::black, 1));
	p.drawRect(contentsRect);
	p.drawText(QRect(sl.width(), 0, skinWidget->width()-qui.width()*3-8-sl.width()-sr.width(), so.height()), Qt::AlignVCenter, skinWidget->windowTitle());
	p.end();
	skinWidget->setMask(widgetMask.mask());
	
	updateButtons();
}

void QSkinObject::loadSkinIni()
{
	QSettings s(QApplication::applicationDirPath() + "/skin/skin.dat", QSettings::IniFormat);
	s.beginGroup("Skin");
	milchglas = s.value("Blur", false).toBool();	
	backgroundColor = QColor(s.value("Hintergrundfarbe", Qt::white).toString());
	titleColor = QColor(s.value("Titelfarbe", QColor(255,100,0)).toString());
	QFont font;
	font.fromString(s.value("Titelschrift", QFont("Comic Sans MS")).toString());
	skinPath = QApplication::applicationDirPath() + "/" + s.value("Skinpfad", QApplication::applicationDirPath() + "skin/blau/").toString();
	titleFont = font;
	s.endGroup();
}
void QSkinObject::paintEvent(QPaintEvent *event)
{
	if(skinWidget)
	{
		
		updateStyle();
		QPainter p(&widgetMask);
		QObjectList ol = skinWidget->children();
		foreach(QObject *o, ol)
		{
			if(o->isWidgetType())
			{
				QWidget* child = dynamic_cast<QWidget*>(o);  
				
				p.drawPixmap(child->geometry(),drawCtrl(child ));
				
				
			}
				
		}
		p.end();
#ifdef WIN32
		updateAlpha(); 

#else
		p.begin(skinWidget);
		p.setRenderHint(QPainter::Antialiasing);
		p.drawPixmap(0,0,widgetMask);
		p.end();
#endif
		
	}

}

QPixmap QSkinObject::drawCtrl(QWidget * widget)
{

	if (!widget)
		return QPixmap();
	QRect r(widget->rect());
	QPixmap res(r.size());
	widget->render(&res, -r.topLeft(), r, QWidget::DrawWindowBackground | QWidget::DrawChildren | QWidget::IgnoreMask);

	return res;

} 

void QSkinObject::timerEvent ( QTimerEvent * event )
{
	if(event->timerId() == skinTimer->timerId())
		skinWidget->repaint();
}

int QSkinObject::customFrameWidth()
{
	return QPixmap(skinPath + "sl.png").width();
}

#ifdef WIN32
void QSkinObject::updateAlpha()
{
	
	//this makes the alpha blending work. just gdi funktions and a conversation from QPixmap to HBITMAP. not really interresting
	
		
	 HBITMAP oldBitmap;
	 HBITMAP hBitmap;	
     SIZE size;
     size.cx = widgetMask.width();
     size.cy = widgetMask.height();
     HDC screenDc = GetDC(NULL);
     POINT pointSource;
     pointSource.x = 0;
     pointSource.y = 0; 
     POINT topPos;
     topPos.x = skinWidget->x();
     topPos.y = skinWidget->y();	
     HDC memDc = CreateCompatibleDC(screenDc);
     BLENDFUNCTION blend;
     blend.BlendOp             = AC_SRC_OVER;
     blend.BlendFlags          = 0;
     blend.SourceConstantAlpha = 255;
     blend.AlphaFormat         = AC_SRC_ALPHA;
	 hBitmap = widgetMask.toWinHBITMAP(QPixmap::PremultipliedAlpha);  // grab a GDI handle from this GDI+ bitmap
	 oldBitmap = (HBITMAP)SelectObject(memDc, hBitmap);
	 
	UpdateLayeredWindow(skinWidget->winId(), screenDc,  &topPos,  &size, memDc,  &pointSource, 0, &blend, ULW_ALPHA);
	
	// 
	ReleaseDC( NULL, screenDc);
	if (hBitmap != NULL)
	{
		SelectObject(memDc, oldBitmap);
		//DeleteObject(hBitmap); // The documentation says that we have to use the Windows.DeleteObject... but since there is no such method I use the normal DeleteObject from Win32 GDI and it's working fine without any resource leak.
		DeleteObject(hBitmap);
	}
	DeleteDC(memDc); 
}

void QSkinObject::setLayered()
{
	SetWindowLong(skinWidget->winId(), 
              GWL_EXSTYLE, 
              GetWindowLong(skinWidget->winId(), GWL_EXSTYLE) | WS_EX_LAYERED); //make shure the QAlphaWidget is realy a windows QAlphaWidget form
}
#endif
void QSkinObject::fastbluralpha(QImage &img, int radius)
{
    if (radius < 1) {
        return;
    }

    QRgb *pix = (QRgb*)img.bits();
    int w   = img.width();
    int h   = img.height();
    int wm  = w-1;
    int hm  = h-1;
    int wh  = w*h;
    int div = radius+radius+1;

    int *r = new int[wh];
    int *g = new int[wh];
    int *b = new int[wh];
    int *a = new int[wh];
    int rsum, gsum, bsum, asum, x, y, i, yp, yi, yw;
    QRgb p;
    int *vmin = new int[qMax(w,h)];

    int divsum = (div+1)>>1;
    divsum *= divsum;
    int *dv = new int[256*divsum];
    for (i=0; i < 256*divsum; ++i) {
        dv[i] = (i/divsum);
    }

    yw = yi = 0;

    int **stack = new int*[div];
    for(int i = 0; i < div; ++i) {
        stack[i] = new int[4];
    }


    int stackpointer;
    int stackstart;
    int *sir;
    int rbs;
    int r1 = radius+1;
    int routsum, goutsum, boutsum, aoutsum;
    int rinsum, ginsum, binsum, ainsum;

    for (y = 0; y < h; ++y){
        rinsum = ginsum = binsum = ainsum
               = routsum = goutsum = boutsum = aoutsum
               = rsum = gsum = bsum = asum = 0;
        for(i =- radius; i <= radius; ++i) {
            p = pix[yi+qMin(wm,qMax(i,0))];
            sir = stack[i+radius];
            sir[0] = qRed(p);
            sir[1] = qGreen(p);
            sir[2] = qBlue(p);
            sir[3] = qAlpha(p);
            
            rbs = r1-abs(i);
            rsum += sir[0]*rbs;
            gsum += sir[1]*rbs;
            bsum += sir[2]*rbs;
            asum += sir[3]*rbs;
            
            if (i > 0){
                rinsum += sir[0];
                ginsum += sir[1];
                binsum += sir[2];
                ainsum += sir[3];
            } else {
                routsum += sir[0];
                goutsum += sir[1];
                boutsum += sir[2];
                aoutsum += sir[3];
            }
        }
        stackpointer = radius;

        for (x=0; x < w; ++x) {

            r[yi] = dv[rsum];
            g[yi] = dv[gsum];
            b[yi] = dv[bsum];
            a[yi] = dv[asum];

            rsum -= routsum;
            gsum -= goutsum;
            bsum -= boutsum;
            asum -= aoutsum;

            stackstart = stackpointer-radius+div;
            sir = stack[stackstart%div];

            routsum -= sir[0];
            goutsum -= sir[1];
            boutsum -= sir[2];
            aoutsum -= sir[3];

            if (y == 0) {
                vmin[x] = qMin(x+radius+1,wm);
            }
            p = pix[yw+vmin[x]];

            sir[0] = qRed(p);
            sir[1] = qGreen(p);
            sir[2] = qBlue(p);
            sir[3] = qAlpha(p);

            rinsum += sir[0];
            ginsum += sir[1];
            binsum += sir[2];
            ainsum += sir[3];

            rsum += rinsum;
            gsum += ginsum;
            bsum += binsum;
            asum += ainsum;

            stackpointer = (stackpointer+1)%div;
            sir = stack[(stackpointer)%div];

            routsum += sir[0];
            goutsum += sir[1];
            boutsum += sir[2];
            aoutsum += sir[3];

            rinsum -= sir[0];
            ginsum -= sir[1];
            binsum -= sir[2];
            ainsum -= sir[3];

            ++yi;
        }
        yw += w;
    }
    for (x=0; x < w; ++x){
        rinsum = ginsum = binsum = ainsum 
               = routsum = goutsum = boutsum = aoutsum 
               = rsum = gsum = bsum = asum = 0;
        
        yp =- radius * w;
        
        for(i=-radius; i <= radius; ++i) {
            yi=qMax(0,yp)+x;

            sir = stack[i+radius];

            sir[0] = r[yi];
            sir[1] = g[yi];
            sir[2] = b[yi];
            sir[3] = a[yi];

            rbs = r1-abs(i);

            rsum += r[yi]*rbs;
            gsum += g[yi]*rbs;
            bsum += b[yi]*rbs;
            asum += a[yi]*rbs;

            if (i > 0) {
                rinsum += sir[0];
                ginsum += sir[1];
                binsum += sir[2];
                ainsum += sir[3];
            } else {
                routsum += sir[0];
                goutsum += sir[1];
                boutsum += sir[2];
                aoutsum += sir[3];
            }

            if (i < hm){
                yp += w;
            }
        }

        yi = x;
        stackpointer = radius;

        for (y=0; y < h; ++y){
            pix[yi] = qRgba(dv[rsum], dv[gsum], dv[bsum], dv[asum]);

            rsum -= routsum;
            gsum -= goutsum;
            bsum -= boutsum;
            asum -= aoutsum;

            stackstart = stackpointer-radius+div;
            sir = stack[stackstart%div];

            routsum -= sir[0];
            goutsum -= sir[1];
            boutsum -= sir[2];
            aoutsum -= sir[3];

            if (x==0){
                vmin[y] = qMin(y+r1,hm)*w;
            }
            p = x+vmin[y];

            sir[0] = r[p];
            sir[1] = g[p];
            sir[2] = b[p];
            sir[3] = a[p];

            rinsum += sir[0];
            ginsum += sir[1];
            binsum += sir[2];
            ainsum += sir[3];

            rsum += rinsum;
            gsum += ginsum;
            bsum += binsum;
            asum += ainsum;

            stackpointer = (stackpointer+1)%div;
            sir = stack[stackpointer];

            routsum += sir[0];
            goutsum += sir[1];
            boutsum += sir[2];
            aoutsum += sir[3];

            rinsum -= sir[0];
            ginsum -= sir[1];
            binsum -= sir[2];
            ainsum -= sir[3];

            yi += w;
        }
    }
    delete [] r;
    delete [] g;
    delete [] b;
    delete [] a;
    delete [] vmin;
    delete [] dv;

    for(int i = 0; i < div; ++i) {
        delete [] stack[i];
    }
    delete [] stack;
}
