import csv

data = open("log.lteudp_test2-2", 'r')
lines = data.readlines()

for line in lines:
    string1 = line.split(" ")
    if(string1[0] == "TraceDelay:"):
        list1 = list(string1[16][1:-3])
        print(string1[16][1:-3])
        with open("lteudp_test2-2.csv", 'w', newline="") as csvfile:
            writer = csv.writer(csvfile)
            writer.writerow(list1)

