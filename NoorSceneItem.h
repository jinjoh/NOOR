#ifndef NOOR_SCENE_ITEM_H
#define NOOR_SCENE_ITEM_H

#include <QObject>
#include <QSqlQuery>
#include <QVariant>

class NoorSceneItem : public QObject
{

	Q_OBJECT

	public:
	
	NoorSceneItem();

	virtual void load(int id){};
	virtual void newItem(){};
	virtual void generateThumbnail(){};
	virtual QWidget* view(){};
	virtual QWidget* edit(){};
	virtual QPixmap *thumbnail(){};

	virtual QString content(){};
	virtual QString type(){};	

};

#endif
