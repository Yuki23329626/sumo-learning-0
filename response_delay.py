import csv
import sys
import os
from typing import get_type_hints

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

g1 = 9.56
g2 = 21.56
gt = 48.81
e = 116.62
xor = 0
h1 = 2.07
h2 = 14.32

tEncryptionAsymmetric = 4*g1 + g2
tDecryptionAsymmetric = g1
tRekey = 2*g1 + xor
tAuthorization = 3*g1
tCompare = e
tReEncryption = xor
tReDecryption = 3*g1 + 2*g2 + xor
tEncryptionTK = 6
tDecryptionTK = 6

print("Encryption:", tEncryptionAsymmetric)
print("Decryption:", tDecryptionAsymmetric)
print("Rekey:", tRekey)
print("Authorization:", tAuthorization)
print("Compare:", tCompare)
print("ReEncryption:", tReEncryption)
print("ReDecryption:", tReDecryption)
print("EncryptionTK:", tEncryptionTK)
print("DecryptionTK:", tDecryptionTK)

tSdnDisabledUpload = 41.55
tSdnDisabledDownload = 15.25
tSdnEnabledUpload = 30.01
tSdnEnabledDownload = 15.10

n = 1

# trans into seconds

bpreetUpload = (tSdnDisabledUpload + tEncryptionAsymmetric + tEncryptionTK) / 1000
bpreetRequest = (tSdnDisabledUpload + tSdnDisabledDownload + tSdnDisabledDownload + tSdnDisabledUpload + tEncryptionAsymmetric + n*tCompare + 2*tAuthorization + tRekey + tReEncryption + tReDecryption + tDecryptionTK) / 1000

OurSchemeUpload = (tSdnEnabledUpload + tEncryptionAsymmetric + tEncryptionTK) / 1000
OurSchemeRequest = (3*tSdnEnabledUpload + 2*tSdnEnabledDownload + 1*tSdnEnabledDownload + tEncryptionAsymmetric + (n+2)*tCompare + 2*tAuthorization + tRekey + tReEncryption + tReDecryption + tDecryptionTK) / 1000

onoffResponseDelay = [[0]*3 for i in range(5)]

for i in range(5):
    for j in range(3):
        onoffResponseDelay[i][j] = ((mu_[i][j] / (lambda_[i] + mu_[i][j])) * OurSchemeRequest) + (lambda_[i] / (lambda_[i] + mu_[i][j])) * ( (1 / mu_[i][j]) + OurSchemeRequest)

print("===== OurSchemeRequestResponseDelay =====\n")
print(OurSchemeRequest)
print("\n")

print("===== bpreetRequestResponseDelay =====\n")
print(bpreetRequest)
print("\n")

print("===== onoffResponseDelay =====\n")
for i in range(5):
    for j in range(3):
        print("\nT_online:", online_time[i], ",T_offline:", offline_time[i][j])
        print("Delay:", onoffResponseDelay[i][j])
    print("\n")