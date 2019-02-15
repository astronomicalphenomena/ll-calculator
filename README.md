# Scientific Calculator
This program uses [RapidJson](https://github.com/Tencent/rapidjson/) to save and load settings.
## What can it do?
It can calculate any supported expression,save user-defined functions and change some settings.
## How to get it?
[Download](https://github.com/LuKaderZ/ll-calculator/releases) the latest program.
## How to use it?
Enter any correct expression and press Enter then it will print the result.<br/>
**Please don't enter any space,it will let the program stop.**
# Introduction
## Supported Operators:
```cpp
+ - * / % ^
! sqrt() abs() lg() ln()
sin() cos() tan()
arcsin() arccos() arctan()
sinh() cosh() tanh()
arsinh() arcosh() artanh()
min(x,y) max(x,y) logx(y)
```
## User-defined function:
### Create
You can enter this instruction to create your function.<br/>
**-f**<br/>
Then enter the function expression to save your function.<br/>
Format:***functionName*(*parameterName1*,*parameterName2*...)=*expression***<br/>
(e.g.):**add(x,y)=x+y**
### Delete
You enter this instruction to delete your function.<br/>
**-df**
### Show
You enter this instruction to show your functions.<br/>
**-sf**
## Special operands:
This program provides 2 constants and 2 variables.<br/>
**pi e ans rand#**
## Other instructions:
### -rad
Calculate in radian
### -reg
Calculate in degree
### -fix
Set decimal digits
### -load
Load settings
### -save
Save settings
### -test
Print test information before it prints the result
### -exit
Close the program
### =
Do the last step
