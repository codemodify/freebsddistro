#ifndef SKINBUTTON_H
#define SKINBUTTON_H

#include "SkinWidget.h"

/**
 * A skinned button.
 *
 * @see http://www.codeskipper.com/article/1/
 * @author Tanguy Krotoff
 */
class SkinButton : public SkinWidget {
	Q_OBJECT
public:

	SkinButton(QWidget * button);

	virtual void setPixmaps(QPixmap * regular, QPixmap * active, QPixmap * hover, QPixmap * disable);
};

#endif	//SKINBUTTON_H
