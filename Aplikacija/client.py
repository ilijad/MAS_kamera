#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import socket
import io
import time
#from PIL import Image

#s = socket.socket()  # create a socket object
TCP_IP = '192.168.1.10'
host= TCP_IP
port = 7
TCP_PORT = 7
BUFFER_SIZE = 2048
MESSAGE = b"Hello,hfgjh World"
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

s.connect((TCP_IP, TCP_PORT))
s.send(MESSAGE)
print("Sending...")
f = open("photo.jpg","wb")  # stream instead of file
imageChunk = s.recv(2048)
while(imageChunk):
    print("Receiving...")
    f.write(imageChunk)
    imageChunk = s.recv(2048)
print("Done receiving image")
s.shutdown(socket.SHUT_WR)
s.close()  # close the socket when done
#img = Image.open(f)
#img.show()
f.close()
