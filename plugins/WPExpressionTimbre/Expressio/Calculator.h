#include <iostream>
#include <ctype.h>
#include <cmath>
#include <algorithm>
#include <boost/math/constants/constants.hpp>

#include "Expressio.hpp"

class Calculator : public Expressio::Evaluator<double>
{
public:
	Calculator();
	void setExpression(const std::string &expression);
	double evaluate() const;
private:
	std::vector<PostfixToken> postfix;
	std::string expression;
};
