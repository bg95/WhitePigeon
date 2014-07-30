LIBPATH = -L ../../lib
LIBS = -lWPScore -lWPPlugin
INCLUDEPATH = -I ../.. -I ../../include -I ../../include/core -I .. -I /usr/include/qt5 -I /usr/include/qt5/QtCore -I /usr/include/qt5/QtMultimedia
OPT = -std=c++11 -O2
PLUGINNAME = WPExpressionTimbre
CPPS = $(PLUGINNAME).cpp ../../core/WPTimbre.cpp Expressio/Calculator.cpp


../lib$(PLUGINNAME).so : $(CPPS)
	g++ $(CPPS) -fPIC -shared -o ../lib$(PLUGINNAME).so $(LIBPATH) $(LIBS) $(INCLUDEPATH) $(OPT)
