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
			for (int i = 1; i < n; i++)
				result *= i;
			return result * n;
		}
		else
			throw "[N!] N non natural number";
	}
}
void PrintColorfully(string content, WORD wAttributes, bool isLine = false)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wAttributes);
	cout << content;
	if (isLine)
		cout << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
}
bool Calculate(string &expression, const bool &isRadian, double &answer)
{
	try
	{
		string *a_expression[512] = {};
		int a_expression_pointer = 0;
		Stack<string *> operator_stack;
		int operand_count = 0;
		int operator_count = 1;
		string *temp = nullptr;
		for (string::iterator temp_character = expression.begin(); temp_character < expression.end(); temp_character++)
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
					while (!operator_stack.empty())
					{
						if (*operator_stack.GetTop() != "(")
						{
							a_expression[a_expression_pointer] = operator_stack.pop();
							a_expression_pointer++;
						}
						else
							break;
					}
					if (!operator_stack.empty())
						operator_stack.pop();
					else
						throw "missing '('";
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
			else if (*temp == "%")
			{
				if (temp_character == expression.end() - 1)
					*temp = "%%";
				else
					if (!('0' <= *(temp_character + 1) && *(temp_character + 1) <= '9'))
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
			if (*(a_expression[a_expression_pointer] = operator_stack.pop()) == "(")
				throw "missing ')'";
			a_expression_pointer++;
		}
		if (isTesting)
		{
			for (int i = 0; i < a_expression_pointer; i++)
			{
				PrintColorfully(*a_expression[i], FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				cout << " ";
			}
			cout << endl;
		}
		auto GetValue = [isRadian](double(*function_name)(double _X), double parameter)
		{
			if (isRadian)
				return function_name(parameter);
			else
				return function_name(parameter) * 180 / _PI;
		};
		if (a_expression[0] == nullptr)
			throw "no operand";
		else if (!('0' <= (*a_expression[0])[0] && (*a_expression[0])[0] <= '9'))
		{
			if (*a_expression[0] != "pi" && *a_expression[0] != "e" && *a_expression[0] != "ans" && *a_expression[0] != "rand#")
				throw "unknown error";
		}
		Stack<double> result;
		a_expression_pointer = 0;
		while (a_expression[a_expression_pointer] != nullptr)
		{
			if ('0' <= (*a_expression[a_expression_pointer])[0] && (*a_expression[a_expression_pointer])[0] <= '9')
				result.push(stod(*a_expression[a_expression_pointer]));
			else
			{
				try
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
							throw "[A%B] A or B non integers";
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
							throw "[sqrt(N)] N is negative";
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
					else if (*a_expression[a_expression_pointer] == "arcsin")
						result.push(GetValue(asin, result.pop()));
					else if (*a_expression[a_expression_pointer] == "arccos")
						result.push(GetValue(acos, result.pop()));
					else if (*a_expression[a_expression_pointer] == "arctan")
						result.push(GetValue(atan, result.pop()));
					else if (*a_expression[a_expression_pointer] == "arsinh")
						result.push(GetValue(asinh, result.pop()));
					else if (*a_expression[a_expression_pointer] == "arcosh")
						result.push(GetValue(acosh, result.pop()));
					else if (*a_expression[a_expression_pointer] == "artanh")
						result.push(GetValue(atanh, result.pop()));
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
						else if (*a_expression[a_expression_pointer] == "sinh")
							result.push(sinh(n));
						else if (*a_expression[a_expression_pointer] == "cosh")
							result.push(cosh(n));
						else if (*a_expression[a_expression_pointer] == "tanh")
							result.push(tanh(n));
						else
						{
							*a_expression[a_expression_pointer] = "unknown operator: " + *a_expression[a_expression_pointer];
							throw a_expression[a_expression_pointer]->c_str();
						}
					}
				}
				catch (const char *ERROR_INFORMATION)
				{
					if (ERROR_INFORMATION == "Stack is Empty")
						throw "missing operand";
					else
						throw ERROR_INFORMATION;
				}
			}
			delete a_expression[a_expression_pointer];
			a_expression_pointer++;
		}
		answer = result.pop();
		return true;
	}
	catch (const char *ERROR_INFORMATION)
	{
		cout << endl;
		PrintColorfully("ERROR: ", FOREGROUND_RED);
		PrintColorfully(ERROR_INFORMATION, FOREGROUND_GREEN, true);
		return false;
	}
}
struct function
{
	bool isEnabled = false;
	string function_name = "";
	int parameter_count = 0;
	string parameter_name = "";
	string function_expression = "";
};
void ExpanseFunction(string &expression, const function &id)
{
	string::size_type pos = 0;
	while ((pos = expression.find(id.function_name, pos)) != string::npos)
	{
		expression.replace(pos, id.function_name.size(), id.function_expression);
		string::size_type first_replaced_pos = pos;
		pos += id.function_expression.size();
		string::size_type last_replaced_pos = pos;
		string *parameter_expression = new string[id.parameter_count];
		for (int i = 0; i < id.parameter_count; i++)
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
				pos++;
			}
			pos++;
		}
		expression.erase(last_replaced_pos, pos - last_replaced_pos);
		for (int i = 0; i < id.parameter_count; i++)
		{
			for (pos = first_replaced_pos; pos <= last_replaced_pos; pos++)
			{
				if (expression[pos] == id.parameter_name[i])
				{
					expression.replace(pos, 1, parameter_expression[i]);
					last_replaced_pos += parameter_expression[i].size() - 1;
				}
			}
		}
		pos = first_replaced_pos;
	}
}
int FIXnum = 0;
bool isRadian = false;
double answer = 0.0;
const int MAX_FUNCTIONS_COUNT = 32;
function functions[MAX_FUNCTIONS_COUNT];
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
	const Value &_functions = document["functions"];
	for (int i = 0; i < created_count; i++)
	{
		functions[i].isEnabled = true;
		functions[i].function_name = _functions[i]["function_name"].GetString();
		functions[i].parameter_count = _functions[i]["parameter_count"].GetInt();
		functions[i].parameter_name = _functions[i]["parameter_name"].GetString();
		functions[i].function_expression = _functions[i]["function_expression"].GetString();
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
	for (int i = 0; i < MAX_FUNCTIONS_COUNT; i++)
	{
		if (functions[i].isEnabled)
		{
			output.StartObject();
			output.Key("function_name");
			output.String(functions[i].function_name.c_str());
			output.Key("parameter_count");
			output.Int(functions[i].parameter_count);
			output.Key("parameter_name");
			output.String(functions[i].parameter_name.c_str());
			output.Key("function_expression");
			output.String(functions[i].function_expression.c_str());
			output.EndObject();
		}
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
BOOL WINAPI HandlerRoutine(DWORD dwCtrlType)
{
	if (CTRL_CLOSE_EVENT == dwCtrlType)
		SaveSettings();
	return TRUE;
}
int main()
{
	SetConsoleCtrlHandler(HandlerRoutine, TRUE);
	srand(time(NULL));
	PrintColorfully("Welcome", FOREGROUND_RED | FOREGROUND_INTENSITY);
	PrintColorfully(" to ", FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	PrintColorfully("Scientific Calculator", FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY, true);
	Stack<int> functions_pointers;
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
			else if (content == "-f")
			{
				if (created_count == MAX_FUNCTIONS_COUNT)
					PrintColorfully("The list of functions is full", FOREGROUND_RED | FOREGROUND_GREEN, true);
				else
				{
					int temp_pointer;
					if (functions_pointers.empty())
						temp_pointer = created_count;
					else
						temp_pointer = functions_pointers.pop();
					functions[temp_pointer].isEnabled = true;
					cin >> content;
					string::size_type pos = 0;
					for (; content[pos] != '('; pos++)
						functions[temp_pointer].function_name += content[pos];
					functions[temp_pointer].function_name += "(";
					pos++;
					string::size_type first_parameter_pos = pos;
					for (; content[pos] != ')'; pos++)
						if (content[pos] == ',')
							functions[temp_pointer].parameter_count++;
					functions[temp_pointer].parameter_count++;
					for (; first_parameter_pos != pos; first_parameter_pos++)
						if (('A' <= content[first_parameter_pos] && content[first_parameter_pos] <= 'Z') || ('a' <= content[first_parameter_pos] && content[first_parameter_pos] <= 'z'))
							functions[temp_pointer].parameter_name += content[first_parameter_pos];
					while (content[pos] != '=')
						pos++;
					pos++;
					for (; pos != content.size(); pos++)
						functions[temp_pointer].function_expression += content[pos];
					functions[temp_pointer].function_expression = "(" + functions[temp_pointer].function_expression + ")";
					created_count++;
				}
			}
			else if (content == "-sf")
			{
				for (int i = 0; i < MAX_FUNCTIONS_COUNT; i++)
				{
					if (!functions[i].isEnabled)
						continue;
					PrintColorfully("NO: ", FOREGROUND_RED | FOREGROUND_GREEN);
					PrintColorfully(to_string(i), FOREGROUND_GREEN, true);
					PrintColorfully("Function Name: ", FOREGROUND_RED | FOREGROUND_GREEN);
					PrintColorfully(functions[i].function_name, FOREGROUND_GREEN, true);
					PrintColorfully("Parameter Count: ", FOREGROUND_RED | FOREGROUND_GREEN);
					PrintColorfully(to_string(functions[i].parameter_count), FOREGROUND_GREEN, true);
					PrintColorfully("Parameter Name: ", FOREGROUND_RED | FOREGROUND_GREEN);
					PrintColorfully(functions[i].parameter_name, FOREGROUND_GREEN, true);
					PrintColorfully("Function Exprssion: ", FOREGROUND_RED | FOREGROUND_GREEN);
					PrintColorfully(functions[i].function_expression, FOREGROUND_GREEN, true);
					cout << endl;
				}
			}
			else if (content == "-df")
			{
				PrintColorfully("function name?", FOREGROUND_RED | FOREGROUND_GREEN);
				string function_name;
				cin >> function_name;
				for (int i = 0; i < MAX_FUNCTIONS_COUNT; i++)
				{
					if (functions[i].isEnabled)
					{
						if (functions[i].function_name == function_name)
						{
							functions[i].isEnabled = false;
							functions_pointers.push(i);
							created_count--;
						}
					}
				}
			}
			else if (content == "-exit")
				break;
			else if (content == "-fix")
			{
				PrintColorfully("FIX number? ", FOREGROUND_RED | FOREGROUND_GREEN);
				cin >> FIXnum;
				cout << setprecision(FIXnum);
			}
			else if (content == "-load")
				LoadSettings();
			else if (content == "-save")
				SaveSettings();
			else if (content == "-test")
				isTesting = true;
			else
				PrintColorfully("ERROR INSTRUCTION", FOREGROUND_RED, true);
		}
		else
		{
			if (content == "=")
				Calculate(expression, isRadian, answer);
			else
			{
				expression = content;
				bool isCalculateSuccessfully;
				if (created_count == 0)
					isCalculateSuccessfully = Calculate(expression, isRadian, answer);
				else
				{
					for (int i = 0; i < MAX_FUNCTIONS_COUNT; i++)
					{
						if (functions[i].isEnabled)
							ExpanseFunction(expression, functions[i]);
					}
					if (isTesting)
						PrintColorfully(expression, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY, true);
					isCalculateSuccessfully = Calculate(expression, isRadian, answer);
				}
				if (isCalculateSuccessfully)
				{
					cout.setf(ios::fixed);
					PrintColorfully("result: ", FOREGROUND_RED | FOREGROUND_GREEN);
					PrintColorfully(to_string(answer), FOREGROUND_GREEN, true);
				}
			}
		}
	}
	return 0;
}