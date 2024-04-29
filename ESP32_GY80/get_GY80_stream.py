import requests
import struct  # 用于将二进制数据解析为float
import numpy as np  # 用于处理浮点数组

# 设置ESP32的IP地址和端口
esp32_ip = "192.168.1.13"  # 替换为您的ESP32的实际IP地址
port = 80

# 设置URL
url = f"http://{esp32_ip}:{port}/data"

# 发送HTTP GET请求
response = requests.get(url)

# 检查响应状态
if response.status_code == 200:
    # 从响应中获取二进制数据
    data_bytes = response.content

    # 确保数据长度符合预期
    if len(data_bytes) == 48:  # 12个float，每个float 4字节
        # 将二进制数据转换为float数组
        float_data = struct.unpack('f' * 12, data_bytes)

        # 将数据转换为numpy数组，便于处理
        float_array = np.array(float_data)

        # 打印数据
        print("Received data:", float_array)
    else:
        print("Unexpected data length:", len(data_bytes))
else:
    print("Failed to get data from ESP32, status code:", response.status_code)
