#ifndef _WPSIMPLIFIEDCHINESETOPINYIN_H
#define _WPSIMPLIFIEDCHINESETOPINYIN_H
#include <QtGui>
#include <QtNetwork>
#include <QtCore>
#include <QtWebKit>
#include <QtWebKitWidgets/QWebFrame>
#include <QtWebKitWidgets/QWebPage>
#include <QtWebKitWidgets/QWebView>
#include "../WPScore/WPLib.h"

class WPSimplifiedChineseToPinyin
{
	public:
		WPSimplifiedChineseToPinyin();
		~WPSimplifiedChineseToPinyin();
		QString getPinyin(const QString &);
		static char toToneless(const QChar &);
		std::vector <std::string> parseTonelessPinyin(const QString &);
		void writeCodeAccordingToWeb();
	private:
		QWebView MyWebView;
		TrieNode <char> *TrieRoot;
};

#endif
