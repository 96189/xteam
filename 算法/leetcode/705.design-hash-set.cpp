/*
 * @lc app=leetcode.cn id=705 lang=cpp
 *
 * [705] Design HashSet
 */
class MyHashSet {
public:
    /** Initialize your data structure here. */
    MyHashSet() {
        
    }
    
    void add(int key) {
        if (!contains(key))
        {
            hashv_.push_back(key);
        }
    }
    
    void remove(int key) {
        auto it = find(hashv_.begin(), hashv_.end(), key);
        if (it != hashv_.end())
        {
            hashv_.erase(it);
        }
    }
    
    /** Returns true if this set contains the specified element */
    bool contains(int key) {
        auto it = find(hashv_.begin(), hashv_.end(), key);
        return it != hashv_.end();
    }
private:
    vector<int> hashv_;
};

/**
 * Your MyHashSet object will be instantiated and called as such:
 * MyHashSet* obj = new MyHashSet();
 * obj->add(key);
 * obj->remove(key);
 * bool param_3 = obj->contains(key);
 */

