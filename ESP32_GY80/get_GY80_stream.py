import requests
import struct
import numpy as np

# 设置ESP32的IP地址和端口
esp32_ip = "192.168.1.13"  # 替换为您的ESP32的实际IP地址
port = 80

# 设置URL
url = f"http://{esp32_ip}:{port}/data"

# 发送HTTP GET请求
response = requests.get(url, stream=True)

# 检查响应状态
if response.status_code == 200:
    # 从响应中获取二进制数据
    data_bytes = b""
    remaining_bytes = 48  # 期望接收的总字节数
    while remaining_bytes > 0:
        chunk = response.raw.read(remaining_bytes)
        data_bytes += chunk
        remaining_bytes -= len(chunk)
        if remaining_bytes > 0:
            print(f"Incomplete data received, waiting for {remaining_bytes} more bytes...")

    # 确保数据长度符合预期
    if len(data_bytes) == 48:  # 12个float，每个float 4字节
        # 将二进制数据转换为float数组
        float_data = struct.unpack('f' * 12, data_bytes)
        # 将数据转换为numpy数组，便于处理
        float_array = np.array(float_data)
        # 打印数据
        np.set_printoptions(precision=2, suppress=True)
        print("Received data:", float_array)
    else:
        print("Unexpected data length:", len(data_bytes))
else:
    print("Failed to get data from ESP32, status code:", response.status_code)