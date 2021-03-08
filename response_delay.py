import csv
import sys
import os

online_time = [30, 60, 90, 120, 150]
offline_time = [[0]*3 for i in range(5)]
for i in range(5):
    offline_time[i][0] = online_time[i]*1
    offline_time[i][1] = online_time[i]*1.5
    offline_time[i][2] = online_time[i]*2

for i in range(5):
    for j in range(3):
        print(offline_time[i][j], end = " ")
    print("\n")

lambda_ = [1/30, 1/60, 1/90, 1/120, 1/150]
mu_ = [[0]*3 for i in range(5)]
for i in range(5):
    for j in range(3):
        mu_[i][j] = 1/offline_time[i][j]

for i in range(5):
    for j in range(3):
        print(mu_[i][j], end = " ")
    print("\n")

tEncryptionAsymmetric = 59.8
tDecryptionAsymmetric = 9.56
tRekey = 19.12
tAuthorization = 28.26
tCompare = 116.62
tReEncryption = 0
tReDecryption = 43.12
tEncryptionTK = 6
tDecryptionTK = 6

tSdnDisabledUpload = 41.55
tSdnDisabledDownload = 15.25
tSdnEnabledUpload = 30.01
tSdnEnabledDownload = 15.10

n = 1

bpreetUpload = tSdnDisabledUpload + tEncryptionAsymmetric + tEncryptionTK
bpreetRequest = tSdnDisabledUpload + tSdnDisabledDownload + tSdnDisabledDownload + tSdnDisabledUpload + tEncryptionAsymmetric + n*tCompare + 2*tAuthorization + tRekey + tReEncryption + tReDecryption + tDecryptionTK

OurSchemeUpload = tSdnEnabledUpload + tEncryptionAsymmetric + tEncryptionTK
OurSchemeRequest = 3*tSdnEnabledUpload + 2*tSdnEnabledDownload + 1*tSdnEnabledDownload + tEncryptionAsymmetric + (n+1)*tCompare + 2*tAuthorization + tRekey + tReEncryption + tReDecryption + tDecryptionTK

onoffResponseDelay = [[0]*3 for i in range(5)]

for i in range(5):
    for j in range(3):
        onoffResponseDelay[i][j] = (mu_[i][j] / (lambda_[i] + mu_[i][j])) * OurSchemeRequest + (lambda_[i] / (lambda_[i] + mu_[i][j])) * ( 1 / mu_[i][j] + OurSchemeRequest)

print("===== oracleResponseDelay =====\n")
print(OurSchemeRequest)
print("\n")

print("===== onoffResponseDelay =====\n")
for i in range(5):
    for j in range(3):
        print(onoffResponseDelay[i][j], end=" ")
    print("\n")