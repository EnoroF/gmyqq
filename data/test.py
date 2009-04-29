#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import gtk
import gtk.glade

class MainWindow():
    def __init__(self):
        self.gladefile="gmyqq.glade"
        self.xml=gtk.glade.XML(self.gladefile)
        self.window = self.xml.get_widget("main_window")

        self.window.set_title("gmyqq")
        self.window.set_default_size(800, 600)
        self.window.connect('destroy', gtk.main_quit)      
        self.window.show_all();
        
def main():
    win = MainWindow();
    gtk.main()


if __name__ == '__main__':
    main()
