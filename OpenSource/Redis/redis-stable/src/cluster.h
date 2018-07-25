#ifndef __CLUSTER_H
#define __CLUSTER_H

/*-----------------------------------------------------------------------------
 * Redis cluster data structures, defines, exported API.
 *----------------------------------------------------------------------------*/

#define CLUSTER_SLOTS 16384
#define CLUSTER_OK 0          /* Everything looks ok */
#define CLUSTER_FAIL 1        /* The cluster can't work */
#define CLUSTER_NAMELEN 40    /* sha1 hex length */
#define CLUSTER_PORT_INCR 10000 /* Cluster port = baseport + PORT_INCR */

/* The following defines are amount of time, sometimes expressed as
 * multiplicators of the node timeout value (when ending with MULT). */
#define CLUSTER_DEFAULT_NODE_TIMEOUT 15000
#define CLUSTER_DEFAULT_SLAVE_VALIDITY 10 /* Slave max data age factor. */
#define CLUSTER_DEFAULT_REQUIRE_FULL_COVERAGE 1
#define CLUSTER_DEFAULT_SLAVE_NO_FAILOVER 0 /* Failover by default. */
#define CLUSTER_FAIL_REPORT_VALIDITY_MULT 2 /* Fail report validity. */
#define CLUSTER_FAIL_UNDO_TIME_MULT 2 /* Undo fail if master is back. */
#define CLUSTER_FAIL_UNDO_TIME_ADD 10 /* Some additional time. */
#define CLUSTER_FAILOVER_DELAY 5 /* Seconds */
#define CLUSTER_DEFAULT_MIGRATION_BARRIER 1
#define CLUSTER_MF_TIMEOUT 5000 /* Milliseconds to do a manual failover. */
#define CLUSTER_MF_PAUSE_MULT 2 /* Master pause manual failover mult. */
#define CLUSTER_SLAVE_MIGRATION_DELAY 5000 /* Delay for slave migration. */

/* Redirection errors returned by getNodeByQuery(). */
#define CLUSTER_REDIR_NONE 0          /* Node can serve the request. */
#define CLUSTER_REDIR_CROSS_SLOT 1    /* -CROSSSLOT request. */
#define CLUSTER_REDIR_UNSTABLE 2      /* -TRYAGAIN redirection required */
#define CLUSTER_REDIR_ASK 3           /* -ASK redirection required. */
#define CLUSTER_REDIR_MOVED 4         /* -MOVED redirection required. */
#define CLUSTER_REDIR_DOWN_STATE 5    /* -CLUSTERDOWN, global state. */
#define CLUSTER_REDIR_DOWN_UNBOUND 6  /* -CLUSTERDOWN, unbound slot. */

struct clusterNode;

/* clusterLink encapsulates everything needed to talk with a remote node. */
// 包含了与其他节点进行通讯所需的全部信息
typedef struct clusterLink {
    mstime_t ctime;             /* Link creation time */
    int fd;                     /* TCP socket file descriptor */
    // 输出缓冲区
    sds sndbuf;                 /* Packet send buffer */
    // 输入缓冲区
    sds rcvbuf;                 /* Packet reception buffer */
    // 与当前连接相关的节点
    struct clusterNode *node;   /* Node related to this link if any, or NULL */
} clusterLink;

/* Cluster node flags and macros. */
#define CLUSTER_NODE_MASTER 1     /* The node is a master */
#define CLUSTER_NODE_SLAVE 2      /* The node is a slave */
#define CLUSTER_NODE_PFAIL 4      /* Failure? Need acknowledge */
#define CLUSTER_NODE_FAIL 8       /* The node is believed to be malfunctioning */
#define CLUSTER_NODE_MYSELF 16    /* This node is myself */
#define CLUSTER_NODE_HANDSHAKE 32 /* We have still to exchange the first ping */
#define CLUSTER_NODE_NOADDR   64  /* We don't know the address of this node */
#define CLUSTER_NODE_MEET 128     /* Send a MEET message to this node */
#define CLUSTER_NODE_MIGRATE_TO 256 /* Master elegible for replica migration. */
#define CLUSTER_NODE_NOFAILOVER 512 /* Slave will not try to failver. */
#define CLUSTER_NODE_NULL_NAME "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"

#define nodeIsMaster(n) ((n)->flags & CLUSTER_NODE_MASTER)
#define nodeIsSlave(n) ((n)->flags & CLUSTER_NODE_SLAVE)
#define nodeInHandshake(n) ((n)->flags & CLUSTER_NODE_HANDSHAKE)
#define nodeHasAddr(n) (!((n)->flags & CLUSTER_NODE_NOADDR))
#define nodeWithoutAddr(n) ((n)->flags & CLUSTER_NODE_NOADDR)
#define nodeTimedOut(n) ((n)->flags & CLUSTER_NODE_PFAIL)
#define nodeFailed(n) ((n)->flags & CLUSTER_NODE_FAIL)
#define nodeCantFailover(n) ((n)->flags & CLUSTER_NODE_NOFAILOVER)

/* Reasons why a slave is not able to failover. */
#define CLUSTER_CANT_FAILOVER_NONE 0
#define CLUSTER_CANT_FAILOVER_DATA_AGE 1
#define CLUSTER_CANT_FAILOVER_WAITING_DELAY 2
#define CLUSTER_CANT_FAILOVER_EXPIRED 3
#define CLUSTER_CANT_FAILOVER_WAITING_VOTES 4
#define CLUSTER_CANT_FAILOVER_RELOG_PERIOD (60*5) /* seconds. */

/* clusterState todo_before_sleep flags. */
#define CLUSTER_TODO_HANDLE_FAILOVER (1<<0)
#define CLUSTER_TODO_UPDATE_STATE (1<<1)
#define CLUSTER_TODO_SAVE_CONFIG (1<<2)
#define CLUSTER_TODO_FSYNC_CONFIG (1<<3)

/* Message types.
 *
 * Note that the PING, PONG and MEET messages are actually the same exact
 * kind of packet. PONG is the reply to ping, in the exact format as a PING,
 * while MEET is a special PING that forces the receiver to add the sender
 * as a node (if it is not already in the list). */
#define CLUSTERMSG_TYPE_PING 0          /* Ping */
#define CLUSTERMSG_TYPE_PONG 1          /* Pong (reply to Ping) */
#define CLUSTERMSG_TYPE_MEET 2          /* Meet "let's join" message */
#define CLUSTERMSG_TYPE_FAIL 3          /* Mark node xxx as failing */
#define CLUSTERMSG_TYPE_PUBLISH 4       /* Pub/Sub Publish propagation */
#define CLUSTERMSG_TYPE_FAILOVER_AUTH_REQUEST 5 /* May I failover? */
#define CLUSTERMSG_TYPE_FAILOVER_AUTH_ACK 6     /* Yes, you have my vote */
#define CLUSTERMSG_TYPE_UPDATE 7        /* Another node slots configuration */
#define CLUSTERMSG_TYPE_MFSTART 8       /* Pause clients for manual failover */
#define CLUSTERMSG_TYPE_COUNT 9         /* Total number of message types. */

/* This structure represent elements of node->fail_reports. */
typedef struct clusterNodeFailReport {
    struct clusterNode *node;  /* Node reporting the failure condition. */
    mstime_t time;             /* Time of the last report from this node. */
} clusterNodeFailReport;

typedef struct clusterNode {
    mstime_t ctime; /* Node object creation time. */
    char name[CLUSTER_NAMELEN]; /* Node name, hex string, sha1-size */
    // 记录节点角色(主从) 节点状态(在线下线)
    int flags;      /* CLUSTER_NODE_... */
    // 阶段
    uint64_t configEpoch; /* Last configEpoch observed for this node */
    // CLUSTER_SLOTS/8 * 8bit = CLUSTER_SLOTS个bit
    // 节点位图 1表示slot当前由本节点处理 0表示slot并非本节点处理
    unsigned char slots[CLUSTER_SLOTS/8]; /* slots handled by this node */
    // 当前节点负责处理的slot数量
    int numslots;   /* Number of slots handled by this node */
    // 如果本节点是master,当前字段记录slave的个数
    int numslaves;  /* Number of slave nodes, if this is a master */
    // 从节点数组
    struct clusterNode **slaves; /* pointers to slave nodes */
    // 如果当前节点是从节点则指向主节点
    struct clusterNode *slaveof; /* pointer to the master node. Note that it
                                    may be NULL even if the node is a slave
                                    if we don't have the master node in our
                                    tables. */
    mstime_t ping_sent;      /* Unix time we sent latest ping */
    mstime_t pong_received;  /* Unix time we received the pong */
    mstime_t fail_time;      /* Unix time when FAIL flag was set */
    mstime_t voted_time;     /* Last time we voted for a slave of this master */
    mstime_t repl_offset_time;  /* Unix time we received offset for this node */
    mstime_t orphaned_time;     /* Starting time of orphaned master condition */
    long long repl_offset;      /* Last known repl offset for this node. */
    // 节点ip
    char ip[NET_IP_STR_LEN];  /* Latest known IP address of this node */
    // 节点作为客户端的port
    int port;                   /* Latest known clients port of this node */
    // 节点作为集群节点的port
    int cport;                  /* Latest known cluster port of this node. */
    // 节点连接相关
    clusterLink *link;          /* TCP/IP link with this node */
    // 链表记录了所有其他节点对该节点的下线报告
    list *fail_reports;         /* List of nodes signaling this as failing */
} clusterNode;

// 以当前节点的视角记录集群状态的结构
typedef struct clusterState {
    // 当前集群节点结构
    clusterNode *myself;  /* This node */
    // 集群当前阶段
    uint64_t currentEpoch;
    // 集群当前状态
    int state;            /* CLUSTER_OK, CLUSTER_FAIL, ... */
    int size;             /* Num of master nodes with at least one slot */
    // 集群节点名单
    // key => node_name, val => clusterNode
    dict *nodes;          /* Hash table of name -> clusterNode structures */
    // 集群节点黑名单
    dict *nodes_black_list; /* Nodes we don't re-add for a few seconds. */
    // 当前节点所负责的槽位正在迁出到哪个节点
    // index下标 指示第index个slot
    // migrating_slots_to[index]表示第index个slot需要迁移到的目标节点
    clusterNode *migrating_slots_to[CLUSTER_SLOTS];
    // 当前节点正在从哪个节点将某个槽位迁入到本节点中
    // index表示第index个slot
    // importing_slots_from[index]表示正在迁移的slot来自哪个节点
    clusterNode *importing_slots_from[CLUSTER_SLOTS];
    // 负责处理各slot的节点
    clusterNode *slots[CLUSTER_SLOTS];
    // 保存slot和keys的映射关系
    uint64_t slots_keys_count[CLUSTER_SLOTS];
    // 某一个槽slot上的所有key
    rax *slots_to_keys;
    /* The following fields are used to take the slave state on elections. */
    mstime_t failover_auth_time; /* Time of previous or next election. */
    // 节点获得的投票数量
    int failover_auth_count;    /* Number of votes received so far. */
    // 1表示节点已经向其他节点发送了投票请求
    int failover_auth_sent;     /* True if we already asked for votes. */
    int failover_auth_rank;     /* This slave rank for current auth request. */
    uint64_t failover_auth_epoch; /* Epoch of the current election. */
    int cant_failover_reason;   /* Why a slave is currently not able to
                                   failover. See the CANT_FAILOVER_* macros. */
    /* Manual failover state in common. */
    // 手动故障转移的时间限制
    mstime_t mf_end;            /* Manual failover time limit (ms unixtime).
                                   It is zero if there is no MF in progress. */
    /* Manual failover state of master. */
    clusterNode *mf_slave;      /* Slave performing the manual failover. */
    /* Manual failover state of slave. */
    long long mf_master_offset; /* Master offset the slave needs to start MF
                                   or zero if stil not received. */
    int mf_can_start;           /* If non-zero signal that the manual failover
                                   can start requesting masters vote. */
    /* The followign fields are used by masters to take state on elections. */
    uint64_t lastVoteEpoch;     /* Epoch of the last vote granted. */
    int todo_before_sleep; /* Things to do in clusterBeforeSleep(). */
    /* Messages received and sent by type. */
    long long stats_bus_messages_sent[CLUSTERMSG_TYPE_COUNT];
    long long stats_bus_messages_received[CLUSTERMSG_TYPE_COUNT];
    long long stats_pfail_nodes;    /* Number of nodes in PFAIL status,
                                       excluding nodes without address. */
} clusterState;

/* Redis cluster messages header */

/* Initially we don't know our "name", but we'll find it once we connect
 * to the first node, using the getsockname() function. Then we'll use this
 * address for all the next messages. */
typedef struct {
    // 刚开始节点的名字是随机的16进制字符串
    // 当MEET信息发送并得到回复之后,集群就会为节点设置正式的名字
    char nodename[CLUSTER_NAMELEN];
    // 最后一次向该节点发送 PING 消息的时间戳
    uint32_t ping_sent;
    // 最后一次从该节点接收到 PONG 消息的时间戳
    uint32_t pong_received;
    char ip[NET_IP_STR_LEN];  /* IP address last time it was seen */
    uint16_t port;              /* base port last time it was seen */
    uint16_t cport;             /* cluster port last time it was seen */
    // 节点的标识值
    uint16_t flags;             /* node->flags copy */
    // 对齐字节 不使用
    uint32_t notused1;
} clusterMsgDataGossip;

typedef struct {
    char nodename[CLUSTER_NAMELEN];
} clusterMsgDataFail;

typedef struct {
    uint32_t channel_len;
    uint32_t message_len;
    /* We can't reclare bulk_data as bulk_data[] since this structure is
     * nested. The 8 bytes are removed from the count during the message
     * length computation. */
    unsigned char bulk_data[8];
} clusterMsgDataPublish;

typedef struct {
    uint64_t configEpoch; /* Config epoch of the specified instance. */
    char nodename[CLUSTER_NAMELEN]; /* Name of the slots owner. */
    unsigned char slots[CLUSTER_SLOTS/8]; /* Slots bitmap. */
} clusterMsgDataUpdate;

union clusterMsgData {
    /* PING, MEET and PONG */
    struct {
        /* Array of N clusterMsgDataGossip structures */
        clusterMsgDataGossip gossip[1];
    } ping;

    /* FAIL */
    struct {
        clusterMsgDataFail about;
    } fail;

    /* PUBLISH */
    struct {
        clusterMsgDataPublish msg;
    } publish;

    /* UPDATE */
    struct {
        clusterMsgDataUpdate nodecfg;
    } update;
};

#define CLUSTER_PROTO_VER 1 /* Cluster bus protocol version. */

// 集群消息结构
typedef struct {
    char sig[4];        /* Siganture "RCmb" (Redis Cluster message bus). */
    // 消息长度(header+payload)
    uint32_t totlen;    /* Total length of this message */
    uint16_t ver;       /* Protocol version, currently set to 1. */
    uint16_t port;      /* TCP base port number. */
    // 消息类型
    uint16_t type;      /* Message type */
    // 消息正文包含的节点信息数量
    // 只在发送 MEET PING PONG 这三种Gossip协议消息时使用
    uint16_t count;     /* Only used for some kind of messages. */
    // 消息发送者的阶段
    uint64_t currentEpoch;  /* The epoch accordingly to the sending node. */
    // 若消息发送者为master 则记录消息发送者的阶段
    // 若消息发送者为slave 则记录消息发送者正在复制的主节点的阶段
    uint64_t configEpoch;   /* The config epoch if it's a master, or the last
                               epoch advertised by its master if it is a
                               slave. */
    // 节点的复制偏移
    uint64_t offset;    /* Master replication offset if node is a master or
                           processed replication offset if node is a slave. */
    // 消息发送者的名字
    char sender[CLUSTER_NAMELEN]; /* Name of the sender node */
    // 消息发送者目前的slot指派信息
    unsigned char myslots[CLUSTER_SLOTS/8];
    // 若消息发送者为master 则记录的是REDIS_NODE_NULL_NAME
    // 若消息发送者为slave 则记录的是消息发送者正在复制的主节点的名字
    char slaveof[CLUSTER_NAMELEN];
    char myip[NET_IP_STR_LEN];    /* Sender IP, if not all zeroed. */
    char notused1[34];  /* 34 bytes reserved for future usage. */
    // 消息发送者的端口
    uint16_t cport;      /* Sender TCP cluster bus port */
    // 消息发送者的标识
    uint16_t flags;      /* Sender node flags */
    // 消息发送者所处的集群状态
    unsigned char state; /* Cluster state from the POV of the sender */
    // 消息标志
    unsigned char mflags[3]; /* Message flags: CLUSTERMSG_FLAG[012]_... */
    // 消息的正文 payload
    union clusterMsgData data;
} clusterMsg;

#define CLUSTERMSG_MIN_LEN (sizeof(clusterMsg)-sizeof(union clusterMsgData))

/* Message flags better specify the packet content or are used to
 * provide some information about the node state. */
#define CLUSTERMSG_FLAG0_PAUSED (1<<0) /* Master paused for manual failover. */
#define CLUSTERMSG_FLAG0_FORCEACK (1<<1) /* Give ACK to AUTH_REQUEST even if
                                            master is up. */

/* ---------------------- API exported outside cluster.c -------------------- */
clusterNode *getNodeByQuery(client *c, struct redisCommand *cmd, robj **argv, int argc, int *hashslot, int *ask);
int clusterRedirectBlockedClientIfNeeded(client *c);
void clusterRedirectClient(client *c, clusterNode *n, int hashslot, int error_code);

#endif /* __CLUSTER_H */
