#ifndef JAKDOJADE_TVECTOR_CPP
#define JAKDOJADE_TVECTOR_CPP
#include "tVector.h"

template<class T>
tVector<T>::tVector() : m_size(BUFFOR_SIZE), m_len(0) {
    m_data = new T[m_size];
}

template<class T>
tVector<T>::tVector(const tVector<T>& _vec) {
    m_size = _vec.m_size;
    m_len = _vec.m_len;
    m_data = new T[m_size];
}

template<class T>
tVector<T>::tVector(tVector<T>&& _vec) noexcept {
    m_size = _vec.m_size;
    m_len = _vec.m_len;
    m_data = _vec.m_data;

    _vec.m_size = 0;
    _vec.m_len = 0;
    _vec.m_data = nullptr;
}


template<class T>
tVector<T>::tVector(std::initializer_list<T> _initList) : m_size(BUFFOR_SIZE), m_len(0) {
    m_data = new T[m_size];
    for (auto v : _initList) push(v);
}

template<class T>
tVector<T>& tVector<T>::operator=(tVector<T>&& _other) {
    if (this != &_other) {
        delete[] m_data;

        m_size = _other.m_size;
        m_len = _other.m_len;
        m_data = _other.m_data;

        _other.m_size = 0;
        _other.m_len = 0;
        _other.m_data = nullptr;
    }
    return *this;
}

template<class T>
tVector<T>& tVector<T>::operator=(const tVector<T>& _other) {
    if (this != &_other) {
        delete[] m_data;

        m_size = _other.m_size;
        m_len = _other.m_len;
        m_data = new T[m_size];
        for (int i = 0; i < m_size; i++)
            m_data[i] = _other.m_data[i];
    }
    return *this;
}

template<class T>
T* tVector<T>::getData() const {
    return m_data;
}

template<class T>
void tVector<T>::push(const T& _element) {
    if (m_len % m_size == 0 && m_len != 0) {
        int _new_size = m_size + BUFFOR_SIZE;
        auto* _new_data = new T[_new_size];

        for (int i = 0; i < m_size; i++) {
            if (i < m_size)
                _new_data[i] = m_data[i];
        }

        m_size = _new_size;
        _new_data[m_len++] = _element;

        m_data = _new_data;
    }
    else m_data[m_len++] = _element;
}

template<class T>
void tVector<T>::push(T&& _element) {
    if (m_len % m_size == 0 && m_len != 0) {
        int _new_size = m_size + BUFFOR_SIZE;
        auto* _new_data = new T[_new_size];

        for (int i = 0; i < m_size; i++) {
            if (i < m_size)
                _new_data[i] = m_data[i];
        }

        m_size = _new_size;
        _new_data[m_len++] = _element;

        m_data = _new_data;
    }
    else m_data[m_len++] = _element;
}

template<class T>
T tVector<T>::pop() {
    if (m_len == 0) throw std::out_of_range("Trying to pop from empty vector");
    return m_data[--m_len];
}

template<class T>
int tVector<T>::len() const {
    return m_len;
}

template<class T>
int tVector<T>::size() const {
    return m_size;
}

template<class T>
T& tVector<T>::operator[](const int& _indx) const {
    if (_indx > m_size - 1) return m_data[m_size - 1];
    if (_indx < 0) {
        if (_indx < -m_size) return m_data[0];
        return m_data[m_size + _indx];
    }
    return m_data[_indx];
}

template<class T>
void tVector<T>::remove(const int& _indx) {
    if (_indx < 0 || _indx > m_len - 1)
        throw std::out_of_range("Index out of range.");
    for (int i = _indx; i < m_len - 1; i++)
        m_data[i] = m_data[i + 1];
    m_len--;
}

template<class T>
void tVector<T>::insert(const T& _element, const int& _indx) {
    if (_indx == m_len) {
        push(_element);
        return;
    }
    if (_indx < -m_size || _indx > m_size)
        throw std::out_of_range("Index out of range.");

    push(_element);

    int new_indx = _indx;
    if (new_indx < 0) new_indx += m_size;

    if (new_indx != m_len - 1) {
        if (new_indx > m_len - 1)
            for (int i = m_len - 1; i < new_indx; i++)
                std::swap(m_data[i], m_data[i + 1]);
        else
            for (int i = m_len - 1; i > new_indx; i--)
                std::swap(m_data[i], m_data[i - 1]);
    }

}

template<class T>
tVector<T> tVector<T>::_reversed_() {
    tVector<T> tmp;

    tmp.m_data = new T[m_len];
    tmp.m_len = m_len;

    for (int i = 0, j = m_len - 1; i < m_len; i++, j--)
        tmp[i] = m_data[j];

    return tmp;
}

template<class T>
tVector<T>& tVector<T>::reverse() {
    for (int i = 0, j = m_len - 1; i < j; i++, j--)
        std::swap(m_data[i], m_data[j]);

    return *this;
}


template<class T>
void tVector<T>::clear() {
    delete[] m_data;
    m_size = BUFFOR_SIZE;
    m_len = 0;
    m_data = new T[m_size];
}

template<class T>
T* tVector<T>::begin() {
    return m_data;
}

template<class T>
T* tVector<T>::end() {
    return &(m_data[m_len]);
}

template<class T>
tVector<T>::~tVector() {
    delete[] m_data;
}

template<class T>
std::ostream& operator<<(std::ostream& _out, const tVector<T>& _vector) {
    _out << "[";
    for (int i = 0; i < _vector.m_len; i++) {
        _out << _vector.m_data[i];
        if (i < _vector.m_len - 1)
            _out << ", ";
    }
    _out << "]";
    return _out;
}

template<class T>
void tVector<T>::resize(int _size) {
    T* new_box = new T[_size];
    for (int i = 0; i < m_len; i++) {
        if (i < m_len) new_box[i] = std::move(m_data[i]);
    }
    m_size = _size;
    m_data = new_box;
}

template<class T>
T tVector<T>::first() {
    if (!m_len)
        throw std::out_of_range("Trying to get value from an empty Vector!");
    T result = m_data[0];

    for (int i = 0; i < m_len - 1; i++)
        m_data[i] = m_data[i + 1];

    m_len--;

    return result;
}

template<class T>
void tVector<T>::print() const {
    std::cout << "[";
    for (int i = 0; i < m_len; i++) {
        if (i > 0) std::cout << ", ";
        std::cout << m_data[i];
    }
    std::cout << "]" << std::endl;
}

#endif