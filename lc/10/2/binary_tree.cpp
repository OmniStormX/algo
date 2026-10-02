/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left),
 * right(right) {}
 * };
 */

struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode() : val(0), left(nullptr), right(nullptr) {}
  TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
  TreeNode(int x, TreeNode *left, TreeNode *right)
      : val(x), left(left), right(right) {}
};
#include <bits/stdc++.h>

#define debug(x)                                                               \
  cerr << "[debug] Line:" << __LINE__ << " " << #x << " : " << x << std::endl
using namespace std;
class Solution {
public:
  vector<vector<int>> zigzagLevelOrder(TreeNode *root) {
    vector<vector<int>> d = {};

    auto dfs = [&d](auto &&self, TreeNode *r, int dep) -> void {
      if (r == nullptr)
        return;
      if (dep == d.size()) {
        d.push_back({});
      }
      d[dep].push_back(r->val);
      if (r->left != nullptr) {
        self(self, r->left, dep + 1);
      }
      if (r->right != nullptr) {
        self(self, r->right, dep + 1);
      }
    };

    dfs(dfs, root, 0);
    for (int i = 0; i < d.size(); i++) {
      if (i & 1) {
        reverse(d[i].begin(), d[i].end());
      }
    }
    return d;
  }
};