from Crypto.Cipher import Blowfish
import threading
import Queue

cipher = ""
with open("C:\Users\Kevin\Downloads\inception.txt (2)\\flag.png.enc.dbl-bfsh-32-cbc",'rb') as f:
  cipher = f.read()



def dbl_bfsh_enc(s,k):
  k1 = '\0' + k[1:4]
  k2 = '\0' + k[5:8]

  
  iv = s[0:8]
  ret = iv
  
  c1 = Blowfish.new(k1,Blowfish.MODE_CBC,iv)
  c2 = Blowfish.new(k2,Blowfish.Mode_CBC,iv)

  ret += c2.encrypt(c1.encrypt(s))

  return ret


q = Queue.Queue()

tasks = Queue.Queue(maxsize=1000)

def dbl_bfsh_dec(s,k):
  k1 = '\0' + k[1:4]
  k2 = '\0' + k[5:8]
  
  iv = s[0:8]
  s = s[8:]
  
  c1 = Blowfish.new(k1,Blowfish.MODE_CBC,iv)
  c2 = Blowfish.new(k2,Blowfish.MODE_CBC,iv)

  q.put((k,c1.decrypt(c2.decrypt(s))))


done = 0
def watchQ():
  global done
  print "Watching Queue"
  while True:
    a,b = q.get(True)
    #print a,b
    done += 1
    if b[0:8] == filesig:
      print "******* KEY FOUND:",a,repr(b),"***********"
      sys.exit(0)
    q.task_done()

def tasker():
  while True:
    k = tasks.get()
    t = threading.Thread(target=dbl_bfsh_dec,args=(cipher[0:16],key))
    t.daemon = True
    t.start()
    tasks.task_done()


t = threading.Thread(target=tasker)
t.daemon = True
t.start()

t = threading.Thread(target=watchQ)
t.daemon = True
t.start()

filesig = [137, 80, 78, 71, 13, 10, 26, 10]
filesig = "".join(chr(x) for x in filesig)
keyspace = "ABCDEFGHIJKLMNOPQSTUVWXYZ"
print repr(filesig)
for a in keyspace:
  for b in keyspace:
    for c in keyspace:
      for d in keyspace:
        print a,b,c,d,q.qsize(),done,tasks.qsize()
        for e in keyspace:
          for f in keyspace:
            key = (chr(0) + a + b + c +
                   chr(0) + d + e + f)
            tasks.put(key)
            

              
                    

