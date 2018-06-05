
STL空间配置器:

一级空间适配器:
提供以下接口
// 提供内存不足时的处理方法__malloc_alloc_oom_handler
void (* __set_malloc_handler(void (*__f)()))()
// 调用系统malloc，失败调用上述 __malloc_alloc_oom_handler
void* allocate(size_t __n)
// 调用系统realloc,失败调用上述 __malloc_alloc_oom_handler
void* reallocate(void* __p, size_t /* old_sz */, size_t __new_sz)
// 调用系统free
void deallocate(void* __p, size_t /* __n */)


二级空间适配器:
基本策略:
>128Byte的内存申请,丢给一级空间适配器处理.
<=128Byte的内存申请,由二级空间适配器处理.每一个内存申请按照8Byte对齐,因此二级空间适配器可处理的内存size为8、16、24、32、40、48、56、64、72、80、88、96、104、112、120、128共16个.内部采用内存池来维护内存的申请和释放,内存池数据结构为16个单元的数组,每个单元存储一个链表,该链表维护size的n个节点.

初始状态:
  // 内存池起始地址
  static char* _S_start_free;
  // 内存池结束地址
  static char* _S_end_free;
  static size_t _S_heap_size;
  
内存池初始化:
    allocate(24Byte)            // 假设申请20Byte的内存,倍对齐调整为24Byte,返回一个24Byte内存的地址.
    -> _S_refill(20)            // 内部一次申请20个节点,返回第一个节点,其余19个节点组织为链表,挂载到free_list[24]上
    -> _S_chunk_alloc(24,20)    // 申请24*20=480Byte,内存池为空,向系统申请2倍的空间大小,即2*480=960Byte,一半的内存480Byte给到链表,一半的内存                                     // 480Byte留给内存池.此时_S_heap_size=480,假设_S_start_free=0x00804800,_S_end_free=0x0x00805280
    
内存分配:
void* allocate(size_t __n)
  若已经存在free_list[__n]的链表,则从链表头摘除一个节点,返回即可.
  若不存在free_list[__n]的链表,则需要申请20个__n大小的节点,返回1个,其余19个填充到free_list[__n]的链表链表.

内存回收:
void deallocate(void* __p, size_t __n)
  索引__n大小的内存所在的list,free_list[__n],头插法插入链表.
  
内存池中不够分配的情况:
char* _S_chunk_alloc(size_t __size,int& __nobjs)
//todo

