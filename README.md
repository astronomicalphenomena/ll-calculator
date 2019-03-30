# Scientific Calculator
This program uses [RapidJson](https://github.com/Tencent/rapidjson/) to save and load settings.

- **What can it do?**
  - It can calculate any correct expression,save user-defined functions and change some settings.
- **How to get it?**
  - [Download](https://github.com/LuKaderZ/ll-calculator/releases) the latest program or compile the latest code.
- **How to use it?**
  - Enter any correct expression and press **Enter** key then it will print the result.

# Introduction
### Supported Operators:
```cpp
+ - * / % ^
! sqrt() abs() lg() ln()
sin() cos() tan()
arcsin() arccos() arctan()
sinh() cosh() tanh()
arsinh() arcosh() artanh()
min(x,y) max(x,y) logx(y)
```
### User-defined function:
| instruction name |      function      |
|:----------------:|:------------------:|
|        -f        | create a function  |
|       -df        | delete a function  |
|       -sf        | show all functions |

Functional expression format:***functionName*(*parameterName1*,*parameterName2*...)=*expression***<br/>
(e.g.):**add(x,y)=x+y**

### Special operands:

- pi
- e
- ans
- rand#

### Other instructions:
| instruction name |                      function                      |
|:----------------:|:--------------------------------------------------:|
|       -rad       |                Calculate in radian                 |
|       -reg       |                Calculate in degree                 |
|       -fix       |                 Set decimal digits                 |
|      -load       |                   Load settings                    |
|      -save       |                   Save settings                    |
|      -test       | Print test information before it prints the result |
|      -exit       |                 Close the program                  |
|        =         |                  Do the last step                  |

# License (MIT)

Copyright (c) 2019 LuKaderZ

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
