import csv
import sys
import shlex
import os
filename = " ".join(map(shlex.quote, sys.argv[1:]))
print("filename: ", filename)

if(filename == "" or not os.path.exists(filename)):
    print("please type the correct file name of the log")
    exit()

data = open(filename, 'r')
lines = data.readlines()

count_sent = 0
count = 0
sum = 0

sendPacketStart = {}

lastType = ""
lastLineAddress = ""
with open("csv_"+filename+".csv", 'w') as csvfile:
    for line in lines:
        #if(count == 10):
        #    break
        string1 = line.split("(")
        # print("string1[0]", string1[0])
        if(string1[0] == "OnOffApplication:SendPacket"):
            clientAddress = string1[1][2:-2]
            #print("clientAddress", clientAddress)
            lastType = "SendPacket"
            lastLineAddress = clientAddress
            continue
        if(lastLineAddress != "" and lastType == "SendPacket"):
            string2 = line.split(" ")
            #print("string2", string2)
            if lastLineAddress in sendPacketStart:
                lastType = ""
                lastLineAddress = ""
                continue
            sendPacketStart[lastLineAddress] = string2[2][:-1]
            print("sendPacketDic", sendPacketStart)
            print("sendPacketDic[lastLineAdress]:", sendPacketStart[lastLineAddress])
            lastType = ""
            lastLineAddress = ""

        

# print("packet_sent: ", count_sent)
# print("packet_received: ", count)
# print("average delay(s): ", sum/count)
