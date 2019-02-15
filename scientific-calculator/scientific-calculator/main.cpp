#include <Windows.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>
#include <ctime>
#include "rapidjson/filewritestream.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "stack.h"
using namespace std;
const double _PI = 3.14159265358979323846;
const double _E = 2.71828182845904523536;
bool isTesting = false;
int GetPriority(const string *str_operator)
{
	if (*str_operator == "+" || *str_operator == "-")
		return 0;
	else if (*str_operator == "^")
		return 2;
	else if (*str_operator == "(")
		return 3;
	else
		return 1;
}
double Factorial(double n)
{
	if (n == 0.0)
		return 1.0;
	else if (n < 0.0)
		throw "[N!] N non natural number";
	else
	{
		if (n - (int)n == 0.0)
		{
			double result = 1.0;
			for (int i = 1; i < n; ++i)
				result *= i;
			return result * n;
		}
		else
			throw "[N!] N non natural number";
	}
}
void PrintColorfully(const string &content, const WORD &wAttributes, bool isLine = false)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wAttributes);
	cout << content;
	if (isLine)
		cout << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
}
bool Calculate(const string &expression, const bool &isRadian, double &answer)
{
	const int MAX_RPN_COUNT = 512;
	string *RPN[MAX_RPN_COUNT] = {};
	int RPN_pointer = 0;
	Stack<string *> operator_stack;
	int operand_count = 0;
	int operator_count = 1;
	string *temp = nullptr;
	try
	{
		for (string::const_iterator iter = expression.begin(); iter < expression.end(); ++iter)
		{
			temp = new string;
			if ('0' <= *iter && *iter <= '9')
			{
				while ('0' <= *iter && *iter <= '9' || *iter == '.')
				{
					*temp += *iter;
					++iter;
					if (iter == expression.end())
						break;
				}
				--iter;
				RPN[RPN_pointer] = temp;
				++operand_count;
				++RPN_pointer;
				continue;
			}
			else if (*iter == ',')
				continue;
			else if ('a' <= *iter && *iter <= 'z')
			{
				while ('a' <= *iter && *iter <= 'z' || *iter == '#')
				{
					*temp += *iter;
					++iter;
					if (iter == expression.end())
						break;
				}
				--iter;
				if (*temp == "pi" || *temp == "e" || *temp == "ans" || *temp == "rand#")
				{
					RPN[RPN_pointer] = temp;
					++operand_count;
					++RPN_pointer;
					continue;
				}
			}
			else
			{
				*temp = *iter;
				if (*temp == ")")
				{
					while (!operator_stack.empty())
					{
						if (*operator_stack.GetTop() != "(")
						{
							RPN[RPN_pointer] = operator_stack.pop();
							++RPN_pointer;
						}
						else
							break;
					}
					if (!operator_stack.empty())
						operator_stack.pop();
					else
						throw "'(' missed";
					continue;
				}
			}
			if (*temp == "-" && operator_count == operand_count + 1)
				*temp = "--";
			else if (*temp == "(" && operand_count == operator_count)
			{
				*temp = "*";
				--iter;
			}
			else if (*temp == "%")
			{
				if (iter == expression.end() - 1)
					*temp = "%%";
				else
					if (!('0' <= *(iter + 1) && *(iter + 1) <= '9'))
						*temp = "%%";
			}
			while (true)
			{
				if (operator_stack.empty())
					break;
				else
				{
					if (*operator_stack.GetTop() == "(")
						break;
					else
					{
						if (GetPriority(temp) > GetPriority(operator_stack.GetTop()))
							break;
						else
						{
							RPN[RPN_pointer] = operator_stack.pop();
							++RPN_pointer;
						}
					}
				}
			}
			if (*temp == "+" || *temp == "-" || *temp == "*" || *temp == "/" || *temp == "^" || *temp == "log")
				++operator_count;
			operator_stack.push(temp);
		}
		while (!operator_stack.empty())
		{
			if (*(RPN[RPN_pointer] = operator_stack.pop()) == "(")
				throw "')' missed";
			++RPN_pointer;
		}
		if (isTesting)
		{
			for (int i = 0; i < RPN_pointer; ++i)
			{
				PrintColorfully(*RPN[i], FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				cout << " ";
			}
			cout << endl;
		}
		auto GetValue = [isRadian](double(*function_name)(double _X), double parameter)
		{
			if (isRadian)
				return function_name(parameter);
			else
				return function_name(parameter) * 180.0 / _PI;
		};
		if (RPN[0] == nullptr)
			throw "no operand";
		else if (!('0' <= (*RPN[0])[0] && (*RPN[0])[0] <= '9'))
			if (*RPN[0] != "pi" && *RPN[0] != "e" && *RPN[0] != "ans" && *RPN[0] != "rand#")
				throw "unknown error";
		Stack<double> result;
		RPN_pointer = 0;
		while (RPN[RPN_pointer] != nullptr)
		{
			if ('0' <= (*RPN[RPN_pointer])[0] && (*RPN[RPN_pointer])[0] <= '9')
				result.push(stod(*RPN[RPN_pointer]));
			else
			{
				try
				{
					if (*RPN[RPN_pointer] == "+")
						result.push(result.pop() + result.pop());
					else if (*RPN[RPN_pointer] == "-")
						result.push(0.0 - result.pop() + result.pop());
					else if (*RPN[RPN_pointer] == "--")
						result.push(0.0 - result.pop());
					else if (*RPN[RPN_pointer] == "*")
						result.push(result.pop() * result.pop());
					else if (*RPN[RPN_pointer] == "/")
					{
						double B;
						if ((B = result.pop()) == 0.0)
							throw "[N/0] divisor is zero";
						else
							result.push(result.pop() / B);
					}
					else if (*RPN[RPN_pointer] == "!")
						result.push(Factorial(result.pop()));
					else if (*RPN[RPN_pointer] == "%")
					{
						double B = result.pop();
						double A = result.pop();
						if (A - (int)A == 0.0 && B - (int)B == 0.0)
							result.push((int)A % (int)B);
						else
							throw "[A%B] A or B non integers";
					}
					else if (*RPN[RPN_pointer] == "%%")
						result.push(result.pop() / 100.0);
					else if (*RPN[RPN_pointer] == "^")
					{
						double B = result.pop();
						double A = result.pop();
						result.push(pow(A, B));
					}
					else if (*RPN[RPN_pointer] == "min")
					{
						double A = result.pop();
						double B = result.pop();
						A < B ? result.push(A) : result.push(B);
					}
					else if (*RPN[RPN_pointer] == "max")
					{
						double B = result.pop();
						double A = result.pop();
						A > B ? result.push(A) : result.push(B);
					}
					else if (*RPN[RPN_pointer] == "log")
						result.push(1.0 / log(result.pop()) * log(result.pop()));
					else if (*RPN[RPN_pointer] == "lg")
						result.push(log10(result.pop()));
					else if (*RPN[RPN_pointer] == "ln")
						result.push(log(result.pop()));
					else if (*RPN[RPN_pointer] == "sqrt")
					{
						if (result.GetTop() < 0.0)
							throw "[sqrt(N)] N is negative";
						result.push(sqrt(result.pop()));
					}
					else if (*RPN[RPN_pointer] == "abs")
						result.push(abs(result.pop()));
					else if (*RPN[RPN_pointer] == "pi")
						result.push(_PI);
					else if (*RPN[RPN_pointer] == "e")
						result.push(_E);
					else if (*RPN[RPN_pointer] == "ans")
						result.push(answer);
					else if (*RPN[RPN_pointer] == "rand#")
						result.push(rand());
					else if (*RPN[RPN_pointer] == "arcsin")
						result.push(GetValue(asin, result.pop()));
					else if (*RPN[RPN_pointer] == "arccos")
						result.push(GetValue(acos, result.pop()));
					else if (*RPN[RPN_pointer] == "arctan")
						result.push(GetValue(atan, result.pop()));
					else if (*RPN[RPN_pointer] == "arsinh")
						result.push(GetValue(asinh, result.pop()));
					else if (*RPN[RPN_pointer] == "arcosh")
						result.push(GetValue(acosh, result.pop()));
					else if (*RPN[RPN_pointer] == "artanh")
						result.push(GetValue(atanh, result.pop()));
					else
					{
						double n = 0.0;
						if (isRadian == true)
							n = result.pop();
						else
							n = result.pop() * _PI / 180.0;
						if (*RPN[RPN_pointer] == "sin")
							result.push(sin(n));
						else if (*RPN[RPN_pointer] == "cos")
							result.push(cos(n));
						else if (*RPN[RPN_pointer] == "tan")
							result.push(tan(n));
						else if (*RPN[RPN_pointer] == "sinh")
							result.push(sinh(n));
						else if (*RPN[RPN_pointer] == "cosh")
							result.push(cosh(n));
						else if (*RPN[RPN_pointer] == "tanh")
							result.push(tanh(n));
						else
						{
							*RPN[RPN_pointer] = "unknown operator: " + *RPN[RPN_pointer];
							throw RPN[RPN_pointer]->c_str();
						}
					}
				}
				catch (const char *ERROR_INFORMATION)
				{
					if (ERROR_INFORMATION == "Stack is Empty")
						throw "operand missed";
					else
						throw ERROR_INFORMATION;
				}
			}
			++RPN_pointer;
		}
		for (int i = 0; i < MAX_RPN_COUNT; i++)
			delete RPN[i];
		answer = result.pop();
		return true;
	}
	catch (const char *ERROR_INFORMATION)
	{
		cout << endl;
		PrintColorfully("ERROR: ", FOREGROUND_RED);
		PrintColorfully(ERROR_INFORMATION, FOREGROUND_GREEN, true);
		for (int i = 0; i < MAX_RPN_COUNT; i++)
			delete RPN[i];
		return false;
	}
}
struct function
{
	string function_name = "";
	int parameter_count = 0;
	string parameter_name = "";
	string function_expression = "";
	void ExpanseFunction(string &expression)
	{
		string::size_type pos = 0;
		while ((pos = expression.find(function_name, pos)) != string::npos)
		{
			expression.replace(pos, function_name.size(), function_expression);
			string::size_type first_replaced_pos = pos;
			pos += function_expression.size();
			string::size_type last_replaced_pos = pos;
			string *parameter_expression = new string[parameter_count];
			for (int i = 0; i < parameter_count; ++i)
			{
				Stack<char> left_parenthesis;
				while (true)
				{
					if (expression[pos] == '(')
					{
						parameter_expression[i] += expression[pos];
						left_parenthesis.push('L');
					}
					else if (expression[pos] == ')' && !left_parenthesis.empty())
					{
						parameter_expression[i] += expression[pos];
						left_parenthesis.pop();
					}
					else if ((expression[pos] == ')' || expression[pos] == ',') && left_parenthesis.empty())
						break;
					else
						parameter_expression[i] += expression[pos];
					++pos;
				}
				++pos;
			}
			expression.erase(last_replaced_pos, pos - last_replaced_pos);
			for (int i = 0; i < parameter_count; ++i)
			{
				for (pos = first_replaced_pos; pos <= last_replaced_pos; ++pos)
				{
					if (expression[pos] == parameter_name[i])
					{
						expression.replace(pos, 1, parameter_expression[i]);
						last_replaced_pos += parameter_expression[i].size() - 1;
					}
				}
			}
			pos = first_replaced_pos;
		}
	}
};
int FIXnum = 0;
bool isRadian = false;
double answer = 0.0;
vector<function*> functions;
int created_count = 0;
void LoadSettings()
{
	using namespace rapidjson;
	FILE *file_pointer = fopen("settings.json", "rb");
	char readBuffer[65536];
	FileReadStream is(file_pointer, readBuffer, sizeof(readBuffer));
	Document document;
	document.ParseStream(is);
	fclose(file_pointer);
	FIXnum = document["FIXnum"].GetInt();
	isRadian = document["isRadian"].GetBool();
	answer = document["answer"].GetDouble();
	created_count = document["functions_count"].GetInt();
	functions.clear();
	const Value &_functions = document["functions"];
	for (int i = 0; i < created_count; ++i)
	{
		function *importing = new function;
		importing->function_name = _functions[i]["function_name"].GetString();
		importing->parameter_count = _functions[i]["parameter_count"].GetInt();
		importing->parameter_name = _functions[i]["parameter_name"].GetString();
		importing->function_expression = _functions[i]["function_expression"].GetString();
		functions.push_back(importing);
	}
}
void SaveSettings()
{
	using namespace rapidjson;
	StringBuffer settings;
	Writer<StringBuffer> output(settings);
	output.StartObject();
	output.Key("FIXnum");
	output.Int(FIXnum);
	output.Key("isRadian");
	output.Bool(isRadian);
	output.Key("answer");
	output.Double(answer);
	output.Key("functions_count");
	output.Int(created_count);
	output.Key("functions");
	output.StartArray();
	for (vector<function*>::iterator iter = functions.begin(); iter < functions.end(); ++iter)
	{
		output.StartObject();
		output.Key("function_name");
		output.String((*iter)->function_name.c_str());
		output.Key("parameter_count");
		output.Int((*iter)->parameter_count);
		output.Key("parameter_name");
		output.String((*iter)->parameter_name.c_str());
		output.Key("function_expression");
		output.String((*iter)->function_expression.c_str());
		output.EndObject();
	}
	output.EndArray();
	output.EndObject();
	Document document;
	document.Parse(settings.GetString());
	FILE *file_pointer = fopen("settings.json", "wb");
	char writeBuffer[65536];
	FileWriteStream os(file_pointer, writeBuffer, sizeof(writeBuffer));
	Writer<FileWriteStream> writer(os);
	document.Accept(writer);
	fclose(file_pointer);
}
int main()
{
	srand(time(NULL));
	PrintColorfully("Welcome", FOREGROUND_RED | FOREGROUND_INTENSITY);
	PrintColorfully(" to ", FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	PrintColorfully("Scientific Calculator", FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY, true);
	string expression;
	string content;
	while (cin >> content)
	{
		if (content[0] == '-')
		{
			if (content == "-rad")
				isRadian = true;
			else if (content == "-deg")
				isRadian = false;
			else if (content == "-fix")
			{
				PrintColorfully("FIX number? ", FOREGROUND_RED | FOREGROUND_GREEN);
				cin >> FIXnum;
				cout << setprecision(FIXnum);
			}
			else if (content == "-f")
			{
				function *creating = new function;
				cin >> content;
				string::size_type pos = 0;
				for (; content[pos] != '('; ++pos)
					creating->function_name += content[pos];
				creating->function_name += "(";
				++pos;
				string::size_type first_parameter_pos = pos;
				for (; content[pos] != ')'; ++pos)
					if (content[pos] == ',')
						++creating->parameter_count;
				++creating->parameter_count;
				for (; first_parameter_pos != pos; ++first_parameter_pos)
					if (('A' <= content[first_parameter_pos] && content[first_parameter_pos] <= 'Z') || ('a' <= content[first_parameter_pos] && content[first_parameter_pos] <= 'z'))
						creating->parameter_name += content[first_parameter_pos];
				while (content[pos] != '=')
					++pos;
				++pos;
				for (; pos != content.size(); pos++)
					creating->function_expression += content[pos];
				creating->function_expression = "(" + creating->function_expression + ")";
				functions.push_back(creating);
				++created_count;
			}
			else if (content == "-sf")
			{
				for (vector<function*>::iterator iter = functions.begin(); iter < functions.end(); ++iter)
				{
					PrintColorfully("Function Name: ", FOREGROUND_RED | FOREGROUND_GREEN);
					PrintColorfully((*iter)->function_name, FOREGROUND_GREEN, true);
					PrintColorfully("Parameter Count: ", FOREGROUND_RED | FOREGROUND_GREEN);
					PrintColorfully(to_string((*iter)->parameter_count), FOREGROUND_GREEN, true);
					PrintColorfully("Parameter Name: ", FOREGROUND_RED | FOREGROUND_GREEN);
					PrintColorfully((*iter)->parameter_name, FOREGROUND_GREEN, true);
					PrintColorfully("Function Exprssion: ", FOREGROUND_RED | FOREGROUND_GREEN);
					PrintColorfully((*iter)->function_expression, FOREGROUND_GREEN, true);
					cout << endl;
				}
			}
			else if (content == "-df")
			{
				PrintColorfully("function name?", FOREGROUND_RED | FOREGROUND_GREEN);
				string function_name;
				cin >> function_name;
				bool isDeleted = false;
				for (vector<function*>::iterator iter = functions.begin(); iter < functions.end(); ++iter)
				{
					if ((*iter)->function_name == function_name)
					{
						functions.erase(iter);
						isDeleted = true;
						--created_count;
						break;
					}
				}
				if (!isDeleted)
					PrintColorfully("NOT FOUND", FOREGROUND_RED, true);
			}
			else if (content == "-load")
				LoadSettings();
			else if (content == "-save")
				SaveSettings();
			else if (content == "-test")
				isTesting = true;
			else if (content == "-exit")
				break;
			else
				PrintColorfully("ERROR INSTRUCTION", FOREGROUND_RED, true);
		}
		else
		{
			bool isCalculateSuccessfully;
			if (content == "=")
				isCalculateSuccessfully = Calculate(expression, isRadian, answer);
			else
			{
				expression = content;
				if (created_count == 0)
					isCalculateSuccessfully = Calculate(expression, isRadian, answer);
				else
				{
					for (vector<function*>::iterator iter = functions.begin(); iter < functions.end(); ++iter)
						(*iter)->ExpanseFunction(expression);
					if (isTesting)
						PrintColorfully(expression, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY, true);
					isCalculateSuccessfully = Calculate(expression, isRadian, answer);
				}
			}
			if (isCalculateSuccessfully)
			{
				cout.setf(ios::fixed);
				PrintColorfully("result: ", FOREGROUND_RED | FOREGROUND_GREEN);
				PrintColorfully(to_string(answer), FOREGROUND_GREEN, true);
			}
		}
	}
	return 0;
}