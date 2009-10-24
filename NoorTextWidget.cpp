#include "NoorTextWidget.h"
#include "NoorTextWidget.moc"

NoorTextWidget::NoorTextWidget(QWidget * parent) : QWebView(parent)
{
	//webview = new QWebView;
	//setParent(0);
	setFixedWidth(256);
	setFixedHeight(256);

}

void NoorTextWidget::setContent(QString content)
{
	/*webview->*/setHtml("<html><head><title>Noor</title></head><body bgcolor='#c3d8f7'><table width='100%' height='100%' valign='middle'><tr><td><center><font face='Furat'>" + 
content +
"</font></center></td></tr></table></body></html>");
	setTextSizeMultiplier(2);

	//webview->show();
	qDebug() << "NoorTextWidget: We will set the content here";

}
