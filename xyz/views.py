from django.shortcuts import render
from django.http import JsonResponse, HttpResponseServerError
import requests
from .models import AccelerometerData
from requests.exceptions import Timeout

# Create your views here.
def query_xyz(request):
    try:
        # 发送查询请求到 ADXL345，并获取数据
        response = requests.get('http://192.168.1.10/req', timeout=3)  # 设置超时时间为3秒
        response.raise_for_status()  # 检查响应是否出现错误，如果出现错误会抛出异常
        acc = response.json()  # 假设 ADXL345 返回的数据是 JSON 格式的浮点数组 [x, y, z]

        # 将获取的数据保存到数据库中
        AccelerometerData.objects.create(x=acc[0], y=acc[1], z=acc[2])

        return JsonResponse({'message': 'Query sent and data recorded successfully'})

    except Timeout:
        return HttpResponseServerError('Timeout: Unable to connect to the ADXL345 device.')
    except requests.exceptions.RequestException as e:
        return HttpResponseServerError(f'Error: {str(e)}')
