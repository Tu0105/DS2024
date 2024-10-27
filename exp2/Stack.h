#include "Vector.h"
#include <iostream>
#include <iostream>
#include <cmath>
#include <string>

template <typename T>
class Stack : public Vector<T> {
public:
    // 压入元素到栈顶
    void push(T const& e) {
        this->insert(this->size(), e);
    }

    // 弹出栈顶元素并返回它
    T pop() {
        return this->remove(this->size() - 1);
    }

    // 获取栈顶元素
    T& top() {
        return (*this)[this->size() - 1];
    }

    // 检查栈是否为空
    bool isEmpty() const {
        return this->empty();
    }

    // 获取栈的大小
    Rank size() const {
        return Vector<T>::size();
    }
};

#define N_OPTR 9 //运算符总数
typedef enum { ADD, SUB, MUL, DIV, POW, FAC, L_P, R_P, EOE } Operator; //运算符集合
//加、减、乘、除、乘方、阶乘、左括号、右括号、起始符与终止符

const char pri[N_OPTR][N_OPTR] = { //运算符优先等级 [栈顶] [当前]
    /*              |-------------------- 当 前 运 算 符 --------------------| */
    /*              +      -      *      /      ^      !      (      )      \0 */
    /* --  + */    '>',   '>',   '<',   '<',   '<',   '<',   '<',   '>',   '>',
    /* |   - */    '>',   '>',   '<',   '<',   '<',   '<',   '<',   '>',   '>',
    /* 栈  * */    '>',   '>',   '>',   '>',   '<',   '<',   '<',   '>',   '>',
    /* 顶  / */    '>',   '>',   '>',   '>',   '<',   '<',   '<',   '>',   '>',
    /* 运  ^ */    '>',   '>',   '>',   '>',   '>',   '<',   '<',   '>',   '>',
    /* 算  ! */    '>',   '>',   '>',   '>',   '>',   '>',   ' ',   '>',   '>',
    /* 符  ( */    '<',   '<',   '<',   '<',   '<',   '<',   '<',   '=',   ' ',
    /* |   ) */    ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',
    /* -- \0 */    '<',   '<',   '<',   '<',   '<',   '<',   '<',   ' ',   '='
};
char optrToChar[] = { '+', '-', '*', '/', '^', '!', '(', ')', '\0' };

// 查找运算符对应的优先级
char orderBetween(Operator top, Operator curr) {
    return pri[top][curr];
}

// 获取运算符的枚举值
Operator charToOperator(char op) {
    switch (op) {
    case '+': return ADD;
    case '-': return SUB;
    case '*': return MUL;
    case '/': return DIV;
    case '^': return POW;
    case '!': return FAC;
    case '(': return L_P;
    case ')': return R_P;
    case '\0': return EOE;
    default: return EOE; // 不识别的运算符返回终止符
    }
}

// 计算两个数的二元运算
double calc(double a, Operator op, double b) {
    switch (op) {
    case ADD: return a + b;
    case SUB: return a - b;
    case MUL: return a * b;
    case DIV: return a / b;
    case POW: return pow(a, b);
    default: throw std::invalid_argument("Invalid operator");
    }
}

// 计算单元运算 (阶乘)
double calc(double a, Operator op) {
    if (op == FAC) {
        if (a < 0 || floor(a) != a) throw std::invalid_argument("Invalid factorial");
        double result = 1;
        for (int i = 1; i <= a; ++i) result *= i;
        return result;
    }
    throw std::invalid_argument("Invalid operator");
}

// 解析并计算表达式
double evaluate(const std::string& expr) {
    Stack<double> operands;
    Stack<Operator> operators;

    operators.push(EOE); // 起始符

    size_t i = 0;
    while (!operators.empty()) {
        if (isdigit(expr[i])) { // 处理数字
            double num = 0;
            while (isdigit(expr[i])) num = num * 10 + (expr[i++] - '0');
            operands.push(num);
        }
        else { // 处理运算符
            Operator op = charToOperator(expr[i]);
            switch (orderBetween(operators.top(), op)) {
            case '<': // 栈顶运算符优先级低，当前运算符入栈
                operators.push(op);
                i++;
                break;
            case '=': // 括号匹配，脱括号
                operators.pop();
                i++;
                break;
            case '>': { // 栈顶运算符优先级高，执行运算
                Operator topOp = operators.top();
                operators.pop();
                if (topOp == FAC) { // 单目运算符
                    double a = operands.top(); operands.pop();
                    operands.push(calc(a, topOp));
                }
                else { // 双目运算符
                    double b = operands.top(); operands.pop();
                    double a = operands.top(); operands.pop();
                    operands.push(calc(a, topOp, b));
                }
                break;
            }
            default:
                throw std::invalid_argument("Invalid expression");
            }
        }
    }
    return operands.top();
}
