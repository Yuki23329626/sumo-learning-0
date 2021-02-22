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
        string1 = line.split(" ")
        if(string1[0] == "TraceDelay"):
            count_sent = count_sent + 1
        if(string1[0] == "TraceDelay:"):
            floatDelay = eval(string1[16][1:-3])/1000000000
            sum = sum + floatDelay
            list1 = list(str(floatDelay).split(" "))
            # print(str(eval(string1[16][1:-3])/1000000000))
            writer = csv.writer(csvfile)
            writer.writerow(list1)
            count = count + 1
print("packet_sent: ", count_sent)
print("packet_received: ", count)
print("average delay(s): ", sum/count)
