#ifndef _WPLIB_H
#define _WPLIB_H

#include <algorithm>
#include <iostream>
#include <fstream>
#include <climits>
#include <cstdlib>
#include <vector>
#include <cstdio>
#include <cmath>

template <class T> T checkmax(T &, T);
template <class T> T checkmin(T &, T);
template <class T> T GCD(T, T);
template <class T> T checkmax(T &a, T b)
{
	return (a > b) ? a : a = b;
}

template <class T> T checkmin(T &a, T b)
{
	return (a < b) ? a : a = b;
}

template <class T> T GCD(T a, T b)
{
	if (a < 0)
		a = - a;
	if (b < 0)
		b = - b;
	while (a != 0)
	{
		T c = b % a;
		b = a;
		a = c;
	}
	return b;
}

class Fraction
{
	public:
		Fraction();
		Fraction(const double &);
		Fraction(const int &);
		Fraction(const int &, const int &);
		~Fraction();
		bool operator == (const Fraction &) const;
		bool operator < (const Fraction &) const;
		bool operator <= (const Fraction &) const;
		bool operator > (const Fraction &) const;
		bool operator >= (const Fraction &) const;
		Fraction operator = (const Fraction &);
		Fraction operator - () const;
		Fraction operator + (const Fraction &) const;
		Fraction operator += (const Fraction &);
		Fraction operator - (const Fraction &) const;
		Fraction operator -= (const Fraction &);
		Fraction operator * (const Fraction &) const;
		Fraction operator *= (const Fraction &);
		Fraction operator / (const Fraction &) const;
		Fraction operator /= (const Fraction &);
		Fraction inverse() const;
		double toDouble() const;
		int X, Y;
};

template <class T> class TrieNode
{
	public:
		enum OperationMode{VISIT, EDIT};
		TrieNode();
		~TrieNode();
        TrieNode *go(const T &, const OperationMode = TrieNode::VISIT);
        TrieNode *go(const std::vector <T> &, const OperationMode = TrieNode::VISIT);
		void endanger();
		bool isDangerous();
		void freeAll(); // be careful!
	private:
		bool Jeopardy;
		std::vector < std::pair <T, TrieNode <T> *> > Kids;
};

Fraction stringToFraction(const std::string &);
Fraction doubleToFraction(const double &); // warning!!!
std::string intToString(const int &);
std::string fractionToString(const Fraction &);
int ran();

template <class T> TrieNode <T>::TrieNode()
{
	Jeopardy = 0;
	Kids.clear();
}

template <class T> TrieNode <T>::~TrieNode()
{
	Kids.clear();
}

template <class T> TrieNode <T> *TrieNode <T>::go(const T &Ch, const TrieNode::OperationMode Mode)
{
	TrieNode *Result = NULL;
	for (typename std::vector < std::pair <T, TrieNode <T> *> >::iterator it = Kids.begin(); it != Kids.end(); ++ it)
		if (it->first == Ch)
		{
			Result = it->second;
            break;
		}
	if (Mode == TrieNode::EDIT && !Result)
		Result = new TrieNode;
	Kids.push_back(std::make_pair(Ch, Result));
	return Result;
}

template <class T> TrieNode <T> *TrieNode <T>::go(const std::vector <T> &Str, const TrieNode::OperationMode Mode)
{
	TrieNode *Result = this;
	for (typename std::vector <T>::const_iterator it = Str.begin(); Result && it != Str.end(); ++ it)
		Result = Result->go(*it, Mode);
	return Result;
}

template <class T> void TrieNode <T>::endanger()
{
	Jeopardy = 1;
}

template <class T> bool TrieNode <T>::isDangerous()
{
	return Jeopardy;
}

template <class T> void TrieNode <T>::freeAll()
{
	for (typename std::vector < std::pair <T, TrieNode <T> *> >::iterator it = Kids.begin(); it != Kids.end(); ++ it)
		it->second->freeAll();
	delete this;
}

#endif
