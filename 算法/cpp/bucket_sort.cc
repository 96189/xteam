
// https://gist.github.com/chenshuo/1966839

// 已知数据规模为n 数据范围为M 获取每个数据在序列种的排名(对数据排序)
// 桶/计数排序 O(n) + O(M) 排序不仅与数据规模n相关 与数据范围M也相关
// 遍历数据序列n(可能存在重复得值) 得到每个数据得统计值count(出现得次数)
// 统计每个值得积分(非重复得值规模为M) 得到每个数据得积分值accum
// 递推式accum[i] = accum[i-1] + count[i] ; i > 1 count[i]由分桶得到 
// 最终x的排名为accum[x]+1

#include <vector>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef int32_t Score;
typedef int32_t UserId;
typedef int32_t Rank;
const Score kInvalidScore = -1;

enum ErrorCode
{
	kSucceed,
	kFailed
};

struct Result
{
	ErrorCode error;
	Rank rank;
	int32_t numUsersOfSameScore;

	Result()
		: error(kFailed),
		  rank(0),
		  numUsersOfSameScore(0)
	{

	}
};

// 假设分数区间为[0,1000]
// 用户量为1000 0000
const int kMaxUsers = 10000000; 
const int kMaxScore = 1000;	

const bool kDebug = false;

class ScoreTable
{
  public:
	ScoreTable()
	{
		usersAtScore_.resize(kMaxScore + 1);
		for (UserId userId = 1; userId <= kMaxUsers; ++userId)
		{
			// score范围[0, kMaxScore]
			Score score = rand() % (kMaxScore + 1);
			assert(0 <= score && score <= kMaxScore);
			// 对n规模得数据分桶 计算统计值count[i]
			usersAtScore_[score] += 1;
		}
		// 计算完大于某分数的总人数,保存到usersAboveScore_
        // 计算积分值 accum[i]
		partialSum().swap(usersAboveScore_);
		// 在最低分的人数 + 大于最低分的总人数 == 总人数
		assert(usersAtScore_[0] + usersAboveScore_[0] == kMaxUsers);
	}

	Result getRank(Score score) const
	{
		Result result;
		assert(0 <= score && score <= kMaxScore);
		result.error = kSucceed;
		// score的排名为大于score的总人数+1
		// usersAboveScore_[score]表示大于socre的人数即[score+1,n]的总人数
		// 要计算当前score的排名,为[score+1,n]的总人数+1
		result.rank = usersAboveScore_[score] + 1;
		// 在score分数的总人数
		result.numUsersOfSameScore = usersAtScore_[score];
		return result;
	}

	Result updateScore(Score oldScore, Score newScore)
	{
		Result result;
		if (0 <= newScore && newScore <= kMaxScore)
		{
			if (oldScore == kInvalidScore)
			{
				oldScore = 0;
			}
			else
			{
				assert(0 <= oldScore && oldScore <= kMaxScore);
				// 旧的桶减1
				usersAtScore_[oldScore] -= 1;
				if (usersAtScore_[oldScore] < 0)
				{
					// error !!
					usersAtScore_[oldScore] = 0;
				}
			}
			// 新的桶加1
			usersAtScore_[newScore] += 1;

			// 更新(oldScore, newScore)或者(newScore,oldScore)区间的usersAboveScore_值
			updateCache(oldScore, newScore);
			result = getRank(newScore);
		}
		return result;
	}

  private:
	// 桶的个数,依赖最大的分数 数据范围M
	// 保存在某分数上的总人数 count统计值
	// index为分数
	// usersAtScore_[index]为等于该分数的人数 分桶累加
	std::vector<int32_t> usersAtScore_;	// radix bucket
	// 大小依赖最大分数 数据范围M
	// 保存大于某分数的总人数 accum积分值
	// index为分数
	// usersAboveScore_[index]为大于该分数的人数
	std::vector<int32_t> usersAboveScore_; // partial sum of above

	// 对分数倒序遍历,计算大于某分数的总人数
    // 积分值accum计算
	std::vector<int32_t> partialSum()
	{
		std::vector<int32_t> sum;
		assert(usersAtScore_.size() == kMaxScore + 1);
		sum.resize(usersAtScore_.size());
		int32_t count = 0;
		// 依照递推式 计算accum[i]值
        for (int score = kMaxScore; score >= 0; --score)
		{
			sum[score] = count;
			count = count + usersAtScore_[score];
		}
		return sum;
	}

	// 更新(oldScore, newScore)或者(newScore,oldScore)区间的usersAboveScore_值
	void updateCache(Score oldScore, Score newScore)
	{
		if (oldScore < newScore)
		{
			// 分数变大,区间值依次加1
			for (int i = oldScore; i < newScore; ++i)
			{
				usersAboveScore_[i] += 1;
			}
		}
		else if (oldScore > newScore)
		{
			// 分数减小,区间值依次减1
			for (int i = newScore; i < oldScore; ++i)
			{
				usersAboveScore_[i] -= 1;
			}
		}

		if (kDebug)
		{
			assert(partialSum() == usersAboveScore_);
		}
	}
};

int main()
{
	ScoreTable table;
	for (Score score = kMaxScore; score >= 0; --score)
	{
		Result rank = table.getRank(score);
		printf("%3d %5d %5d\n", score, rank.rank, rank.numUsersOfSameScore);
	}

	{
		Score oldScore = 860; 
		Result oldRank = table.getRank(oldScore);
		printf("\nbefore update\n%3d %5d %5d\n",
			   oldScore, oldRank.rank, oldRank.numUsersOfSameScore);
		Score newScore = 999; 
		Result newRank = table.updateScore(oldScore, newScore);
		printf("after update\n%3d %5d %5d\n\n",
			   newScore, newRank.rank, newRank.numUsersOfSameScore);
	}
	for (Score score = kMaxScore; score >= 0; --score)
	{
		Result rank = table.getRank(score);
		printf("%3d %5d %5d\n", score, rank.rank, rank.numUsersOfSameScore);
	}
}