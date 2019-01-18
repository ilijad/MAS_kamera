#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import wx
from panel import ViewerPanel


class PhotoViewer(wx.Frame):
    def __init__(self):
        wx.Frame.__init__(self, None, title='MAS Photo Viewer')

        panel = ViewerPanel(self)
        self.sizer = wx.BoxSizer(wx.VERTICAL)
        self.sizer.Add(panel, 1, wx.EXPAND)
        self.SetSizer(self.sizer)

        self.sizer.Fit(self)
        self.Centre()

        self.InitUI(panel)

    def InitUI(self, panel):
        menubar = wx.MenuBar()
        fileMenu = wx.Menu()
        imageMenu = wx.Menu()
        aboutMenu = wx.Menu()
        openFile = fileMenu.Append(wx.ID_OPEN, '&Open\tCtrl+O')
        quitProgram = fileMenu.Append(wx.ID_EXIT, '&Quit\tCtrl+Q')
        aboutProgram = aboutMenu.Append(wx.ID_ABOUT, '&About Program')
        greyscale = imageMenu.Append(wx.ID_ANY, '&Greyscale')
        blur = imageMenu.Append(wx.ID_ANY, '&Blur')
        menubar.Append(fileMenu, '&File')
        menubar.Append(imageMenu, '&Image')
        menubar.Append(aboutMenu, '&About')
        self.SetMenuBar(menubar)
        self.Bind(wx.EVT_MENU, self.onQuit, quitProgram)
        self.Bind(wx.EVT_MENU, panel.onBrowse, openFile)
        self.Bind(wx.EVT_MENU, self.onAbout, aboutProgram)
        self.Bind(wx.EVT_MENU, panel.onGreyScale, greyscale)
        self.Bind(wx.EVT_MENU, panel.onBlur, blur)

    def onQuit(self, event):
        self.Close()

    def onAbout(self, event):
        aboutMessage = "MAS projekt 2018./2019.\n \
                        \nAutori: \
                        \nIlija Domislović \
                        \nPetar Grekšić \
                        \nNikola Ugarković \
                        \nLuka Kudra"
        msgBox = wx.MessageDialog(None, message=aboutMessage,
                                  caption='About',
                                  style=wx.ICON_INFORMATION | wx.STAY_ON_TOP | wx.OK)
        if (msgBox.ShowModal() == wx.ID_OK):
            msgBox.Destroy()


def main():
    app = wx.App()
    pw = PhotoViewer()
    pw.Show()
    app.MainLoop()


if __name__ == '__main__':
    main()
