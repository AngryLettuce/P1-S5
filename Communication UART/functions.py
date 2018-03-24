
from PIL import Image, ImageTk
import serial
from threading import Timer


########################## -- Widgets related -- ##########################

def changeImage(label, path):
    'set the image of the label to the image from the path'
    photo = Image.open(path)
    photo = ImageTk.PhotoImage(photo)
    label.configure(image=photo)
    label.photo = photo

def changeLabelText(label, data) : 
    'change the text of a label to data'
    label.configure(text=data)
    label.text = data


########################## -- Dictionnaries -- ##########################

def dskStatusDictionnary(status) :
    'holds the status of the dsk'
    Dict = {  0  : 'INIT', 
              1  : 'IDLE',
              2  : 'Test Init',
              3  : 'Test Acquisition',
              4  : 'Train Init',
              5  : 'Train Acquisition',
              6  : 'Train Codebook Construction',
              7  : 'Error',


             63 : 'Inconnu',             
           }

    if status not in Dict :
        status = 63

    return Dict[status]


def imageDictionnary(Orateur):
    'holds the possible speakers'
    #Picture size : 381 * 285 px

    Dict = {  0  : (r"newAntoine.jpg",       'Antoine'),
              1  : (r"newPascal.jpg",        'Pascal L.'),
              2  : (r"newPascal_B.jpg",      'Pascal B.'),
              3  : (r"newGuillaume.jpg",     'Guillaume'),
              4  : (r"newRaphael.jpg",       'Raphael'),
              5  : (r"newThomas.jpg",        'Thomas'),
              6  : (r"newP_Y.jpg",           'Pierre-Yves! :D'), 
              7  : (r"newJeff.jpg",          'Jeffrey F.'), 
              8  : (r"newVit Hess.jpg",      'Jeffrey R.'), 
              9  : (r"newChuck.jpg",         'Charles'), 
              10 : (r"newGonzo.jpg",         'Cristhian'), 
              11 : (r"newL_P.jpg",           'Tatlock'), 
              12 : (r"newfeu_serviette.jpg", 'RIP serviette'), 
              13 : (r"newbutrice.jpg",       'Butrice'),
                      
              63 : (r"newnoImage.jpg",       'Inconnu'),
             } 

    if Orateur not in Dict :
        Orateur = 63

    return Dict[Orateur]


########################## -- Serial Port -- ##########################
def setupSerialPort(port, baudrate, timeout):
    'setut a serial port connection with a baudrate and a timeout' 
    ser = serial.Serial(port, baudrate, timeout=timeout)
    return ser

def writingSerial(serialPort, data):
    serialPort.write(data)
    #serialPort.write(data.encode())




########################## -- Threading -- ##########################
class RepeatedTimer(object):
    'Create thread object which run a funtion after a delay given'
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

def stopThread(thread):
    thread.stop()


def startThread(thread):
    thread.start()


########################## -- Utility functions -- ##########################

def resizePicture(path, width):
    img = Image.open(path)
    wpercent = (width/float(img.size[0]))
    hsize = int((float(img.size[1])*float(wpercent)))
    img = img.resize((width,hsize), Image.ANTIALIAS)
    img.save('new' + path)

def parity(int_type):
    'Used to calculate the parity of a byte'
    parity = 0
    while (int_type):
        parity = ~parity
        int_type = int_type & (int_type - 1)
    return(parity)


def check_int(data):
    'return true if data is a digit, even if the digit is negative'
    if data[0] in ('-', '+'):
        return data[1:].isdigit()
    return data.isdigit()