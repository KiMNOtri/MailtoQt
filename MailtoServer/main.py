from flask import Flask, request
from MTSData import MTSData
from MTSInfo import MTSInfo
app = Flask(__name__)

helper = MTSInfo()
db = MTSData()

@app.route('/')
def web():
    print("MTS > Web Page Push")
    s_data = "MTS Server\n"
    s_data += "Status : OK"
    db.getUserTableSize()
    return s_data

@app.route('/status')
def serverStatus():
    print("MTS > Server Status Push")
    return helper.ServerStatus()

@app.route('/register')
def serverUserRegister():
    print("MTS > Register Function Push")
    udata = dict()
    udata['uname'] = request.args.get('uname')
    udata['umail'] = request.args.get('umail')
    udata['upasswd'] = request.args.get('upasswd')
    repo = db.addUser(udata)
    # 目前暂时没有注册失败的情况
    return helper.RegisterStatus(repo)

@app.route('/login')
def serverUserLogin():
    # 登陆可以生成 Token 增加安全性
    # 这里就不写了太麻烦了
    print("MTS > Login Function Push")
    loginform = dict()
    loginform['uid'] = request.args.get('uid')
    loginform['upasswd'] = request.args.get('upasswd')
    repo = db.verifyUser(loginform)
    return helper.LoginStatus(repo)

@app.route('/update')
def serverUserUpdate():
    print("MTS > User Update Push")
    form = dict()
    form['uid'] = request.args.get('uid')
    form['uname'] = request.args.get('uname')
    form['umail'] = request.args.get('umail')
    form['upasswd'] = request.args.get('upasswd')
    repo = db.updateUser(form)
    return helper.UpdateStatus(repo)

@app.route('/look')
def serverUserLook():
    print("MTS > User Look Push")
    uid = request.args.get('uid')
    re = db.lookUser(uid)
    if(re==None):
        return helper.UserNotFoundReply()
    else:
        return helper.UserData(re)

@app.route('/updatestatus')
def serverUserStatusUpdate():
    print("MTS > User Status Update Push")
    db.updateStatus(request.args.get('uid'),request.args.get('ustatus'))
    return helper.UpdateUserStatus('success')

@app.route('/contacts')
def serverContactsPush():
    print("MTS > User Contacts Push")
    reply = db.getContact(request.args.get('uid'))
    return helper.ContactData(reply)

@app.route('/addcontacts')
def serverUserContactsAdd():
    print("MTS > User Contacts Add Push")
    re = db.addContact(request.args.get('cuser_l'),request.args.get('cuser_r'))
    return helper.AddContactData(re)

@app.route('/confirmcontacts')
def serverUserConfirmContact():
    print("MTS > User Contacts Confirm")
    re = db.confirmContacts(request.args.get('cuser_l'),request.args.get('cuser_r'))
    return helper.ConfirmContactData(re)

@app.route('/deletecontacts')
def serverUserDeleteContact():
    re = db.deleteContacts(request.args.get('cuser_l'),request.args.get('cuser_r'))
    return helper.DeleteContactData(re)

@app.route('/messagesend',methods=['POST'])
def serverMessageSend():
    if request.method == 'POST':
        data = request.get_json()
        re = db.messageAdd(data['mcontent'],data['msender'],data['mreceiver'])
        return helper.MessageSendData(re)

@app.route('/messageget/send')
def serverMessageGetSend():
    repo = db.messageGetSend(request.args.get('uid'))
    return helper.MessageGetDataSend(repo)

@app.route('/messageget/receive')
def serverMessageGetReceive():
    repo = db.messageGetReceive(request.args.get('uid'))
    return helper.MessageGetDataReceive(repo)

@app.route('/messagecount')
def serverMessageCountSend():
    repo = db.messageCountGet(request.args.get('uid'))
    return helper.MessageCountDataSend(repo)

if __name__ == '__main__':
    print("MTS Server Version 1.5")
    app.run(host='0.0.0.0',port=1145,debug=True)
    