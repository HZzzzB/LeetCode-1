// Time:  O(nlogn)
// Space: O(n)

// Segment Tree solution.
class Solution {
public:
    vector<int> fallingSquares(vector<pair<int, int>>& positions) {
        set<int> index;
        for (const auto& position : positions) {
            index.emplace(position.first);
            index.emplace(position.first + position.second - 1);
        }
        SegmentTree tree(index.size());
        auto max_height = 0;
        vector<int> result;
        for (const auto& position : positions) {
            const auto L = distance(index.begin(), index.find(position.first));
            const auto R = distance(index.begin(), index.find(position.first + position.second - 1));
            const auto h = tree.query(L, R) + position.second;
            tree.update(L, R, h);
            max_height = max(max_height, h);
            result.emplace_back(max_height);
        }
        return result;
    }

private:
    class SegmentTree {
    public:
        SegmentTree(int N)
            : N_(N),
              tree_(2 * N),
              lazy_(N)
        {
            H_ = 1;
            while ((1 << H_) < N) {
                ++H_;
            }
        }

        void update(int L, int R, int h) {
            L += N_; R += N_;
            int L0 = L, R0 = R;
            while (L <= R) {
                if ((L & 1) == 1) {
                    apply(L++, h);
                }
                if ((R & 1) == 0) {
                    apply(R--, h);
                }
                L >>= 1; R >>= 1;
            }
            pull(L0); pull(R0);
        }

        int query(int L, int R) {
            L += N_; R += N_;
            auto result = 0;
            push(L); push(R);
            while (L <= R) {
                if ((L & 1) == 1) {
                    result = max(result, tree_[L++]);
                }
                if ((R & 1) == 0) {
                    result = max(result, tree_[R--]);
                }
                L >>= 1; R >>= 1;
            }
            return result;
        }

    private:
        int N_, H_;
        vector<int> tree_, lazy_;

        void apply(int x, int val) {
            tree_[x] = max(tree_[x], val);
            if (x < N_) {
                lazy_[x] = max(tree_[x], val);
            }
        }

        void pull(int x) {
            while (x > 1) {
                x >>= 1;
                tree_[x] = max(tree_[x * 2], tree_[x * 2 + 1]);
                tree_[x] = max(tree_[x], lazy_[x]);
            }
        }

        void push(int x) {
            for (int h = H_; h > 0; --h) {
                int y = x >> h;
                if (lazy_[y] > 0) {
                    apply(y * 2, lazy_[y]);
                    apply(y * 2 + 1, lazy_[y]);
                    lazy_[y] = 0;
                }
            }
        }
    };
};


// Time:  O(n^2)
// Space: O(n)
class Solution2 {
public:
    vector<int> fallingSquares(vector<pair<int, int>>& positions) {
        vector<int> heights(positions.size());
        for (int i = 0; i < positions.size(); ++i) {
            int left_i, size_i;
            tie(left_i, size_i) = positions[i];
            int right_i = left_i + size_i;
            heights[i] += size_i;
            for (int j = i + 1; j < positions.size(); ++j) {
                int left_j, size_j;
                tie(left_j, size_j) = positions[j];
                int right_j = left_j + size_j;
                if (left_j < right_i and left_i < right_j) {  // intersect
                    heights[j] = max(heights[j], heights[i]);
                }
            }
        }

        vector<int> result;
        for (const auto& height : heights) {
            result.emplace_back(result.empty() ? height : max(result.back(), height));
        }
        return result;
    }
};