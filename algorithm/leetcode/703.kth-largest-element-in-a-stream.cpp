/*
 * @lc app=leetcode.cn id=703 lang=cpp
 *
 * [703] Kth Largest Element in a Stream
 */
class KthLargest {
// public:
//     KthLargest(int k, vector<int>& nums) {
//         k_ = k;
//         int size = nums.size();
//         if (size > 0)
//         {
//             sort(nums.begin(), nums.end(), [](const int a, const int b) -> bool { return a > b; });

//             for (int i = 0; i < k && i < size; ++i)
//                 nums_.push_back(nums[i]);
//         }
//     }
    
//     int add(int val) {
//         if ((int)nums_.size() < k_)
//         {
//             nums_.push_back(val);
//         }
            
//         if (val >= nums_.back())
//         {
//             nums_.pop_back();
//             nums_.push_back(val);
//         }
//         sort(nums_.begin(), nums_.end(),[](const int a, const int b) -> bool {return a > b;});
//         return nums_.back();
//     }
// private:
//     vector<int> nums_;
//     int k_;
public:
    KthLargest(int k, vector<int> &nums)
    {
        k_ = k;
        for (size_t i = 0; i < nums.size(); ++i)
        {
            pq_.push(nums[i]);
            if (pq_.size() > k)
                pq_.pop();
        }
    }
    int add(int val) 
    {
        pq_.push(val);
        if (pq_.size() > k_)
            pq_.pop();
        return pq_.top();
    }

private:
    priority_queue<int, vector<int>, greater<int>> pq_;
    int k_;
};

/**
 * Your KthLargest object will be instantiated and called as such:
 * KthLargest* obj = new KthLargest(k, nums);
 * int param_1 = obj->add(val);
 */

