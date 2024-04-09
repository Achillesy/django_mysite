from django.shortcuts import render
from django.http import HttpResponseServerError
from .models import AccelerometerData
from django.conf import settings
import requests
from requests.exceptions import Timeout

def query_xyz(request):
    try:
        # 发送查询请求到 ADXL345，并获取数据
        response = requests.get(settings.ADXL345_QUERY_URL, timeout=5)
        response.raise_for_status()
        acc_json = response.json()

        # 解析加速度数据
        x = acc_json['x']
        y = acc_json['y']
        z = acc_json['z']

        # 将数据保存到数据库中
        AccelerometerData.objects.create(x=x, y=y, z=z)

        # 获取最近5次记录
        latest_records = AccelerometerData.objects.order_by('-id')[:5]

        # 渲染模板并返回给浏览器
        return render(request, 'xyz/records.html', {'latest_records': latest_records})

    except Timeout:
        return HttpResponseServerError('Timeout: Unable to connect to the ADXL345 device.')
    except requests.exceptions.RequestException as e:
        return HttpResponseServerError(f'Error: {str(e)}')
