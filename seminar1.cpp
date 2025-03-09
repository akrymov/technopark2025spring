#include <assert.h>
#include <iostream>
#include <sstream>

class Queue {
private:
    struct Node {
        Node(Node* next, int value) : next(next), value(value){}

        Node* next;
        int value;
    };
public:
    Queue(): head(nullptr), tail(nullptr){
    }

    ~Queue() {
        Node* cur = head;
        while (cur) {
            Node* node = cur;
            cur = cur->next;
            delete node;
        }
    }

    Queue(const Queue &r) : head(nullptr), tail(nullptr) {
        Node* cur = r.head;
        while (cur) {
            push(cur->value);
            cur = cur->next;
        }
    }

    Queue& operator=(const Queue& r) {
        Node *cur = r.head;
        while (cur) {
            push(cur->value);
            cur = cur->next;
        }
        return *this;
    }

    bool is_empty() const {
        return head == tail && head == nullptr;
    }

    int pop() {
        Node* node = head;
        int result = node->value;
        if (head == tail) {
            head = nullptr;
            tail = nullptr;
        } else {
            head = head->next;
        }
        delete node;
        return result;
    }

    void push(int value) {
        Node* node = new Node(nullptr, value);
        if (head == nullptr && tail == nullptr) {
            head = node;
        } else {
            tail->next = node;
        }
        tail = node;
    }
private:
    Node* head;
    Node* tail;
};

void testQueue() {
    {
        Queue q;
        assert(q.is_empty());
    }
    {
        Queue q;
        assert(q.is_empty());

        int value = 5;
        q.push(value);
        assert(q.pop() == value);

        assert(q.is_empty());
    }
    {
        Queue q;
        assert(q.is_empty());

        for (int i = 0; i < 10; ++i) {
            q.push(i);
        }

        for (int i = 0; i < 10; ++i) {
            assert(q.pop() == i);
        }
        assert(q.is_empty());
    }
    {
        Queue q;
        assert(q.is_empty());

        for (int i = 0; i < 10; ++i) {
            q.push(i);
            assert(q.pop() == i);
        }
        assert(q.is_empty());
    }
    {
        Queue q1;
        assert(q1.is_empty());

        for (int i = 0; i < 10; ++i) {
            q1.push(i);
        }

        Queue q2(q1);
        assert(q2.is_empty() == false);

        for (int i = 0; i < 10; ++i) {
            assert(q2.pop() == i);
        }

        assert(q1.is_empty() == false);
        assert(q2.is_empty());
    }
    std::cout << "testQueue OK" << std::endl;
}

int run(std::istream& in, std::ostream& out) {
    // testQueue();
    int n = 0;
    in >> n;

    Queue q;
    for (int i = 0; i < n; ++i) {
        int command = 0;
        int input_value = 0;
        in >> command >> input_value;
        int result = -1;
        switch(command) {
            case 2: //pop
                if (q.is_empty()) {
                    result = -1;
                } else {
                    result = q.pop();
                }
                if (input_value != result) {
                    out << "NO" << std::endl;
                    return 0;
                }
                break;
            case 3: // push
                q.push(input_value);
                break;
            default:
                out << "NO" << std::endl;
                return 0;
        }
    }
    out << "YES" << std::endl;
    return 0;
}

void testLogic() {
    {
        std::stringstream in;
        std::stringstream out;

        in << "3\n";
        in << "3 44\n";
        in << "3 50\n";
        in << "2 44\n";

        run(in, out);

        assert(out.str() == "YES\n");
    }
    std::cout << "testLogic OK" << std::endl;
}

int main() {
    run(std::cin, std::cout);
    // testLogic();
}
