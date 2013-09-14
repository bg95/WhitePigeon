#ifndef _WPSINGINGVOICEANALYSER_H
#define _WPSINGINGVOICEANALYSER_H
#include "../core/WPWave.h"
#include "../WPScore/WPLib.h"

class WPSingingVoiceAnalyser: public QObject
{
	Q_OBJECT
	public:
		WPSingingVoiceAnalyser();
		~WPSingingVoiceAnalyser();
		void __test();
		void __test3();
	private:
		WPWave W;
		double Fun(double);
		std::map <double, double> Ob;
	public slots:
		void __test2();
};

#endif
