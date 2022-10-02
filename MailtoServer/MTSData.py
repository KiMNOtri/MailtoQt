# 数据库服务器缓冲区
# 用户信息
from itertools import count
from re import I
from selectors import SelectorKey
import pymysql

class MTSUserInfo:
    def __init__(self) -> None:
        self.uid = ''
        self.uname = ''
        self.upasswd = ''
        self.umail = ''
        self.ustatus = ''

class MTSContactsInfo:
    def __init__(self) -> None:
        self.cuser_l = ''
        self.cuser_r = ''
        self.cstatus = ''

class MTSData:

    def __init__(self) -> None:
        pass


    def sqlOpenConnection(self):
        return pymysql.connect(host='localhost',
                     user='api_user',
                     password='studiotri@api_user',
                     database='mailto_db')
        
    # 查询用户表大小
    def getUserTableSize(self):
        sql = 'select count(*) from user'
        dbconnect = self.sqlOpenConnection()
        dbcursor = dbconnect.cursor()
        dbcursor.execute(sql)
        result = dbcursor.fetchall()
        dbcursor.close()
        dbconnect.close()
        print(result)
        return int(result[0][0])

    # 添加用户
    def addUser(self,uinfo):
        print("[DB] Add User "+uinfo['uname'])
        sql = "insert into user(uname,upasswd,umail) values ('"+uinfo['uname'] +"','"+uinfo['upasswd']+"','"+uinfo['umail']+"')"
        dbconnect = self.sqlOpenConnection()
        dbcursor = dbconnect.cursor()
        try:
            dbcursor.execute(sql)

            re = dbcursor.lastrowid
            print(re)
            dbconnect.commit()
            print("[DB] User insert OK")
            
        except:
            print("[DB] User insert ERROR")
            dbconnect.rollback()
            return 'failure'
        dbcursor.close()
        dbconnect.close()
        return re
        

    # 用户认证
    def verifyUser(self,uinfo):
        dbconnect = self.sqlOpenConnection()
        dbcursor = dbconnect.cursor()
        sql = "select * from user where uid="+uinfo['uid']+" and upasswd='"+uinfo['upasswd']+"'"

        dbcursor.execute(sql)
        result = dbcursor.fetchall()
        # print(result)
        if(len(result)!=0):
            dbcursor.close()
            return 'success'
        dbcursor.close()
        dbconnect.close()
        return 'incorrect'

    # 更新用户资料
    def updateUser(self,uinfo):
        sql = "update user set uname='"+uinfo['uname']+"',umail='"+uinfo['umail']+"',upasswd='"+uinfo['upasswd']+"' where uid="+uinfo['uid']
        print("[DB] Update UID "+uinfo['uid']+" Information")
        dbconnect = self.sqlOpenConnection()
        dbcursor = dbconnect.cursor()
        try:
            dbcursor.execute(sql)
            dbconnect.commit()
            print("[DB] User Information Update OK")
            return 'success'
        except:
            print("[DB] User Information Update ERROR")
            dbconnect.rollback()
            return 'failure'

    # 查找用户
    def lookUser(self,uid):
        sql = "select uid,uname,umail,ustatus from user where uid="+uid
        dbconnect = self.sqlOpenConnection()
        dbcursor = dbconnect.cursor()
        dbcursor.execute(sql)
        result = dbcursor.fetchall()
        if(len(result)==0):
            dbcursor.close()
            dbconnect.close()
            return None
        
        i = MTSUserInfo()
        i.uid = result[0][0]
        i.uname = result[0][1]
        i.umail = result[0][2]
        i.ustatus = result[0][3]
        dbcursor.close()
        dbconnect.close()

        return i


    # 更新用户状态
    def updateStatus(self,uid,s):
        print("[DB] Update UID "+uid+" status to "+s)
        dbconnect = self.sqlOpenConnection()
        dbcursor = dbconnect.cursor()
        sql = "update user set ustatus="+s+" where uid="+uid
        try:
            dbcursor.execute(sql)
            dbconnect.commit()
            print("[DB] User Status Update OK")
        except:
            print("[DB] User Status Update ERROR")
            dbconnect.rollback()

        dbcursor.close()
        dbconnect.close()

    def addContact(self,uidl,uidr):
        print("[DB] Add Contact UID "+uidl+" > UID "+uidr)
        re = self.lookUser(uidr)
        print(re)
        if(re == None):
            return 'failure'
        r = self.confirmContactsStatus(uidl,uidr)
        sql1 = "insert into contact(cuser_l,cuser_r,cstatus) values("+uidl+","+uidr+",0)"
        sql2 = "insert into contact(cuser_l,cuser_r,cstatus) values("+uidr+","+uidl+",2)"
        if(r!=-1):
            # 似乎已经是好友了
            if(r==1): return 'already'
            if(r==3):
                # 你不是已经删了好友了 这种情况下可以当作无事发生也可以重新来
                sql1 = "update contact set cstatus=0 where cuser_l="+uidl +" and cuser_r="+uidr
                sql2 = "update contact set cstatus=2 where cuser_l="+uidr +" and cuser_r="+uidl
                pass
            if(r==4):
                # 被删好友了
                sql1 = "update contact set cstatus=0 where cuser_l="+uidl +" and cuser_r="+uidr
                sql2 = "update contact set cstatus=2 where cuser_l="+uidr +" and cuser_r="+uidl
                pass
            if(r==0):
                return 'wait'
            pass
        dbconnect =self.sqlOpenConnection()
        dbcursor = dbconnect.cursor()
        try:
            print("[DB] SQL1 "+sql1)
            print("[DB] SQL2 "+sql2)
            dbcursor.execute(sql1)
            dbcursor.execute(sql2)
            dbconnect.commit()
            dbcursor.close()
            dbconnect.close()
            print("[DB] Contact Add OK")
            return 'success'
        except:
            print("[DB] Contact Add ERROR")
            dbconnect.rollback()
            dbcursor.close()
            dbconnect.close()
            return 'failure'

    def getContact(self,uid):
        print("[DB] UID "+uid+" Contact Get")
        dbconnect = self.sqlOpenConnection()
        sql = "select cuser_r,uname,cstatus from user,contact where contact.cuser_l ="+uid+" and cuser_r = uid"
        dbcursor = dbconnect.cursor()
        dbcursor.execute(sql)
        result = dbcursor.fetchall()
        dbcursor.close()
        dbconnect.close()
        print(result)
        return result

    def confirmContactsStatus(self,cuser_l,cuser_r):
        print("[DB] Confirm User Status "+cuser_l+" > "+cuser_r)
        sql = "select cstatus from contact where cuser_l="+cuser_l+" and cuser_r="+cuser_r
        dbconnect = self.sqlOpenConnection()
        dbcursor = dbconnect.cursor()
        dbcursor.execute(sql)
        result = dbcursor.fetchall()
        if(len(result)==0):
            dbcursor.close()
            dbconnect.close()
            return -1
        dbcursor.close()
        dbconnect.close()
        return int(result[0][0])

    def deleteContacts(self,cuser_l,cuser_r):
        print("[DB] Contact Delete "+cuser_l+" > "+cuser_r)
        sql1 = "update contact set cstatus=3 where cuser_l="+cuser_l +" and cuser_r="+cuser_r
        sql2 = "update contact set cstatus=4 where cuser_l="+cuser_r +" and cuser_r="+cuser_l
        dbconnect = self.sqlOpenConnection()
        dbcursor = dbconnect.cursor()
        try:
            dbcursor.execute(sql1)
            dbcursor.execute(sql2)
            self.dbconnect.commit()
            dbcursor.close()
            dbconnect.close()
            print("[DB] Contact Delete OK")
            return 'success'
        except:
            print("[DB] Contact Delete ERROR")
            self.dbconnect.rollback()
            dbcursor.close()
            dbconnect.close()
            return 'failure'

    def confirmContacts(self,cuser_l,cuser_r):
        print("[DB] Contact Confirm "+cuser_l+" > "+cuser_r)
        sql1 = "update contact set cstatus=1 where cuser_l="+cuser_l +" and cuser_r="+cuser_r
        sql2 = "update contact set cstatus=1 where cuser_l="+cuser_r +" and cuser_r="+cuser_l
        dbconnect = self.sqlOpenConnection()
        dbcursor = dbconnect.cursor()
        try:
            dbcursor.execute(sql1)
            dbcursor.execute(sql2)
            dbconnect.commit()
            dbcursor.close()
            dbconnect.close()
            print("[DB] Contact Confirm OK")
            return 'success'
        except:
            print("[DB] Contact Confirm ERROR")
            self.dbconnect.rollback()
            dbcursor.close()
            dbconnect.close()
            return 'failure'

    def messageAdd(self,mcontent,msender,mreceiver):
        print("[DB] Message Send from "+msender)
        sql = "insert into message(mcontent, msender, mreceiver, mtime) values('"+mcontent+"',"+msender+","+mreceiver+",NOW())"
        reply = self.confirmContactsStatus(msender,mreceiver)
        if(reply!=1): return 'failure'
        dbconnect = self.sqlOpenConnection()
        dbcursor = dbconnect.cursor()
        try:
            dbcursor.execute(sql)
            dbconnect.commit()
            dbcursor.close()
            dbconnect.close()
            print("[DB] Message Send OK")
            return 'success'
        except:
            print("[DB] Message Send ERROR")
            dbconnect.rollback()
            dbcursor.close()
            dbconnect.close()
            return 'failure'

    def messageGetReceive(self,uid):
        print("[DB] Message Get Receiver : "+uid)
        sql = "select msender,uname,mcontent,mtime from user,message where message.mreceiver="+uid+" and msender=uid"
        dbconnect = self.sqlOpenConnection()
        dbcursor = dbconnect.cursor()
        dbcursor.execute(sql)
        result = dbcursor.fetchall()
        print(result)
        dbcursor.close()
        dbconnect.close()
        return result

    def messageGetSend(self,uid):
        print("[DB] Message Get Sender : "+uid)
        sql = "select mreceiver,uname,mcontent,mtime from user,message where message.msender="+uid+" and mreceiver=uid"
        dbconnect = self.sqlOpenConnection()
        dbcursor = dbconnect.cursor()
        dbcursor.execute(sql)
        result = dbcursor.fetchall()
        print(result)
        dbcursor.close()
        dbconnect.close()
        return result

    def messageCountGet(self,uid):
        print("[DB] Message Count Get")
        sql = "select COUNT(*) from message where mreceiver="+uid
        dbconnect = self.sqlOpenConnection()
        dbcursor = dbconnect.cursor()
        dbcursor.execute(sql)
        result = dbcursor.fetchall()
        print(result)
        dbcursor.close()
        dbconnect.close()
        return result
