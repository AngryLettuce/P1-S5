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

        self.train = False
        self.trainButtonPressed   = False
        self.orateurButtonPressed = 0
        self.orateurInDiscussion  = 0
        self.start = False
        self.startInit = False
        self.lastCommand = None
        self.confirmOrateur = False
        self.MaxOrateur = fn.imageDictionnary(0, True) -1

        self.buttonList = []
        
        tk.Frame.__init__(self, master)  

        self.topFrame     = tk.Frame(self)
        self.buttonFrame  = tk.Frame(self)
        self.midFrame     = tk.Frame(self)
        self.labelFrame   = tk.Frame(self)
        self.scalingFrame = tk.Frame(self)
    

        self.OrateurFrame1 = tk.Frame(self)
        self.OrateurFrame2 = tk.Frame(self)
        self.OrateurFrame3 = tk.Frame(self)
        self.OrateurFrame4 = tk.Frame(self)

        self.OrateurFrame = [self.OrateurFrame1, self.OrateurFrame2, self.OrateurFrame3, self.OrateurFrame4]

        self.pack()
        self.topFrame.pack(side='top')
        self.midFrame.pack(side='top')      
        self.buttonFrame.pack(side='top')
        self.labelFrame.pack(side='top')
        self.scalingFrame.pack(side='top')

        fn.setInvisible(self.labelFrame)
        fn.setInvisible(self.scalingFrame)

        for frame in self.OrateurFrame :
            frame.pack(side='top')
            fn.setInvisible(frame)

        
        self.createWidgets()

        #Virtual Serial Port for testing (desktop)
        #self.ser1 = fn.setupSerialPort("COM2", baurate, readingTimeout)
        #self.ser2 = fn.setupSerialPort("COM3", baurate, readingTimeout)  
        
        #Virtual Serial Port (laptop)
        #self.ser1 = fn.setupSerialPort("\\\\.\\CNCA0", baurate, readingTimeout)
        #self.ser2 = fn.setupSerialPort("\\\\.\\CNCB0", baurate, readingTimeout)

        #real serial port with the pic
        self.realSerial = fn.setupSerialPort("COM6", baurate, readingTimeout)  

        self.readingThread    = fn.RepeatedTimer(readingUARTinterval, self.readingThread, self.realSerial)
        self.readingThread.start()


    def createWidgets(self):
        '''Create all the widgets for the GUI but the grid of orateur Buttons'''
        self.orateurPicLabel = tk.Label(self.topFrame)
        self.orateurPicLabel.pack()

        pathAndName = fn.imageDictionnary(self.MaxOrateur)
        fn.changeImage(self.orateurPicLabel, pathAndName[0])

        self.orateurLabel = tk.Label(self.topFrame, text='Orateur : Inconnu ')
        self.orateurLabel.pack()

        self.dskStatusLabel = tk.Label(self.topFrame, text='Statut du DSK : Inconnu')
        self.dskStatusLabel.pack()

        self.scalingOrateur = tk.Scale(self.scalingFrame, from_=2, to_=self.MaxOrateur, orient='horizontal', 
                                       tickinterval=1, length=300)
        self.scalingOrateur.pack(side='left')

        self.confirmOrateur_B = tk.Button(self.scalingFrame, text='Confirm', command= lambda: self.confirmOrateurFunction())
        self.confirmOrateur_B.pack(side='left')

        self.appMessageLabel = tk.Label(self.labelFrame)
        self.appMessageLabel.pack()
        self.appMessageLabel.config(font=("Courier", 20), width=200)

        self.trainButton_B = tk.Button(self.buttonFrame, text='Train', command=lambda: self.trainRoutine())            
        self.trainButton_B.pack(side='left')
        self.trainButton_B.config(height = 3, width = 10 )

        self.start_B = tk.Button(self.buttonFrame,  text='Start',   command=lambda: self.startRoutine())
        self.start_B.pack(side='left')
        self.start_B.config( height = 3, width = 10 )

        self.stopReading_B = tk.Button(self.buttonFrame,  text='Stop',   command=lambda: self.backToInit())
        self.stopReading_B.pack(side='left')
        self.stopReading_B.config(height = 3, width = 10 )

        #self.writingBox = tk.Entry(self.midFrame)
        #self.writingBox.bind('<Return>', lambda x: self.writingSerialButton(self.ser2))
        #self.writingBox.pack()
        #self.writingBox.config(width=30, justify='center')

        self.orateurButtons()  


    #def writingSerialButton(self, serialPort):
    #    data = self.writingBox.get()
    #    if data != '':
    #        if fn.check_int(data) : 
    #            data = int(data)
    #            #saturate the byte for the next operation
    #            if data >= 256 : 
    #                data = 255
    #            data = data.to_bytes(1, 'big')
    #            fn.writingSerial(serialPort, data)

    #        else : 
    #            self.errorMessageBox('Please enter an integer')


    def readingThread(self, serialPort):
        '''read on the serial port and decode the command into an index and a status'''
        data =  fn.readSerial(serialPort)
        if data != b'' : 
            self.lastCommand = int.from_bytes(data, 'big')

            fn._8bitsRead(self.lastCommand, self)


    def trainRoutine(self):
        if not self.start and not self.startInit: 
            self.trainButton_B.config(relief=tk.SUNKEN)

            fn.changeLabelText(self.appMessageLabel, "Selectionnez l'orateur à entrainer")
            fn.setVisible(self.labelFrame)

            for frame in self.OrateurFrame : 
                fn.setVisible(frame)

            self.train = True


    def startRoutine(self):
        if not self.train :
            fn.setVisible(self.labelFrame)
            fn.setVisible(self.scalingFrame)
            fn.changeLabelText(self.appMessageLabel, "Sélectionnez le nombre d'orateur dans la discussion")
            self.startInit =  True

        self.orateurInDiscussion = self.scalingOrateur.get()

        if self.confirmOrateur:
            fn.writingSerial(self.realSerial, (self.orateurInDiscussion << 4) + 2) #send test init command + number of speaker
            self.start = True
            self.start_B.config(relief=tk.SUNKEN)
            fn.changeLabelText(self.appMessageLabel, "Sélectionner les orateurs dans la discussion")
            fn.setInvisible(self.scalingFrame)

            for frame in self.OrateurFrame : 
                fn.setVisible(frame)
                



    def orateurButton(self, index):
        button = self.buttonList[index]
        pathAndName = fn.imageDictionnary(index)
        if self.train : 
            if not self.trainButtonPressed : 
                fn.changeOrateur(index, self)
                fn.changeLabelText(self.appMessageLabel, "Entrainement de " + pathAndName[1])
                button.config(relief=tk.SUNKEN)
                self.trainButtonPressed = True
                fn.writingSerial(self.realSerial, (index << 4) + 4)


        elif self.start : 
            if self.orateurButtonPressed < self.orateurInDiscussion : 
                fn.changeOrateur(index, self)
                fn.changeLabelText(self.appMessageLabel, "Ajout de " + pathAndName[1] +  " à la discussion")
                button.config(relief=tk.SUNKEN)
                fn.writingSerial(self.realSerial, (index << 4) + 2)

                self.orateurButtonPressed += 1 



    def confirmOrateurFunction(self):
        self.confirmOrateur = True
        self.startRoutine()
 


    def backToInit(self) : 
        '''Revert the status of the GUI to the initial one'''
        
        fn.writingSerial(self.realSerial, 255)  #send IDLE command
        self.train = False
        self.trainButtonPressed = False
        self.start = False
        self.startInit = False
        self.confirmOrateur = False
        self.orateurButtonPressed = 0

        self.trainButton_B.config(relief=tk.RAISED)
        self.start_B.config(relief=tk.RAISED)

        self.scalingOrateur.set(0)

        for button in self.buttonList :
            button.config(relief=tk.RAISED)

        fn.setInvisible(self.labelFrame)
        fn.setInvisible(self.scalingFrame)

        for frame in self.OrateurFrame : 
            fn.setInvisible(frame)

        #self.writingBox.delete(0, tk.END)

        fn.changeOrateur(63, self)
        fn.changeDSKStatus(63, self)


    def orateurButtons(self):
        index = 0
        frame = 0

        ## ------------------ First line ------------------ ##
        pathAndName = fn.imageDictionnary(index)
        self.orateurButtons1 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: self.orateurButton(0))
        self.orateurButtons1.pack(side='left')
        self.orateurButtons1.config(height = 1, width = 10 )
        self.buttonList.append(self.orateurButtons1)
        index += 1

        pathAndName = fn.imageDictionnary(index)
        self.orateurButtons2 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: self.orateurButton(1))
        self.orateurButtons2.pack(side='left')
        self.orateurButtons2.config(height = 1, width = 10 )
        self.buttonList.append(self.orateurButtons2)
        index += 1

        pathAndName = fn.imageDictionnary(index)
        self.orateurButtons3 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: self.orateurButton(2))
        self.orateurButtons3.pack(side='left')
        self.orateurButtons3.config(height = 1, width = 10 )
        self.buttonList.append(self.orateurButtons3)
        index += 1

        pathAndName = fn.imageDictionnary(index)
        self.orateurButtons4 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: self.orateurButton(3))
        self.orateurButtons4.pack(side='left')
        self.orateurButtons4.config(height = 1, width = 10 )
        self.buttonList.append(self.orateurButtons4)
        index += 1
        
        frame += 1

        ## ------------------ Second line ------------------ ##
        pathAndName = fn.imageDictionnary(index)
        self.orateurButtons5 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: self.orateurButton(4))
        self.orateurButtons5.pack(side='left')
        self.orateurButtons5.config(height = 1, width = 10 )
        self.buttonList.append(self.orateurButtons5)
        index += 1

        pathAndName = fn.imageDictionnary(index)
        self.orateurButtons6 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: self.orateurButton(5))
        self.orateurButtons6.pack(side='left')
        self.orateurButtons6.config(height = 1, width = 10 )
        self.buttonList.append(self.orateurButtons6)
        index += 1

        pathAndName = fn.imageDictionnary(index)
        self.orateurButtons7 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: self.orateurButton(6))
        self.orateurButtons7.pack(side='left')
        self.orateurButtons7.config(height = 1, width = 10 )
        self.buttonList.append(self.orateurButtons7)
        index += 1

        pathAndName = fn.imageDictionnary(index)
        self.orateurButtons8 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: self.orateurButton(7))
        self.orateurButtons8.pack(side='left')
        self.orateurButtons8.config(height = 1, width = 10 )
        self.buttonList.append(self.orateurButtons8)
        index += 1

        frame += 1

        ## ------------------ Third line ------------------ ##
        pathAndName = fn.imageDictionnary(index)
        self.orateurButtons9 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: self.orateurButton(8))
        self.orateurButtons9.pack(side='left')
        self.orateurButtons9.config(height = 1, width = 10 )
        self.buttonList.append(self.orateurButtons9)
        index += 1

        pathAndName = fn.imageDictionnary(index)
        self.orateurButtons10 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: self.orateurButton(9))
        self.orateurButtons10.pack(side='left')
        self.orateurButtons10.config(height = 1, width = 10 )
        self.buttonList.append(self.orateurButtons10)
        index += 1

        pathAndName = fn.imageDictionnary(index)
        self.orateurButtons11 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: self.orateurButton(10))
        self.orateurButtons11.pack(side='left')
        self.orateurButtons11.config(height = 1, width = 10 )
        self.buttonList.append(self.orateurButtons11)
        index += 1

        #pathAndName = fn.imageDictionnary(index)
        #self.orateurButtons12 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: self.orateurButton(11))
        #self.orateurButtons12.pack(side='left')
        #self.orateurButtons12.config(height = 1, width = 10 )
        #self.buttonList.append(self.orateurButtons12)
        #index += 1

        frame += 1

        ## ------------------ Fourth line ------------------ ##
        #pathAndName = fn.imageDictionnary(index)
        #self.orateurButtons13 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: self.orateurButton(12))
        #self.orateurButtons13.pack(side='left')
        #self.orateurButtons13.config(height = 1, width = 10 )
        #self.buttonList.append(self.orateurButtons13)
        #index += 1

        #pathAndName = fn.imageDictionnary(index)
        #self.orateurButtons14 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: self.orateurButton(13))
        #self.orateurButtons14.pack(side='left')
        #self.orateurButtons14.config(height = 1, width = 10 )
        #self.buttonList.append(self.orateurButtons14)
        #index += 1

        #pathAndName = fn.imageDictionnary(index)
        #self.orateurButtons15 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: self.orateurButton(14))
        #self.orateurButtons15.pack(side='left')
        #self.orateurButtons15.config(height = 1, width = 10 )
        #self.buttonList.append(self.orateurButtons15)
        #index += 1

        #pathAndName = fn.imageDictionnary(index)
        #self.orateurButtons16 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: self.orateurButton(15))
        #self.orateurButtons16.pack(side='left')
        #self.orateurButtons16.config(height = 1, width = 10 )
        #self.buttonList.append(self.orateurButtons16)
        #index += 1
                


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
   
