
## 字符串ngx_str_t
### 常见的字符串操作实现
    字符转整数比较ngx_strncasecmp(u_char *s1, u_char *s2, size_t n)
### base64编解码原理及实现
    ngx_encode_base64(ngx_str_t *dst, ngx_str_t *src)
    ngx_decode_base64(ngx_str_t *dst, ngx_str_t *src)

## 内存池ngx_pool_t
### ngx_pool_t如何实现管理文件资源?
    typedef struct ngx_pool_cleanup_s  ngx_pool_cleanup_t;
### ngx_pool_t的优缺点
    优:高效统一 可管理文件资源
    缺:统一释放 部分资源占用时间过长

## 数组ngx_array_t


## 哈希ngx_hash_t
### 如何解决哈希冲突?
    开链法
### ngx_hash_t的特点
    hash表构建后 不能删除 不能插入
    开链并非真正的链表,而是使用连续的存储空间.初始化时预计算每个桶中会有多少元素
### ngx_hash_init()
### ngx_hash_wildcard_t
    支持带通配符的域名匹配(支持两种类型)
    "*.abc.com" ".abc.com" 或者"mail.xxx.*"

## 链表ngx_chain_t
    typedef struct ngx_buf_s  ngx_buf_t;
    
## 红黑树

## 基数树
