#include <iostream>
#include "BPT.hpp"

int main() {
    std::ios::sync_with_stdio(false);
    int n;
    std::cin >> n;
    CrazyDave::BPlusTree<CrazyDave::String<65>, int> bpt("node",
                                                         "node_storage",
                                                         "block",
                                                         "block_storage");
    while (n--) {
        CrazyDave::String<65> op, index;
        int value;
        std::cin >> op;
        if (op[0] == 'i') {
            std::cin >> index >> value;
            bpt.insert(index, value);
        } else if (op[0] == 'd') {
            std::cin >> index >> value;
            bpt.remove(index, value);
        } else if (op[0] == 'p') {
            bpt.print();
        } else {
            std::cin >> index;
            auto res = bpt.find(index);
            for (auto x: res) {
                std::cout << x << ' ';
            }
            if (res.empty()) {
                std::cout << "null";
            }
            std::cout << std::endl;
        }
    }
    return 0;

}
