
![hubpubsub images](https://github.com/96189/xteam/blob/master/ReadmeImages/hubpubsub.png)

hub
    topic_object对象内部由set存储所有订阅该topic的sub的tcp connection,当pub发布对应的topic到hub时,遍历该set的tcp connection,将topic对应的content
推送到sub.
    hub内部是一张表,映射topic -> topic_object.hub收到sub的message构建对应的topic_object存储到表中.hub收到pub的message查表推送消息.
sub
    sub连接建立后发起订阅操作,send message到hub
pub
    pub连接建立后发起发布操作,send message到hub
    
1、sub和pub启动可以无关顺序？
    topic_object内部存储lastPubTime_,若pub先启动,则会初始化该值.sub启动时若该变量有效则直接推送.
2、Tcp Connection上下文context_？
    tcp connection的context_使用boost::any对象,可保存任意的数据类型,包括stl容器.在本例中保存的是set<string>,存储该连接关注的topic_name
