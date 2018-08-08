# redis bitops的相关应用
    512M = 512 * 1024KB * 1024Byte * 8bit = 4294967296 每一个位可以表示两种状态
    活跃人数统计(每日上线人数统计\最近n天活跃人数统计)
    判断当天是否是第一次登录(每天限领一次的逻辑判断)
    连续登录奖励发放的条件判断

# HyperLogLog
    http://algo.inria.fr/flajolet/Publications/FlFuGaMe07.pdf
    http://algo.inria.fr/flajolet/Publications/DuFl03-LNCS.pdf
    https://research.neustar.biz/2012/10/25/sketch-of-the-day-hyperloglog-cornerstone-of-a-big-data-infrastructure/
    http://content.research.neustar.biz/blog/hll.html