import tkinter as tk
from tkinter.constants import LEFT
"""
# Top level window
frame = tk.Tk()
frame.title("TextBox Input")
frame.geometry('300x100')
# Function for getting Input
# from textbox and printing it 
# at label widget
"""

class myNo:
    def __init__(self,x):
        self.x = x
X = myNo(0)
Y = myNo(0)

"""
def changeNo():

    inp = inputtxt.get(1.0,'end')
    inp = int(inp)
    X.x = inp
    lbl.config(text = ""+str(X.x))
  
# TextBox Creation
inputtxt = tk.Text(frame,
                   height = 2,
                   width = 5)
  
inputtxt.pack()

# Button Creation
printButton = tk.Button(frame,
                        text = "Print", 
                        command = changeNo)
printButton.pack(pady = 10)




inpText2 =  tk.Text(frame,
                   height = 2,
                   width = 5)
inpText2.pack()


printButton2 = tk.Button(frame,
                        text = "Print", 
                        command = changeNo)
printButton2.pack()

# Label Creation
lbl = tk.Label(frame, text = "SynGain")

#lbl.pack(pady = 0)

var = tk.IntVar(value='4')

def changeNo2():
    
    #var = selectBox.get()
    #var = int(var)
    X.x = var
    lbl2.config(text = ""+str(X.x))

selectBox = tk.Spinbox(
    frame,
    from_=1,
    to=10,
    font=('sans-serif', 14), 
    textvariable=var,
    command = changeNo2,
    

)
#var = tk.StringVar(value='0')
selectBox.grid(row = 0, column=0, ipadx=0)

lbl2 = tk.Label(frame,text = "0")
#lbl2.pack(pady = 30)
lbl2.grid(row =0,column = 1,ipadx=10)


frame.mainloop()
"""

import tkinter as tk
 
 
class Window:
    def __init__(self, master):
        frame = tk.Frame(master, width = 200, height = 80)
 
        
 
        self.spinbox = tk.Spinbox(master, from_ = 1, to = 10,
                                  increment = 1, 
                                  command = lambda: self.display(self.display))
        self.spinbox.pack(padx = 5, pady = 20)
 
        text = "Current Value: " + str(self.spinbox.get())
        self.label = tk.Label(master, text = text)
        self.label.pack(padx = 10, pady = 20)
 
        frame.pack(padx = 10, pady = 10, expand = True, fill = tk.BOTH)
 
        self.bindings()

    def display(self,event):
        text = "Current Value: " + str(self.spinbox.get())
        self.label.configure(text = text)

    def bindings(self):
        
        self.spinbox.bind('<Return>', self.display)

 
root = tk.Tk()
window = Window(root)
root.mainloop()