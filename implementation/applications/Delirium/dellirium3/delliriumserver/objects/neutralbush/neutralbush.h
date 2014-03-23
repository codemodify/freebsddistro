
// local headers
#include "objects/gameobject.h"

class NeutralBush : public GameObject
{

public:
	NeutralBush();
	~NeutralBush();

public: // overrides - to e able to do our job
	void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 );
	QPainterPath shape() const;

};
