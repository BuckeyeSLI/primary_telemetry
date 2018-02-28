from tkinter import *
from tkinter import messagebox
from application import application

root = Tk()
root.title("Telephone Database")
root.geometry("400x310")

# add header photo
photo = PhotoImage(file="pic.gif")
Label(root, image=photo ).pack()

# init main app window
app = application(root)

#popup window
def show_all():
	entries = ""
	for number in app.file.keys():
		entries += number + " : " + app.file[number] + "\n"
	messagebox.showinfo(title="All Entries", message=entries)

# add menu
menu = Menu(root)
file_menu = Menu(menu, tearoff=0)
file_menu.add_command(label="View all", command=show_all)
file_menu.add_command(label="Exit", command=root.quit)
menu.add_cascade(label="Menu", menu=file_menu)
root.config(menu=menu)

root.mainloop()
app.file.close()
