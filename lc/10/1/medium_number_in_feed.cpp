// https://leetcode.cn/problems/find-median-from-data-stream
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using ordered_multiset = tree<
    std::pair<int, int>,
    null_type,
    std::less<std::pair<int, int>>,
    rb_tree_tag,
    tree_order_statistics_node_update
>;
class MedianFinder {
    ordered_multiset s;
    int id;
public:
    MedianFinder(): id(0) {
    }

    void addNum(int num) {
        s.insert({num, id++});
    }

    double findMedian() {
        int k = s.size();
        if (k & 1) {
            auto [v, k0] = *s.find_by_order(k / 2);
            return double(v);
        } else {
            auto [v1, k1] = *s.find_by_order(k / 2 - 1);
            auto [v2, k2] = *s.find_by_order(k / 2);
            return (double(v1) + double(v2)) / 2.0;
        }
    }
};

/**
 * Your MedianFinder object will be instantiated and called as such:
 * MedianFinder* obj = new MedianFinder();
 * obj->addNum(num);
 * double param_2 = obj->findMedian();
 */