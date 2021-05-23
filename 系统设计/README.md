

## 接口设计

* [API接口签名校验](https://blog.mimvp.com/article/37824.html)

    * 给调用方分配access_key security_key
    * 调用方调用API时对请求参数进行签名验证，规则为选择某些参数按照一定规则计算摘要，access_key参与签名，timestamp参与签名(防止重放攻击)，计算出sign后添加到请求参数中
    * 接口提供方，查询access_key对应的security_key按照相关的规则计算sign，对比自己计算得出的与参数中的sign是否一致，另外，也要判断当前时间距离参数时间戳的差值(防止重放)

* [API认证、授权和凭证概览](https://juejin.cn/post/6844903807839649806#heading-2)

## 如何编写一个网络框架
