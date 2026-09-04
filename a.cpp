#include <bits/stdc++.h>
#include <vector>

using namespace std;

struct node {
	node* l;
	node* r;
	int val;
	node(int v = -1): l(nullptr), r(nullptr), val(v) {
	}
};

vector<int> mid, back;

vector<int> sub(vector<int>a, int l, int r) {
	vector<int> d;
	d.reserve(r - l + 1);
	for (int i = l; i <= r; i++) {
		d.push_back(a[i]);
	}
	return d;

}

node *build(node* rt, vector<int>m, vector<int>b) {
	if (m.size() == 0) return nullptr;
	node mid = b.back();

	int p = -1;
	for (int i = 0; i < m.size(); i++) {
		if (m[i] == b.back()) {
			p = i;
		}
	}
	vector<int> c = sub(m, 0, p - 1);
	vector<int> cr = sub(b, 0, c.size() - 1);

	mid.l = build(mid.l, c, cr);
	mid.r = build(mid.r, sub(m, p + 1, m.size() - 1), sub(b, c.size(), b.size() - 2));

	return &mid;
}


int main() {
	mid = {9, 3, 15, 20, 7};
	back = {9, 15, 7, 20, 3};
	node rt = node(back.back());

	for (int i = 0; i < mid.size(); i++) {
		if (mid[i] == back.back()) {

		}
	}
	node* root = build(&rt, mid, back);
}