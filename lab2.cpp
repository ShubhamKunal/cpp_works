#include <iostream>

using namespace std;

// Class safe_long_long_t
class safe_long_long_t {
    private:
        long long number;
    public:

        // constructors
        safe_long_long_t(long long num = 0) { number = num; }
        safe_long_long_t(const safe_long_long_t &num) { number = num.number; }

        // operator equal_to(=)
        void operator=(safe_long_long_t const &num) {
            number = num.number;
        }

        // Arithmetic Operators
        friend safe_long_long_t operator+(const safe_long_long_t &variable1, const safe_long_long_t &variable2);
        friend safe_long_long_t operator-(const safe_long_long_t &variable1, const safe_long_long_t &variable2);
        friend safe_long_long_t operator*(const safe_long_long_t &variable1, const safe_long_long_t &variable2);
        friend safe_long_long_t operator/(const safe_long_long_t &variable1, const safe_long_long_t &variable2);

        safe_long_long_t operator+=(const safe_long_long_t &num) {
            number += num.number;
            return *this;
        }

        safe_long_long_t operator-=(const safe_long_long_t &num) {
            number -= num.number;
            return *this;
        }

        safe_long_long_t operator*=(const safe_long_long_t &num) {
            number *= num.number;
            return *this;
        }

        safe_long_long_t operator/=(const safe_long_long_t &num) {
            number /= num.number;
            return *this;
        }

        safe_long_long_t operator++() {
            ++number;
            return safe_long_long_t(number);
        }

        safe_long_long_t operator++(int) {
            safe_long_long_t num(number);
            ++number;
            return num;
        }

        safe_long_long_t operator--() {
            --number;
            return safe_long_long_t(number);
        }

        safe_long_long_t operator--(int) {
            safe_long_long_t num(number);
            --number;
            return num;
        }

        // Comparison Operators
        bool operator<(safe_long_long_t const &num) { return number < num.number; }
        bool operator<=(safe_long_long_t const &num) { return number <= num.number; }
        bool operator>(safe_long_long_t const &num) { return number > num.number; }
        bool operator>=(safe_long_long_t const &num) { return number >= num.number; }
        bool operator!=(safe_long_long_t const &num) { return number != num.number; }
        bool operator==(safe_long_long_t const &num) { return number == num.number; }

        //operator << for output and operator >> for input
        friend ostream &operator<<(ostream &os, const safe_long_long_t &var);
        friend istream &operator>>(istream &is, safe_long_long_t &var);

        void printnum(){
            cout << this->number << endl;
        }

};

safe_long_long_t operator+(const safe_long_long_t &variable1, const safe_long_long_t &variable2) { return safe_long_long_t(variable1.number + variable2.number); }
safe_long_long_t operator-(const safe_long_long_t &variable1, const safe_long_long_t &variable2) { return safe_long_long_t(variable1.number - variable2.number); }
safe_long_long_t operator*(const safe_long_long_t &variable1, const safe_long_long_t &variable2) { return safe_long_long_t(variable1.number * variable2.number); }
safe_long_long_t operator/(const safe_long_long_t &variable1, const safe_long_long_t &variable2) { return safe_long_long_t(variable1.number / variable2.number); }

ostream &operator<<(ostream &os, const safe_long_long_t &var) {
    os << var.number;
    return os;
}

istream &operator>>(istream &is, safe_long_long_t &var) {
    is >> var.number;
    return is;
}

// class safe_c_array_t
class safe_c_array_t {
    private:
        int* a;
        int size;
    public:

        // On creation, takes the size and allocates the array via new
        safe_c_array_t(int len = 0) {
            a = new int[len];
            size = len;
        }

        // You can access values via operator []
        int &operator[](int i) {
            if (i > size) {
                cout << "Out of range" << endl;
                return a[0];
            }
            return a[i];
        }

        // to Copy
        void operator=(const safe_c_array_t &num) {
            if (this->a == num.a) return;
            delete[] a;
            size = num.size;
            a = new int[size];
            copy(num.a, num.a+size, a);
        }

        // On destruction, deletes the allocated array
        ~safe_c_array_t() {
            delete[] a;
        }
};

int main() {

    safe_long_long_t obj1 = safe_long_long_t(8);
    safe_long_long_t obj2 = safe_long_long_t(obj1);
    obj1.printnum();
    obj1.operator++();
    obj1.printnum();
    obj2.printnum();
    obj2.operator=(81);
    obj2.printnum();
    safe_long_long_t obj3 = operator+(obj1,obj2);
    obj1.operator+=(66);
    obj1.printnum();
    operator<<(cout,obj1);

    safe_c_array_t arr1(10);
    safe_c_array_t arr2(3);
    arr2[0] = 9;
    cout << arr2[0] << endl;
    arr1 = arr2;
    cout << arr1[0] << endl;

}
