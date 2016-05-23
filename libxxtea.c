/***********************************************************************
    Copyright 2016-2020 Xu Xianhua
    These sources is free software. Redistributions of source code must
    retain the above copyright notice. Redistributions in binary form
    must reproduce the above copyright notice. You can redistribute it
    freely. You can use it with any free or commercial software.
    These sources is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY. Without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*************************************************************************/

#include <memory.h>
#include <string.h>
// #include "php.h"
#include "libxxtea.h"

static void xxtea_long_encrypt(xxtea_long *v, int n, const xxtea_long *k) {
    if (n < 1) {
        return;
    }
    
    xxtea_long rounds, y, e, p;
    xxtea_long z = v[n-1], sum = 0;
    rounds = XXTEA_ROUNDS(n);
    
    do {
        sum += XXTEA_DELTA;
        e = XXTEA_E(sum);
        
        for(p = 0; p < n-1; p++) {
            y = v[p+1];
            v[p] += XXTEA_MX;
            z = v[p];
        }
        y = v[0];
        v[n-1] += XXTEA_MX;
        z = v[n-1];
    } while (--rounds);
}

static void xxtea_long_decrypt(xxtea_long *v, int n, const xxtea_long *k) {
    if (n < 1) {
        return;
    }
    
    xxtea_long rounds, y, e, p;
    xxtea_long z = v[n-1], sum;
    rounds = XXTEA_ROUNDS(n);
    sum = rounds * XXTEA_DELTA;
    y = v[0];
    
    do {
        e = XXTEA_E(sum);
        
        for (p = n - 1; p > 0; p--) {
            z = v[p-1];
            v[p] -= XXTEA_MX;
            y = v[p];
        }
        
        z = v[n-1];
        v[0] -= XXTEA_MX;
        y = v[0];
        sum -= XXTEA_DELTA;
    } while (--rounds);
}

static int xxtea_to_long_array(const char *bytes, int len, xxtea_long **out_longs) {
	int i;
	int out_len = len >> 2;
	
	if ((len & 3)) {
		out_len += 1;
	}
	
	// 申請內存
	*out_longs = (xxtea_long *)malloc(sizeof(xxtea_long) * out_len);
	// 清空返回字符串, 内存在调用此方法前申请
	memset(*out_longs, 0, sizeof(xxtea_long) * out_len);
	
	for (i = 0; i < len; i++) {
		// 負數的處理比較特殊，不能簡單的轉換，應該使用位運算
		// *(*out_longs + (i >> 2)) |= ((0x00000000 | ((*(bytes + i)) & 0xff)) << ((i & 3) << 3));
		*(*out_longs + (i >> 2)) |= ((xxtea_long)(*(bytes + i)) & 0xff) << ((i & 3) << 3);
	}
	
	return out_len;
}

static int xxtea_to_bytes_array(xxtea_long *longs, int len, char **bytes) {
	// bytes是在外面
	int i;
	int o_len = len << 2;
	
	// 申請內存
	*bytes = (char *)malloc(sizeof(char) * (o_len + 1));
	// 清空返回字符串, 内存申请在调用
	memset(*bytes, '\0', sizeof(char) * (o_len + 1));
	
	for(i = 0; i < len; i++) {
		int j = 0;
		
		for (j = 0; j < 4; j++) {
			*(*bytes + (i << 2) + (3 & j)) = (char)((*(longs + i) >> (j << 3)) & 0xff);
		} 
	}
	
	return strlen(*bytes);
}

int xxtea_bytes_encode(const char *text, int len, const char *key, int k_len, char **out) {
	// xxtea_long数组
	xxtea_long *tl_a;
	int tl_len, tb_len;
	xxtea_long *kl_a;
	
	char *meta_key;
	meta_key = (char *)malloc(sizeof(char) * (KEY_SIZE + 1));
	memset(meta_key, 'x', sizeof(char) * (KEY_SIZE + 1));
	*(meta_key + KEY_SIZE) = '\0';
	
	if (k_len < KEY_SIZE) {
		memcpy(meta_key, key, k_len * sizeof(char));
	} else {
		memcpy(meta_key, key, KEY_SIZE * sizeof(char));
	}
	
	// text
	tl_len = xxtea_to_long_array(text, len, &tl_a);
	
	// key
	xxtea_to_long_array(meta_key, KEY_SIZE, &kl_a);
	
	//encode
	xxtea_long_encrypt(tl_a, tl_len, kl_a);
	
	// to string
	tb_len = xxtea_to_bytes_array(tl_a, tl_len, out);
	
	// free memory
	free(meta_key);
	free(tl_a);
	free(kl_a);
	return tb_len;
}

int xxtea_bytes_decode(const char *text, int len, const char *key, int k_len, char **out) {
	xxtea_long *tl_a, *kl_a;
	int tl_len, rb_len;
	char *meta_key;
	
	meta_key = (char *)malloc(sizeof(char) * (KEY_SIZE + 1));
	memset(meta_key, 'x', sizeof(char) * (KEY_SIZE + 1));
	*(meta_key + KEY_SIZE) = '\0';
	
	if (k_len < KEY_SIZE) {
		memcpy(meta_key, key, k_len * sizeof(char));
	} else {
		memcpy(meta_key, key, KEY_SIZE * sizeof(char));
	}
	
	// // text
	tl_len = xxtea_to_long_array(text, len, &tl_a);
	
	// // key
	xxtea_to_long_array(meta_key, KEY_SIZE, &kl_a);
	// // decode
	xxtea_long_decrypt(tl_a, tl_len, kl_a);
	// // to string
	rb_len = xxtea_to_bytes_array(tl_a, tl_len, out);
	
	// free memory
	free(meta_key);
	free(tl_a);
	free(kl_a);
	return rb_len;
}
