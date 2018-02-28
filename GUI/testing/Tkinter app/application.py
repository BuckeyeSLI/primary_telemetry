from tkinter import *
from tkinter.scrolledtext import *
from tkinter import messagebox
import shelve

class application(Frame):

	def __init__(self, root):
		super().__init__(root)
		self.pack(pady=5)
		# database file
		self.file = shelve.open("phone_numbers.txt", "c")
		# init name number labels, forms
		Label(self, text="Name").grid(sticky=W)
		self.name_entry = Entry(self)
		self.name_entry.grid(row=1, column=0)
		Label(self, text="Number").grid(row=0, column=1, sticky=W)
		self.phone_entry = Entry(self)
		self.phone_entry.grid(row=1, column=1)
		Button(self, text="Submit", command=self.submit).grid(row=1, column=2)
		# seprate frame for radio buttons
		cframe = Frame(self)
		cframe.grid(columnspan=2)
		Label(cframe, text="Command: ").grid()
		self.choice = StringVar()
		self.choice.set(None)
		Radiobutton(cframe, text="-Find", variable=self.choice, value="find").grid(row=0, column=1)
		Radiobutton(cframe, text="-Insert", variable=self.choice, value="insert").grid(row=0, column=2)
		Radiobutton(cframe, text="-Delete", variable=self.choice, value="delete").grid(row=0, column=3)
		# text box with scrollbar
		self.text = ScrolledText(self, width="37", height="8", font=("Arial", 12))
		self.text.grid(columnspan=3)
		self.label = Label(self)
		self.label.grid(columnspan=3)
		# call last after text area created to show past entries
		self.update()

	def submit(self):
		if self.choice.get() == "insert":
			self.add(self.name_entry.get(), self.phone_entry.get())
		if self.choice.get() == "delete":
			self.delete(self.name_entry.get(), self.phone_entry.get())
		if self.choice.get() == "find":
			self.find(self.name_entry.get())

	def find(self, name):
		if name in self.file.keys():
			result = name + " : " + self.file[name]
			messagebox.showinfo(title="Result", message=result)
			self.name_entry.delete(0, END)
			self.phone_entry.delete(0, END)
			self.label["text"] = ""
		elif name:
			self.name_entry.delete(0, END)
			self.phone_entry.delete(0, END)
			self.label["text"] = "Name not found"
		else:
			self.label["text"] = "To search, please give name."

	def add(self, name, number):
		if name and number:
			self.file[name] = number
			self.update()
			self.name_entry.delete(0, END)
			self.phone_entry.delete(0, END)
			self.label["text"] = ""
		else:
			self.label["text"] = "To add entry, please give name and number."

	def delete(self, name, number):
		if name and number:
			if name in self.file.keys():
				del self.file[name]
				self.update()
			self.name_entry.delete(0, END)
			self.phone_entry.delete(0, END)
			self.label["text"] = ""
		else:
			self.label["text"] = "To delete entry, please give name and number."

	def update(self):
		self.text.delete("0.0", END)
		for name in self.file.keys():
			string = name + " - " + self.file[name] + "\n"
			self.text.insert("0.0", string)