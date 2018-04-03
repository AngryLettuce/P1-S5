#Created by Guillaume Roux, 
#this project aim to make a serial communication with a PIC
# and to make a graphical interface showing the picture of someone


import tkinter as tk   
import tkinter.messagebox as tkmessageBox
import functions as fn 

    
class ApplicationProjetS5(tk.Frame):         
    'Main object of the GUI'
    def __init__(self, master=None):
        baurate             = 19200
        readingTimeout      = 1e-6
        readingUARTinterval = 1e-3

        tk.Frame.__init__(self, master)  

        self.buttonFrame = tk.Frame(self)
        self.midFrame = tk.Frame(self)

        self.pack()
        self.buttonFrame.pack(side='bottom')
        self.midFrame.pack(side='bottom')         
        
        self.createWidgets()

        #Virtual Serial Port for testing (desktop)
        #self.ser1 = fn.setupSerialPort("COM2", baurate, readingTimeout)
        #self.ser2 = fn.setupSerialPort("COM3", baurate, readingTimeout)  
        
        #Virtual Serial Port (laptop)
        #self.ser1 = fn.setupSerialPort("\\\\.\\CNCA0", baurate, readingTimeout)
        #self.ser2 = fn.setupSerialPort("\\\\.\\CNCB0", baurate, readingTimeout)

        #real serial port with the pic
        self.realSerial = fn.setupSerialPort("COM5", baurate, readingTimeout)  

        self.readingThread    = fn.RepeatedTimer(readingUARTinterval, self.readSerial, self.realSerial)
        self.readingThread.start()

        #self.imageCycleThread = fn.RepeatedTimer(2, self.cycleImage)
        #self.imageCycleThread.start()

    def createWidgets(self):

        self.orateurPicLabel = tk.Label(self)
        self.orateurPicLabel.pack()

        pathAndName = fn.imageDictionnary(63)
        fn.changeImage(self.orateurPicLabel, pathAndName[0])

        self.orateurLabel = tk.Label(self, text='Orateur : Inconnu ')
        self.orateurLabel.pack()

        self.dskStatusLabel = tk.Label(self, text='Statut du DSK : Inconnu')
        self.dskStatusLabel.pack()

        self.writingSerial_B = tk.Button(self.buttonFrame, text='Write', command=lambda: self.writingSerialButton(self.realSerial))
        self.writingSerial_B.pack(side='left')
        self.writingSerial_B.config(height = 3, width = 10)

        self.readSerial_B = tk.Button(self.buttonFrame,  text='Start',   command=lambda: fn.startThread(self.readingThread))
        self.readSerial_B.pack(side='left')
        self.readSerial_B.config( height = 3, width = 10 )

        self.stopReading_B = tk.Button(self.buttonFrame,  text='Stop',   command=lambda: fn.stopThread(self.readingThread))
        self.stopReading_B.pack(side='left')
        self.stopReading_B.config(height = 3, width = 10 )

        #self.scalingOrateur = tk.Scale(self.midFrame, from_=0, to_=14, orient='horizontal', tickinterval=2, length=200, command= lambda x:self.scalingOrateurFunction)
        #self.scalingOrateur.pack()

        self.writingBox = tk.Entry(self.midFrame)
        self.writingBox.bind('<Return>', lambda x: self.writingSerialButton(self.realSerial))
        self.writingBox.pack()

        #self.trainButton_B = tk.Button(self, text='Training', command=lambda: self.cycleImage())            
        #self.trainButton_B.grid(columnspan=1, row=3, column=1)


    def scalingOrateurFunction(self):
        return


    def cycleImage(self):
        index = int(self.orateurIndex)
        
        index += 1
        if index > self.numberOfOrateur :
            index = 0

        self.orateurIndex = str(index)
        fn.changeImage(self.orateurPicLabel, self.orateurIndex)

        pathAndName = ImageDictionnary(self.orateurIndex)
        fn.changeLabelText(self.orateurLabel, 'Orateur : ' + pathAndName[1])        


    def readSerial(self, serialPort):
        data = serialPort.read(9999)
        #data = data.decode('ascii')

        if data != b'' : 
            print(data)
            data = int.from_bytes(data, 'big')

            #Set the orateur picture and the name bellow 
            if data >= 192:
                data -= 192 
                pathAndName = fn.imageDictionnary(data)
                fn.changeImage(self.orateurPicLabel, pathAndName[0])
                fn.changeLabelText(self.orateurLabel, 'Orateur : ' + pathAndName[1])

            #Set the dsk status label 
            elif data < 192 and data >= 128 : 
                data -= 128
                status = fn.dskStatusDictionnary(data)
                fn.changeLabelText(self.dskStatusLabel, 'Statut du DSK : ' + status)


    def writingSerialButton(self, serialPort):
        data = self.writingBox.get()
        if data != '':
            if fn.check_int(data) : 
                data = int(data)
                #saturate the byte for the next operation
                if data >= 256 : 
                    data = 255
                data = data.to_bytes(1, 'big')
                fn.writingSerial(serialPort, data)

            else : 
                self.errorMessageBox('Please enter an integer')



    def errorMessageBox(self, message):
        tkmessageBox.showinfo("Error", message)


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
   
