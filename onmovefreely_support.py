#! /usr/bin/env python
#  -*- coding: utf-8 -*-
#
# Support module generated by PAGE version 4.26
#  in conjunction with Tcl version 8.6
#    Jan 19, 2020 09:07:11 PM CET  platform: Linux

import sys, os, glob
# python/c wrapper (swig)
import omx2gpx
from tkinter import filedialog, messagebox, PhotoImage
# base64 encoded images
from images import OUT_OF_JAIL

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

# read headers and display data
def get_omx_infos_in_dir(directory):
    infos = []
    for f in glob.glob(os.path.join(directory, "*.OMH")):
        recordname = f.replace(".OMH", "")
        if os.path.exists(recordname + ".OMD"):
            infos.append(os.path.basename(recordname))
    return infos

# handler for 'Export to GPX' button
# 1. check if input dir is selected
# 2. check if output dir are selected
# 3. check if at least of OMx record is selected
# If one of these condition is False, nothing more happens
# Else
# Use omx2gpx to convert each record to GPX and output on stdout
# TODO: get outputs in the app, with stats about exportation
def export_btn_handler():    
    input_dir = w.InputDirEntry.get()
    output_dir = w.OutputDirEntry.get()
    l = w.Listbox1
    
    if len(input_dir) == 0:
        messagebox.showinfo("OnMoveFreely", "Please select an input directory")
    elif len(output_dir) == 0:
        messagebox.showinfo("OnMoveFreely", "Please select an output directory")
    elif len(l.curselection()) == 0:
            messagebox.showinfo("OnMoveFreely", "Please select at least one record to export")
    else:
        output = []
        for index in l.curselection():
            output.append(
                omx2gpx.pywrap(
                    os.path.join(input_dir, l.get(index)),
                    output_dir)
            )

        print("\n".join(output))

# handler for 'Select input directory' button
# 1. Open directory
# 2. Look for OMD/OMH files
def input_btn_handler():
    d = filedialog.askdirectory()

    e = w.InputDirEntry
    l = w.Listbox1

    e.configure(state='normal')
    e.delete(0, tk.END)
    l.delete(0, tk.END)

    infos = get_omx_infos_in_dir(d)
    if len(infos) == 0:
        messagebox.showinfo("OnMoveFreely", "No OMx files in selected directory")
    else:
        e.insert(0, d)
        l.insert(0, *infos)

    e.configure(state='readonly')
    
# handler for 'Select output directory' button
def output_btn_handler():
    d = filedialog.askdirectory()
    e = w.OutputDirEntry
    e.configure(state='normal')
    e.delete(0, tk.END)
    e.insert(0, d)
    e.configure(state='readonly')

def quit():
    destroy_window()

def init(top, gui, *args, **kwargs):
    global w, top_level, root
    w = gui
    top_level = top
    root = top

    render = PhotoImage(data=OUT_OF_JAIL)
    label = w.Label3
    label.configure(image=render)
    label.image = render
    
def destroy_window():
    # Function which closes the window.
    global top_level
    top_level.destroy()
    top_level = None

if __name__ == '__main__':
    import onmovefreely
    onmovefreely.vp_start_gui()




