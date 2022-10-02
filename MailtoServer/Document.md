# MTS API Document

**Server Host : http://localhost:1145**

## Response JSON Struct

{
    "mts":"#DOMAIN_NAME#"
    // like status, contacts, etc..
}

## Domain & Usage

### /status
**GET**
Server Status

{'mts':'status','content':'ok'}
服务器正常

{'mts':'status','content':'down'}
服务器拒绝请求

### /register/uname/umail/upasswd
**GET**
用户注册

{'mts':'register','content':'#uid#'}
注册成功，返回 UID

{'mts':'register','content':'failure'}
注册失败（暂未实现注册失败情况）

### /login/uid/upasswd
**GET**
Login Process

{'mts':'login','content':'success'}
登陆成功

{'mts':'login','content':'incorrect'}
用户名或密码错误

**** DEPRECATED ****
{'mts':'login','content':'notfound'}
没这个UID

### /update/uid/uname/umail/upasswd
**GET**
Update User Information

{'mts':'update','content':'success'}
修改资料成功

{'mts':'update','content':'failure'}
没找到 UID

### /look/uid
**GET**
Get User Information

{'mts':'look','content':'success','uid':'#uid#','uname':'#uname#','umail':'#uname#','ustatus','#status#'}
找到了这个人的信息

{'mts':'look','content':'failure'}
没有这个人

### /updatestatus/#uid#/#ustatus#
**GET**
ustatus=0/ustatus=1
返回固定 success

### /contacts/#uID#
**GET**
Get user contacts list
{'mts':'contacts','content':[{'uname':'xxx','uid':'xxx','ustatus':'xxx'},{'uname':'xxx','uid':'xxx'},...]}
得到联系人信息，包括用户名，用户ID，联系人状态


### /addcontacts/#cuser_l#/#cuser_r#
**GET**
Add user contact
默认添加方 cstatus 0 需要接受方 cstatus 2
如果对方同意，将两人的 cstatus 都设定为 1

{'mts':'addcontacts','content':'success'}
操作成功
{'mts':'addcontacts','content':'failure'}
操作失败
{'mts':'addcontacts','content':'already'}
已经是好友了
{'mts':'addcontacts','content':'wait'}
已经发送好友申请，需要等待对方同意

### /confirmcontacts/#cuser_l#/#cuser_r#
**GET**
允许添加为好友，改写服务器 cstatus 双向为 1

### /deletecontacts/#cuser_l#/#cuser_r#
**GET**
删除好友，删除方 cstatus 3 接收方 cstatus 4

### /messagesend/#mcontent#/#msender#/#mreceiver#
**POST**
消息发送方法

POST DATA
data['mcontent'],data['msender'],data['mreceiver']

{'mts':'messagesend','content':'success'}
消息已经投递完成
{'mts':'messagesend','content':'failure'}
还不是好友

### /messageget/send/#uid#
**GET**
获取发送者为 UID 消息

{'mts':'messagegetsend','content':[
    {'mreceiver':XXX,
    'mname':XXX,
    'mcontent':XXX,
    'mtime':XXX}
]}

### /messageget/receive/#uid#
**GET**
获取接受者为 UID 的消息

{'mts':'messagegetreceive','content':[
    {'msender':XXX,
    'mname':XXX
    'mcontent':XXX,
    'mtime':XXX}
]}

[NEW]
### /messagecount/#uid#
返回云端消息数量 UID 为接收者
{'mts':'messagecount','content':10}



