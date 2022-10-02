from MTSData import MTSData
import json


class MTSInfo:
    def __init__(self) -> None:
        pass

    def ServerStatus(self):
        print("MTS > Pause JSON Server Status")
        data = {'mts':'status','content':'ok'};
        return json.dumps(data)

    def RegisterStatus(self,code):
        print("MTS > Pause JSON Register Status")
        print(code)
        data = {'mts':'register','content':str(code)};   
        return json.dumps(data)

    def LoginStatus(self,code):
        print("MTS > Pause JSON Login Status")
        data = {'mts':'login','content':code}
        return json.dumps(data)

    def UpdateStatus(self,code):
        print("MTS > Pause JSON User Update Status")
        data = {'mts':'update','content':code}
        return json.dumps(data)

    def UserNotFoundReply(self):
        print("MTS > Pause JSON User Not Found")
        data = {'mts':'look','content':'failure'}
        return json.dumps(data)
    
    def UserData(self,s):
        print("MTS > Pause JSON User Found Result")
        data = {'mts':'look','content':'success','uid':s.uid,'uname':s.uname,'umail':s.uname,'ustatus':s.ustatus}
        return json.dumps(data)

    def ContactData(self,sqldata):
        print("MTS > Pause JSON Contact Data")
        re_list = list()
        for i in sqldata:
            uu = dict()
            uu['uid']=int(i[0])
            uu['uname']=i[1]
            uu['ustatus']=i[2]
            re_list.append(uu)
        data = {'mts':'contacts'}
        data['content'] = re_list
        return json.dumps(data)

    def AddContactData(self,re):
        print("MTS > Pause JSON Add Contact Result Data")
        data = {'mts':'addcontacts','content':re}
        return json.dumps(data)

    def ConfirmContactData(self,re):
        print("MTS > Pause JSON Confirm Contact Result Data")
        data = {'mts':'confirmcontacts','content':re}
        return json.dumps(data)

    def DeleteContactData(self,re):
        print("MTS > Pause JSON Delete Contact Result Data")
        data = {'mts':'deletecontacts','content':re}
        return json.dumps(data)

    def UpdateUserStatus(self,re):
        print("MTS > Pause JSON Update User Status")
        data = {'mts':'updatestatus','content':re}
        return json.dumps(data)

    def MessageSendData(self,re):
        print("MTS > Pause JSON Message Send Status")
        data = {'mts':'messagesend','content':re}
        return json.dumps(data)

    def MessageGetDataSend(self,re):
        print("MTS > Pause JSON Message Send Data")
        re_list = list()
        for i in re:
            uu = dict()
            uu['mreceiver']=int(i[0])
            uu['mname']=i[1]
            uu['mcontent']=i[2]
            uu['mtime']=i[3].strftime('%Y-%m-%d %H:%M:%S')
            re_list.append(uu)
        data = {'mts':'messagegetsend'}
        data['content'] = re_list
        return json.dumps(data)

    def MessageGetDataReceive(self,re):
        print("MTS > Pause JSON Message Receive Data")
        re_list = list()
        for i in re:
            uu = dict()
            uu['msender']=int(i[0])
            uu['mname']=i[1]
            uu['mcontent']=i[2]
            uu['mtime']=i[3].strftime('%Y-%m-%d %H:%M:%S')
            re_list.append(uu)
        data = {'mts':'messagegetreceive'}
        data['content'] = re_list
        return json.dumps(data)

    def MessageCountDataSend(self,re):
        print("MTS > Pause JSON Message Count")
        data = {'mts':'messagecount','content':re[0][0]}
        return json.dumps(data)

    

