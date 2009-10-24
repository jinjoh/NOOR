#ifndef NOOR_TEXT_WIDGET_H
#define NOOR_TEXT_WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QWebView>

class NoorTextWidget : public QWebView
{

	Q_OBJECT

	public:

	NoorTextWidget(QWidget * parent = 0);
	void setContent(QString content);

	
	//private:
	//QWebView *webview;


};


#endif
