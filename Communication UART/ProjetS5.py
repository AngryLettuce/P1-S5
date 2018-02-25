#Created by Guillaume Roux, this project aim to make a UART communication with a PIC and to make a graphical interface showing the picture of someone


import tkinter as tk   
from PIL import Image, ImageTk
import os
import serial
from time import sleep


count = 0 

def ImageDictionnary(Orateur):
    Dict = {"Guillaume" : r"C:\Users\Guill\Pictures\Death_of_Pablo_Escobar.jpg",
            "Pascal"    : r"C:\Users\Guill\Pictures\Pascal.PNG"
            } 
    image = Image.open(Dict[Orateur])
    photo = ImageTk.PhotoImage(image)

    return photo



def CycleImage():
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

def setupSerialPort(port):
    ser = serial.Serial(port, 38400, timeout=0.001)
    return ser

def readSerial():
    data = ser2.read(9999)
    test =  data.decode('ascii')
    ReadingLabel.configure(text=data.decode('ascii'))
    ReadingLabel.text = data.decode('ascii')

def writeSerial():
    data = writingBox.get()
    ser1.write(data.encode())

    readSerial()

class Application(tk.Frame):         
    global ser1
    global ser2

    ser1 = setupSerialPort("\\\\.\\CNCA0")
    ser2 = setupSerialPort("\\\\.\\CNCB0")

    def __init__(self, master=None):
        tk.Frame.__init__(self, master)  
        self.grid()                      
        self.createWidgets()


    def createWidgets(self):
        self.quitButton = tk.Button(self,  text='Exit',     command=self.quit)         
        self.quitButton.grid()   

        self.TrainButton = tk.Button(self, text='Training', command=lambda: CycleImage())            
        self.TrainButton.grid()

        #self.ReadSerial = tk.Button(self,  text='Read Serial',  command=lambda: readSerial())
        #self.ReadSerial.grid()

        self.WriteSerial = tk.Button(self, text='Write Serial', command=lambda: writeSerial())
        self.WriteSerial.grid()

        global writingBox
        writingBox = tk.Entry(self)
        writingBox.grid()

        global ReadingLabel
        ReadingLabel = tk.Label(self)
        ReadingLabel.grid()

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
   
ser1.close()
ser2.close()