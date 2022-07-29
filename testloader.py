import tkinter as tk
 
import tkinter as tk
 
class GreetingApp(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title('Greeting Application')
        self.geometry("300x300")
 
        self.name_var = tk.StringVar()
        
        self.entry = tk.Entry(textvariable=self.name_var)
        self.create_widgets()
     
    def create_widgets(self):
        self.description_label = tk.Label(self, text="Enter your name:")
        self.description_label.grid(column=0, row=0)
 
        #self.entry = tk.Entry(self, textvariable=self.name_var)
        self.entry.grid(column=1, row=0)
        self.entry.bind('<Return>',self.create_greeting_message)
 
        self.greeting_label = tk.Label(self)
        self.greeting_label.grid(column=0, row=1, columnspan=2)
     
    def create_greeting_message(self, *args):
        name_entered = self.name_var.get()
 
        greeting_message = name_entered

    
        self.greeting_label['text'] = greeting_message
        
 
if __name__ == "__main__":
    app = GreetingApp()
    app.mainloop()