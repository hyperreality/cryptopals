default: all

TARGETS=bin/01_base64 bin/02_fixed_xor bin/03_single_byte_xor bin/04_detect_single_char_xor bin/05_repeating_key_xor bin/06_break_repeated_key_xor bin/07_aes_ecb bin/08_detect_aes_cbc \
	bin/09_pkcs7 bin/10_aes_cbc bin/11_aes_oracle bin/12_prepend_ecb bin/14_prepend_ecb_2 bin/15_strip_padding bin/16_cbc_bitflip \
	bin/17_padding_oracle bin/18_aes_ctr bin/20_break_fixed_nonce_ctr bin/21_mersenne_twister bin/22_crack_mersenne bin/23_clone_mersenne bin/24_mt19937_stream \
        bin/25_ctr_random_access bin/26_ctr_bitflip bin/27_cbc_iv_equals_key bin/28_hmac-sha1

CFLAGS=-std=c99 -Wall -Werror -Wextra -Wno-unused -pedantic -g -O3

all: $(TARGETS) test

bin/01_base64: src/01_base64.c
bin/02_fixed_xor: src/02_fixed_xor.c
bin/03_single_byte_xor: src/03_single_byte_xor.c
bin/04_detect_single_char_xor: src/04_detect_single_char_xor.c
bin/05_repeating_key_xor: src/05_repeating_key_xor.c
bin/06_break_repeated_key_xor: src/06_break_repeated_key_xor.c
bin/07_aes_ecb: src/07_aes_ecb.c -lcrypto
bin/08_detect_aes_cbc: src/08_detect_aes_cbc.c

bin/09_pkcs7: src/09_pkcs7.c -lcrypto
bin/10_aes_cbc: src/10_aes_cbc.c -lcrypto
bin/11_aes_oracle: src/11_aes_oracle.c -lcrypto
bin/12_prepend_ecb: src/12_prepend_ecb.c -lcrypto
bin/14_prepend_ecb_2: src/14_prepend_ecb_2.c -lcrypto
bin/15_strip_padding: src/15_strip_padding.c -lcrypto
bin/16_cbc_bitflip: src/16_cbc_bitflip.c -lcrypto

bin/17_padding_oracle: src/17_padding_oracle.c -lcrypto
bin/18_aes_ctr: src/18_aes_ctr.c -lcrypto
bin/20_break_fixed_nonce_ctr: src/20_break_fixed_nonce_ctr.c -lcrypto
bin/21_mersenne_twister: src/21_mersenne_twister.c
bin/22_crack_mersenne: src/22_crack_mersenne.c
bin/23_clone_mersenne: src/23_clone_mersenne.c
bin/24_mt19937_stream: src/24_mt19937_stream.c

bin/25_ctr_random_access: src/25_ctr_random_access.c -lcrypto
bin/26_ctr_bitflip: src/26_ctr_bitflip.c -lcrypto
bin/27_cbc_iv_equals_key: src/27_cbc_iv_equals_key.c -lcrypto
bin/28_hmac-sha1: src/28_hmac-sha1.c libs/hash/sha1.c

bin/%:
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	$(RM) $(TARGETS) *.o 

