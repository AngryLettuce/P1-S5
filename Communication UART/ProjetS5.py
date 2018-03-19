#Created by Guillaume Roux, 
#this project aim to make a sserial communication with a PIC
# and to make a graphical interface showing the picture of someone


import tkinter as tk   
from PIL import Image, ImageTk
import os
import serial
from time import sleep
from threading import Timer


def ImageDictionnary(Orateur):
    #Picture size : 381 * 285 px
    Dict = {'0' : (r"noImage.jpg",       'Inconnu'),
            '1' : (r"Pablo_Escobar.jpg", 'Pablo Escobar'),
            '2' : (r"Pascal.PNG",        'Pascal L.'),
            '3' : (r"Pascal.jpg",        'Pascal L.'),
            '4' : (r"Guillaume.jpg",     'Guillaume'),
            '5' : (r"Pascal_B.jpg",      'Pascal B.'),
            '6' : (r"Guillaume2.jpg",    'Guillaume'),
            '7' : (r"P_Y.jpg",           'Pierre-Yves'), 

            } 

    if Orateur not in Dict :
        Orateur = '0'

    return Dict[Orateur]


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

    
class ApplicationProjetS5(tk.Frame):         


    def __init__(self, master=None):
        baurate             = 38400
        readingTimeout      = 1e-6
        readingUARTinterval = 1e-3

        tk.Frame.__init__(self, master)  

        self.buttonFrame = tk.Frame(self)
        self.bottomFrame = tk.Frame(self)

        self.pack()
        self.buttonFrame.pack(side='bottom')
        self.bottomFrame.pack(side='bottom')         
        
        self.createWidgets()

        #self.ser1 = self.setupSerialPort("COM2", baurate, readingTimeout)
        #self.ser2 = self.setupSerialPort("COM3", baurate, readingTimeout)      

        self.readingThread = RepeatedTimer(readingUARTinterval, self.readSerial)

        self.ser1 = self.setupSerialPort("\\\\.\\CNCA0", baurate, readingTimeout)
        self.ser2 = self.setupSerialPort("\\\\.\\CNCB0", baurate, readingTimeout)

    def createWidgets(self):


        self.orateurPicLabel = tk.Label(self)
        self.orateurPicLabel.pack()

        pathAndName = ImageDictionnary('0')
        photo = Image.open(pathAndName[0])
        photo = ImageTk.PhotoImage(photo)
        self.orateurPicLabel.configure(image=photo)
        self.orateurPicLabel.photo = photo

        self.orateurLabel = tk.Label(self, text='Orateur : Inconnu ')
        self.orateurLabel.pack()

        self.readingLabel = tk.Label(self, text='Statut du DSK : ')
        self.readingLabel.pack()

        self.writingSerial_B = tk.Button(self.buttonFrame, text='Write', command=lambda: self.writingSerialButton())
        self.writingSerial_B.pack(side='left')
        self.writingSerial_B.config(height = 3, width = 10)

        self.readSerial_B = tk.Button(self.buttonFrame,  text='Start', command=lambda: self.startThread(self.readingThread))
        self.readSerial_B.pack(side='left')
        self.readSerial_B.config( height = 3, width = 10 )

        self.stopReading_B = tk.Button(self.buttonFrame,  text='Stop', command=lambda: self.stopThread(self.readingThread))
        self.stopReading_B.pack(side='left')
        self.stopReading_B.config(height = 3, width = 10 )

        self.writingBox = tk.Entry(self.bottomFrame)
        self.writingBox.pack()
        #self.writingBox.grid(columnspan=1, row=1, column=6)


        #self.trainButton_B = tk.Button(self, text='Training', command=lambda: self.cycleImage())            
        #self.trainButton_B.grid(columnspan=1, row=3, column=1)



    def setupSerialPort(self, port, baudrate, timeout):
        ser = serial.Serial(port, baudrate, timeout=timeout)
        return ser

    def cycleImage(self, index):

        self.orateurIndex = index
        pathAndName = ImageDictionnary(index)
        photo = Image.open(pathAndName[0])
        photo = ImageTk.PhotoImage(photo)
        self.orateurPicLabel.configure(image=photo)
        self.orateurPicLabel.photo = photo

        return pathAndName

    def readSerial(self):
        data = self.ser2.read(9999)
        data = data.decode('ascii')

        if 'ind' in data :
            pathAndName = self.cycleImage(data[3:])
            self.orateurLabel.configure(text= 'Orateur : ' + pathAndName[1])
            self.orateurLabel.text = 'Orateur : ' + pathAndName[1]

        elif 'status' in  data : 
            self.readingLabel.configure(text= 'Statut du DSK : ' + data[6:])
            self.readingLabel.text = 'Statut du DSK : ' + data[6:]

        #Write what ever is sent
        #elif data != '':
        #    self.readingLabel.configure(text=data)
        #    self.readingLabel.text = data


    def writingSerial(self):
        data = self.writingBox.get()
        self.ser1.write(data.encode())


    def writingSerialButton(self):
        self.writingSerial()


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
MyRoot.geometry("%dx%d+0+0" % (w/2.8, h/1.7)) 

app.mainloop()
   
app.ser1.close()
app.ser2.close()