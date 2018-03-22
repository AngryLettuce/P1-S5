#Created by Guillaume Roux, 
#this project aim to make a serial communication with a PIC
# and to make a graphical interface showing the picture of someone


import tkinter as tk   
from PIL import Image, ImageTk
import os
import serial
from time import sleep
from threading import Timer


def ImageDictionnary(Orateur):
    #Picture size : 381 * 285 px

    DictB = { 0    : (r"noImage.jpg",       'Inconnu'),
              1    : (r"Antoine2.jpg",      'Antoine'),
              2    : (r"Pascal.PNG",        'Pascal L.'),
              4    : (r"Pascal_B.jpg",      'Pascal B.'),
              8    : (r"Guillaume.jpg",     'Guillaume'),
              16   : (r"Raphael.jpg",       'Raphael'),
              32   : (r"Thomas.jpg",        'Thomas'),
              64   : (r"P_Y.jpg",           'Pierre-Yves! :D'), 
              128  : (r"Jeff.jpg",          'Jeffrey F.'), 
              256  : (r"Vit Hess.jpg",      'Jeffrey R.'), 
              512  : (r"Chuck.jpg",         'Charles'), 
              1024 : (r"Gonzo.jpg",         'Cristhian'), 
              2048 : (r"L_P.jpg",           'Tatlock'), 
              4096 : (r"feu_serviette.jpg", 'RIP serviette'), 
              8192 : (r"butrice.jpg",       'Butrice'), 
             } 


    Dict = {'a'  : (r"noImage.jpg",      'Inconnu'),
            'b'  : (r"Antoine2.jpg",     'Antoine'),
            'c'  : (r"Pascal.PNG",       'Pascal L.'),
            'd'  : (r"Pascal_B.jpg",     'Pascal B.'),
            'e'  : (r"Guillaume.jpg",    'Guillaume'),
            'f'  : (r"Raphael.jpg",      'Raphael'),
            'g'  : (r"Thomas.jpg",       'Thomas'),
            'i'  : (r"P_Y.jpg",          'Pierre-Yves'), 
            'j'  : (r"Jeff.jpg",         'Jeffrey F.'), 
            'k'  : (r"Vit Hess.jpg",     'Jeffrey R.'), 
            'l' : (r"Chuck.jpg",         'Charles'), 
            'm' : (r"Gonzo.jpg",         'Cristhian'), 
            'n' : (r"L_P.jpg",           'Tatlock'), 
            'o' : (r"feu_serviette.jpg", 'RIP serviette'), 
            'p' : (r"butrice.jpg",       'Butrice'), 
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
        baurate             = 19200
        readingTimeout      = 1e-6
        readingUARTinterval = 1e-3

        self.orateurIndex = 0
        self.numberOfOrateur = 14

        tk.Frame.__init__(self, master)  

        self.buttonFrame = tk.Frame(self)
        self.bottomFrame = tk.Frame(self)

        self.pack()
        self.buttonFrame.pack(side='bottom')
        self.bottomFrame.pack(side='bottom')         
        
        self.createWidgets()

        #Virtual Serial Port for testing (desktop)
        #self.ser1 = self.setupSerialPort("COM2", baurate, readingTimeout)
        #self.ser2 = self.setupSerialPort("COM3", baurate, readingTimeout)  
        
        #Virtual Serial Port (laptop)
        #self.ser1 = self.setupSerialPort("\\\\.\\CNCA0", baurate, readingTimeout)
        #self.ser2 = self.setupSerialPort("\\\\.\\CNCB0", baurate, readingTimeout)

        #real serial port with the pic
        #self.realSerial = self.setupSerialPort("COM5", baurate, readingTimeout)  

        #self.readingThread    = RepeatedTimer(readingUARTinterval, self.readSerial, self.realSerial)
        #self.imageCycleThread = RepeatedTimer(2, self.cycleImage)

        #self.imageCycleThread.start()

    def createWidgets(self):

        self.orateurPicLabel = tk.Label(self)
        self.orateurPicLabel.pack()

        pathAndName = ImageDictionnary('a')
        photo = Image.open(pathAndName[0])
        photo = ImageTk.PhotoImage(photo)
        self.orateurPicLabel.configure(image=photo)
        self.orateurPicLabel.photo = photo

        self.orateurLabel = tk.Label(self, text='Orateur : Inconnu ')
        self.orateurLabel.pack()

        self.readingLabel = tk.Label(self, text='Statut du DSK : ')
        self.readingLabel.pack()

        self.writingSerial_B = tk.Button(self.buttonFrame, text='Write', command=lambda: self.writingSerialButton(self.realSerial))
        self.writingSerial_B.pack(side='left')
        self.writingSerial_B.config(height = 3, width = 10)

        self.readSerial_B = tk.Button(self.buttonFrame,  text='Start', command=lambda: self.startThread(self.readingThread))
        self.readSerial_B.pack(side='left')
        self.readSerial_B.config( height = 3, width = 10 )

        self.stopReading_B = tk.Button(self.buttonFrame,  text='Stop', command=lambda: self.stopThread(self.readingThread))
        self.stopReading_B.pack(side='left')
        self.stopReading_B.config(height = 3, width = 10 )

        #self.scalingOrateur = tk.Scale(self.bottomFrame, from_=0, to_=14, orient='horizontal', tickinterval=2, length=200, command=self.scalingOrateurFunction)
        #self.scalingOrateur.pack()

        self.writingBox = tk.Entry(self.bottomFrame)
        self.writingBox.pack()

        #self.trainButton_B = tk.Button(self, text='Training', command=lambda: self.cycleImage())            
        #self.trainButton_B.grid(columnspan=1, row=3, column=1)


    def setupSerialPort(self, port, baudrate, timeout):
        ser = serial.Serial(port, baudrate, timeout=timeout)
        return ser


    def changeImage(self, label, index):
        pathAndName = ImageDictionnary(index)
        path = pathAndName[0]

        photo = Image.open(path)
        photo = ImageTk.PhotoImage(photo)
        label.configure(image=photo)
        label.photo = photo


    def cycleImage(self):
        index = int(self.orateurIndex)
        
        index += 1
        if index > self.numberOfOrateur :
            index = 0
        self.orateurIndex = str(index)

        self.changeImage(self.orateurPicLabel, self.orateurIndex)

        pathAndName = ImageDictionnary(self.orateurIndex)
        self.changeLabelText(self.orateurLabel, 'Orateur : ' + pathAndName[1])        


    def scalingOrateurFunction(self, yo ):
        index = self.scalingOrateur.get()
        self.changeImage(self.orateurPicLabel, str(index))


    def changeLabelText(self, label, data) : 
        'change the text of a label to data'
        label.configure(text=data)
        label.text = data


    def readSerial(self, serialPort):
        data = serialPort.read(9999)
        data = data.decode('ascii')

        if 'i' in data :
            index = data[1:]
            pathAndName = ImageDictionnary(index)
            self.changeImage(self.orateurPicLabel, index)
            self.changeLabelText(self.orateurLabel, 'Orateur : ' + pathAndName[1])

        elif 'status' in  data : 
            self.changeLabelText(self.readingLabel, 'Statut du DSK : ' + data[6:])

        #Write what ever is sent
        if data != '':
            self.readingLabel.configure(text=data)
            self.readingLabel.text = data
            self.changeImage(self.orateurPicLabel, data)


    def writingSerial(self, serialPort):
        data = self.writingBox.get()
        serialPort.write(data.encode())


    def writingSerialButton(self, serialPort):
        self.writingSerial(serialPort)


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
   
#app.ser1.close()
#app.ser2.close()