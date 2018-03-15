#Created by Guillaume Roux, 
#this project aim to make a sserial communication with a PIC
# and to make a graphical interface showing the picture of someone


import tkinter as tk   
from PIL import Image, ImageTk
import os
import serial
from time import sleep
from threading import Timer

class RepeatedTimer(object):
    def __init__(self, interval, function, *args, **kwargs):
        self._timer     = None
        self.interval   = interval
        self.function   = function
        self.args       = args
        self.kwargs     = kwargs
        self.is_running = False
        #self.start()

    def _run(self):
        self.is_running = False
        self.start()
        self.function(*self.args, **self.kwargs)

    def start(self):
        if not self.is_running:
            self._timer = Timer(self.interval, self._run)
            self._timer.start()
            self.is_running = True

    def stop(self):
        self._timer.cancel()
        self.is_running = False

    
def ImageDictionnary(Orateur):
    Dict = {"Guillaume" : r"C:\Users\Guill\Pictures\Death_of_Pablo_Escobar.jpg",
            "Pascal"    : r"C:\Users\Guill\Pictures\Pascal.PNG"
            } 
    image = Image.open(Dict[Orateur])
    photo = ImageTk.PhotoImage(image)

    return photo


class ApplicationProjetS5(tk.Frame):         


    def __init__(self, master=None):
        baurate = 38400
        readingTimeout = 1e-6

        tk.Frame.__init__(self, master)  
        self.grid()                      
        self.createWidgets()
        self.ser1 = self.setupSerialPort("COM2", baurate, readingTimeout)
        self.ser2 = self.setupSerialPort("COM3", baurate, readingTimeout)      

        self.readingThread = RepeatedTimer(0.001, self.readSerial)

        #self.ser1 = self.setupSerialPort("\\\\.\\CNCA0", baurate, readingTimeout)
        #self.ser2 = self.setupSerialPort("\\\\.\\CNCB0", baurate, readingTimeout)
        self.cyclePhoto = 0

    def createWidgets(self):
        #self.quitButton = tk.Button(self,  text='Exit',     command=self.quit)         
        #self.quitButton.grid(row=1, column=9)   

        self.writingSerial_B = tk.Button(self, text='Write', command=lambda: self.writingSerialButton())
        self.writingSerial_B.grid(columnspan=1, row=1, column=1)
        self.writingSerial_B.config( height = 3, width = 10 )

        self.writingBox = tk.Entry(self)
        self.writingBox.grid(columnspan=1, row=1, column=6)

        self.readSerial_B = tk.Button(self,  text='Read', command=lambda: self.startThread(self.readingThread))
        self.readSerial_B.grid(columnspan=1, row=4, columns=2)
        self.readSerial_B.config( height = 3, width = 10 )

        self.stopReading_B = tk.Button(self,  text='Stop', command=lambda: self.stopThread(self.readingThread))
        self.stopReading_B.grid(columnspan=1, row=5, columns=2)
        self.stopReading_B.config( height = 3, width = 10 )

        self.readingLabel = tk.Label(self, text='This is the reading Label')
        self.readingLabel.grid(columnspan=1, row=1, column=7)

        #self.trainButton_B = tk.Button(self, text='Training', command=lambda: self.cycleImage())            
        #self.trainButton_B.grid(columnspan=1, row=3, column=1)

        self.orateurPicLabel = tk.Label(self)
        self.orateurPicLabel.grid(columnspan=15, row=4, column=5)

    def setupSerialPort(self, port, baudrate, timeout):
        ser = serial.Serial(port, baudrate, timeout=timeout)
        return ser

    def cycleImage(self):
        if self.cyclePhoto == 0 : 
            OrateurVariable = "Guillaume"
            self.cyclePhoto = 1
        else :
            OrateurVariable = "Pascal"
            self.cyclePhoto = 0 

        photo = ImageDictionnary(OrateurVariable)
        self.orateurPicLabel.configure(image=photo)
        self.orateurPicLabel.photo = photo

    def readSerial(self):
        data = self.ser2.read(9999)
        data = data.decode('ascii')
        if data != '':
            self.readingLabel.configure(text=data)
            self.readingLabel.text = data

    def writingSerial(self):
        data = self.writingBox.get()
        self.ser1.write(data.encode())


    def writingSerialButton(self):
        self.writingSerial()
        #self.readSerial()

    def stopThread(self, thread):
        thread.stop()

    def startThread(self, thread):
        thread.start()






				##############  __  __   _   ___ _  _  ##############
				############## |  \/  | /_\ |_ _| \| | ##############
				############## | |\/| |/ _ \ | || .` | ##############
				############## |_|  |_/_/ \_\___|_|\_| ##############



MyRoot = tk.Tk()

app = ApplicationProjetS5(MyRoot)                       
app.master.title('Projet S5 P01')    

w = MyRoot.winfo_screenwidth()
h = MyRoot.winfo_screenheight()
MyRoot.geometry("%dx%d+0+0" % (w, h))



app.mainloop()
   
app.ser1.close()
app.ser2.close()