/*
 * @lc app=leetcode.cn id=690 lang=cpp
 *
 * [690] 员工的重要性
 */
/*
// Employee info
class Employee {
public:
    // It's the unique ID of each node.
    // unique id of this employee
    int id;
    // the importance value of this employee
    int importance;
    // the id of direct subordinates
    vector<int> subordinates;
};
*/
class Solution {
public:
    int getImportance(vector<Employee*> employees, int id) {
        if (employees.empty())
            return 0;
        static map<int, Employee *> id2employee;
        for (auto &emPtr : employees)
        {
            id2employee[emPtr->id] = emPtr;
        }
        if (!id2employee[id])
            return 0;
        int sum = id2employee[id]->importance;
        auto &&sub = id2employee[id]->subordinates;
        for (size_t i = 0; i < sub.size(); ++i)
        {
            int subId = sub[i];
            sum += id2employee[subId]->importance;
            if (!id2employee[subId]->subordinates.empty())
            {
                for (auto & id : id2employee[subId]->subordinates) 
                    sub.push_back(id);
            }
        }
        return sum;
    }
};

