
data = open("log.lteudp_test2-2", 'r')
lines = data.readlines()

for line in lines:
    test = line.split(" ")[0]
    print(test)
