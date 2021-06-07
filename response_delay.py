import csv
import sys
import os
from typing import get_type_hints
import matplotlib.pyplot as plt

# online_time = [10, 20, 30, 40, 50]
# offline_time = [[0]*3 for i in range(5)]
# for i in range(5):
#     offline_time[i][0] = online_time[i]*1
#     offline_time[i][1] = online_time[i]*1.5
#     offline_time[i][2] = online_time[i]*2

# for i in range(5):
#     for j in range(3):
#         print(offline_time[i][j], end = " ")
#     print("\n")

# lambda_ = [1/30, 1/60, 1/90, 1/120, 1/150]
# mu_ = [[0]*3 for i in range(5)]
# for i in range(5):
#     for j in range(3):
#         mu_[i][j] = 1/offline_time[i][j]

# for i in range(5):
#     for j in range(3):
#         print(mu_[i][j], end = " ")
#     print("\n")

g1 = 9.56
g2 = 21.56
gt = 48.81
e = 116.62
xor = 0
h1 = 2.07
h2 = 14.32

t_encryption_asymmetric = 4*g1 + g2
t_decryption_asymmetric = g1
t_rekey = 2*g1 + xor
t_authorization = 3*g1
t_compare = e
t_reEncryption = xor
t_reDecryption = 3*g1 + 2*g2 + xor
t_encryption_TK = 6.63
t_decryptionTK = 6

print("Encryption:", t_encryption_asymmetric)
print("Decryption:", t_decryption_asymmetric)
print("Rekey:", t_rekey)
print("Authorization:", t_authorization)
print("Compare:", t_compare)
print("ReEncryption:", t_reEncryption)
print("ReDecryption:", t_reDecryption)
print("EncryptionTK:", t_encryption_TK)
print("DecryptionTK:", t_decryptionTK)

t_upload_1k_lte = 4.43
t_download_1k_lte = 3.26

t_upload_1m_lte = 4545.35
t_download_1m_lte = 3340.03

t_upload_05k_lte = 2.21
t_download_05k_lte = 1.63

t_upload_1k_sdn = 3.88
t_dwonload_1k_sdn = 2.98

t_upload_1m_sdn = 3973.65
t_dwonload_1m_sdn = 3058.92

t_upload_05k_sdn = 1.94
t_dwonload_05k_sdn = 1.49

n = 1

# trans into seconds

bpreetUpload = (t_upload_05k_lte + t_encryption_asymmetric + t_encryption_TK) / 1000
bpreetRequest = (t_upload_05k_lte + t_download_05k_lte + t_download_1m_lte + t_upload_1m_lte + t_encryption_asymmetric + n*t_compare + 2*t_authorization + t_rekey + t_reEncryption + t_reDecryption + t_decryptionTK) / 1000

OurSchemeUpload = (max(t_upload_05k_sdn, t_upload_1m_sdn) + t_encryption_asymmetric + t_encryption_TK) / 1000
OurSchemeRequest = (3*t_upload_05k_sdn + 2*t_dwonload_05k_sdn + 1*t_dwonload_1m_sdn + t_encryption_asymmetric + (n+2)*t_compare + 2*t_authorization + t_rekey + t_reEncryption + t_reDecryption + t_decryptionTK) / 1000

# onoffResponseDelay = [[0]*3 for i in range(5)]

# for i in range(5):
#     for j in range(3):
#         onoffResponseDelay[i][j] = ((mu_[i][j] / (lambda_[i] + mu_[i][j])) * OurSchemeRequest) + (lambda_[i] / (lambda_[i] + mu_[i][j])) * ( (1 / mu_[i][j]) + OurSchemeRequest)


print("\nbpreet upload delay:")
print(bpreetUpload)
print("bpreet request delay:")
print(bpreetRequest)

print("\nour scheme upload delay:")
print(OurSchemeUpload)
print("our scheme request delay:")
print(OurSchemeRequest)



# print("\n===== onoffResponseDelay =====")
# for i in range(5):
#     for j in range(3):
#         print("\nT_online:", online_time[i], ",T_offline:", offline_time[i][j])
#         print("Delay:", onoffResponseDelay[i][j])
#     # print("\n")

t_online = [10, 20, 30, 40, 50]
t_offline_1 = []
t_offline_1_5 = []
t_offline_2 = []

for i in range(5):
    t_offline_1.append(t_online[i]*1)
    t_offline_1_5.append(t_online[i]*1.5)
    t_offline_2.append(t_online[i]*2)

print(t_offline_1)
print(t_offline_1_5)
print(t_offline_2)

lambda_ = []
mu_1 = []
mu_1_5 = []
mu_2 = []

for i in range(5):
    lambda_.append(1/t_online[i])
    mu_1.append(1/t_offline_1[i])
    mu_1_5.append(1/t_offline_1_5[i])
    mu_2.append(1/t_offline_2[i])

print(lambda_)
print(mu_1)
print(mu_1_5)
print(mu_2)

def get_onoff_response_delay(mu_, lambda_, delay_):
    return (mu_/(lambda_+mu_)*delay_) + (lambda_/(lambda_+mu_)*((1/mu_)+delay_))

onoffResponseDelay_our_scheme = []
onoffResponseDelay_bpreet_1 = []
onoffResponseDelay_bpreet_1_5 = []
onoffResponseDelay_bpreet_2 = []

for i in range(5):
    onoffResponseDelay_our_scheme.append(OurSchemeRequest/60)
    onoffResponseDelay_bpreet_1.append(get_onoff_response_delay(mu_1[i], lambda_[i], bpreetRequest/60))
    onoffResponseDelay_bpreet_1_5.append(get_onoff_response_delay(mu_1_5[i], lambda_[i], bpreetRequest/60))
    onoffResponseDelay_bpreet_2.append(get_onoff_response_delay(mu_2[i], lambda_[i], bpreetRequest/60))

print("\nonoffResponseDelay:")
print(onoffResponseDelay_our_scheme)
print(onoffResponseDelay_bpreet_1)
print(onoffResponseDelay_bpreet_1_5)
print(onoffResponseDelay_bpreet_2)

plt.title('Average delay(min) with M/M/1 queuing model')
plt.xlabel('Online time(min)')
plt.ylabel('Response Delay(min)')
plt.plot(t_online, onoffResponseDelay_our_scheme, color='green', marker='o', markersize=3)
plt.plot(t_online, onoffResponseDelay_bpreet_1, color='blue', linestyle=':', marker='o', markersize=3)
plt.plot(t_online, onoffResponseDelay_bpreet_1_5, color='brown', linestyle='--', marker='o', markersize=3)
plt.plot(t_online, onoffResponseDelay_bpreet_2, color='red', linestyle='-.', marker='o', markersize=3)

for i in range(5):
    plt.text(t_online[i]-2, onoffResponseDelay_our_scheme[i]+1.5, str(onoffResponseDelay_our_scheme[i])[:5])
    plt.text(t_online[i]-2, onoffResponseDelay_bpreet_1[i]+1.5, str(onoffResponseDelay_bpreet_1[i])[:5])
    plt.text(t_online[i]-2, onoffResponseDelay_bpreet_1_5[i]+1.5, str(onoffResponseDelay_bpreet_1_5[i])[:5])
    plt.text(t_online[i]-2, onoffResponseDelay_bpreet_2[i]+1.5, str(onoffResponseDelay_bpreet_2[i])[:5])

plt.legend(['our scheme, 1/1', 'bpreet, 1/2', 'bpreet, 2/5', 'bpreet, 1/3'])

plt.show()