#include "WPProperty.h"

WPProperty::WPProperty()
{
}

WPProperty::WPProperty(const WPInterval &I, const std::string &S)
{
	Interval = I;
	Arg = S;
}

WPProperty::~WPProperty()
{
}

WPInterval WPProperty::getInterval() const
{
	return Interval;
}

std::string WPProperty::getArg() const
{
	return Arg;
}

void WPProperty::lengthen(const Fraction &F)
{
	Interval.lengthen(F);
}

void WPProperty::shiftRight(const Fraction &F)
{
	Interval.shiftRight(F);
}

bool WPProperty::operator < (const WPProperty &P) const
{
	if (Interval.begin() < P.getInterval().begin())
		return 1;
	if (Interval.begin() == P.getInterval().begin() && Interval.end() < P.getInterval().end())
		return 1;
	if (Interval.begin() == P.getInterval().begin() && Interval.end() == P.getInterval().end() && Arg < P.getArg())
		return 1;
	return 0;
}

QString WPProperty::setEntry(const std::string &O, const QString &K)
{
	QString Result = getEntry(O);
	deleteEntry(O);
	QByteArray BA = K.toUtf8();
	BA = BA.toPercentEncoding();
	std::string Key = BA.constData();
	Arg += "--" + O + "=" + Key + " ";
	return Result;
}

void WPProperty::deleteEntry(const std::string &O)
{
	if (!existEntry(O))
		return;
	std::size_t Pos = FindEntry(O);
	std::string Remain = Arg.substr(0, Pos);
	std::size_t NextPos = Arg.find(' ', Pos + 1);
	Arg = Remain + Arg.substr(NextPos);
}

bool WPProperty::existEntry(const std::string &O) const
{
	if (FindEntry(O) == std::string::npos)
		return 0;
	return 1;
}

QString WPProperty::getEntry(const std::string &O) const
{
	if (!existEntry(O))
		return "";
	std::size_t Pos = FindEntry(O);
	std::size_t NextPos = Arg.find(' ', Pos + 1);
	std::size_t EqPos = Arg.find('=', Pos + 1);
	std::string Key = Arg.substr(EqPos + 1, NextPos - EqPos - 1);
	QByteArray BA(Key.c_str(), Key.size());
	BA = QByteArray::fromPercentEncoding(BA);
	return QString::fromUtf8(BA);
}

std::size_t WPProperty::FindEntry(const std::string &O) const
{
	return Arg.find(" --" + O + "=");
}

std::vector <WPProperty> filterPrefix(const std::vector <WPProperty> &VP, const std::string &Pfx)
{
	std::vector <WPProperty> Result;
	for (std::vector <WPProperty> :: const_iterator it = VP.begin(); it != VP.end(); ++ it)
		if (it->getArg().find(Pfx) == 0)
			Result.push_back(*it);
	return Result;
}
