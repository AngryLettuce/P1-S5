import json
import codecs
import os
from pylatex import Document, Section, Subsection, Command
from pylatex.utils import bold, NoEscape

def readJsonFile(filePath):
    jsonLines = codecs.open(filePath, encoding='utf-8')
    jsonLines =  jsonLines.readlines()
    return jsonLines



def getUser(logbook):
    logbookdict = {"antoine_logbook"   : "Antoine",
                   "charles_logbook"   : "Charles",
                   "cristhian_logbook" : "Cristhian",
                   "guillaume_logbook" : "Guillaume",
                   "jeff_f_logbook"    : "Jeffrey F.",
                   "jeff_r_logbook"    : "Jeffrey R.",
                   "pascal_b_logbook"  : "Pascal B.",
                   "pascal_l_logbook"  : "Pascal L.",
                   "p-y_logbook"       : "Pierre-Yves",
                   "raphael_logbook"   : "Raphael",
                   "thomas_logbook"    : "Thomas",
                  }
    return logbookdict[logbook]


def formatjsonLine(jsonline) :
    textsplit = jsonline.split("\"")
    comment   = textsplit[3:]
    splittedcomment = '' 

    for comm in comment[:-1] : 
        #comm = comm.replace(',\n', '\n')
        splittedcomment += comm 
    
    splittedcomment = splittedcomment.split(r'\n')

    datetime = splittedcomment[0]
    keyissue = splittedcomment[1]
    comments  = splittedcomment[2:]
    splittedKeyIssue = keyissue.split(':')
    key   = splittedKeyIssue[0]
    issue = splittedKeyIssue[1]

    datetime = datetime.split(".")
    datetime = datetime[0]
    datetime = datetime.split("T")
    date     = datetime[0]
    time     = datetime[1]

    appendComment = ""
    comments = [comment for comment in comments if comment != '']
    for comment in comments : 
        appendComment += comment
        appendComment += "\n"


    return (date, time, key, issue, appendComment)



				##############  __  __   _   ___ _  _  ##############
				############## |  \/  | /_\ |_ _| \| | ##############
				############## | |\/| |/ _ \ | || .` | ##############
				############## |_|  |_/_/ \_\___|_|\_| ##############


slackExportPath = r"C:\Users\Guill\Downloads\P1-S5 Slack export Apr 9 2018"
directoryFolder = os.listdir(slackExportPath)
logbooks = [folder for folder in directoryFolder if "logbook" in folder]
doc = Document(default_filepath = "LogBook" )

for logbook in logbooks : 
    logBookUser = getUser(logbook)
    
    jsontotalLines = []
    jsonfiles = os.listdir(slackExportPath + '\\' + logbook)
    for jsonfile in jsonfiles :
        jsontotalLines.extend(readJsonFile(slackExportPath + '\\' + logbook + '\\' + jsonfile))
         
    with doc.create(Section("Logbook de " + logBookUser)) : 
        for line in jsontotalLines : 
            if 'text' in line : 
                logbookEntry = formatjsonLine(line)
                doc.append(NoEscape(r"\textbf{Date : }"))
                doc.append(bold(logbookEntry[0]))
                doc.append(bold(","))
                doc.append(NoEscape(r"\textbf{ Heure : }"))
                doc.append(bold(logbookEntry[1]))
                doc.append("\n")
      
                doc.append(bold(logbookEntry[2]))
                doc.append(bold(" : "))
                doc.append(bold(logbookEntry[3]))
                doc.append("\n\n")

                doc.append(logbookEntry[4] + "\n")

        doc.append(NoEscape(r"\newpage"))

doc.generate_tex()

