import os
from Crypto.Cipher import AES
from pkcs7 import PKCS7Encoder
padder = PKCS7Encoder()

AES_KEY = os.urandom(16)


def kv_pairs(cookie):
    arr = [p.split("=") for p in cookie.split("&")]
    dic = {}
    for k, v in arr:
        dic[k] = v

    return dic


def profile_for(email):
    email = email.replace('&', ' ').replace('=', ' ')
    cookie = "email=" + email + "&uid=10&role=user"

    return cookie


def encrypt(profile):
    aes = AES.new(AES_KEY, AES.MODE_ECB)
    padded = padder.encode(profile)
    ciphertext = aes.encrypt(padded)

    return ciphertext


def decrypt(ciphertext):
    aes = AES.new(AES_KEY, AES.MODE_ECB)
    padded = aes.decrypt(ciphertext)
    plaintext = padder.decode(padded)

    return plaintext


if __name__ == '__main__':
    # email=1337@1337.com&uid=10&role=
    profile = profile_for("1337@1337.com")
    norole = encrypt(profile)[0:32]

    # admin
    profile = profile_for(
        "0123456789admin\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b")
    admin = encrypt(profile)[16:32]

    payload = norole + admin

    dec = decrypt(payload)
    print(kv_pairs(dec))
