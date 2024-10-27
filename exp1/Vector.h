typedef int Rank;
#include<stdlib.h>
#include<utility>


#define DEFAULT_CAPACITY  3 //默认的初始容量（实际应用中可设置为更大）

template <typename T> class Vector { //向量模板类

    friend int main();

protected:
    Rank _size; Rank _capacity;  T* _elem; //规模、容量、数据区
    void copyFrom(T const* A, Rank lo, Rank hi); //复制数组区间A[lo, hi)
    void expand(); //空间不足时扩容
    void shrink(); //装填因子过小时压缩
    bool bubble(Rank lo, Rank hi); //扫描交换
    void bubbleSort(Rank lo, Rank hi); //起泡排序算法
    Rank maxItem(Rank lo, Rank hi); //选取最大元素
    void selectionSort(Rank lo, Rank hi); //选择排序算法
    void merge(Rank lo, Rank mi, Rank hi); //归并算法
    void mergeSort(Rank lo, Rank hi); //归并排序算法
    void heapSort(Rank lo, Rank hi); //堆排序（稍后结合完全堆讲解）
    Rank partition(Rank lo, Rank hi); //轴点构造算法
    void quickSort(Rank lo, Rank hi); //快速排序算法
    void shellSort(Rank lo, Rank hi); //希尔排序算法
public:
    // 构造方法
    Vector(Rank c = DEFAULT_CAPACITY) //容量为c的空向量
    {
        _elem = new T[_capacity = c]; _size = 0;
    }
    Vector(Rank c, Rank s, T v) //容量为c、规模为s、所有元素初始为v；s<=c
    {
        _elem = new T[_capacity = c]; for (_size = 0; _size < s; _elem[_size++] = v);
    }
    Vector(T const* A, Rank n) { copyFrom(A, 0, n); } //数组整体复制
    Vector(T const* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); } //区间
    Vector(Vector<T> const& V) { copyFrom(V._elem, 0, V._size); } //向量整体复制
    Vector(Vector<T> const& V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); } //区间
    // 析构方法
    ~Vector() { delete[] _elem; } //释放内部空间
    // 只读访问接口
    Rank size() const { return _size; } //规模
    bool empty() const { return !_size; } //判空
    Rank find(T const& e) const { return find(e, 0, _size); } //无序向量整体查找
    Rank find(T const& e, Rank lo, Rank hi) const; //无序向量区间查找
    Rank select(Rank k) { return quickSelect(_elem, _size, k); } //从无序向量中找到第k大的元素
    Rank search(T const& e) const //有序向量整体查找
    {
        return (0 >= _size) ? -1 : search(e, 0, _size);
    }
    Rank search(T const& e, Rank lo, Rank hi) const; //有序向量区间查找
    // 可写访问接口
    T& operator[] (Rank r); //重载下标操作符，可以类似于数组形式引用各元素
    const T& operator[] (Rank r) const; //仅限于做右值的重载版本
    Vector<T>& operator= (Vector<T> const&); //重载赋值操作符，以便直接克隆向量
    T remove(Rank r); //删除秩为r的元素
    Rank remove(Rank lo, Rank hi); //删除秩在区间[lo, hi)之内的元素
    Rank insert(Rank r, T const& e); //插入元素
    Rank insert(T const& e) { return insert(_size, e); } //默认作为末元素插入
    void sort(Rank lo, Rank hi); //对[lo, hi)排序
    void sort() { sort(0, _size); } //整体排序
    void unsort(Rank lo, Rank hi); //对[lo, hi)置乱
    void unsort() { unsort(0, _size); } //整体置乱
    Vector<T> findByModulusRange(double m1, double m2) const;
    Rank dedup(); //无序去重
    Rank uniquify(); //有序去重
    // 遍历
    void traverse(void (*) (T&)); //遍历（使用函数指针，只读或局部性修改）
    template <typename VST> void traverse(VST&); //遍历（使用函数对象，可全局性修改）
}; //Vector

//基于复制的构造方法
template <typename T> void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi) {
    _elem = new T[_capacity = 2 * (hi - lo)];_size = 0;
    while (lo < hi)
    {
        _elem[_size++] = A[lo++];
    }
}

//重载向量赋值操作符
template <typename T> Vector<T>& Vector<T>::operator=(Vector<T> const& V) {
    if (_elem) delete[] _elem;
    copyFrom(V._elem, 0, V.size());
    return *this;
}

//扩容
template <typename T> void Vector<T>::expand() {
    if (_size < _capacity) return;
    if (_capacity < DEFAULT_CAPACITY) _capacity = DEFAULT_CAPACITY;
    T* oldElem = _elem; _elem = new T[_capacity <<= 1];
    for (int i = 0; i < _size; i++)
        _elem[i] = oldElem[i];
    delete[] oldElem;
}

//缩容
template <typename T> void Vector<T>::shrink() {
    if (_capacity < DEFAULT_CAPACITY << 1) return;
    if (_size << 2 > _capacity) return;
    T* oldElem = _elem; _elem = new T[_capacity >>= 1];
    for (int i = 0; i < _size; i++) _elem[i] = oldElem[i];
    delete[] oldElem;
}

//重载向量操作符
template <typename T> T& Vector<T>::operator[](Rank r) {
    return _elem[r];
}

//整体置乱
template <typename T> void permute(Vector<T>& V) {
    for (int i = V.size(); i > 0; i--)
        swap(V[i - 1], V[rand() % i]);
}

//向量区间置乱
template <typename T> void Vector<T>::unsort(Rank lo, Rank hi) {
    T* V = _elem + lo;
    for (Rank i = hi - lo; i > 0; i--)
        std::swap(V[i - 1], V[rand() % i]);
}

//无序向量元素查找
template <typename T> Rank Vector<T>::find(T const& e, Rank lo, Rank hi) const {
    while ((lo < hi--) && (e != _elem[hi]));
    return hi;
}

//向量元素插入
template <typename T> Rank Vector<T>::insert(Rank r, T const& e) {
    expand();
    for (int i = _size; i > r; i--) _elem[i] = _elem[i - 1];
    _elem[r] = e; _size++;
    return r;
}

//向量区间删除
template <typename T> int Vector<T>::remove(Rank lo, Rank hi) {
    if (lo == hi) return 0;
    while (hi < _size) _elem[lo++] = _elem[hi++];
    _size = lo;
    shrink();
    return hi - lo;
}

//向量单元素删除
template <typename T> T Vector<T>::remove(Rank r) {
    T e = _elem[r];
    remove(r, r + 1);
    return e;
}

//无序向量清除重复元素
template <typename T> Rank Vector<T>::dedup() {
    int oldSize = _size;
    Rank i = 1;
    while (i < _size)
        (find(_elem[i], 0, i) < 0) ?
        i++ : remove(i);
    return oldSize - _size;
}

//遍历
template <typename T> void Vector<T>::traverse(void (*visit)(T&)) {
    for (int i = 0; i < _size; i++) visit(_elem[i]);
}

template <typename T> template <typename VST> void Vector<T>::traverse(VST& visit) {
    for (int i = 0; i < _size; i++) visit(_elem[i]);
}

//基于遍历实现increase()
template <typename T> struct Increase {
    virtual void operator()(T& e) { e++; }
};

template <typename T> void increase(Vector<T>& V) {
    V.traverse(Increase<T>());
}

//唯一化(高效版)
template <typename T> Rank Vector<T>::uniquify() {
    Rank i = 0, j = 0;
    while (++j < _size)
        if (_elem[i] != _elem[j])
            _elem[++i] = _elem[j];
    _size = ++i; shrink();
    return j - i;
}

//二分查找
// 二分查找
template <typename T> static Rank binsearch(T* A, T const& e, Rank lo, Rank hi) {
    while (lo < hi) {
        Rank mi = (lo + hi) >> 1;
        (e < A[mi]) ? hi = mi : lo = mi + 1;
    }
    return --lo;
}


//冒泡排序
template <typename T>
void Vector<T>::sort(Rank lo, Rank hi) {
    for (int i = lo; i < hi - 1; i++) {
        for (int j = lo; j < hi - i - 1; j++) {
            if (_elem[j].modulus() > _elem[j + 1].modulus()) {
                std::swap(_elem[j], _elem[j + 1]);
            }
        }
    }
}

//起泡排序
template <typename T> void Vector<T>::bubbleSort(Rank lo, Rank hi) {
    for (Rank i = lo; i < hi - 1; i++) {
        for (Rank j = lo; j < hi - i - 1; j++) {
            if (_elem[j].modulus() > _elem[j + 1].modulus() ||
                (_elem[j].modulus() == _elem[j + 1].modulus() && _elem[j].real > _elem[j + 1].real)) {
                std::swap(_elem[j], _elem[j + 1]);
            }
        }
    }
}

//归并排序
template <typename T> void Vector<T>::mergeSort(Rank lo, Rank hi) {
    if (hi - lo <= 1) return; // 当只有一个元素时，无需排序
    Rank mid = (lo + hi) / 2;
    mergeSort(lo, mid);
    mergeSort(mid, hi);
    merge(lo, mid, hi);
}

template <typename T>
void Vector<T>::merge(Rank lo, Rank mi, Rank hi) {
    int n1 = mi - lo;
    int n2 = hi - mi;

    T* L = new T[n1];
    T* R = new T[n2];

    for (int i = 0; i < n1; i++)
        L[i] = _elem[lo + i];
    for (int j = 0; j < n2; j++)
        R[j] = _elem[mi + j];

    int i = 0, j = 0, k = lo;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            _elem[k++] = L[i++];
        }
        else {
            _elem[k++] = R[j++];
        }
    }
    while (i < n1) {
        _elem[k++] = L[i++];
    }
    while (j < n2) {
        _elem[k++] = R[j++];
    }

    delete[] L;
    delete[] R;
}

template <typename T> Vector<T> Vector<T>::findByModulusRange(double m1, double m2) const {
    Vector<T> result; // 创建一个新的向量来存储结果
    for (Rank i = 0; i < _size; i++) {
        double mod = _elem[i].modulus(); // 计算模
        if (mod >= m1 && mod < m2) {
            result.insert(result.size(), _elem[i]); // 插入结果向量
        }
    }
    return result; // 返回结果向量
}
