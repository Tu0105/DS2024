typedef int Rank;
#include<stdlib.h>
#include<utility>


#define DEFAULT_CAPACITY  3 //Ĭ�ϵĳ�ʼ������ʵ��Ӧ���п�����Ϊ����

template <typename T> class Vector { //����ģ����

    friend int main();

protected:
    Rank _size; Rank _capacity;  T* _elem; //��ģ��������������
    void copyFrom(T const* A, Rank lo, Rank hi); //������������A[lo, hi)
    void expand(); //�ռ䲻��ʱ����
    void shrink(); //װ�����ӹ�Сʱѹ��
    bool bubble(Rank lo, Rank hi); //ɨ�轻��
    void bubbleSort(Rank lo, Rank hi); //���������㷨
    Rank maxItem(Rank lo, Rank hi); //ѡȡ���Ԫ��
    void selectionSort(Rank lo, Rank hi); //ѡ�������㷨
    void merge(Rank lo, Rank mi, Rank hi); //�鲢�㷨
    void mergeSort(Rank lo, Rank hi); //�鲢�����㷨
    void heapSort(Rank lo, Rank hi); //�������Ժ�����ȫ�ѽ��⣩
    Rank partition(Rank lo, Rank hi); //��㹹���㷨
    void quickSort(Rank lo, Rank hi); //���������㷨
    void shellSort(Rank lo, Rank hi); //ϣ�������㷨
public:
    // ���췽��
    Vector(Rank c = DEFAULT_CAPACITY) //����Ϊc�Ŀ�����
    {
        _elem = new T[_capacity = c]; _size = 0;
    }
    Vector(Rank c, Rank s, T v) //����Ϊc����ģΪs������Ԫ�س�ʼΪv��s<=c
    {
        _elem = new T[_capacity = c]; for (_size = 0; _size < s; _elem[_size++] = v);
    }
    Vector(T const* A, Rank n) { copyFrom(A, 0, n); } //�������帴��
    Vector(T const* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); } //����
    Vector(Vector<T> const& V) { copyFrom(V._elem, 0, V._size); } //�������帴��
    Vector(Vector<T> const& V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); } //����
    // ��������
    ~Vector() { delete[] _elem; } //�ͷ��ڲ��ռ�
    // ֻ�����ʽӿ�
    Rank size() const { return _size; } //��ģ
    bool empty() const { return !_size; } //�п�
    Rank find(T const& e) const { return find(e, 0, _size); } //���������������
    Rank find(T const& e, Rank lo, Rank hi) const; //���������������
    Rank select(Rank k) { return quickSelect(_elem, _size, k); } //�������������ҵ���k���Ԫ��
    Rank search(T const& e) const //���������������
    {
        return (0 >= _size) ? -1 : search(e, 0, _size);
    }
    Rank search(T const& e, Rank lo, Rank hi) const; //���������������
    // ��д���ʽӿ�
    T& operator[] (Rank r); //�����±������������������������ʽ���ø�Ԫ��
    const T& operator[] (Rank r) const; //����������ֵ�����ذ汾
    Vector<T>& operator= (Vector<T> const&); //���ظ�ֵ���������Ա�ֱ�ӿ�¡����
    T remove(Rank r); //ɾ����Ϊr��Ԫ��
    Rank remove(Rank lo, Rank hi); //ɾ����������[lo, hi)֮�ڵ�Ԫ��
    Rank insert(Rank r, T const& e); //����Ԫ��
    Rank insert(T const& e) { return insert(_size, e); } //Ĭ����ΪĩԪ�ز���
    void sort(Rank lo, Rank hi); //��[lo, hi)����
    void sort() { sort(0, _size); } //��������
    void unsort(Rank lo, Rank hi); //��[lo, hi)����
    void unsort() { unsort(0, _size); } //��������
    Vector<T> findByModulusRange(double m1, double m2) const;
    Rank dedup(); //����ȥ��
    Rank uniquify(); //����ȥ��
    // ����
    void traverse(void (*) (T&)); //������ʹ�ú���ָ�룬ֻ����ֲ����޸ģ�
    template <typename VST> void traverse(VST&); //������ʹ�ú������󣬿�ȫ�����޸ģ�
}; //Vector

//���ڸ��ƵĹ��췽��
template <typename T> void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi) {
    _elem = new T[_capacity = 2 * (hi - lo)];_size = 0;
    while (lo < hi)
    {
        _elem[_size++] = A[lo++];
    }
}

//����������ֵ������
template <typename T> Vector<T>& Vector<T>::operator=(Vector<T> const& V) {
    if (_elem) delete[] _elem;
    copyFrom(V._elem, 0, V.size());
    return *this;
}

//����
template <typename T> void Vector<T>::expand() {
    if (_size < _capacity) return;
    if (_capacity < DEFAULT_CAPACITY) _capacity = DEFAULT_CAPACITY;
    T* oldElem = _elem; _elem = new T[_capacity <<= 1];
    for (int i = 0; i < _size; i++)
        _elem[i] = oldElem[i];
    delete[] oldElem;
}

//����
template <typename T> void Vector<T>::shrink() {
    if (_capacity < DEFAULT_CAPACITY << 1) return;
    if (_size << 2 > _capacity) return;
    T* oldElem = _elem; _elem = new T[_capacity >>= 1];
    for (int i = 0; i < _size; i++) _elem[i] = oldElem[i];
    delete[] oldElem;
}

//��������������
template <typename T> T& Vector<T>::operator[](Rank r) {
    return _elem[r];
}

// �����汾�����ز�����
template <typename T>
const T& Vector<T>::operator[](Rank r) const {
    if (r < 0 || r >= _size) {
        throw std::out_of_range("Index out of range");
    }
    return _elem[r];
}

//��������
template <typename T> void permute(Vector<T>& V) {
    for (int i = V.size(); i > 0; i--)
        swap(V[i - 1], V[rand() % i]);
}

//������������
template <typename T> void Vector<T>::unsort(Rank lo, Rank hi) {
    T* V = _elem + lo;
    for (Rank i = hi - lo; i > 0; i--)
        std::swap(V[i - 1], V[rand() % i]);
}

//��������Ԫ�ز���
template <typename T> Rank Vector<T>::find(T const& e, Rank lo, Rank hi) const {
    while ((lo < hi--) && (e != _elem[hi]));
    return hi;
}

//����Ԫ�ز���
template <typename T> Rank Vector<T>::insert(Rank r, T const& e) {
    expand();
    for (int i = _size; i > r; i--) _elem[i] = _elem[i - 1];
    _elem[r] = e; _size++;
    return r;
}

//��������ɾ��
template <typename T> int Vector<T>::remove(Rank lo, Rank hi) {
    if (lo == hi) return 0;
    while (hi < _size) _elem[lo++] = _elem[hi++];
    _size = lo;
    shrink();
    return hi - lo;
}

//������Ԫ��ɾ��
template <typename T> T Vector<T>::remove(Rank r) {
    T e = _elem[r];
    remove(r, r + 1);
    return e;
}

//������������ظ�Ԫ��
template <typename T> Rank Vector<T>::dedup() {
    int oldSize = _size;
    Rank i = 1;
    while (i < _size)
        (find(_elem[i], 0, i) < 0) ?
        i++ : remove(i);
    return oldSize - _size;
}

//����
template <typename T> void Vector<T>::traverse(void (*visit)(T&)) {
    for (int i = 0; i < _size; i++) visit(_elem[i]);
}

template <typename T> template <typename VST> void Vector<T>::traverse(VST& visit) {
    for (int i = 0; i < _size; i++) visit(_elem[i]);
}

//���ڱ���ʵ��increase()
template <typename T> struct Increase {
    virtual void operator()(T& e) { e++; }
};

template <typename T> void increase(Vector<T>& V) {
    V.traverse(Increase<T>());
}

//Ψһ��(��Ч��)
template <typename T> Rank Vector<T>::uniquify() {
    Rank i = 0, j = 0;
    while (++j < _size)
        if (_elem[i] != _elem[j])
            _elem[++i] = _elem[j];
    _size = ++i; shrink();
    return j - i;
}

//���ֲ���
// ���ֲ���
template <typename T> static Rank binsearch(T* A, T const& e, Rank lo, Rank hi) {
    while (lo < hi) {
        Rank mi = (lo + hi) >> 1;
        (e < A[mi]) ? hi = mi : lo = mi + 1;
    }
    return --lo;
}


//ð������
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

//��������
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

//�鲢����
template <typename T> void Vector<T>::mergeSort(Rank lo, Rank hi) {
    if (hi - lo <= 1) return; // ��ֻ��һ��Ԫ��ʱ����������
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
    Vector<T> result; // ����һ���µ��������洢���
    for (Rank i = 0; i < _size; i++) {
        double mod = _elem[i].modulus(); // ����ģ
        if (mod >= m1 && mod < m2) {
            result.insert(result.size(), _elem[i]); // ����������
        }
    }
    return result; // ���ؽ������
}
