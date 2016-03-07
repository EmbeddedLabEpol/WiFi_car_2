#!/usr/bin/python
import sys
import socket
import time
from sys import stdout


my_bytes = " "
#print("parametry: %s" % sys.argv)
# create a socket object
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# get local machine name
if len(sys.argv) >= 3 :
    host = sys.argv[1]
    port = int(sys.argv[2])
    #print("podales dobre argumenty")
else:
    print("adres i port domyslny ")
    host = "192.168.1.144"
    port = int(8833)
if len(sys.argv) == 4 :
    if sys.argv[3] == "stop" :
         my_bytes = "stop server"
    elif sys.argv[3] == "temperatura":
         my_bytes = "temperatura"
# connection to hostname on the port.
s.connect((host, port))
######################## main loop  ###########################
while True:
    if my_bytes == "temperatura":
        s.send ("RS232 get temperature")
        #print(" odbieramy")
        tm = s.recv(256)
        print(tm)
        break
    t = time.strftime("%H:%M")
    my_bytes = raw_input(" %s iDom: " % t)
    print("> %s" % my_bytes.decode('ascii'))
    if my_bytes == "exit" :
        s.send(my_bytes)

        tm = s.recv(256)
        print( tm)
        break
    s.send(my_bytes)
    while True :
        tm = s.recv(256)
        #print(tm)
        stdout.write(tm)

        if (tm[1]  == "E") and (tm[2] == "N") and (tm[3]== "D"):
           # print ("END.")
            break
        s.send("OK")





    # Receive no more than 1024 bytes
s.close()