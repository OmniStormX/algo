#include <algorithm>
#include <vector>
class Solution {
public:
    int hIndex(std::vector<int>& citations) {
        int n = citations.size();
        int l = 0, r = n + 1;

        auto check = [citations](int x) {
            return std::ranges::count_if(citations, [x](int h) {
                return h >= x;
            }) >= x;
        };

        while (l < r - 1) {
            int m = (l + r) >> 1;
            if (check(m)) {
                l = m;
            } else {
                r = m;
            }
        }
        return l;
    }
};