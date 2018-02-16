#Created by Guillaume Roux, this project aim to make a UART communication with a PIC and to make a graphical interface showing the picture of someone


import tkinter as tk   
from PIL import Image, ImageTk


count = 0 

def ImageDictionnary(Orateur):
    Dict = {"Guillaume" : r"C:\Users\Guill\Pictures\Death_of_Pablo_Escobar.jpg",
            "Pascal"    : r"C:\Users\Guill\Pictures\Pascal.PNG"
            } 
    image = Image.open(Dict[Orateur])
    photo = ImageTk.PhotoImage(image)

    return photo



def CycleImage(parent):
    global count

    if count == 0 : 
        OrateurVariable = "Guillaume"
        count = 1
    else :
        OrateurVariable = "Pascal"
        count = 0 

    photo = ImageDictionnary(OrateurVariable)
    OrateurPicLabel.configure(image=photo)
    OrateurPicLabel.photo = photo

class Application(tk.Frame):         
    

    def __init__(self, master=None):
        tk.Frame.__init__(self, master)  
        self.grid()                      
        self.createWidgets()

    def createWidgets(self):
        self.quitButton = tk.Button(self, text='Exit', command=self.quit)         
        self.quitButton.grid()   

        self.TrainButton = tk.Button(self, text='Training', command=lambda: CycleImage(self))            
        self.TrainButton.grid()

        global OrateurPicLabel
        OrateurPicLabel = tk.Label(self)
        OrateurPicLabel.grid()


				##############  __  __   _   ___ _  _  ##############
				############## |  \/  | /_\ |_ _| \| | ##############
				############## | |\/| |/ _ \ | || .` | ##############
				############## |_|  |_/_/ \_\___|_|\_| ##############


app = Application()                       
app.master.title('Projet S5 P01')    
app.mainloop()
