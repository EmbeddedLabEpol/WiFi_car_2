#!/usr/bin/python
import socket
import time
import sys

import pygame.camera
import pygame.image

pygame.camera.init()

cameras = pygame.camera.list_cameras()

print "Using camera %s ..." % cameras[0]

webcam = pygame.camera.Camera(cameras[0],(640,480),"RGB")

webcam.start()

# grab first frame
img = webcam.get_image()

WIDTH = img.get_width()
HEIGHT = img.get_height()
#screen = pygame.display.set_mode( ( WIDTH, HEIGHT ) )
#pygame.display.set_caption("pyGame Camera View")



#HOST = "192.168.254.149"
HOST = "10.8.0.7"
import os
bashCommand = "ifconfig wlan0 | grep 192.168 | awk '{print $2}' | sed -e 's/addr://g'"
import subprocess
#HOST = subprocess.check_output(['bash','-c', bashCommand])


print "adres to : " , HOST
PORT = 8834
work = True
max_size_pocket = 64000
while True:
        work = True
        sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_STREAM) # TCP
        sock.bind((HOST, PORT))
        sock.listen(True)
        connection, client_address = sock.accept()
        msg=" "
        while work:
               # for e in pygame.event.get() :

                #        if e.type == pygame.QUIT :
                 #            sys.exit()

                # draw frame

                # grab next frame
             
                img = webcam.get_image()
 #                screen.blit(img, (0,0))
                #pygame.display.flip()
                pygame.image.save(img, "/mnt/ramdisk/image.mjpg")
                _file = open('/mnt/ramdisk/image.mjpg','rb')
                fp = _file.read()
                _file.close()
                #print fp
		#fp   = pygame.image.tostring(img,"RGB")
                foto_len = len(fp)
                foto_len_tmp = 0 #foto_len
                print "dlugosc to : ",foto_len
                while work:
                        print " pierwsze odbieranie"
                        data = connection.recv(max_size_pocket) # buffer size is 1024 bytes
                        print " odebralem pierwsze!"
                        if data == "END#":
                                 work = False
                                 break
                        print "received message:", data," from: " 
                        #time.sleep(1)
                        #print " loop"
                        if foto_len_tmp+max_size_pocket < foto_len:
                                 print "wysylam zawartosc",foto_len_tmp, " ", foto_len_tmp+max_size_pocket
                                 try:
					connection.send (fp[foto_len_tmp:foto_len_tmp+max_size_pocket] )
                                 except:
					work = False
					break
				 foto_len_tmp = foto_len_tmp + max_size_pocket
                                 print "wysylam zawartosc"
                        else :
                                 print "wyslam ostatnia paczke "
                                 try:
					connection.send (fp[foto_len_tmp:foto_len] )
                                 except:
                        		work = False
					break
				 print " ostatnia paczka"
                                 break
                print " odbieram"
		try:                
			data  = connection.recv (max_size_pocket) # buffer size is 1024 bytes
		except:
                        work = False

                print "koniec odbierania  teraz wysylam"
                try:
			connection.send("END#")
		except: 
			work = False
                print "wyslalem"
 
