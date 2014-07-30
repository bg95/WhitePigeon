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
};
