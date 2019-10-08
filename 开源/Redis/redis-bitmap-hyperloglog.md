## redis bitops的相关应用
    活跃人数统计(每日上线人数统计\最近n天活跃人数统计)
    判断当天是否是第一次登录(每天限领一次的逻辑判断)
    连续登录奖励发放的条件判断

## 位图操作
    redis位图存储范围512M = 512 * 1024KB * 1024Byte * 8bit = 42亿(4294967296) 每一个位可以表示两种状态
    SETBIT key offset value                     // 对 key 所储存的字符串值 设置或清除指定偏移量上的位(bit)
        setbitCommand
    GETBIT key offset                           // 对 key 所储存的字符串值 获取指定偏移量上的位(bit)
        getbitCommand
    BITOP AND|OR|XOR|NOT destkey key [key ...]  // 对一个或多个保存二进制位的字符串 key 进行位元操作 并将结果保存到 destkey 上
        bitopCommand
    BITCOUNT key [start] [end]                  // 计算给定字符串中 被设置为 1 的比特位的数量
        bitcountCommand
    

## HyperLogLog
    http://algo.inria.fr/flajolet/Publications/FlFuGaMe07.pdf
    http://algo.inria.fr/flajolet/Publications/DuFl03-LNCS.pdf
    https://research.neustar.biz/2012/10/25/sketch-of-the-day-hyperloglog-cornerstone-of-a-big-data-infrastructure/
    http://content.research.neustar.biz/blog/hll.html