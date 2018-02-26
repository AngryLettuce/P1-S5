#Created by Guillaume Roux, 
#this project aim to make a sserial communication with a PIC
# and to make a graphical interface showing the picture of someone


import tkinter as tk   
from PIL import Image, ImageTk
import os
import serial
from time import sleep
import threading

    
def ImageDictionnary(Orateur):
    Dict = {"Guillaume" : r"C:\Users\Guill\Pictures\Death_of_Pablo_Escobar.jpg",
            "Pascal"    : r"C:\Users\Guill\Pictures\Pascal.PNG"
            } 
    image = Image.open(Dict[Orateur])
    photo = ImageTk.PhotoImage(image)

    return photo


class Application(tk.Frame):         

    def __init__(self, master=None):
        tk.Frame.__init__(self, master)  
        self.grid()                      
        self.createWidgets()
        self.ser1 = self.setupSerialPort("\\\\.\\CNCA0")
        self.ser2 = self.setupSerialPort("\\\\.\\CNCB0")
        self.CyclePhoto = 0

    def createWidgets(self):
        #self.quitButton = tk.Button(self,  text='Exit',     command=self.quit)         
        #self.quitButton.grid(row=1, column=3)   

        self.TrainButton = tk.Button(self, text='Training', command=lambda: self.CycleImage())            
        self.TrainButton.grid(row=1, column=1)

        self.ReadSerial = tk.Button(self,  text='Read Serial',  command=lambda: self.readSerial())
        self.ReadSerial.grid()

        self.WriteSerial = tk.Button(self, text='Write Serial', command=lambda: self.writingSerialButton())
        self.WriteSerial.grid(row=1, column=2)

        self.writingBox = tk.Entry(self)
        self.writingBox.grid(columnspan=4, row=2, column=1)

        self.ReadingLabel = tk.Label(self)
        self.ReadingLabel.grid(columnspan=4, row=3, column=4)

        self.OrateurPicLabel = tk.Label(self)
        self.OrateurPicLabel.grid(columnspan=15, row=4, column=5)

    def setupSerialPort(self, port):
        ser = serial.Serial(port, 38400, timeout=0.000001)
        return ser

    def CycleImage(self):
        if self.CyclePhoto == 0 : 
            OrateurVariable = "Guillaume"
            self.CyclePhoto = 1
        else :
            OrateurVariable = "Pascal"
            self.CyclePhoto = 0 

        photo = ImageDictionnary(OrateurVariable)
        self.OrateurPicLabel.configure(image=photo)
        self.OrateurPicLabel.photo = photo

    def readSerial(self):
        data = self.ser2.read(9999)
        self.ReadingLabel.configure(text=data.decode('ascii'))
        self.ReadingLabel.text = data.decode('ascii')

    def writeSerial(self):
        data = self.writingBox.get()
        self.ser1.write(data.encode())


    def writingSerialButton(self):
        self.writeSerial()
        #self.readSerial()




				##############  __  __   _   ___ _  _  ##############
				############## |  \/  | /_\ |_ _| \| | ##############
				############## | |\/| |/ _ \ | || .` | ##############
				############## |_|  |_/_/ \_\___|_|\_| ##############



MyRoot = tk.Tk()

app = Application(MyRoot)                       
app.master.title('Projet S5 P01')    

w = MyRoot.winfo_screenwidth()
h = MyRoot.winfo_screenheight()
MyRoot.geometry("%dx%d+0+0" % (w, h))

app.mainloop()
   
app.ser1.close()
app.ser2.close()