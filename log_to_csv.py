
data = open("log.lteudp_test2-2", 'r')
lines = data.readlines()

count = 0
for line in lines:
    print("Line{}: {}".format(count, line.strip()))
    count = count + 1