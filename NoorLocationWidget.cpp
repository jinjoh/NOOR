#include "NoorLocationWidget.h"
#include "NoorLocationWidget.moc"

NoorLocationWidget::NoorLocationWidget() : Marble::MarbleWidget()
{
	zoomView(1000);
	//goHome();

	setFixedWidth(256);
	setFixedHeight(256);
	setMapThemeId( 0 );
	setAnimationsEnabled( true );
	zoomViewBy(350);
}
