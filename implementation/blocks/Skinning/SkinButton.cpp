/*
 * WengoPhone, a voice over Internet phone
 * Copyright (C) 2004-2005  Wengo
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "SkinButton.h"

#include <QtGui/QWidget>
#include <QtGui/QPainter>
#include <QtGui/QPixmap>

#include <iostream>
using namespace std;

SkinButton::SkinButton(QWidget * button) : SkinWidget(button) {
}

void SkinButton::setPixmaps(QPixmap * regular, QPixmap * active, QPixmap * hover, QPixmap * disable) {
	if (!regular || regular->isNull()) {
		delete regular;
		regular = new QPixmap(_widget->width(), _widget->height());
		regular->fill(_widget->parentWidget(), _widget->x(), _widget->y());
	}
	SkinWidget::setPixmaps(regular, active, hover, disable);
}
