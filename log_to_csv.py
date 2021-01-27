
data = open("log.lteudp_test2-2", 'r')
lines = data.readlines()

for line in lines:
    string1 = line.split(" ")
    if(string1[0] == "TraceDelay:"):
        print(string1[16])
        break

