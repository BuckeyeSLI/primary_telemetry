from tkinter import *
from phoneDir import *
import shelve

rootFile = shelve.open("numbers.bat")

def add_number():
	rootFile[nameField.get()] = numberField.get()
	string = nameField.get() + " " + numberField.get()
	Label(bottomFrame, text=string).pack()

root = Tk()
root.title("Phonebook")
root.geometry("300x300")

frame = Frame(root)
frame.grid()

bottomFrame = Frame(root)
bottomFrame.grid()

for x in rootFile:
	string = x + " : " + rootFile[x] 
	Label(bottomFrame, text=string).pack()

nameLabel = Label(frame, text="Name")
nameLabel.grid(row=1, column=1)

nameField = Entry(frame)
nameField.grid(row=2, column=1, padx=5)

numberLabel= Label(frame, text="Number")
numberLabel.grid(row=1, column=2)

numberField = Entry(frame)
numberField.grid(row=2, column=2, padx=5)

submitButton = Button(frame, text="Add to Phonebook", command=add_number)
submitButton.grid(row=3, columnspan=3, pady=5)


root.mainloop()
