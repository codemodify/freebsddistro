
// local headers
#include "objects/gameobject.h"

class MeTarget : public GameObject
{

public:
	MeTarget();
	~MeTarget();

public: // overrides - to e able to do our job
	void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 );
	QPainterPath shape() const;

};
