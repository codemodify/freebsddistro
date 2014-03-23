#ifndef __PageFormat_H__
#define __PageFormat_H__


namespace PageFormat
{

	typedef enum
	{
		eBegin=0, 

		eA4 = 0,

		eEnd,
		eCount=eEnd

	} Format;

	typedef struct 
	{
		qreal _x;
		qreal _y;
		qreal _w;
		qreal _h;
	} Size;

	typedef struct _A4_ : public Size 
	{
		_A4_()
		{
			_x = 0;
			_y = 0;
			_w = 793.7;
			_h = 1122.5;
		}
	} A4;
	
};

extern PageFormat::Size* pageFormats[ PageFormat::eCount ];

#endif
