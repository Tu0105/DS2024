#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "Vector.cpp" 

class Complex {
public:
    double real;  // 实部
    double imag;  // 虚部

    Complex(double r = 0, double i = 0) : real(r), imag(i) {}

    // 计算复数的模
    double modulus() const {
        return std::sqrt(real * real + imag * imag);
    }

    // 重载输出运算符
    friend std::ostream& operator<<(std::ostream& os, const Complex& c) {
        os << "(" << c.real << ", " << c.imag << ")";
        return os;
    }

    // 重载相等运算符
    bool operator==(const Complex& other) const {
        return real == other.real && imag == other.imag;
    }

    // 重载不等于运算符
    bool operator!=(const Complex& other) const {
        return real != other.real || imag != other.imag;
    }

    bool operator<=(const Complex& other) const {
        // 先比较模，如果相同则比较实部
        if (this->modulus() < other.modulus()) {
            return true;
        }
        else if (this->modulus() > other.modulus()) {
            return false;
        }
        else {
            return this->real <= other.real; // 模相同则比较实部
        }
    }
    
};
void generateRandomArray(Vector<Complex>& vec, int size);
void generateOrderedArray(Vector<Complex>& vec, int size);
void generateReverseOrderedArray(Vector<Complex>& vec, int size);
double measureTime(void (Vector<Complex>::* sortMethod)(int, int), Vector<Complex>& vec, int lo, int hi);

// 生成随机数组
void generateRandomArray(Vector<Complex>& vec, int size) {
    for (int i = 0; i < size; ++i) {
        vec.insert(Complex(rand() % 10, rand() % 10));
    }
}

// 生成顺序数组
void generateOrderedArray(Vector<Complex>& vec, int size) {
    for (int i = 0; i < size; ++i) {
        vec.insert(Complex(i, i));
    }
}

// 生成逆序数组
void generateReverseOrderedArray(Vector<Complex>& vec, int size) {
    for (int i = size - 1; i >= 0; --i) {
        vec.insert(Complex(i, i));
    }
}

// 计时函数
double measureTime(void (Vector<Complex>::* sortMethod)(int, int), Vector<Complex>& vec, int lo, int hi) {
    clock_t start = clock();
    (vec.*sortMethod)(lo, hi);
    clock_t end = clock();
    return double(end - start) / CLOCKS_PER_SEC;
}

int main() {
    srand(static_cast<unsigned>(time(0)));  // 初始化随机数生成器

    // 生成一个随机的无序复数向量
    Vector<Complex> vec;
    for (int i = 0; i < 10; ++i) {
        vec.insert(Complex(rand() % 10, rand() % 10));  // 插入随机复数
    }

    std::cout << "无序复数向量: ";
    vec.traverse([](Complex& c) { std::cout << c << " "; });
    std::cout << std::endl;

    // 置乱
    vec.unsort();
    std::cout << "置乱后的复数向量: ";
    vec.traverse([](Complex& c) { std::cout << c << " "; });
    std::cout << std::endl;

    // 查找（实部和虚部均相同的复数）
    Complex searchFor(3, 4);
    Rank foundIndex = vec.find(searchFor, 0, vec.size());
    if (foundIndex >= 0) {
        std::cout << "找到复数: " << vec[foundIndex] << std::endl;
    }
    else {
        std::cout << "未找到复数: " << searchFor << std::endl;
    }

    // 插入新复数
    vec.insert(1, Complex(1, 1));
    std::cout << "插入后的复数向量: ";
    vec.traverse([](Complex& c) { std::cout << c << " "; });
    std::cout << std::endl;

    // 删除复数
    vec.remove(2);
    std::cout << "删除后的复数向量: ";
    vec.traverse([](Complex& c) { std::cout << c << " "; });
    std::cout << std::endl;

    // 唯一化
    vec.dedup();
    std::cout << "唯一化后的复数向量: ";
    vec.traverse([](Complex& c) { std::cout << c << " "; });
    std::cout << std::endl;

    vec.bubbleSort(0, vec.size());  // 调用冒泡排序
    std::cout << "冒泡排序后的复数向量: ";
    vec.traverse([](Complex& c) { std::cout << c << " "; });
    std::cout << std::endl;

    vec.mergeSort(0, vec.size());  // 调用归并排序
    std::cout << "归并排序后的复数向量: ";
    vec.traverse([](Complex& c) { std::cout << c << " "; });
    std::cout << std::endl;

    // 打印排序后的结果
    std::cout << "根据模排序后的复数向量: ";
    vec.traverse([](Complex& c) { std::cout << c << " "; });
    std::cout << std::endl;

    // 查找模在指定范围内的复数
    double m1 = 1.0, m2 = 5.0;
    Vector<Complex> filtered = vec.findByModulusRange(m1, m2);
    std::cout << "模在 [" << m1 << ", " << m2 << ") 范围内的复数: ";
    filtered.traverse([](Complex& c) { std::cout << c << " "; });
    std::cout << std::endl;

    const int SIZE = 1000;  // 测试规模
    

    // 顺序数组测试
    generateOrderedArray(vec, SIZE);
    double timeBubbleOrdered = measureTime(&Vector<Complex>::bubbleSort, vec, 0, vec.size());
    double timeMergeOrdered = measureTime(&Vector<Complex>::mergeSort, vec, 0, vec.size());

    // 乱序数组测试
    vec = Vector<Complex>();  // 重置向量
    generateRandomArray(vec, SIZE);
    double timeBubbleRandom = measureTime(&Vector<Complex>::bubbleSort, vec, 0, vec.size());
    double timeMergeRandom = measureTime(&Vector<Complex>::mergeSort, vec, 0, vec.size());

    // 逆序数组测试
    vec = Vector<Complex>();  // 重置向量
    generateReverseOrderedArray(vec, SIZE);
    double timeBubbleReverse = measureTime(&Vector<Complex>::bubbleSort, vec, 0, vec.size());
    double timeMergeReverse = measureTime(&Vector<Complex>::mergeSort, vec, 0, vec.size());

    // 打印结果
    std::cout << "顺序数组：冒泡排序时间 = " << timeBubbleOrdered << "s, 归并排序时间 = " << timeMergeOrdered << "s\n";
    std::cout << "乱序数组：冒泡排序时间 = " << timeBubbleRandom << "s, 归并排序时间 = " << timeMergeRandom << "s\n";
    std::cout << "逆序数组：冒泡排序时间 = " << timeBubbleReverse << "s, 归并排序时间 = " << timeMergeReverse << "s\n";

    return 0;

}
