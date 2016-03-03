### Server
import socket
from threading import *
import socket
import pygame
import pygame.camera
from pygame.locals import *


def server():

    	print "\nServer started at " + str(socket.gethostbyname(socket.gethostname())) + " at port " + str(90)	
    	port = 90
    	serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    	serversocket.bind(("192.168.43.182",port))
	print " przed listen"
    	serversocket.listen(10)
    	pygame.camera.init()
	print " dalej"
	cameras = pygame.camera.list_cameras()

    	cam = pygame.camera.Camera(cameras[0],(640,480),"RGB")
    	cam.start()
    	img = pygame.Surface((640,480))
    	while True:
    		connection, address = serversocket.accept()
    		print "GOT_CONNECTION"
    		cam.get_image(img)
    		data = pygame.image.tostring(img,"RGB")
    		connection.sendall(data)
    		connection.close()

server()
