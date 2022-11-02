struct Node {
    int val;
    Node *left;
    Node *right;
};

Jaki będzie wynik powzszego algorytmu:
{
vector<vector<int>> output;
        if (!root) {
            return output;
        }
        std::queue<TreeNode*> queue;
        queue.push(root);
        bool should_reverse = false;
        while(!queue.empty()) {
            int temp_queue_size = queue.size();
            output.push_back({});
            for(int i = 0; i < temp_queue_size; ++i) {
                auto node = queue.front();
                queue.pop();
                if(node->left) {
                    queue.push(node->left);
                }
                if(node->right) {
                    queue.push(node->right);
                }

                output.back().push_back(node->val);
            }
            if(should_reverse) {
                std::reverse(output.back().begin(), output.back().end());
            }
            should_reverse = !should_reverse;
        }
        return output;

}