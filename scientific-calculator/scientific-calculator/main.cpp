#include <Windows.h>
#include <iostream>
#include <iomanip>
#include <string>
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
		throw "MATH ERROR 0";
	else
	{
		if (n - (int)n == 0.0)
		{
			double result = 1.0;
			for (int i = 1; i < n; i++)
				result *= i;
			return result * n;
		}
		else
			throw "MATH ERROR 0";
	}
}
void Calculate(string &expression, const bool &isRadian, double &answer)
{
	try
	{
		string *a_expression[512] = {};
		int a_expression_pointer = 0;
		Stack<string *> operator_stack;
		int operand_count = 0;
		int operator_count = 1;
		string *temp = nullptr;
		for (string::iterator temp_character = expression.begin(); temp_character != expression.end(); temp_character++)
		{
			temp = new string;
			if ('0' <= *temp_character && *temp_character <= '9')
			{
				while ('0' <= *temp_character && *temp_character <= '9' || *temp_character == '.')
				{
					*temp += *temp_character;
					temp_character++;
					if (temp_character == expression.end())
						break;
				}
				temp_character--;
				a_expression[a_expression_pointer] = temp;
				a_expression_pointer++;
				operand_count++;
				continue;
			}
			else if (*temp_character == ',')
				continue;
			else if ('a' <= *temp_character && *temp_character <= 'z')
			{
				while ('a' <= *temp_character && *temp_character <= 'z' || *temp_character == '#')
				{
					*temp += *temp_character;
					temp_character++;
					if (temp_character == expression.end())
						break;
				}
				temp_character--;
				if (*temp == "pi" || *temp == "e" || *temp == "ans" || *temp == "rand#")
				{
					a_expression[a_expression_pointer] = temp;
					a_expression_pointer++;
					operand_count++;
					continue;
				}
			}
			else
			{
				*temp = *temp_character;
				if (*temp == ")")
				{
					while (*operator_stack.GetTop() != "(")
					{
						a_expression[a_expression_pointer] = operator_stack.pop();
						a_expression_pointer++;
					}
					operator_stack.pop();
					continue;
				}
			}
			if (*temp == "-" && operator_count == operand_count + 1)
				*temp = "--";
			else if (*temp == "(" && operand_count == operator_count)
			{
				*temp = "*";
				temp_character--;
			}
			else if (*temp == "%" && temp_character != expression.end())
				if (!('0' <= *(temp_character + 1) && *(temp_character + 1) <= '9'))
					*temp = "%%";
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
						if (GetPriority(temp) >= GetPriority(operator_stack.GetTop()))
							break;
						else
						{
							a_expression[a_expression_pointer] = operator_stack.pop();
							a_expression_pointer++;
						}
					}
				}
			}
			if (*temp == "+" || *temp == "-" || *temp == "*" || *temp == "/" || *temp == "^" || *temp == "log")
				operator_count++;
			operator_stack.push(temp);
		}
		while (!operator_stack.empty())
		{
			a_expression[a_expression_pointer] = operator_stack.pop();
			a_expression_pointer++;
		}
		Stack<double> result;
		a_expression_pointer = 0;
		while (a_expression[a_expression_pointer] != nullptr)
		{
			if ('0' <= (*a_expression[a_expression_pointer])[0] && (*a_expression[a_expression_pointer])[0] <= '9')
				result.push(stod(*a_expression[a_expression_pointer]));
			else
			{
				if (*a_expression[a_expression_pointer] == "+")
					result.push(result.pop() + result.pop());
				else if (*a_expression[a_expression_pointer] == "-")
					result.push(0.0 - result.pop() + result.pop());
				else if (*a_expression[a_expression_pointer] == "--")
					result.push(0.0 - result.pop());
				else if (*a_expression[a_expression_pointer] == "*")
					result.push(result.pop() * result.pop());
				else if (*a_expression[a_expression_pointer] == "/")
					result.push(1.0 / result.pop() * result.pop());
				else if (*a_expression[a_expression_pointer] == "!")
					result.push(Factorial(result.pop()));
				else if (*a_expression[a_expression_pointer] == "%")
				{
					double B = result.pop();
					double A = result.pop();
					if (A - (int)A == 0 && B - (int)B == 0)
						result.push((int)A % (int)B);
					else
						throw "MATH ERROR 1";
				}
				else if (*a_expression[a_expression_pointer] == "%%")
					result.push(result.pop() / 100.0);
				else if (*a_expression[a_expression_pointer] == "^")
				{
					double B = result.pop();
					double A = result.pop();
					result.push(pow(A, B));
				}
				else if (*a_expression[a_expression_pointer] == "log")
					result.push(1.0 / log(result.pop()) * log(result.pop()));
				else if (*a_expression[a_expression_pointer] == "lg")
					result.push(log10(result.pop()));
				else if (*a_expression[a_expression_pointer] == "ln")
					result.push(log(result.pop()));
				else if (*a_expression[a_expression_pointer] == "sqrt")
				{
					if (result.GetTop() < 0)
						throw "MATH ERROR 2";
					result.push(sqrt(result.pop()));
				}
				else if (*a_expression[a_expression_pointer] == "abs")
					result.push(abs(result.pop()));
				else if (*a_expression[a_expression_pointer] == "pi")
					result.push(_PI);
				else if (*a_expression[a_expression_pointer] == "e")
					result.push(_E);
				else if (*a_expression[a_expression_pointer] == "ans")
					result.push(answer);
				else if (*a_expression[a_expression_pointer] == "rand#")
					result.push(rand());
				else
				{
					double n = 0.0;
					if (isRadian == true)
						n = result.pop();
					else
						n = result.pop() * _PI / 180.0;
					if (*a_expression[a_expression_pointer] == "sin")
						result.push(sin(n));
					else if (*a_expression[a_expression_pointer] == "cos")
						result.push(cos(n));
					else if (*a_expression[a_expression_pointer] == "tan")
						result.push(tan(n));
					else if (*a_expression[a_expression_pointer] == "arcsin")
						result.push(asin(n));
					else if (*a_expression[a_expression_pointer] == "arccos")
						result.push(acos(n));
					else if (*a_expression[a_expression_pointer] == "arctan")
						result.push(atan(n));
					else if (*a_expression[a_expression_pointer] == "sinh")
						result.push(sinh(n));
					else if (*a_expression[a_expression_pointer] == "cosh")
						result.push(cosh(n));
					else if (*a_expression[a_expression_pointer] == "tanh")
						result.push(tanh(n));
					else if (*a_expression[a_expression_pointer] == "arsinh")
						result.push(asinh(n));
					else if (*a_expression[a_expression_pointer] == "arocsh")
						result.push(acosh(n));
					else if (*a_expression[a_expression_pointer] == "artanh")
						result.push(atanh(n));
					else
						throw "MATH ERROR 3";
				}
			}
			delete a_expression[a_expression_pointer];
			a_expression_pointer++;
		}
		answer = result.pop();
	}
	catch (const char *ERROR_INFORMATION)
	{
		cout << endl;
		cout << "ERROR:" << ERROR_INFORMATION << endl;
	}
	cout.setf(ios::fixed);
	cout << "result=" << answer << endl;
}
void ExpanseFunction(string &expression, const string &function_name, const string &function_expression, int parameter_count, const char *parameter_name)
{
	string::size_type pos = 0;
	while ((pos = expression.find(function_name, pos)) != string::npos)
	{
		expression.replace(pos, function_name.size(), function_expression);
		string::size_type first_replaced_pos = pos;
		pos += function_expression.size();
		string::size_type last_replaced_pos = pos;
		string *parameter_expression = new string[parameter_count];
		for (int i = 0; i < parameter_count; i++)
		{
			while (true)
			{
				if (expression[pos] != ',' && expression[pos] != ')')
					parameter_expression[i] += expression[pos];
				else
					break;
				pos++;
			}
			pos++;
		}
		expression.erase(last_replaced_pos, pos - last_replaced_pos);
		string::size_type replaced_pos = first_replaced_pos;
		for (int i = 0; i < parameter_count; i++)
			for (first_replaced_pos = replaced_pos; first_replaced_pos < last_replaced_pos; first_replaced_pos++)
				if (expression[first_replaced_pos] == parameter_name[i])
					expression.replace(first_replaced_pos, 1, parameter_expression[i]);
	}
}
string functions[32][2] = {};
int parameter_count[32] = {};
string parameter_name[32] = {};
int functions_count = 0;
int FIXnum = 0;
bool isRadian = false;
double answer = 0.0;
void LoadSettings()
{
	using namespace rapidjson;
	FILE *file_pointer = fopen("settings.json", "rb");
	char readBuffer[65536];
	FileReadStream is(file_pointer, readBuffer, sizeof(readBuffer));
	Document document;
	document.ParseStream(is);
	fclose(file_pointer);
	const Value &_functions = document["functions"];
	for (int i = 0; i < 32; i++)
	{
		functions[i][0] = _functions[i][0].GetString();
		functions[i][0] = _functions[i][1].GetString();
	}
	const Value &_parameter_count = document["parameter_count"];
	for (int i = 0; i < 32; i++)
		parameter_count[i] = _parameter_count[i].GetInt();
	const Value &_parameter_name = document["parameter_name"];
	for (int i = 0; i < 32; i++)
		parameter_name[i] = _parameter_name[i].GetString();
	functions_count = document["functions_count"].GetInt();
	FIXnum = document["FIXnum"].GetInt();
	isRadian = document["isRadian"].GetBool();
	answer = document["answer"].GetDouble();
}
void SaveSettings()
{
	using namespace rapidjson;
	StringBuffer settings;
	Writer<StringBuffer> output(settings);
	output.StartObject();
	output.Key("functions");
	output.StartArray();
	for (int i = 0; i < 32; i++)
	{
		output.StartArray();
		output.String(functions[i][0].c_str());
		output.String(functions[i][1].c_str());
		output.EndArray();
	}
	output.EndArray();
	output.Key("parameter_count");
	output.StartArray();
	for (int i = 0; i < 32; i++)
		output.Int(parameter_count[i]);
	output.EndArray();
	output.Key("parameter_name");
	output.StartArray();
	for (int i = 0; i < 32; i++)
		output.String(parameter_name[i].c_str());
	output.EndArray();
	output.Key("functions_count");
	output.Int(functions_count);
	output.Key("FIXnum");
	output.Int(FIXnum);
	output.Key("isRadian");
	output.Bool(isRadian);
	output.Key("answer");
	output.Double(answer);
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
BOOL WINAPI HandlerRoutine(DWORD dwCtrlType)
{
	if (CTRL_CLOSE_EVENT == dwCtrlType)
		SaveSettings();
	return TRUE;
}
int main()
{
	SetConsoleCtrlHandler(HandlerRoutine, TRUE);
	system("COLOR 02");
	srand(time(NULL));
	string expression;
	string content;
	while (cin >> content)
	{
		if (content == "Rad")
			isRadian = true;
		else if (content == "Deg")
			isRadian = false;
		else if (content == "restart")
			Calculate(expression, isRadian, answer);
		else if (content == "function")
		{
			if (functions_count == 32)
				cout << "The list of functions is full." << endl;
			else
			{
				cin >> content;
				string::size_type pos = 0;
				for (; content[pos] != '('; pos++)
					functions[functions_count][0] += content[pos];
				pos++;
				string::size_type first_parameter_pos = pos;
				for (; content[pos] != ')'; pos++)
					if (content[pos] == ',')
						parameter_count[functions_count]++;
				parameter_count[functions_count]++;
				for (; first_parameter_pos != pos; first_parameter_pos++)
					if ('a' <= content[first_parameter_pos] && content[first_parameter_pos] <= 'z')
						parameter_name[functions_count] += content[first_parameter_pos];
				while (content[pos] != '=')
					pos++;
				pos++;
				for (; pos != content.size(); pos++)
					functions[functions_count][1] += content[pos];
				functions_count++;
			}
		}
		else if (content == "showFunctions")
		{
			for (int i = 0; i < functions_count; i++)
				cout << "Function " << functions_count << endl
				<< "Function Name:" << functions[i][0] << endl
				<< "Parameter Number:" << parameter_count[i] << endl
				<< "Parater Name:" << parameter_name[i] << endl
				<< "Function Exprssion:" << functions[i][1] << endl;
		}
		else if (content == "deleteFunctions")
			functions_count = 0;
		else if (content == "exit")
			break;
		else if (content == "FIX")
		{
			cout << "FIX num?";
			cin >> FIXnum;
			cout << setprecision(FIXnum);
		}
		else if (content == "load")
			LoadSettings();
		else if (content == "save")
			SaveSettings();
		else
		{
			expression = content;
			if (functions_count == 0)
				Calculate(expression, isRadian, answer);
			else
			{
				for (int i = 0; i < functions_count; i++)
					ExpanseFunction(expression, functions[i][0], functions[i][1], parameter_count[i], parameter_name[i].c_str());
				Calculate(expression, isRadian, answer);
			}
		}
	}
	return 0;
}