default: all

TARGETS=bin/01_base64 bin/02_fixed_xor bin/03_single_byte_xor bin/04_detect_single_char_xor bin/05_repeating_key_xor bin/06_break_repeated_key_xor bin/07_aes_ecb bin/08_detect_aes_cbc \
	bin/09_pkcs7 bin/10_aes_cbc bin/11_aes_oracle bin/12_prepend_ecb bin/15_strip_padding bin/16_cbc_bitflip

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
bin/15_strip_padding: src/15_strip_padding.c -lcrypto
bin/16_cbc_bitflip: src/16_cbc_bitflip.c -lcrypto

bin/%:
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	$(RM) $(TARGETS) *.o 

