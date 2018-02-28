from tkinter import *

root = Tk()
root.title("The title")

def Save():
	print("Save")

def SaveAs():
	print("Save As")

def Undo():
	print("Undo")

menu = Menu(root)
root.config(menu=menu)

fileMenu = Menu(menu, tearoff=False)
fileMenu.add_command(label="Save", command=Save)
fileMenu.add_command(label="Save As", command=SaveAs)

editMenu = Menu(menu, tearoff=False)
editMenu.add_command(label="undo", command=Undo)

viewMenu = Menu(menu, tearoff=False)
viewMenu.add_command(label="Layout Views")

menu.add_cascade(label="File", menu=fileMenu)
menu.add_cascade(label="Edit", menu=editMenu)
menu.add_cascade(label="View", menu=viewMenu)


#~~~~~~~~~~ toolbar ~~~~~~~~~~~~~~~~~~~~~#

toolbar = Frame(root, bg="navy")

button1 = Button(toolbar, text="Insert image")
button1.pack(side=LEFT, padx=4, pady=4)
button2 = Button(toolbar, text="Print")
button2.pack(side=LEFT, padx=4, pady=4)

toolbar.pack(side=TOP, fill=X)

#~~~~~~~~~~~~~~~~~~~ Status Bar ~~~~~~~~~~~~~~~~~~~#

statusBar = Label(root, text="Doing something...", bd=1, relief=SUNKEN, anchor=W)
statusBar.pack(side=BOTTOM, fill=X)





root.mainloop()
