#include <iostream>

#include <array>
#include <iostream>
#include <map>
#include <queue>
#include <vector>
#include <assert.h>


// std::set< std::pair<int, int>> d;
// std::vector<int> dist;
// int v;
// auto it = d.find({dist[v], v});
// if (it != d.end() && dist[v] > dist[u] + w(u, v) ) {
//     d.erase(it);
//     d.insert({dist[u] + w(u, v), v});
//     dist[v] = dist[u] + w(u, v);
// }


static const size_t LINE_SIZE = 3;
static const size_t FIELD_SIZE = LINE_SIZE * LINE_SIZE;

using FieldArray = std::array<uint8_t, FIELD_SIZE>; // uint8_t a[FIELD_SIZE];

const FieldArray EMPTY_STATE = {
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
};

const FieldArray GOAL_STATE = {
    1, 2, 3,
    4, 5, 6,
    7, 8, 0
};

class FieldState {
public:
    explicit FieldState(const FieldArray& arr) {
        for (int i = 0; i < arr.size(); i++) {
            state[i] = arr[i];
            if (arr[i] == 0) {
                zero_pos = i;
            }
        }
    }
    FieldState(const FieldState& f) : state(f.state), zero_pos(f.zero_pos) {}

    std::vector<FieldState> getNextVertices() const {
        std::vector<FieldState> result;

         if (zero_pos % LINE_SIZE != 0) { //moveLeft
            FieldState fs(*this);
            fs.moveLeft();
            result.push_back(fs);
         }
         
         if ( (zero_pos + 1) % LINE_SIZE != 0) { //moveRight
            FieldState fs(*this);
            fs.moveRight();
            result.push_back(fs);
         }
         
         if ( zero_pos >= LINE_SIZE ) { //moveUp
            FieldState fs(*this);
            fs.moveUp();
            result.push_back(fs);
         }
         
        //  if ( zero_pos <= FIELD_SIZE - LINE_SIZE ) { //moveDown
         if ( zero_pos < FIELD_SIZE - LINE_SIZE ) { //moveDown
            FieldState fs(*this);
            fs.moveDown();
            result.push_back(fs);
         }
         return result;
    }

    void print() const {
        for (int i = 0; i < state.size(); ++i) {
            std::cout << (int)state[i] << " ";
            if ((i + 1) % LINE_SIZE == 0 ) {
                std::cout << std::endl;
            } 
        }
        std::cout << "--------" << std::endl;
    }

    bool operator==(const FieldState& r) const {
        return state == r.state;
    }

    bool operator!=(const FieldState& r) const {
        return state != r.state;
    }
    
    bool operator<(const FieldState& r) const {
        return state < r.state;
    }
private:
    void moveDown() {
        size_t new_zero_pos = zero_pos + LINE_SIZE;
        std::swap(state[zero_pos], state[new_zero_pos]);
        zero_pos = new_zero_pos;
    }

    void moveUp() {
        size_t new_zero_pos = zero_pos - LINE_SIZE;
        std::swap(state[zero_pos], state[new_zero_pos]);
        zero_pos = new_zero_pos;
    }

    void moveLeft() {
        size_t new_zero_pos = zero_pos - 1;
        std::swap(state[zero_pos], state[new_zero_pos]);
        zero_pos = new_zero_pos;
    }

    void moveRight() {
        size_t new_zero_pos = zero_pos + 1;
        std::swap(state[zero_pos], state[new_zero_pos]);
        zero_pos = new_zero_pos;
    }

    FieldArray state;
    size_t zero_pos;
};


void find_solve(const FieldState& start) {
    // cur, parent
    std::map<FieldState, FieldState> parent;

    FieldState goal_state(GOAL_STATE);
    FieldState empty_state(EMPTY_STATE);

    std::queue<FieldState> q;
    parent.insert({start, empty_state});
    q.push(start);

    bool is_found = false;
    int steps_count = 0;
    while (!q.empty()) {
        FieldState cur = q.front();
        q.pop();

        steps_count++;
        
        if (cur == goal_state) {
            is_found = true;
            break;
        }

        for (auto child : cur.getNextVertices()) {
            if (parent.find(child) == parent.end()) {
                parent.insert({child, cur});
                q.push(child);
            }
        }
    }

    if (is_found) {
        std::cout << "Solution is found." << std::endl;
        FieldState cur = goal_state; 
        while (cur != empty_state) {
            cur.print();

            auto it = parent.find(cur);
            cur = it->second;
        }
    } else {
        std::cout << "Solution is not found." << std::endl;
        std::cout << "Steps count = " << steps_count << std::endl;
    }
}

int main() {
    std::cout << "sem8" << std::endl;
    FieldState arr(
        {
            1,8,2,
            0,4,3,
            7,5,6
        }
    );
    find_solve(arr);
    return 0;
}
