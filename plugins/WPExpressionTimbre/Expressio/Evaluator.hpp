#ifndef EVALUATOR_HPP
#define EVALUATOR_HPP

#include <algorithm>
#include <boost/optional.hpp>
#include <boost/variant.hpp>
#include <cmath>
#include <ctype.h>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include "Function.hpp"

namespace Expressio {

/**
 * An expression evaluator class which can evaluate an expression consisting of (ValueType)s.
 *
 * The following functions have to be defined for ValueType:
 *
 * @code
 * std::istream& operator>>(std::istream&, ValueType&);
 * ValueType operator+(const ValueType&, const ValueType&);
 * ValueType operator-(const ValueType&, const ValueType&);
 * ValueType operator*(const ValueType&, const ValueType&);
 * ValueType operator/(const ValueType&, const ValueType&);
 * ValueType operator-(const ValueType&);
 * ValueType std::pow(const ValueType&, const ValueType&);
 * @endcode
 *
 * All builtin integer types (`int`, `short`, ...), floating point types (`float`, `double`, ...) 
 * and complex types (`std::complex<double>`, ...) can naturally be used to instantiate `Evaluator`,
 * as they provide the required interface.
 */
template<class ValueType>
class Evaluator
{
public:
	using ArgumentIterator = typename std::vector<ValueType>::const_iterator;
	
	/**
	 * A type which can constructed with any callable object of the form:
	 * @code ValueType(ValueType...) @endcode
	 */
	using Function = Detail::Function<ValueType, ArgumentIterator>;

	/**
	 * Add user-defined function to the evaluator
	 *
	 * @param funcName The name of the function
	 * @throws std::invalid_argument Constant with same name exists
	 * @param function The function
	 */
	void addFunction(const std::string& funcName, const Function& function)
	{
		if (constantsMap.find(funcName) != constantsMap.end())
			throw std::invalid_argument("Constant with same name (" + funcName + ") exists");
		
		functionMap.emplace(funcName, function);
	}
	
	/**
	 * Add user-defined constant to the evaluator
	 *
	 * @param constantName The name of the constant
	 * @throws std::invalid_argument Function with same name exists
	 * @param value The constant's value
	 */
	void addConstant(const std::string& constantName, const ValueType& value)
	{
		if (functionMap.find(constantName) != functionMap.end())
			throw std::invalid_argument("Function with same name (" + constantName + ") exists");
		
		constantsMap.emplace(constantName, value);
	}
	
	/**
	 * Set user-defined function to the evaluator, overwrite existing function with the same name
	 *
	 * @param funcName The name of the function
	 * @throws nothing
	 * @param function The function
	 */
	void setFunction(const std::string& funcName, const Function& function)
	{
		//functionMap.insert(funcName, function);
		functionMap[funcName] = function;
	}
	
	/**
	 * Set user-defined constant to the evaluator, overwrite existing constant with the same name
	 *
	 * @param constantName The name of the constant
	 * @throws nothing
	 * @param value The constant's value
	 */
	void setConstant(const std::string& constantName, const ValueType& value)
	{
		//constantsMap.insert(constantName, value);
		constantsMap[constantName] = value;
	}
	
	/**
	 * Evaluate the expression
	 *
	 * @param expression The expression to evaluate
	 * @throws std::invalid_argument Invalid or erroneous expression
	 * @return the result of the evaluation
	 */
	ValueType evaluate(const std::string& expression) const
	{
		return evaluatePostfix(infixToPostfix(tokenize(expression)));		
	}

//private:
protected:
	struct Comma { };      ///< Represents a comma
	struct LeftParen { };  ///< Represents a left parenthesis
	struct RightParen { }; ///< Represents a right parenthesis

	/**
	 * The Operator struct represents either a unary or binary operator.
	 *
	 * An operator is basically a specialized form of a function, so it naturally
	 * derives from Function.
	 */
	struct Operator : public Function
	{
		int precedence; ///< The precedence of the operator
		
		/// Associativity type
		enum class Associativity
		{
			left, ///< Operators of equal precedence will be evaluated from left to right
			right ///< Operators of equal precedence will be evaluated from right to left
		} associativity; ///< The associativity of the operator

		/**
		 * Construct an operator
		 * 
		 * @param function The underlying callable object
		 * @param precedence The precedence of the operator
		 * @param a The associativity of the operator
		 */
		template<class Func>
		Operator(const Func& function, int precedence, Associativity a) :
			Function(function), precedence(precedence), associativity(a) { }
	};

	/**
	 * The ConstantName struct represents a constant name.
	 */
	struct ConstantName : public std::string
	{
		ConstantName(std::string str) :
			std::string(str) { }
	};

	/// A string to operator dictionary. Same for all Evaluators, and therfore static (and constant).
	static const std::unordered_map<std::string, Operator> operatorMap;
	
	/// A string to function dictionary. Can be modified during the lifetime of the Evalutor.
	/// Each user-defined function has to be present here before evaluation.
	std::unordered_map<std::string, Function> functionMap;
	
	/// A constant to value dictionary. Can be modified during the lifetime of the Evalutor.
	/// Each user-defined constant has to be present here before evaluation.
	std::unordered_map<std::string, ValueType> constantsMap;

	/// A token in an infix expression. This can be either a value, a comma, a parenthesis, an operator or a function.
	/// Or the name of a constant
	using InfixToken = boost::variant<ValueType, ConstantName, Comma, LeftParen, RightParen, Operator, Function>;
	
	/// A token in a postfix expression. This can be either a value or a function
	/// Or the name of a constant
	using PostfixToken = boost::variant<ValueType, ConstantName, Function>;

	/**
	 * Get the first sequence of consecutive characters that obey the character predicate requirements.
	 * 
	 * @param s The string to search
	 * @param func Callable object of the form @code bool(char) @endcode that returns true when the character
	 *        satisfies the requirements.
	 * @return The string representation of the sequence, or an empty string if the sequence is empty.
	 */
	template<class CharacterPredicate>
	static std::string nextPredicatedString(const std::string& s, CharacterPredicate func)
	{
		auto firstBadCharacter = std::find_if_not(s.begin(), s.end(), func);
		return s.substr(0, firstBadCharacter - s.begin()); // return the substring [0, firstBadCharacterIndex)
	}
	
	/**
	 * Search the operator map for the symbol and return the corresponding operator if found.
	 * 
	 * @param operatorSymbol The operator's symbol
	 * @throws std::invalid_argument Invalid operator symbol
	 * @return The corresponding operator
	 */
	static Operator getOperator(const std::string& operatorSymbol)
	{
		auto pair = operatorMap.find(operatorSymbol);
	
		if (pair == operatorMap.end())
			throw std::invalid_argument("Syntax error: Unknown operator " + operatorSymbol);
	
		return pair->second; // The second item in the pair is the operator itself
	}
	
	/**
	 * Search the function map for the function's name and return the corresponding function if found.
 	 * 
	 * @param functionName The function's name
	 * @throws std::invalid_argument Invalid function name
	 * @return The corresponding function
	 */
	Function getFunction(const std::string& functionName) const
	{
		auto pair = functionMap.find(functionName);
	
		if (pair == functionMap.end())
			throw std::invalid_argument("Syntax error: Unknown function " + functionName);
	
		return pair->second; // The second item in the pair is the function itself
	}
	
	/**
	 * Search the constants map for the constants's name and return the corresponding value if found.
 	 * 
	 * @param constantName The constant's name
	 * @return The corresponding value or `boost::optional<ValueType>` null
	 */
	boost::optional<ValueType> getConstant(const std::string& constantName) const
	{
		auto pair = constantsMap.find(constantName);
	
		if (pair == constantsMap.end())
			return { }; // null, essentially
	
		return pair->second; // The second item in the pair is the value
	}
	
	/**
	 * Strip and replace leading unary minuses in the expression with the alternative unary negation symbol.
	 *
	 * @param expression The expression string
	 * @return The new expression string
	 */
	std::string handleLeadingUnaryNegation(const std::string& expression) const
	{
		std::size_t unaryMinusCount = 0;
		while (expression[unaryMinusCount] == '-')
			++unaryMinusCount;
		
		// The new string doesn't contain the leading minuses
		std::string newExpr(expression, unaryMinusCount);
		
		// If the number of minuses was odd, unary negation ('@') is added to the expression's begining
		if (unaryMinusCount % 2 == 1)
			newExpr.insert(0, 1, '@');
		
		return newExpr;
	}
	
	/**
	 * Tokenize the expression string to a list of infix tokens.
	 * 
	 * @param expr The expression string
	 * @throws std::invalid_argument Invalid expression
	 * @return A vector of infix tokens representing the expression
	 */
	std::vector<InfixToken> tokenize(std::string expr) const
	{
		// Remove all whitespace from the expression
		expr.erase(std::remove_if(expr.begin(), expr.end(), ::isspace), expr.end());
		expr = handleLeadingUnaryNegation(expr);
		
		std::vector<InfixToken> tokens;
		InfixToken prevToken; // Used to determine whether a minus represents substraction or unary negation.
		
		while (expr.length() > 0)
		{
			InfixToken token;
			std::string subStr;
			std::stringstream valueStream;
			ValueType value;
	
			// The minus represents negation if the previous token
			// wasn't either a value or a right parenthesis			
			if (expr[0] == '-' && !boost::get<ValueType>(&prevToken) && !boost::get<RightParen>(&prevToken))
				expr[0] = '@';
			
			char c = expr[0];
				
			if (!isalnum(c) && !isspace(c) && std::string("(),.").find(c) == std::string::npos) // Operator
			{			
				token = getOperator(std::string(1, c));
				expr.erase(0, 1);
			}		
			else if ([&]() -> bool { valueStream << expr; return valueStream >> value; }()) // Value
			{
				token = value;
				
				// copy the rest of valueStream (after numeric input has been discarded)
				// back into the expression
				expr = std::string(std::istreambuf_iterator<char>(valueStream),
				                         std::istreambuf_iterator<char>());
			}
			else if (c == '(') // Left parenthesis
			{
				token = LeftParen();
				expr.erase(0, 1);
			}
			else if (c == ')') // Right parenthesis
			{
				token = RightParen();
				expr.erase(0, 1);
			}
			else if (c == ',') // Comma
			{
				token = Comma();
				expr.erase(0, 1);
			}
			else if (!isdigit(c) && (subStr = nextPredicatedString(expr,
			         [](char c) { return isalnum(c) || c == '_'; })) != "") // Function or constant
			{
				if (auto constantValue = getConstant(subStr)) // if constant
				{
					//token = *constantValue;
					token = ConstantName(subStr); //record the constant name in case of modification of the value before evaluation
				}
				else // if function
					token = getFunction(subStr);
				
				expr.erase(0, subStr.length());
			}
			else
			{
				throw std::invalid_argument("Syntax error: Invalid input");
			}
			
			tokens.emplace_back(token);
			prevToken = token;
		}
		
		return tokens;
	}
	
	/**
	 * Convert the infix expression (consisting of infix tokens) to a postfix expression
	 * (consisting of postfix tokens). This function performs the Sunting Yard algorithm.
	 *
	 * @param infixTokens A vector of infix tokens representing an infix expression
	 * @return A vector of postfix tokens representing a postfix expression
	 */
	std::vector<PostfixToken> infixToPostfix(const std::vector<InfixToken>& infixTokens) const
	{
		// A token in a FunctionStack is either a function, an operator or a left-parenthesis
		using FunctionToken = boost::variant<Function, Operator, LeftParen>;
		// The function stack is a stack of function tokens
		using FunctionStack = std::stack<FunctionToken, std::vector<FunctionToken>>;
	
		std::vector<PostfixToken> output;
		FunctionStack functionStack;
	
		// boost::variant visitor for InfixTokens. Will be called for each token
		// in 'infixTokens'
		class InfixTokenVisitor : public boost::static_visitor<void>
		{
			std::vector<PostfixToken>& output;
			FunctionStack& functionStack;
	
			// boost::variant visitor for FunctionTokens. Inserts the function token
			// to the output vector when it's a function (operator are functions as well).
			// Returns whether the FunctionToken is a function (operator are functions as well).
			class FunctionTokenVisitor : public boost::static_visitor<bool>
			{
				std::vector<PostfixToken>& output;
	
			public:
				FunctionTokenVisitor(std::vector<PostfixToken>& output) :
					output(output) { }
	
				bool operator()(const LeftParen&) const
				{
					return true;
				}
	
				bool operator()(const Function& f) const
				{
					output.emplace_back(f);
					return false;
				}
			};
	
		public:
			InfixTokenVisitor(std::vector<PostfixToken>& output, FunctionStack& functionStack):
				output(output), functionStack(functionStack) { }
	
			// Insert values to the output vector
			void operator()(const ValueType& x) const
			{
				output.emplace_back(x);
			}
	
			// Insert constant names to the output vector
			void operator()(const ConstantName& x) const
			{
				output.emplace_back(x);
			}
	
			// Insert functions to the function stack
			void operator()(const Function& f) const
			{
				functionStack.emplace(f);
			}
	
			// Insert the left parenthesis to the function stack
			void operator()(const LeftParen& lp) const
			{
				functionStack.emplace(lp);
			}
	
			// Pop out all functions in the function stack to the output vecto
			// till a left parenthesis is matched
			void operator()(const RightParen&) const
			{
				while (!functionStack.empty())
				{
					if (boost::apply_visitor(FunctionTokenVisitor(output), functionStack.top()))
					{
						functionStack.pop(); // Pop the left parenthesis out too
						return;
					}
	
					functionStack.pop();
				}
	
				throw std::invalid_argument("Syntax error: Mismatched parentheses");
			}
	
			// Pop out all functions in the function stack to the output vector
			// till a left parenthesis (of a function call) is matched
			void operator()(const Comma&) const
			{
				while (!functionStack.empty())
				{
					if (boost::apply_visitor(FunctionTokenVisitor(output), functionStack.top()))
						return;
	
					functionStack.pop();
				}
	
				throw std::invalid_argument("Syntax error: Mismatched parentheses or misplaced comma");
			}
	
			// Place operators according to precdence and associativity rules
			void operator()(const Operator& op) const
			{
				// boost::variant visitor for FunctionTokens. Returns true when the function token
				// is placed on the output vector.
				class OperatorVisitor : public boost::static_visitor<bool>
				{
					const Operator& op1;
					std::vector<PostfixToken>& output;
	
				public:
					OperatorVisitor(const Operator& op1, std::vector<PostfixToken>& output) :
						op1(op1), output(output) { }
	
					bool operator()(const LeftParen&) const
					{
						return false;
					}
	
					bool operator()(const Function& f) const
					{
						output.emplace_back(f);
						return true;
					}
	
					bool operator()(const Operator& op2) const
					{
						if ((op1.associativity == Operator::Associativity::left &&
						     op1.precedence == op2.precedence) ||
						    (op1.precedence < op2.precedence))
						{
							output.emplace_back(op2);
							return true;
						}
	
						return false;
					}
				};
	
				// Do not consider prefix unary operators, as they are always added by order
				while (op.getArgsNum() != 1 && !functionStack.empty() &&
				       boost::apply_visitor(OperatorVisitor(op, output), functionStack.top()))
				{
					functionStack.pop();
				}
	
				// After all higher-precedence operators have been popped out,
				// add the current one to the top
				functionStack.emplace(op);
			}
		};
	
		// Apply the infix token visitor for each token
		for (const auto& token : infixTokens)
			boost::apply_visitor(InfixTokenVisitor(output, functionStack), token);
	
		// boost::variant visitor for FunctionTokens levt-over on the FunctionStack.
		// This places all the remaining functions onto the output vector.
		class FunctionStackLeftOverVisior : public boost::static_visitor<void>
		{
			std::vector<PostfixToken>& output;
	
		public:
			FunctionStackLeftOverVisior(std::vector<PostfixToken>& output):
				output(output) { }
	
			void operator()(const LeftParen&) const
			{
				throw std::invalid_argument("Syntax error: Mismatched parentheses");
			}
	
			void operator()(const Function& f) const
			{
				output.emplace_back(f);
			}
		};
	
		// Apply the left over visitor for the left over functions on the function stack
		while (!functionStack.empty())
		{
			boost::apply_visitor(FunctionStackLeftOverVisior(output), functionStack.top());
			functionStack.pop();
		}
	
		return output;
	}
	
	/**
	 * Evaluate the postfix expression (consisting of postfix tokens), and return the result.
	 *
	 * @param postfixTokens A vector of postfix tokens representing the postfix expression
	 * @return The result of the evaluation
	 */
	ValueType evaluatePostfix(const std::vector<PostfixToken>& postfixTokens) const
	{
		std::vector<ValueType> valueStack;
	
		// boost::variant visitor for PostfixTokens. This performs the reverse polish notation
		// evaluation algorithm, where each value is pushed on the stack and each function evaluates
		// the popped out arguments from the stack and pushed the result to the stack.
		class PostfixTokenVisitor : public boost::static_visitor<void>
		{
			std::vector<ValueType>& valueStack;
			const std::unordered_map<std::string, ValueType>& constantsMap;
	
		public:
			PostfixTokenVisitor(std::vector<ValueType>& valueStack, const std::unordered_map<std::string, ValueType>& constantsMap) :
				valueStack(valueStack), constantsMap(constantsMap) { }
	
			void operator()(const ValueType& v) const
			{
				valueStack.emplace_back(v);
			}
	
			void operator()(const ConstantName& v) const
			{
				auto pair = constantsMap.find(v);
				valueStack.emplace_back((*pair).second);
			}
	
			void operator()(const Function& f) const
			{
				if (valueStack.size() < f.getArgsNum())
					throw std::invalid_argument("Syntax error: Not enough arguments!");
	
				const auto endArgs = valueStack.end();
				const auto beginArgs = endArgs - f.getArgsNum();
	
				auto result = f(beginArgs, endArgs);
				valueStack.erase(beginArgs, endArgs);
				valueStack.emplace_back(result);
			}
		};
	
		// Apply the postfix token visitor for each postfix token
		for (const auto& token : postfixTokens)
			boost::apply_visitor(PostfixTokenVisitor(valueStack, constantsMap), token);
	
		if (valueStack.size() > 1)
			throw std::invalid_argument("Syntax error: Too many arguments!");
		if (valueStack.size() == 0)
			throw std::invalid_argument("Syntax error: No expression to evaluate!");
	
		// Return the value at the top of the stack
		return valueStack.back();		
	}
};

template<class ValueType>
const std::unordered_map<std::string, typename Evaluator<ValueType>::Operator> Evaluator<ValueType>::operatorMap
{
	{ "+" , { [](ValueType x, ValueType y) { return x + y;          }, 1, Operator::Associativity::left  } },
	{ "-" , { [](ValueType x, ValueType y) { return x - y;          }, 1, Operator::Associativity::left  } },
	{ "*" , { [](ValueType x, ValueType y) { return x * y;          }, 2, Operator::Associativity::left  } },
	{ "/" , { [](ValueType x, ValueType y) { if (y == ValueType(0))
	                                             throw std::invalid_argument("Math error: Division by zero");
	                                         return x / y;          }, 2, Operator::Associativity::left  } },
	{ "@" , { [](ValueType x)              { return -x;             }, 3, Operator::Associativity::right } },
	{ "^" , { [](ValueType x, ValueType y) { return std::pow(x, y); }, 4, Operator::Associativity::right } },
};

} // namespace Expressio

#endif // EVALUATOR_HPP
