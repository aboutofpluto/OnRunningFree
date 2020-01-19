#! /usr/bin/env python
#  -*- coding: utf-8 -*-
#
# GUI module generated by PAGE version 4.26
#  in conjunction with Tcl version 8.6
#    Jan 19, 2020 09:08:42 PM CET  platform: Linux

import sys

try:
    import Tkinter as tk
except ImportError:
    import tkinter as tk

try:
    import ttk
    py3 = False
except ImportError:
    import tkinter.ttk as ttk
    py3 = True

import onmovefreely_support

def vp_start_gui():
    '''Starting point when module is the main routine.'''
    global val, w, root
    root = tk.Tk()
    top = Toplevel1 (root)
    onmovefreely_support.init(root, top)
    root.mainloop()

w = None
def create_Toplevel1(root, *args, **kwargs):
    '''Starting point when module is imported by another program.'''
    global w, w_win, rt
    rt = root
    w = tk.Toplevel (root)
    top = Toplevel1 (w)
    onmovefreely_support.init(w, top, *args, **kwargs)
    return (w, top)

def destroy_Toplevel1():
    global w
    w.destroy()
    w = None

class Toplevel1:
    def __init__(self, top=None):
        '''This class configures and populates the toplevel window.
           top is the toplevel containing window.'''
        _bgcolor = '#d9d9d9'  # X11 color: 'gray85'
        _fgcolor = '#000000'  # X11 color: 'black'
        _compcolor = '#d9d9d9' # X11 color: 'gray85'
        _ana1color = '#d9d9d9' # X11 color: 'gray85'
        _ana2color = '#ececec' # Closest X11 color: 'gray92'
        font10 = "-family {DejaVu Sans} -size 12 -weight normal -slant"  \
            " roman -underline 1 -overstrike 0"
        self.style = ttk.Style()
        if sys.platform == "win32":
            self.style.theme_use('winnative')
        self.style.configure('.',background=_bgcolor)
        self.style.configure('.',foreground=_fgcolor)
        self.style.configure('.',font="TkDefaultFont")
        self.style.map('.',background=
            [('selected', _compcolor), ('active',_ana2color)])

        top.geometry("533x465+387+132")
        top.minsize(1, 1)
        top.maxsize(1905, 1170)
        top.resizable(1, 1)
        top.title("New Toplevel 1")
        top.configure(highlightcolor="black")

        self.InputDirBtn = tk.Button(top)
        self.InputDirBtn.place(relx=0.019, rely=0.086, height=33, width=200)
        self.InputDirBtn.configure(command=onmovefreely_support.input_btn_handler)
        self.InputDirBtn.configure(disabledforeground="#b8a786")
        self.InputDirBtn.configure(font="-family {DejaVu Sans} -size 12")
        self.InputDirBtn.configure(text='''Select input directory''')
        self.InputDirBtn.bind('<Button-2>',lambda e:onmovefreely_support.button_handler2(e,2))
        self.InputDirBtn.bind('<Button-3>',lambda e:onmovefreely_support.button_handler2(e,3))

        self.QuitButton = tk.Button(top)
        self.QuitButton.place(relx=0.863, rely=0.839, height=33, width=61)
        self.QuitButton.configure(command=onmovefreely_support.quit)
        self.QuitButton.configure(disabledforeground="#b8a786")
        self.QuitButton.configure(font="-family {DejaVu Sans} -size 12")
        self.QuitButton.configure(text='''Quit''')

        self.OutputDirBtn = tk.Button(top)
        self.OutputDirBtn.place(relx=0.019, rely=0.194, height=33, width=200)
        self.OutputDirBtn.configure(activebackground="#f9f9f9")
        self.OutputDirBtn.configure(command=onmovefreely_support.output_btn_handler)
        self.OutputDirBtn.configure(disabledforeground="#b8a786")
        self.OutputDirBtn.configure(font="-family {DejaVu Sans} -size 12")
        self.OutputDirBtn.configure(text='''Select output directory''')
        self.OutputDirBtn.bind('<Button-2>',lambda e:onmovefreely_support.button_handler2(e,2))
        self.OutputDirBtn.bind('<Button-3>',lambda e:onmovefreely_support.button_handler2(e,3))

        self.Listbox1 = tk.Listbox(top)
        self.Listbox1.place(relx=0.019, rely=0.387, relheight=0.43
                , relwidth=0.957)
        self.Listbox1.configure(background="white")
        self.Listbox1.configure(font="TkFixedFont")

        self.InputDirEntry = tk.Entry(top)
        self.InputDirEntry.place(relx=0.413, rely=0.108, height=23
                , relwidth=0.563)
        self.InputDirEntry.configure(background="white")
        self.InputDirEntry.configure(font="TkFixedFont")
        self.InputDirEntry.configure(state='readonly')

        self.ExportBtn = tk.Button(top)
        self.ExportBtn.place(relx=0.019, rely=0.839, height=33, width=200)
        self.ExportBtn.configure(activebackground="#f9f9f9")
        self.ExportBtn.configure(command=onmovefreely_support.export_btn_handler)
        self.ExportBtn.configure(disabledforeground="#b8a786")
        self.ExportBtn.configure(font="-family {DejaVu Sans} -size 12")
        self.ExportBtn.configure(text='''Export selected to gpx''')
        self.ExportBtn.bind('<Button-2>',lambda e:onmovefreely_support.button_handler2(e,2))
        self.ExportBtn.bind('<Button-3>',lambda e:onmovefreely_support.button_handler2(e,3))

        self.Label1 = tk.Label(top)
        self.Label1.place(relx=0.019, rely=0.022, height=23, width=111)
        self.Label1.configure(font=font10)
        self.Label1.configure(text='''1. Directories''')

        self.OutputDirEntry = tk.Entry(top)
        self.OutputDirEntry.place(relx=0.413, rely=0.215, height=23
                , relwidth=0.563)
        self.OutputDirEntry.configure(background="white")
        self.OutputDirEntry.configure(font="TkFixedFont")
        self.OutputDirEntry.configure(selectbackground="#c4c4c4")
        self.OutputDirEntry.configure(state='readonly')

        self.Label1_6 = tk.Label(top)
        self.Label1_6.place(relx=0.0, rely=0.323, height=23, width=111)
        self.Label1_6.configure(activebackground="#f9f9f9")
        self.Label1_6.configure(font="-family {DejaVu Sans} -size 12 -underline 1")
        self.Label1_6.configure(text='''2. Records''')

        self.TSeparator1 = ttk.Separator(top)
        self.TSeparator1.place(relx=0.019, rely=0.301, relwidth=0.957)

        self.TSeparator1_7 = ttk.Separator(top)
        self.TSeparator1_7.place(relx=0.019, rely=0.946, relwidth=0.957)

        self.Label2 = tk.Label(top)
        self.Label2.place(relx=0.019, rely=0.953, height=21, width=161)
        self.Label2.configure(text='''Powered by Out of Pluto''')

if __name__ == '__main__':
    vp_start_gui()





