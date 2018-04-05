#Created by Guillaume Roux, 
'''This project aim to make a serial communication with a PIC
   and to make a graphical interface showing the picture of someone'''


import tkinter as tk   
import tkinter.messagebox as tkmessageBox
import functions as fn 

    
class ApplicationProjetS5(tk.Frame):         
    'Main object of the GUI'
    def __init__(self, master=None):
        baurate             = 19200
        readingTimeout      = 1e-6
        readingUARTinterval = 1e-3

        self.visible = False

        tk.Frame.__init__(self, master)  

        self.topFrame    = tk.Frame(self)
        self.buttonFrame = tk.Frame(self)
        self.midFrame    = tk.Frame(self)


        self.OrateurFrame1 = tk.Frame(self)
        self.OrateurFrame2 = tk.Frame(self)
        self.OrateurFrame3 = tk.Frame(self)
        self.OrateurFrame4 = tk.Frame(self)

        self.OrateurFrame = [self.OrateurFrame1, self.OrateurFrame2, self.OrateurFrame3, self.OrateurFrame4]

        self.pack()
        self.topFrame.pack(side='top')
        self.midFrame.pack(side='top')      
        self.buttonFrame.pack(side='top')

        for frame in self.OrateurFrame :
            frame.pack(side='top')
            fn.setInvisible(frame)

        
        self.createWidgets()

        #Virtual Serial Port for testing (desktop)
        #self.ser1 = fn.setupSerialPort("COM2", baurate, readingTimeout)
        #self.ser2 = fn.setupSerialPort("COM3", baurate, readingTimeout)  
        
        #Virtual Serial Port (laptop)
        self.ser1 = fn.setupSerialPort("\\\\.\\CNCA0", baurate, readingTimeout)
        self.ser2 = fn.setupSerialPort("\\\\.\\CNCB0", baurate, readingTimeout)

        #real serial port with the pic
        #self.realSerial = fn.setupSerialPort("COM6", baurate, readingTimeout)  

        self.readingThread    = fn.RepeatedTimer(readingUARTinterval, self.readingThread, self.ser1)
        self.readingThread.start()

        #self.imageCycleThread = fn.RepeatedTimer(2, self.cycleImage)
        #self.imageCycleThread.start() 

    def createWidgets(self):

        self.orateurPicLabel = tk.Label(self.topFrame)
        self.orateurPicLabel.pack()

        pathAndName = fn.imageDictionnary(63)
        fn.changeImage(self.orateurPicLabel, pathAndName[0])

        self.orateurLabel = tk.Label(self.topFrame, text='Orateur : Inconnu ')
        self.orateurLabel.pack()

        self.dskStatusLabel = tk.Label(self.topFrame, text='Statut du DSK : Inconnu')
        self.dskStatusLabel.pack()

        #self.writingSerial_B = tk.Button(self.buttonFrame, text='Write', command=lambda: self.writingSerialButton(self.ser2))
        #self.writingSerial_B.pack(side='left')
        #self.writingSerial_B.config(height = 3, width = 10)

        self.trainButton_B = tk.Button(self.buttonFrame, text='Train', command=lambda: self.trainRoutine())            
        self.trainButton_B.pack(side='left')
        self.trainButton_B.config(height = 3, width = 10 )

        self.readSerial_B = tk.Button(self.buttonFrame,  text='Start',   command=lambda: fn.startThread(self.readingThread))
        self.readSerial_B.pack(side='left')
        self.readSerial_B.config( height = 3, width = 10 )

        self.stopReading_B = tk.Button(self.buttonFrame,  text='Stop',   command=lambda: fn.stopThread(self.readingThread))
        self.stopReading_B.pack(side='left')
        self.stopReading_B.config(height = 3, width = 10 )

        self.writingBox = tk.Entry(self.midFrame)
        self.writingBox.bind('<Return>', lambda x: self.writingSerialButton(self.ser2))
        self.writingBox.pack()

        self.orateurButtons()


    def orateurButtons(self):

        index = 0
        frame = 0

        ## ------------------ First line ------------------ ##
        pathAndName = fn.imageDictionnary(index)
        self.orateurButtons1 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: fn.changeOrateurRoutine(0, self))
        self.orateurButtons1.pack(side='left')
        self.orateurButtons1.config(height = 1, width = 10 )
        index += 1

        pathAndName = fn.imageDictionnary(index)
        self.orateurButtons2 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: fn.changeOrateurRoutine(1, self))
        self.orateurButtons2.pack(side='left')
        self.orateurButtons2.config(height = 1, width = 10 )
        index += 1

        pathAndName = fn.imageDictionnary(index)
        self.orateurButtons3 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: fn.changeOrateurRoutine(2, self))
        self.orateurButtons3.pack(side='left')
        self.orateurButtons3.config(height = 1, width = 10 )
        index += 1

        pathAndName = fn.imageDictionnary(index)
        self.orateurButtons4 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: fn.changeOrateurRoutine(3, self))
        self.orateurButtons4.pack(side='left')
        self.orateurButtons4.config(height = 1, width = 10 )
        index += 1
        
        frame += 1

        ## ------------------ Second line ------------------ ##
        pathAndName = fn.imageDictionnary(index)
        self.orateurButtons5 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: fn.changeOrateurRoutine(4, self))
        self.orateurButtons5.pack(side='left')
        self.orateurButtons5.config(height = 1, width = 10 )
        index += 1

        pathAndName = fn.imageDictionnary(index)
        self.orateurButtons6 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: fn.changeOrateurRoutine(5, self))
        self.orateurButtons6.pack(side='left')
        self.orateurButtons6.config(height = 1, width = 10 )
        index += 1

        pathAndName = fn.imageDictionnary(index)
        self.orateurButtons7 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: fn.changeOrateurRoutine(6, self))
        self.orateurButtons7.pack(side='left')
        self.orateurButtons7.config(height = 1, width = 10 )
        index += 1

        pathAndName = fn.imageDictionnary(index)
        self.orateurButtons8 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: fn.changeOrateurRoutine(7, self))
        self.orateurButtons8.pack(side='left')
        self.orateurButtons8.config(height = 1, width = 10 )
        index += 1

        frame += 1

        ## ------------------ Third line ------------------ ##
        pathAndName = fn.imageDictionnary(index)
        self.orateurButtons9 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: fn.changeOrateurRoutine(8, self))
        self.orateurButtons9.pack(side='left')
        self.orateurButtons9.config(height = 1, width = 10 )
        index += 1

        pathAndName = fn.imageDictionnary(index)
        self.orateurButtons10 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: fn.changeOrateurRoutine(9, self))
        self.orateurButtons10.pack(side='left')
        self.orateurButtons10.config(height = 1, width = 10 )
        index += 1

        pathAndName = fn.imageDictionnary(index)
        self.orateurButtons11 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: fn.changeOrateurRoutine(10, self))
        self.orateurButtons11.pack(side='left')
        self.orateurButtons11.config(height = 1, width = 10 )
        index += 1

        #pathAndName = fn.imageDictionnary(index)
        #self.orateurButtons12 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: fn.changeOrateurRoutine(11, self))
        #self.orateurButtons12.pack(side='left')
        #self.orateurButtons12.config(height = 1, width = 10 )
        #index += 1

        frame += 1

        ## ------------------ Fourth line ------------------ ##
        #pathAndName = fn.imageDictionnary(index)
        #self.orateurButtons13 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: fn.changeOrateurRoutine(12, self))
        #self.orateurButtons13.pack(side='left')
        #self.orateurButtons13.config(height = 1, width = 10 )
        #index += 1

        #pathAndName = fn.imageDictionnary(index)
        #self.orateurButtons14 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: fn.changeOrateurRoutine(13, self))
        #self.orateurButtons14.pack(side='left')
        #self.orateurButtons14.config(height = 1, width = 10 )
        #index += 1

        #pathAndName = fn.imageDictionnary(index)
        #self.orateurButtons15 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: fn.changeOrateurRoutine(14, self))
        #self.orateurButtons15.pack(side='left')
        #self.orateurButtons15.config(height = 1, width = 10 )
        #index += 1

        #pathAndName = fn.imageDictionnary(index)
        #self.orateurButtons16 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: fn.changeOrateurRoutine(15, self))
        #self.orateurButtons16.pack(side='left')
        #self.orateurButtons16.config(height = 1, width = 10 )
        #index += 1


    def cycleImage(self):
        index = int(self.orateurIndex)
        
        index += 1
        if index > self.numberOfOrateur :
            index = 0

        self.orateurIndex = str(index)
        fn.changeImage(self.orateurPicLabel, self.orateurIndex)

        pathAndName = ImageDictionnary(self.orateurIndex)
        fn.changeLabelText(self.orateurLabel, 'Orateur : ' + pathAndName[1])        


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


    def readingThread(self, serialPort):
       data =  fn.readSerial(serialPort)
       if data != b'' : 
            data = int.from_bytes(data, 'big')
   
            #fn._2x8bitsRead(data, self)
            fn._8bitsRead(data, self)


    def trainRoutine(self):

        if self.visible : 
            for frame in self.OrateurFrame : 
                 fn.setInvisible(frame)
            self.visible = False

                
        else :
            for frame in self.OrateurFrame : 
                fn.setVisible(frame)
            self.visible = True

                


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
   
