import socket
from PIL import Image
import pygame,sys
import pygame.camera
from pygame.locals import *
import time

host = "192.168.43.141"
port = 1890
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((host, port))


pygame.init()
pygame.camera.init()
cam = pygame.camera.Camera("/dev/video0",(320,240))
cam.start()

while 1:
        image = cam.get_image()
        data = pygame.image.tostring(image,"RGB")
        s.send(data)     

s.close()
print "recieved", repr(data)
