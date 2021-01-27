import csv

data = open("log.lteudp_test2-2", 'r')
lines = data.readlines()

count = 0
sum = 0
with open("lteudp_test2-2.csv", 'w') as csvfile:
    for line in lines:
        #if(count == 10):
        #    break
        string1 = line.split(" ")
        if(string1[0] == "TraceDelay:"):
            floatDelay = eval(string1[16][1:-3])/1000000000
            sum = sum + floatDelay
            list1 = list(str(floatDelay).split(" "))
            # print(str(eval(string1[16][1:-3])/1000000000))
            writer = csv.writer(csvfile)
            writer.writerow(list1)
            count = count + 1
print("count: ", count)
print("average delay(ms): ", sum/count)