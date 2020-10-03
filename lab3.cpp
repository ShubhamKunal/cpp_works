#include <iostream>
#include <cassert>

using namespace std;

class int_list_t {
private:
    struct node_t {
        int value;
        struct node_t *next;
        struct node_t *prev;
        node_t(int new_value) : value(new_value), next(nullptr), prev(nullptr) {};
    };
public:
    int_list_t(); // O(1)
    int_list_t(const int_list_t &other); // COPY $other list O(other.size)
    int_list_t(size_t count, int value); // create list $count size and fill it with $value O($count)
    ~int_list_t();    // O(size)

    int_list_t &operator=(const int_list_t &other); // O(size + other.size())

    int &operator[](size_t pos); // O(min($pos, size - $pos))
    const int operator[](size_t pos) const; // O(min($pos, size - $pos))

    int &back();    // tail element O(1)
    const int back() const; // O(1)

    int &front();    // head element O(1)
    const int front() const; // O(1)

    void clear(); // O(size)
    size_t size() const; // O(1)
    bool empty() const; // O(1)

    void insert(size_t pos, int new_val); // insert element $new_val BEFORE element with index $pos O(min($pos, size - $pos))
    void push_front(int new_val); // O(1)
    void push_back(int new_val);    // O(1)

    void erase(size_t pos); // remove element with index $pos O(min($pos, size - $pos))
    void pop_front(); // O(1)
    void pop_back();  // O(1)

    int_list_t splice(size_t start_pos, size_t count); // splice part of list into result (not copy!) O($start_pos + $count)
    int_list_t &merge(int_list_t &other); // merge two lists, after operation $other must be valid empty list O(1)

    void reverse(); // O(size)
    void swap(int_list_t &other); // O(1)

    friend istream &operator>>(istream &stream, int_list_t &list);    // O(size)
    friend ostream &operator<<(ostream &stream, const int_list_t &list); // O(size)

private:
    size_t size_;
    node_t *head;
	node_t *tail;

    node_t* pointer_at(size_t position);
    const node_t* pointer_at(size_t position) const;
};

int_list_t::int_list_t() : size_(0), head(nullptr), tail(nullptr) {}

int_list_t::int_list_t(const int_list_t &other) : size_(0), head(nullptr), tail(nullptr) {
    for (node_t* it = other.head; it != nullptr; it = it->next) {
        push_back(it->value);
    }
}

int_list_t::int_list_t(size_t count, int value) : size_(0), head(nullptr), tail(nullptr) {
    for (size_t i = 0; i < count; ++i) {
        push_back(value);
    }
}

int_list_t::~int_list_t() {
    node_t* it = head;
    while (it != nullptr) {
        node_t* it_next = it->next;
        delete it;
        it = it_next;
    }
    // clear();
}

int_list_t::node_t* int_list_t::pointer_at(size_t pos) {
    assert(pos < size_);
    node_t* res;
    if (pos < size_ / 2) {
        res = head;
        for (size_t i = 0; i < pos; ++i) {
            res = res->next;
        }
    } else {
        res = tail;
        for (size_t i = size_ - 1; i > pos; --i) {
            res = res->prev;
        }
    }
    return res;
}

const int_list_t::node_t* int_list_t::pointer_at(size_t pos) const {
    assert(pos < size_);
    node_t* res;
    if (pos < size_ / 2) {
        res = head;
        for (size_t i = 0; i < pos; ++i) {
            res = res->next;
        }
    } else {
        res = tail;
        for (size_t i = size_ - 1; i > pos; --i) {
            res = res->prev;
        }
    }
    return res;
}

int_list_t &int_list_t::operator=(const int_list_t &other) {
    if (&other == this) {
        return *this;
    }
    clear();
    for (node_t* it = other.head; it != nullptr; it = it->next) {
        push_back(it->value);
    }
    return *this;
}

int &int_list_t::operator[](size_t pos) {
    return pointer_at(pos)->value;
}

const int int_list_t::operator[](size_t pos) const {
    return pointer_at(pos)->value;
}

int &int_list_t::back() {
    assert(!empty());
    return tail->value;
}

const int int_list_t::back() const {
    assert(!empty());
    return tail->value;
}

int &int_list_t::front() {
    assert(!empty());
    return head->value;
}

const int int_list_t::front() const {
    assert(!empty());
    return head->value;
}

void int_list_t::clear() {
    node_t* it = head;
    while (it != nullptr) {
        node_t* it_next = it->next;
        delete it;
        it = it_next;
    }
    head = tail = nullptr;
    size_ = 0;
}

size_t int_list_t::size() const {
	return size_;
}

bool int_list_t::empty() const {
	return size_ == 0;
}

void int_list_t::insert(size_t pos, int new_val) {
    if (pos == size_) {
        push_back(new_val);
        return ;
    }
    if (pos == 0) {
        push_front(new_val);
        return ;
    }

    node_t *it = pointer_at(pos);
    node_t *new_node = new node_t(new_val);
    new_node->prev = it->prev;
    if (new_node->prev != nullptr) {
        new_node->prev->next = new_node;
    }
    it->prev = new_node;
    new_node->next = it;
    ++size_;
}

void int_list_t::push_front(int new_val) {
	node_t *new_node = new node_t(new_val);
	new_node->next = head;
    if (head != nullptr) {
        head->prev = new_node;
    }
	head = new_node;
    if (size_ == 0) {
        tail = head;
    }
	++size_;
}

void int_list_t::push_back(int new_val) {
    node_t *new_node = new node_t(new_val);
    new_node->prev = tail;
    if (tail != nullptr) {
        tail->next = new_node;
    }
    tail = new_node;
    if (size_ == 0) {
        head = tail;
    }
    ++size_;
}

void int_list_t::erase(size_t pos) {
    node_t* it = pointer_at(pos);
    if (it->prev != nullptr) {
        it->prev->next = it->next;
    } else {
        head = it->next;
    }

    if (it->next != nullptr)  {
        it->next->prev = it->prev;
    } else {
        tail = it->prev;
    }

    delete it;
	--size_;
}

void int_list_t::pop_front() {
    assert(!empty());
    node_t* new_head = head->next;
    if (new_head != nullptr) {
        new_head->prev = nullptr;
    }
    delete head;
    head = new_head;
	--size_;
}

void int_list_t::pop_back() {
    assert(!empty());
	node_t *new_tail= tail->prev;
    if (new_tail != nullptr) {
        new_tail->next = nullptr;
    }
	delete tail;
	tail = new_tail;
	--size_;
}

int_list_t int_list_t::splice(size_t start_pos, size_t count) {
    assert(start_pos + count <= size_);
    if (count == 0) {
        return int_list_t();
    }

    int_list_t res;
    res.head = pointer_at(start_pos);
    res.tail = pointer_at(start_pos + count - 1);

    node_t *prev_head = res.head->prev;
    node_t *next_tail = res.tail->next;
    if (prev_head) {
        prev_head->next = next_tail;
    } else {
        head = next_tail;
    }

    if (next_tail) {
        next_tail->prev = prev_head;
    } else {
        tail = prev_head;
    }

    res.head->prev = res.tail->next = nullptr;
    res.size_ = count;

    size_ -= count;
    return res;
}

int_list_t &int_list_t::merge(int_list_t &other) {
	if (empty()) {
        head = other.head;
    } else {
        tail->next = other.head;
    }
    other.head->prev = tail;
    tail = other.tail;
    size_ += other.size_;
    other.head = nullptr;
    other.tail = nullptr;
    other.size_ = 0;
    return *this;
}

void int_list_t::reverse() {
	node_t *temp_node = head;
	node_t *next_node = head->next;
	for (size_t i = 1; i < size_; ++i) {
		temp_node->next = temp_node->prev;
		temp_node->prev = next_node;
		temp_node = next_node;
		next_node = next_node->next;
	}
	tail->next = tail->prev;
	tail->prev = nullptr;
	tail = head;
	head = temp_node;
}

void int_list_t::swap(int_list_t &other) {
	std::swap(size_, other.size_);
	std::swap(head, other.head);
	std::swap(tail, other.tail);
}

istream &operator>>(istream &stream, int_list_t &list) {
    int temp = 0;
    list.clear();
    while (stream >> temp) {
        list.push_back(temp);
    }
    return stream;
}

ostream &operator<<(ostream &stream, const int_list_t &list) {
    stream << '[';
    if (!list.empty()) {
        for (int_list_t::node_t *it = list.head; it->next != nullptr; it = it->next) {
            stream << it->value << ", ";
        }
        stream << list.back();
    }
    stream << ']';
    return stream;
}

int main(int argc, char** argv) {
    int_list_t list_of_numbers;
    cin >> list_of_numbers;
    cout << "init " << list_of_numbers << endl;
    int_list_t list_copied(list_of_numbers);
    cout << "copy " << list_copied << endl;
    int_list_t list_assigned=list_of_numbers;
    cout << "assign " << list_assigned << endl;

    cout << "front " << list_of_numbers.front() << endl;
    cout << "back " << list_of_numbers.back() << endl;
    list_of_numbers.insert(3, 7);
    cout << "insert 3 7 " << list_of_numbers << endl;
    list_of_numbers.push_front(-1);
    cout << "push front -1 "<< list_of_numbers << endl;
    list_of_numbers.push_back(5);
    cout << "push back 5 " << list_of_numbers << endl;
    list_of_numbers.erase(4);
    cout << "erase 4 " << list_of_numbers << endl;
    list_of_numbers.pop_front();
    cout << "pop front " << list_of_numbers << endl;
    list_of_numbers.pop_back();
    cout << "pop back " << list_of_numbers << endl;
    int_list_t new_list = list_of_numbers.splice(1, 2);
    cout << "new_list after splice 1 2: " << new_list << endl;
    cout << "list_of_numbers after splice: " << list_of_numbers << endl;
    new_list.swap(list_of_numbers);
    cout << "new_list after swap: " << new_list << endl;
    cout << "list_of_numbers after swap: " << list_of_numbers << endl;
    new_list.reverse();
    cout << "new_list after reverse(): " << new_list << endl;
    list_of_numbers.merge(new_list);
    cout << "new_list after merge: " << new_list << endl;
    cout << "list_of_numbers after merge: " << list_of_numbers << endl;
    return 0;
}
