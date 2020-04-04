/*=========================================================
> File Name: calexpression.cpp
> Author:hooray
> Mail:hoorayitt@gmail.com 
> Created Time: 2019年05月04日 星期六 21时55分59秒
> =======================================================*/
/*
11、算术表达式求值 (选做) （栈）
[问题描述]
　　一个算术表达式是由操作数(operand)、运算符(operator)和界限符(delimiter)组成的。假设操作数是正实数，运算符只含加减
乘除等四种运算符，界限符有左右括号和表达式起始、结束符“#”，如：#6+15*（21-8/4）#。引入表达式起始、结束符是为了方便。
编程利用“运算符优先法”求算术表达式的值。
[基本要求]
（1） 从键盘或文件读入一个合法的算术表达式，输出正确的结果。
（2） 显示输入序列和栈的变化过程。
（3） 考虑算法的健壮性，当表达式错误时，要给出错误原因的提示。
（4） 实现非整数的处理。
*/
#include<iostream>
using namespace std;
#include<stdio.h>
#include<stdlib.h>
#include<stack>
#include<string.h>

#define MAX_LENGTH 50
stack<char>Ch;//存储操作符 
stack<char>Ch2;//存储ch的逆置，实现ch栈的遍历 
stack<double>Num;//存储操作数 
stack<double>Num2;//存储Num栈的逆置，实现遍历 
class P{
public:
    int flag;//记录当前为操作数还是操作符，0--数，1--符
    double num;//0
    char c;//1
};
P p[MAX_LENGTH];//存储表达式 

bool error = false;
char* errorMsg = new char[100];
class Result{
public:
	bool flag;
	char *msg;
	double value;
	Result(bool f,char *m,double v):flag(f),msg(m),value(v){}
};

bool In(char c)//判断是否为操作符 
{
    if(c=='#'||c=='('||c==')'||c=='+'||c=='-'||c=='*'||c=='/')
        return true;
    return false;
}

void input(char *s)
{
    int num=0;
    for(unsigned int i=0;i<strlen(s);i++)
    {
        if(In(s[i]))//若当前字符为操作符，存储
        {
            p[num].flag=1;
            p[num++].c = s[i];
        }
        else if(s[i]>='0'&&s[i]<='9')//若为操作数，则将字符串该位置往后映射，隔离出前面为double的部分。
        {
            double t;
            char k[30];
            sscanf(&s[i],"%lf%s",&t,k);	//该函数在此处的功能就是
            //将字符串前面的double格式的内容存入t，字符串剩余部分存入k
            strcpy(s,k);//更新字符串
            i=-1;//字符串从0再次遍历
            p[num].flag = 0;
            p[num++].num = t;
        }
        else if(s[i]=='.')
        {
            sprintf(errorMsg, "小数点的位置有误");
			error = true;
        }
        else
        {
            sprintf(errorMsg,"出现非法字符%c",s[i]);
			error = true;
        }

    }

    if(p[0].flag==1&&p[0].c!='(')
    {
		sprintf(errorMsg, "开头出错");
		error = true;
    }

}
char Precede(char top,char c)//比较字符串优先级关系，以及对括号的准确性检测 
{

    if(top=='+'||top=='-')
    {
        if(c=='+'||c=='-'||c==')'||c=='#')
        {
            return '>';
        }
        else return '<';
    }
    if(top=='*'||top=='/')
    {
        if(c=='(')
        {
            return '<';

        }
        else{
            return '>';
        }
    }
    if(top=='(')
    {
        if(c=='#'||c=='(')
        {
            sprintf(errorMsg, "错误：缺少 ）");
			error = true;
            return '=';
        }
        else if(c==')')
        {
            return '=';
        }
        else{
            return '<';
        }
    }
    if(top==')')
    {
        return '>';
    }
    if(top=='#')
    {
        if(c=='#')
        {
            return '=';
        }
        else if(c==')')
        {
            sprintf(errorMsg, "错误：缺少 （");
			error = true;
            return '=';
        }
        else return '<';
    }
}
double Operat(double a, char t, double b)//运算a t b。 
{
    if(t=='*')
        return a*b;
    else if(t=='+')
        return a+b;
    else if(t=='-')
        return a-b;
    else if(t=='/')//除数不能为零
    {
        if(b<0.00001&&b>-0.00001)
        {
            sprintf(errorMsg, "错误：当前公式中除数为零");
			error = true;
			return 0;
        }
		else
			return a/b;
    }
}
Result* calExpression(char *s)
{
	error = false;

    input(s);//将表达式存储
	if(error) return new Result(true, errorMsg, 0);
    int n=0;
    Ch.push('#');


    while(p[n].flag==0||p[n].c!='#'||Ch.top()!='#')//当栈顶为#并且当前字符也为#时退出循环
    {

        if(p[n].flag==0)//若为操作数，则存入Num栈
        {
            Num.push(p[n].num);
            n++;
        }
        else//若为符号
        {
            switch(Precede(Ch.top(),p[n].c))//判断优先级关系
            {
            case '<':{
                Ch.push(p[n].c);
                n++;//读取下一个
                break;
            }
            case'=':{//去括号
				if(error) return new Result(true, errorMsg, 0);
                Ch.pop();
                n++;//读取下一个
                break;

            }
            case'>':{//出栈进行运算
                char t;
                double a,b;
                t = Ch.top();
                Ch.pop();
                b = Num.top();
                Num.pop();
                a = Num.top();
                Num.pop();
                Num.push(Operat(a,t,b));//将结果压栈
				if(error) return new Result(true, errorMsg, 0);

                break;
            }
            }
        }
    }
    return new Result(false, errorMsg,Num.top());
}
