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
with open("csv_"+filename+".csv", 'w') as csvfile:
    for line in lines:
        #if(count == 10):
        #    break
        string1 = line.split("(")
        print("string1[0]", string1[0])
        if(string1[0] == "OnOffApplication:SendPacket"):
            clientAddress = string1[0][1:-1]
            print(clientAddress)
        

# print("packet_sent: ", count_sent)
# print("packet_received: ", count)
# print("average delay(s): ", sum/count)
