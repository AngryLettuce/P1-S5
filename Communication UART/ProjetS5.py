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
        
        self.typeOfSpeaker = fn.appStyle.humain # Set it to True for the animal application (Warning use trainning with caution in this mode)
       
        
        self.train = False
        self.trainButtonPressed   = False
        self.orateurButtonPressed = []
        self.orateurInDiscussion  = 0
        self.start = False
        self.startInit = False
        self.OngoinConv = False
        self.lastCommand = None
        self.confirmOrateur = False
        self.MaxOrateur = fn.imageDictionnary(0, getlength=True) -1

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
        self.realSerial = fn.setupSerialPort("COM2", baurate, readingTimeout)
        self.ser2 = fn.setupSerialPort("COM3", baurate, readingTimeout)  
        
        #Virtual Serial Port (laptop)
        #self.ser1 = fn.setupSerialPort("\\\\.\\CNCA0", baurate, readingTimeout)
        #self.ser2 = fn.setupSerialPort("\\\\.\\CNCB0", baurate, readingTimeout)

        #real serial port with the pic
        #self.realSerial = fn.setupSerialPort("COM6", baurate, readingTimeout)  

        self.readingThread    = fn.RepeatedTimer(readingUARTinterval, self.readingThread, self.ser2)
        self.readingThread.start()


    def createWidgets(self):
        '''Create all the widgets for the GUI but the grid of orateur Buttons'''
        self.orateurPicLabel = tk.Label(self.topFrame)
        self.orateurPicLabel.pack()

        pathAndName = fn.imageDictionnary(self.MaxOrateur, speakerType=self.typeOfSpeaker)
        fn.changeImage(self.orateurPicLabel, pathAndName[0])

        self.orateurLabel = tk.Label(self.topFrame, text='Speaker : Unknown')
        self.orateurLabel.pack()
        self.orateurLabel.config(font=("Open Sans", 12), width=200)

        self.dskStatusLabel = tk.Label(self.topFrame, text='Unknown')
        self.dskStatusLabel.pack()
        self.dskStatusLabel.config(font=("Open Sans", 12), width=200)


        self.scalingOrateur = tk.Scale(self.scalingFrame, from_=2, to_=self.MaxOrateur, orient='horizontal', 
                                       tickinterval=1, length=300)
        self.scalingOrateur.pack(side='left')

        self.confirmOrateur_B = tk.Button(self.scalingFrame, text='Confirm', command=lambda: self.confirmOrateurFunction())
        self.confirmOrateur_B.pack(side='left')

        self.appMessageLabel = tk.Label(self.labelFrame)
        self.appMessageLabel.pack()
        self.appMessageLabel.config(font=("Open Sans", 20), width=200)

        self.trainButton_B = tk.Button(self.buttonFrame, text='Train', command=lambda: self.trainRoutine())            
        self.trainButton_B.pack(side='left')
        self.trainButton_B.config(height = 3, width = 10 )

        self.start_B = tk.Button(self.buttonFrame,  text='Start',   command=lambda: self.startRoutine())
        self.start_B.pack(side='left')
        self.start_B.config( height = 3, width = 10 )

        self.stopReading_B = tk.Button(self.buttonFrame,  text='Stop',   command=lambda: self.backToInit())
        self.stopReading_B.pack(side='left')
        self.stopReading_B.config(height = 3, width = 10 )



        self.popupMenu = tk.Menu(self, tearoff=0)
        self.popupMenu.add_command(label="Animal",   command=lambda: self.changeApp(True))
        self.popupMenu.add_command(label="Humanoid", command=lambda: self.changeApp(False))


        self.orateurButtons()  



    def readingThread(self, serialPort):
        '''read on the serial port and decode the command into an index and a status'''
        data =  fn.readSerial(serialPort)
        if data != b'' :#and data != b'\x00' : 
            print(data)
            currentCommand = int.from_bytes(data, 'big')
            currentCommand -= 16 #patch pour protection contre les données 
            fn._8bitsRead(currentCommand, self)
            if currentCommand & 0x0F == 3 and not self.OngoinConv:
                fn.changeLabelText(self.appMessageLabel, "Conversation en cours...")
                self.OngoinConv = True




    def trainRoutine(self):
        if not self.start: 
            if self.startInit :
                fn.setInvisible(self.scalingFrame)
            self.trainButton_B.config(relief=tk.SUNKEN)

            fn.changeLabelText(self.appMessageLabel, "Selectionnez l'orateur à entrainer")
            fn.setVisible(self.labelFrame)

            for frame in self.OrateurFrame : 
                fn.setVisible(frame)

            self.train = True


    def startRoutine(self):
        if not self.trainButtonPressed :
            if self.train : 
                for frame in self.OrateurFrame : 
                    fn.setInvisible(frame)
                    self.trainButton_B.config(relief=tk.RAISED)

            fn.setVisible(self.labelFrame)
            fn.setVisible(self.scalingFrame)
            fn.changeLabelText(self.appMessageLabel, "Sélectionnez le nombre d'orateurs dans la discussion")
            self.startInit =  True

        self.orateurInDiscussion = self.scalingOrateur.get()

        if self.confirmOrateur:
            fn.writingSerial(self.realSerial, (self.orateurInDiscussion << 4) + fn.command.test_init) #send test init command + number of speaker
            self.start = True
            self.start_B.config(relief=tk.SUNKEN)
            fn.changeLabelText(self.appMessageLabel, "Sélectionnez les orateurs dans la discussion")
            fn.setInvisible(self.scalingFrame)

            for frame in self.OrateurFrame : 
                fn.setVisible(frame)
                

    def orateurButton(self, index):
        button = self.buttonList[index]
        pathAndName = fn.imageDictionnary(index, speakerType=self.typeOfSpeaker)
        if self.train : 
            if not self.trainButtonPressed : 
                fn.writingSerial(self.realSerial, (index << 4) + fn.command.train_init) 
                fn.changeLabelText(self.appMessageLabel, "Entrainement de " + pathAndName[1])
                button.config(relief=tk.SUNKEN)
                self.trainButtonPressed = True


        elif self.start : 
            if (len(self.orateurButtonPressed) < self.orateurInDiscussion) and (button not in self.orateurButtonPressed): 
                fn.writingSerial(self.realSerial, (index << 4) + fn.command.test_init)
                fn.changeLabelText(self.appMessageLabel, "Ajout de " + pathAndName[1] +  " à la discussion")
                button.config(relief=tk.SUNKEN)

                self.orateurButtonPressed.append(button)


    def confirmOrateurFunction(self):
        self.confirmOrateur = True
        self.startRoutine()
 

    def backToInit(self) : 
        '''Revert the status of the GUI to the initial one'''
        
        fn.writingSerial(self.realSerial, fn.command.IDLE)  #send IDLE command

        self.train = False
        self.trainButtonPressed = False
        self.start = False
        self.startInit = False
        self.OngoinConv = False
        self.confirmOrateur = False

        self.orateurButtonPressed = []

        self.trainButton_B.config(relief=tk.RAISED)
        self.start_B.config(relief=tk.RAISED)

        self.scalingOrateur.set(0)

        for button in self.buttonList :
            button.config(relief=tk.RAISED)

        fn.setInvisible(self.labelFrame)
        fn.setInvisible(self.scalingFrame)

        for frame in self.OrateurFrame : 
            fn.setInvisible(frame)


        fn.changeOrateur(14, self)
        fn.changeDSKStatus(14, self)


    def changeApp(self, appStyleEnum):
        if not self.startInit and not self.train : 
            if appStyleEnum == fn.appStyle.animal : 
                fn.writingSerial(self.realSerial, (0xF << 4) + fn.command.animalApp) # Send the command to switch to animal app

            elif appStyleEnum == fn.appStyle.humain : 
                fn.writingSerial(self.realSerial, (0xF << 4) + fn.command.humainApp) # Send the command to switch to humain app

            elif appStyleEnum == fn.appStyle.phoneme : 
                fn.writingSerial(self.realSerial, (0xF << 4) + fn.command.phonemeApp) # Send the command to switch to humain app

            self.typeOfSpeaker = appStyleEnum
            self.configButtonsText()

    
    def configButtonsText(self) : 
        index = 0 
        for button in self.buttonList : 
            pathAndName = fn.imageDictionnary(index, speakerType=self.typeOfSpeaker)

            fn.changeLabelText(button, pathAndName[1])

            index += 1


    def orateurButtons(self):
        index = 0
        frame = 0

        ## ------------------ First line ------------------ ##
        pathAndName = fn.imageDictionnary(index, speakerType=self.typeOfSpeaker)
        self.orateurButtons1 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: self.orateurButton(0))
        self.orateurButtons1.pack(side='left')
        self.orateurButtons1.config(height = 1, width = 10 )
        self.buttonList.append(self.orateurButtons1)
        index += 1

        pathAndName = fn.imageDictionnary(index, speakerType=self.typeOfSpeaker)
        self.orateurButtons2 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: self.orateurButton(1))
        self.orateurButtons2.pack(side='left')
        self.orateurButtons2.config(height = 1, width = 10 )
        self.buttonList.append(self.orateurButtons2)
        index += 1

        pathAndName = fn.imageDictionnary(index, speakerType=self.typeOfSpeaker)
        self.orateurButtons3 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: self.orateurButton(2))
        self.orateurButtons3.pack(side='left')
        self.orateurButtons3.config(height = 1, width = 10 )
        self.buttonList.append(self.orateurButtons3)
        index += 1

        pathAndName = fn.imageDictionnary(index, speakerType=self.typeOfSpeaker)
        self.orateurButtons4 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: self.orateurButton(3))
        self.orateurButtons4.pack(side='left')
        self.orateurButtons4.config(height = 1, width = 10 )
        self.buttonList.append(self.orateurButtons4)
        index += 1
        
        frame += 1

        ## ------------------ Second line ------------------ ##
        pathAndName = fn.imageDictionnary(index, speakerType=self.typeOfSpeaker)
        self.orateurButtons5 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: self.orateurButton(4))
        self.orateurButtons5.pack(side='left')
        self.orateurButtons5.config(height = 1, width = 10 )
        self.buttonList.append(self.orateurButtons5)
        index += 1

        pathAndName = fn.imageDictionnary(index, speakerType=self.typeOfSpeaker)
        self.orateurButtons6 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: self.orateurButton(5))
        self.orateurButtons6.pack(side='left')
        self.orateurButtons6.config(height = 1, width = 10 )
        self.buttonList.append(self.orateurButtons6)
        index += 1

        pathAndName = fn.imageDictionnary(index, speakerType=self.typeOfSpeaker)
        self.orateurButtons7 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: self.orateurButton(6))
        self.orateurButtons7.pack(side='left')
        self.orateurButtons7.config(height = 1, width = 10 )
        self.buttonList.append(self.orateurButtons7)
        index += 1

        pathAndName = fn.imageDictionnary(index, speakerType=self.typeOfSpeaker)
        self.orateurButtons8 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: self.orateurButton(7))
        self.orateurButtons8.pack(side='left')
        self.orateurButtons8.config(height = 1, width = 10 )
        self.buttonList.append(self.orateurButtons8)
        index += 1

        frame += 1

        ## ------------------ Third line ------------------ ##
        pathAndName = fn.imageDictionnary(index, speakerType=self.typeOfSpeaker)
        self.orateurButtons9 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: self.orateurButton(8))
        self.orateurButtons9.pack(side='left')
        self.orateurButtons9.config(height = 1, width = 10 )
        self.buttonList.append(self.orateurButtons9)
        index += 1

        pathAndName = fn.imageDictionnary(index, speakerType=self.typeOfSpeaker)
        self.orateurButtons10 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: self.orateurButton(9))
        self.orateurButtons10.pack(side='left')
        self.orateurButtons10.config(height = 1, width = 10 )
        self.buttonList.append(self.orateurButtons10)
        index += 1

        pathAndName = fn.imageDictionnary(index, speakerType=self.typeOfSpeaker)
        self.orateurButtons11 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: self.orateurButton(10))
        self.orateurButtons11.pack(side='left')
        self.orateurButtons11.config(height = 1, width = 10 )
        self.buttonList.append(self.orateurButtons11)
        index += 1

        pathAndName = fn.imageDictionnary(index, speakerType=self.typeOfSpeaker)
        self.orateurButtons12 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: self.orateurButton(11))
        self.orateurButtons12.pack(side='left')
        self.orateurButtons12.config(height = 1, width = 10 )
        self.buttonList.append(self.orateurButtons12)
        index += 1

        frame += 1

        ## ------------------ Fourth line ------------------ ##
        pathAndName = fn.imageDictionnary(index, speakerType=self.typeOfSpeaker)
        self.orateurButtons13 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: self.orateurButton(12))
        self.orateurButtons13.pack(side='left')
        self.orateurButtons13.config(height = 1, width = 10 )
        self.buttonList.append(self.orateurButtons13)
        index += 1

        pathAndName = fn.imageDictionnary(index, speakerType=self.typeOfSpeaker)
        self.orateurButtons14 = tk.Button(self.OrateurFrame[frame], text=pathAndName[1], command=lambda: self.orateurButton(13))
        self.orateurButtons14.pack(side='left')
        self.orateurButtons14.config(height = 1, width = 10 )
        self.buttonList.append(self.orateurButtons14)
        index += 1

         

    def errorMessageBox(self, message):
        tkmessageBox.showinfo("Error", message)


				##############  __  __   _   ___ _  _  ##############
				############## |  \/  | /_\ |_ _| \| | ##############
				############## | |\/| |/ _ \ | || .` | ##############
				############## |_|  |_/_/ \_\___|_|\_| ##############



MyRoot = tk.Tk()


app = ApplicationProjetS5(MyRoot)                       
app.master.title('Projet S5 P01')

popup = tk.Menu(app, tearoff=0)
popup.add_command(label="Animal",   command= lambda: app.changeApp(fn.appStyle.animal)) 
popup.add_command(label="Humanoid", command= lambda: app.changeApp(fn.appStyle.humain))
popup.add_command(label="Phoneme",  command= lambda: app.changeApp(fn.appStyle.phoneme))



def do_popup(event):
    # display the popup menu
    try:
        popup.tk_popup(event.x_root, event.y_root, 0)

    finally:
        popup.grab_release()

app.topFrame.bind("<Button-3>", do_popup)

w = MyRoot.winfo_screenwidth()
h = MyRoot.winfo_screenheight()
MyRoot.geometry("%dx%d+0+0" % (w, h)) 

app.mainloop()
   
