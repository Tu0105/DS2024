#include <iostream>
#include <cstdlib> // For rand()
#include <ctime>   // For time()
#include "Stack.h"  // 引入 Stack.h



// 计算最大矩形面积的函数
int largestRectangleArea(const Vector<int>& heights) {
    Stack<int> indices;
    int maxArea = 0;
    int n = heights.size();

    for (int i = 0; i <= n; ++i) {
        while (!indices.isEmpty() && (i == n || heights[i] < heights[indices.top()])) {
            int height = heights[indices.pop()];
            int width = indices.isEmpty() ? i : i - indices.top() - 1;
            maxArea = std::max(maxArea, height * width);
        }
        indices.push(i);
    }

    return maxArea;
}

// 随机生成数据进行测试
void generateRandomTestCases() {
    srand(static_cast<unsigned>(time(0))); // 初始化随机种子
    for (int t = 0; t < 10; ++t) {
        int n = rand() % 10 + 1; // 生成 1 到 10 之间的随机数
        Vector<int> heights; // 使用自定义 Vector 类存储高度

        // 随机生成柱子高度
        for (int i = 0; i < n; ++i) {
            heights.insert(heights.size(), rand() % 10); // 使用 insert 方法插入数据
        }

        // 输出测试数据
        std::cout << "Test case " << t + 1 << ": ";
        for (int i = 0; i < heights.size(); ++i) {
            std::cout << heights[i] << " ";
        }
        std::cout << "\nMax area: " << largestRectangleArea(heights) << "\n\n";
    }
}


int main() {
    Stack<int> stack;
    stack.push(10);
    stack.push(20);
    stack.push(30);

    std::cout << "栈顶元素: " << stack.top() << std::endl; // 输出: 30
    stack.pop();
    std::cout << "弹出后新的栈顶元素: " << stack.top() << std::endl; // 输出: 20

    std::cout << "栈的大小: " << stack.size() << std::endl; // 输出: 2

    std::string expr;
    std::cout << "请输入表达式：";
    std::getline(std::cin, expr);
    try {
        double result = evaluate(expr);
        std::cout << "计算结果: " << result << std::endl;
    }
    catch (const std::invalid_argument& e) {
        std::cout << "无效的表达式: " << e.what() << std::endl;
    }

    generateRandomTestCases();

    return 0;
}
