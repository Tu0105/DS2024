#include "Vector.h"
#include <iostream>
#include <iostream>
#include <cmath>
#include <string>

template <typename T>
class Stack : public Vector<T> {
public:
    // ѹ��Ԫ�ص�ջ��
    void push(T const& e) {
        this->insert(this->size(), e);
    }

    // ����ջ��Ԫ�ز�������
    T pop() {
        return this->remove(this->size() - 1);
    }

    // ��ȡջ��Ԫ��
    T& top() {
        return (*this)[this->size() - 1];
    }

    // ���ջ�Ƿ�Ϊ��
    bool isEmpty() const {
        return this->empty();
    }

    // ��ȡջ�Ĵ�С
    Rank size() const {
        return Vector<T>::size();
    }
};

#define N_OPTR 9 //���������
typedef enum { ADD, SUB, MUL, DIV, POW, FAC, L_P, R_P, EOE } Operator; //���������
//�ӡ������ˡ������˷����׳ˡ������š������š���ʼ������ֹ��

const char pri[N_OPTR][N_OPTR] = { //��������ȵȼ� [ջ��] [��ǰ]
    /*              |-------------------- �� ǰ �� �� �� --------------------| */
    /*              +      -      *      /      ^      !      (      )      \0 */
    /* --  + */    '>',   '>',   '<',   '<',   '<',   '<',   '<',   '>',   '>',
    /* |   - */    '>',   '>',   '<',   '<',   '<',   '<',   '<',   '>',   '>',
    /* ջ  * */    '>',   '>',   '>',   '>',   '<',   '<',   '<',   '>',   '>',
    /* ��  / */    '>',   '>',   '>',   '>',   '<',   '<',   '<',   '>',   '>',
    /* ��  ^ */    '>',   '>',   '>',   '>',   '>',   '<',   '<',   '>',   '>',
    /* ��  ! */    '>',   '>',   '>',   '>',   '>',   '>',   ' ',   '>',   '>',
    /* ��  ( */    '<',   '<',   '<',   '<',   '<',   '<',   '<',   '=',   ' ',
    /* |   ) */    ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',
    /* -- \0 */    '<',   '<',   '<',   '<',   '<',   '<',   '<',   ' ',   '='
};
char optrToChar[] = { '+', '-', '*', '/', '^', '!', '(', ')', '\0' };

// �����������Ӧ�����ȼ�
char orderBetween(Operator top, Operator curr) {
    return pri[top][curr];
}

// ��ȡ�������ö��ֵ
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
    default: return EOE; // ��ʶ��������������ֹ��
    }
}

// �����������Ķ�Ԫ����
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

// ���㵥Ԫ���� (�׳�)
double calc(double a, Operator op) {
    if (op == FAC) {
        if (a < 0 || floor(a) != a) throw std::invalid_argument("Invalid factorial");
        double result = 1;
        for (int i = 1; i <= a; ++i) result *= i;
        return result;
    }
    throw std::invalid_argument("Invalid operator");
}

// ������������ʽ
double evaluate(const std::string& expr) {
    Stack<double> operands;
    Stack<Operator> operators;

    operators.push(EOE); // ��ʼ��

    size_t i = 0;
    while (!operators.empty()) {
        if (isdigit(expr[i])) { // ��������
            double num = 0;
            while (isdigit(expr[i])) num = num * 10 + (expr[i++] - '0');
            operands.push(num);
        }
        else { // ���������
            Operator op = charToOperator(expr[i]);
            switch (orderBetween(operators.top(), op)) {
            case '<': // ջ����������ȼ��ͣ���ǰ�������ջ
                operators.push(op);
                i++;
                break;
            case '=': // ����ƥ�䣬������
                operators.pop();
                i++;
                break;
            case '>': { // ջ����������ȼ��ߣ�ִ������
                Operator topOp = operators.top();
                operators.pop();
                if (topOp == FAC) { // ��Ŀ�����
                    double a = operands.top(); operands.pop();
                    operands.push(calc(a, topOp));
                }
                else { // ˫Ŀ�����
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
