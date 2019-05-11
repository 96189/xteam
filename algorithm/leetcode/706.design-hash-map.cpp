/*
 * @lc app=leetcode.cn id=706 lang=cpp
 *
 * [706] Design HashMap
 */
class MyHashMap {
public:
    /** Initialize your data structure here. */
    MyHashMap() {
        
    }
    
    /** value will always be non-negative. */
    void put(int key, int value) {
        for (int i = hashMap_.size(); i <= key; ++i)
            hashMap_.push_back(-1);
        hashMap_[key] = value;
    }
    
    /** Returns the value to which the specified key is mapped, or -1 if this map contains no mapping for the key */
    int get(int key) {
        if (key >= hashMap_.size())
            return -1;
        return hashMap_[key];
    }
    
    /** Removes the mapping of the specified value key if this map contains a mapping for the key */
    void remove(int key) {
        if (key >= hashMap_.size())
            return;
        hashMap_[key] = -1;
    }
private:
    vector<int> hashMap_;
};

/**
 * Your MyHashMap object will be instantiated and called as such:
 * MyHashMap* obj = new MyHashMap();
 * obj->put(key,value);
 * int param_2 = obj->get(key);
 * obj->remove(key);
 */

