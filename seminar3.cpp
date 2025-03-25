#include <iostream>
#include <vector>
#include <assert.h>
#include <algorithm>

template<typename T>
struct DefaultComparator {
    bool operator()(const T& l, const T& r) const {
        return l < r;
    }
};

template <typename T, typename Comparator = DefaultComparator<T>>
int partition(T *arr, int l, int r, Comparator comp) {
    int pivot_idx = l + (r - l) / 2;
    std::swap(arr[pivot_idx], arr[r - 1]);

    int i = l;
    int j = r - 1;
    T& pivot = arr[r - 1];

    while (i < j) {
        while ( comp(arr[i], pivot) ) {
            i++;
        }
        // arr[i] >= pivot

        for (; !comp(arr[j], pivot); j--);
        // arr[j] < pivot

        if (i < j) {
            std::swap(arr[i], arr[j]);
            i++;
            j--;
        }
    }
    std::swap(arr[i], arr[r - 1]);
    return i;
}

template <typename T, typename Comparator = DefaultComparator<T>>
void kth_element(T * arr, int size, int k, Comparator comp = Comparator()) {
    int l = 0;
    int r = size;
    while (l < r) {
        int m = partition(arr, l, r, comp);
        if (m == k) {
            return;
        } else if (m < k) {
            l = m + 1;
        } else {
            r = m;
        }
    }
}

void test() {
    {
        std::vector<int> v = {5,4,3,2};
        int k = 3;
        kth_element(&v[0], v.size(), k);

        int value = v[k];

        std::sort(v.begin(), v.end());
        assert(value == v[k]);
        std::cout << "OK1" << std::endl;
    }
    {
        std::vector<int> v = {5};
        int k = 0;
        kth_element(&v[0], v.size(), k);

        int value = v[k];

        std::sort(v.begin(), v.end());
        assert(value == v[k]);
        std::cout << "OK1" << std::endl;
    }
    {
        std::vector<int> v = {2,2};
        int k = 1;
        kth_element(&v[0], v.size(), k);

        int value = v[k];

        std::sort(v.begin(), v.end());
        assert(value == v[k]);
        std::cout << "OK3" << std::endl;
    }
    std::cout << "OK" << std::endl;
}

void CountingSort1(int *a, int n, int k) {
    int r = k + 1;
    int *c = new int[r];
    for (int i = 0; i < r; ++i)
        c[i] = 0;
    for (int i = 0; i < n; ++i)
        ++c[a[i]];
    int pos = 0;
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c[i]; ++j) {
            a[pos++] = i;
        }
    }
    delete[] c;
}

void CountingSort2(int *a, int n, int k) {
    int r = k + 1;
    int *c = new int[r];
    for (int i = 0; i < r; ++i)
        c[i] = 0;
    for (int i = 0; i < n; ++i)
        ++c[a[i]];
    for (int i = 1; i < r; ++i) {
        c[i] += c[i - 1]; // Концы групп.
    }
    int *b = new int[n];
    for (int i = n - 1; i >= 0; --i) { // Проход с конца.
        b[--c[a[i]]] = a[i];
    }
    delete[] c;
    for (int i = 0; i < n; ++i) {
        a[i] = b[i];
    }
    delete[] b;
}

struct Person {
    std::string name;
    int year;

    bool operator<(const Person& r) const {
        return year < r.year;
    }

    bool operator==(const Person& r) const {
        return year == r.year;
    }

    Person() = default;

    Person(const Person& p) : name(p.name), year(p.year) {
    }

    Person(const std::string& s, int y) : name(s), year(y){}

    Person& operator=(const Person& p) {
        year = p.year;
        name = p.name;
        return *this;
    }
};

template<typename T>
void CountingSort(T *a, int n, int k, int (*get_key)(const T&)) {
    int r = k + 1;
    int *c = new int[r];
    for (int i = 0; i < r; ++i)
        c[i] = 0;
    for (int i = 0; i < n; ++i) {
        int key = get_key(a[i]);
        ++c[key];
    }
    for (int i = 1; i < r; ++i) {
        c[i] += c[i - 1]; // Концы групп.
    }
    T *b = new T[n];
    for (int i = n - 1; i >= 0; --i) { // Проход с конца.
        int key = get_key(a[i]);
        b[--c[key]] = a[i];
    }
    delete[] c;
    for (int i = 0; i < n; ++i) {
        a[i] = b[i];
    }
    delete[] b;
}

template<typename T>
void sort_cmp(T* arr, int size) {
    T* arr_cmp = new T[size];
    std::copy(&arr[0], &arr[size], &arr_cmp[0]);
    std::sort(&arr_cmp[0], &arr_cmp[size]);

    for (int i = 0; i < size; ++i) {
        assert(arr[i] == arr_cmp[i]);
    }
    delete[] arr_cmp;
}

void testCountingSort1() {
    {
        std::vector<int> v = {5,4,3,2};
        CountingSort1(&v[0], v.size(), 5);
        sort_cmp(&v[0], v.size());
        std::cout << "cs1 ok" << std::endl;
    }
}

void testCountingSort2() {
    {
        std::vector<int> v = {5,4,3,2};
        CountingSort2(&v[0], v.size(), 5);
        sort_cmp(&v[0], v.size());
        std::cout << "cs2 ok" << std::endl;
    }
}

void testCountingSort() {
    {
        std::vector<Person> v = {
            {"ivan", 1986},
            {"petr", 1960},
            {"gleb", 2005}};
        CountingSort<Person>(&v[0], v.size(), 2005, [](const Person& p) {
            return p.year;
        });
        sort_cmp<Person>(&v[0], v.size());
        std::cout << "cs3 ok" << std::endl;
    }
}

int main() {
    test();
    testCountingSort1();
    testCountingSort2();
    testCountingSort();
    return 0;
}
