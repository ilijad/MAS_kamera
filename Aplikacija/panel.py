#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import wx
from PIL import Image
import requests
import io
import imghdr
import socket
from subprocess import call


class ViewerPanel(wx.Panel):
    def __init__(self, parent):
        wx.Panel.__init__(self, parent)

        width, height = wx.DisplaySize()
        self.photoMaxHeight = height - 300
        self.photoMaxWidth = width - 840
        self.photoMaxSize = height
        self.pathToPicture = None
        self.layout()

    def layout(self):
        self.mainSizer = wx.BoxSizer(wx.VERTICAL)
        hSizer = wx.BoxSizer(wx.HORIZONTAL)
        hSizer2 = wx.BoxSizer(wx.HORIZONTAL)

        img = wx.Image(self.photoMaxWidth, self.photoMaxHeight)
        # img.Replace(0, 0, 0, 200, 200, 200)  # change empty image to grey
        bmp = wx.Bitmap(img)
        mask = wx.Mask(bmp, wx.BLACK)
        bmp.SetMask(mask)
        img = bmp.ConvertToImage()
        self.imageCtrl = wx.StaticBitmap(self, wx.ID_ANY, wx.Bitmap(img))

        self.photoTxt = wx.TextCtrl(self, size=(self.photoMaxHeight - 200, -1))
        self.photoTxt.Bind(wx.EVT_SET_FOCUS, self.onFocus)
        self.photoTxt.Bind(wx.EVT_KILL_FOCUS, self.onKillFocus)
        loadImageButton = wx.Button(self, label='Get image from URL')
        loadImageButton.Bind(wx.EVT_BUTTON, self.onLoad)
        self.imageLabel = wx.StaticText(self, label="")
        self.imageLoaded = False
        self.imageBlurred = False
        self.imageGreyscaled = False
        openClientButton = wx.Button(self, label='Get image from Pynq Camera')
        openClientButton.Bind(wx.EVT_BUTTON, self.onOpenClient)
        openClientButton.SetFocus()
        self.urlLabel = wx.StaticText(self, label="Enter URL address of an image")
        self.clientLabel = wx.StaticText(self, label="Load the image from Pynq Camera")
        hSizer.Add(self.urlLabel, 0, wx.ALL, 10)
        hSizer.Add(self.photoTxt, 0, wx.ALL, 5)
        hSizer.Add(loadImageButton, 0, wx.ALL, 5)
        
        hSizer2.Add(self.clientLabel, 0, wx.ALL, 10)
        hSizer2.Add(openClientButton, 0, wx.ALL, 5)
        
        self.mainSizer.Add(hSizer2, 0, wx.ALL, 10)
        self.mainSizer.Add(hSizer, 0, wx.ALL, 10)
        self.mainSizer.Add(self.imageCtrl, 0, wx.ALL | wx.CENTER, 10)
        self.mainSizer.Add(self.imageLabel, 0, wx.ALL | wx.CENTER, 5)
        self.SetSizer(self.mainSizer)

    def onBrowse(self, event):
        # Browse for a file
        wildcard = \
            "Image files (*.jpg, *.png, *.gif, *.bmp)|*.jpg;*.png;*.gif;*.bmp"
        dialog = wx.FileDialog(None, "Choose a file",
                               wildcard=wildcard,
                               style=wx.FD_OPEN)
        if dialog.ShowModal() == wx.ID_OK:
            self.pathToPicture = dialog.GetPath()

        dialog.Destroy()
        self.onView()

    def onView(self):
        imageName = self.pathToPicture
        img = wx.Image(imageName, wx.BITMAP_TYPE_ANY)
        # scale the image, preserving the aspect ratio
        W = img.GetWidth()
        H = img.GetHeight()
        if W > H:
            NewW = self.photoMaxSize
            NewH = self.photoMaxSize * H / W
        else:
            NewH = self.photoMaxSize
            NewW = self.photoMaxSize * W / H
        img = img.Scale(NewW, NewH)

        self.imageCtrl.SetBitmap(wx.Bitmap(img))
        self.imageLabel.SetLabel(imageName)
        self.imageLoaded = True
        self.imageBlurred = False
        self.imageGreyscaled = False
        self.Refresh()
        self.mainSizer.Layout()

    def onLoad(self, event):
        url = self.photoTxt.GetValue()
        i = getImage(url)
        bmp = PIL2wx(i)
        img = bmp.ConvertToImage()
        # scale the image, preserving the aspect ratio
        W = img.GetWidth()
        H = img.GetHeight()
        if W > H:
            NewW = self.photoMaxSize
            NewH = self.photoMaxSize * H / W
        else:
            NewH = self.photoMaxSize
            NewW = self.photoMaxSize * W / H
        img = img.Scale(NewW, NewH)

        self.imageCtrl.SetBitmap(wx.Bitmap(img))
        self.imageLabel.SetLabel(url)
        self.imageLoaded = True
        self.imageBlurred = False
        self.imageGreyscaled = False
        self.Refresh()
        self.mainSizer.Layout()

    def onOpenClient(self, event):
        imageName = "PHOTO.JPG"
        host, port, i = openClient(imageName)
        bmp = PIL2wx(i)
        img = bmp.ConvertToImage()
        # scale the image, preserving the aspect ratio
        W = img.GetWidth()
        H = img.GetHeight()
        if W > H:
            NewW = self.photoMaxSize
            NewH = self.photoMaxSize * H / W
        else:
            NewH = self.photoMaxSize
            NewW = self.photoMaxSize * W / H
        img = img.Scale(NewW, NewH)

        self.imageCtrl.SetBitmap(wx.Bitmap(img))
        self.imageLabel.SetLabel(str(host) + ":" + str(port) +
                                 " -- " + str(imageName))
        self.imageLoaded = True
        self.imageBlurred = False
        self.imageGreyscaled = False
        self.Refresh()
        self.mainSizer.Layout()

    def onFocus(self, event):
        if self.photoTxt.GetValue() == "Enter URL":
            self.photoTxt.SetValue("")
        event.Skip()

    def onKillFocus(self, event):
        if self.photoTxt.GetValue() == "":
            self.photoTxt.SetValue("Enter URL")
        event.Skip()

    def onGreyScale(self, event):
        if self.imageLoaded is True:
            if self.imageGreyscaled is False:
                bmp = self.imageCtrl.GetBitmap()
                img = bmp.ConvertToImage()
                greyScale = img.ConvertToGreyscale()
                # scale the image, preserving the aspect ratio
                W = greyScale.GetWidth()
                H = greyScale.GetHeight()
                if W > H:
                    NewW = self.photoMaxSize
                    NewH = self.photoMaxSize * H / W
                else:
                    NewH = self.photoMaxSize
                    NewW = self.photoMaxSize * W / H
                greyScale = greyScale.Scale(NewW, NewH)

                self.imageCtrl.SetBitmap(wx.Bitmap(greyScale))
                self.imageLabel.SetLabel("Grey scale")
                self.imageGreyscaled = True
                self.Refresh()
                self.mainSizer.Layout()
            else:
                print("Image is already greyscaled")
        else:
            print("There is no loaded image")

    def onBlur(self, event):
        if self.imageLoaded is True:
            if self.imageBlurred is False:
                bmp = self.imageCtrl.GetBitmap()
                img = bmp.ConvertToImage()
                # scale the image, preserving the aspect ratio
                W = img.GetWidth()
                H = img.GetHeight()
                img = img.BlurHorizontal(10)
                img = img.BlurVertical(10)
                if W > H:
                    NewW = self.photoMaxSize
                    NewH = self.photoMaxSize * H / W
                else:
                    NewH = self.photoMaxSize
                    NewW = self.photoMaxSize * W / H
                img = img.Scale(NewW, NewH)

                self.imageCtrl.SetBitmap(wx.Bitmap(img))
                self.imageLabel.SetLabel("Blurred")
                self.imageBlurred = True
                self.Refresh()
                self.mainSizer.Layout()
            else:
                print("Image is already blurred")
        else:
            print("There is no loaded image")


def PIL2wx(image):
    width, height = image.size
    return wx.Bitmap.FromBuffer(width, height, image.convert("RGB").tobytes())


def getImage(url):
    try:
        response = requests.get(url, stream=True)
        if response.status_code == 200:
            response.raw.decode_content = True

            # Grab the first 100 bytes as potential image header
            header = response.raw.read(100)
            extension = imghdr.what(None, h=header)
            if extension is not None:
                print("Found: " + extension)
                # Get the rest of the image
                data = header + response.raw.read()
                data = io.BytesIO(data)
                img = Image.open(data)
                return img
            else:
                print("No extension read in header, still trying to return the image")
                data = header + response.raw.read()
                data = io.BytesIO(data)
                img = Image.open(data)
                return img

        else:
            print("Received error " + str(response.status.code))
    except requests.ConnectionError as exc:
        print(exc)


def openClient(imageName):
    #s = socket.socket()  # create a socket object
    # host = socket.gethostname()  # get local machine name
    # port = 12345
    TCP_IP = '192.168.1.10'
    TCP_PORT = 7
    host = TCP_IP
    port = TCP_PORT
    BUFFER_SIZE = 2048
    MESSAGE = b"PHOTO.JPG"
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    s.connect((TCP_IP, TCP_PORT))
    # s.connect((host, port))
    s.send(MESSAGE)
    print("Sending...")
    # f = io.BytesIO()
    f = open("enk", "wb")
    imageChunk = s.recv(2048)
    print("Receiving...")
    while (imageChunk):
        
        f.write(imageChunk)
        imageChunk = s.recv(2048)
    print("Done receiving image")
    s.shutdown(socket.SHUT_WR)
    s.close()  # close the socket when done
    f.close()
    call(["./dekripcija.exe"])  # if it is compiled for Windows
    # call(["./dekripcija"])  # if it is compiled for Linux 
    img = Image.open("dekriptirano.jpg")
    return host, port, img