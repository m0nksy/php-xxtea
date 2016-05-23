# 簡單介紹
* PHP使用的XXTEA算法擴展
* C實現，性能更好
* 相比其他人的庫，對二進制流與32位整形數組的處理不相同，更高效，不依賴機器CPU對整形數據在內存中的存儲方案限制（與大小端無關)
* 使用簡單

# 舉例
```php
<?php
$content = 'Meitu Vs IDreamsky, Meitu WIN, Meitu WIN!';
$key = "caomima";
$a = xxtea_encode($content, $key);
$b = xxtea_decode($a, $key);

if ($b == $content) {
	echo "success!";
} else {
	echo "fail!";
}
```
