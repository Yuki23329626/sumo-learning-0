import csv
import sys
import shlex
import os
import glob
import matplotlib.pyplot as plt
import shutil
import json

# get each packet according to their uid to locate its source and destination
# in order to collect its info [From, To, TxTime, RxTime, Delay]
def get_packet(fin_dir):
    uid  = {}
    for filename in glob.glob(os.path.join(fin_dir, "log.*")):
        print("getting packet info in file:", filename)
        uid[filename] = {}
        data = open(filename, 'r')
        lines = data.readlines()
        for line in lines:
            string = line.split(" ")
            if string[0] == "TraceDelay" :
                if int(string[7]) not in uid[filename]:
                    uid[filename][int(string[7])] = ["", string[5], 0, 0, 0] # To Address
            if string[0] == "TraceDelay:":
                if int(string[10]) in uid[filename]:
                    uid[filename][int(string[10])][0] = string[5] # From Address
                    uid[filename][int(string[10])][2] = eval(string[12][:-2])/1000000000 # TX in sec
                    uid[filename][int(string[10])][3] = eval(string[14][:-2])/1000000000 # RX in sec
                    uid[filename][int(string[10])][4] = eval(string[16][:-3])/1000000000 # Delay in sec
        #break # uncomment this line if you want to do a quick test
    packet = {}
    for filename in uid.keys():
        for item in uid[filename]:
            #print("item:",item)
            #print(uid[filename][item])
            if uid[filename][item][0] == '':
                continue
            key = uid[filename][item][0] + " " + uid[filename][item][1]
            if key not in packet:
                packet[key] = []
                packet[key].append([uid[filename][item][2], uid[filename][item][3], uid[filename][item][4]])
            else:
                packet[key].append([uid[filename][item][2], uid[filename][item][3], uid[filename][item][4]])
    return packet

def getHOTime(finDir):
    handover_time = {}
    list_handover_time = []
    files = glob.glob(os.path.join(finDir, "*0"))
    with open(files[0], "r") as fin:
        lines = fin.readlines()
        for line in lines:
            string1 = line.split(" ")
            if string1[0] != "NotifyHandoverEndOkUe":
                continue
            if int(string1[5][:-1]) in handover_time:
                handover_time[int(string1[5][:-1])].append(float(string1[2][:6]))
            else:
                handover_time[int(string1[5][:-1])]=[float(string1[2][:6])]
        fin.close()
        list_handover_time = (sorted(handover_time.items(), key=lambda x:x[0]))
    #for item in list_handover_time:
        #print("IMSI:", item[0])
        #print("HO_time:", item[1])
    return list_handover_time # list_handover_time[IMSI] = handover_time


prefix_name = " ".join(map(shlex.quote, sys.argv[1:]))
print("prefix_name: ", prefix_name)

dir_udp_sdn = prefix_name + "udp_sdn"

dir_udp_lte = prefix_name + "udp_lte"

dir_output = prefix_name

if(dir_udp_sdn == "" or not os.path.exists(dir_udp_sdn)):
    print("please check if the dir_udp_sdn is exist")
    exit()

if(dir_udp_lte == "" or not os.path.exists(dir_udp_lte)):
    print("please check if the dir_udp_lte is exist")
    exit()

if not os.path.exists(dir_output):
    print("\nmaking dir for output files:", dir_output)
    os.makedirs(dir_output)
print("\ndir_output:", dir_output, "has exist!")

# sacn each log in dir

path_packet_sdn = dir_output + "/packet_sdn_.json"
path_packet_lte = dir_output + "/packet_lte_.json"

if(not os.path.exists(path_packet_sdn)):
    packet_sdn = get_packet(dir_udp_sdn)
    json.dump(packet_sdn, open(path_packet_sdn,'w'))
else:
    print("get packet info from file:", path_packet_sdn)
    packet_sdn = json.load(open(path_packet_sdn,'r'))

if(not os.path.exists(path_packet_lte)):
    packet_lte = get_packet(dir_udp_lte)
    json.dump(packet_lte, open(path_packet_lte,'w'))
else:
    print("get packet info from file:", path_packet_lte)
    packet_lte = json.load(open(path_packet_lte,'r'))

# get handover time in sdn

list_handover_time = getHOTime(dir_udp_sdn)

#print(list_handover_time[0][0])

#for item in list_handover_time:
#    print("IMSI:", item[0])
#    for handover_time in item[1]:
#        print("handover_time:", handover_time)

# average delay SDN
route_upload = 0
route_download = 0

upload_1k_delay_sum = 0
upload_1m_delay_sum = 0

download_1k_delay_sum = 0
download_1m_delay_sum = 0

for route in packet_sdn:
    #print("route:", route)
    start = packet_sdn[route][0][0]
    end = packet_sdn[route][-1][1]
    num = len(packet_sdn[route])
    #print("time spent:", (end-start))
    #print("packet received:", num)
    throughput_per_sec = num*1024/(end-start)
    #print("thoughput per sec:", throughput_per_sec)
    delay_1k = 1024/throughput_per_sec
    #print("1k byte packet delay:", delay_1k)
    delay_1m = 1024*1024/throughput_per_sec
    #print("1M byte packet delay", delay_1m)
    if route.split(" ")[0] == "1.0.0.2":
        route_download = route_download + 1
        download_1k_delay_sum = download_1k_delay_sum + delay_1k
        download_1m_delay_sum = download_1m_delay_sum + delay_1m
    else:
        route_upload = route_upload + 1
        upload_1k_delay_sum = upload_1k_delay_sum + delay_1k
        upload_1m_delay_sum = upload_1m_delay_sum + delay_1m

print("\nSDN:\n")
print("average upload delay(1k byte):")
print(upload_1k_delay_sum/route_upload)
print("average upload delay(1m byte):")
print(upload_1m_delay_sum/route_upload)
print("average download delay(1k byte):")
print(download_1k_delay_sum/route_download)
print("average download delay(1m byte):")
print(download_1m_delay_sum/route_download)

# average delay LTE
route_upload = 0
route_download = 0

upload_1k_delay_sum = 0
upload_1m_delay_sum = 0

download_1k_delay_sum = 0
download_1m_delay_sum = 0

for route in packet_lte:
    #print("route:", route)
    start = packet_lte[route][0][0]
    end = packet_lte[route][-1][1]
    num = len(packet_lte[route])
    #print("time spent:", (end-start))
    #print("packet received:", num)
    throughput_per_sec = num*1024/(end-start)
    #print("thoughput per sec:", throughput_per_sec)
    delay_1k = 1024/throughput_per_sec
    #print("1k byte packet delay:", delay_1k)
    delay_1m = 1024*1024/throughput_per_sec
    #print("1M byte packet delay", delay_1m)
    if route.split(" ")[0] == "1.0.0.2":
        route_download = route_download + 1
        download_1k_delay_sum = download_1k_delay_sum + delay_1k
        download_1m_delay_sum = download_1m_delay_sum + delay_1m
    else:
        route_upload = route_upload + 1
        upload_1k_delay_sum = upload_1k_delay_sum + delay_1k
        upload_1m_delay_sum = upload_1m_delay_sum + delay_1m

print("\nLTE\n")
print("average upload delay(1k byte):")
print(upload_1k_delay_sum/route_upload)
print("average upload delay(1m byte):")
print(upload_1m_delay_sum/route_upload)
print("average download delay(1k byte):")
print(download_1k_delay_sum/route_download)
print("average download delay(1m byte):")
print(download_1m_delay_sum/route_download)



