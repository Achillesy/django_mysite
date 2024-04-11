from django.shortcuts import render
from django.http import HttpResponse, HttpResponseServerError
from .models import GY80Data
from django.conf import settings
import requests
from requests.exceptions import Timeout
import json

def query_xyz(request):
    try:
        # 发送查询请求到 ADXL345，并获取数据
        response = requests.get(settings.ADXL345_QUERY_URL, timeout=5)
        response.raise_for_status()
        acc_json = response.json()

        # 将数据保存到数据库中
        GY80Data.objects.create(
            acc_x=acc_json['acc_x'],
            acc_y=acc_json['acc_y'],
            acc_z=acc_json['acc_z'],
            gyro_x=acc_json['gyro_x'],
            gyro_y=acc_json['gyro_y'],
            gyro_z=acc_json['gyro_z'],
            mag_x=acc_json['mag_x'],
            mag_y=acc_json['mag_y'],
            mag_z=acc_json['mag_z'],
            temperature=acc_json['temperature'],
            pressure=acc_json['pressure'],
            altitude=acc_json['altitude']
        )

        return HttpResponse('Data saved successfully.')

        # # 将整个 JSON 对象保存到数据库中
        # GY80Data.objects.create(data=json.dumps(acc_json))

        # # 获取最近保存的记录
        # latest_record = GY80Data.objects.latest('date')
        # # 格式化最新记录的相关信息成字符串
        # latest_record_str = f"Latest Record:\nX: {latest_record.x}\nY: {latest_record.y}\nZ: {latest_record.z}\nDate: {latest_record.date}"

        # # 返回格式化后的字符串作为响应
        # return HttpResponse(latest_record_str)

        # # 解析加速度数据
        # x = acc_json['x']
        # y = acc_json['y']
        # z = acc_json['z']

        # # 将数据保存到数据库中
        # GY80Data.objects.create(x=x, y=y, z=z)

        # # 获取最近5次记录
        # latest_records = GY80Data.objects.order_by('-id')[:5]

        # # 渲染模板并返回给浏览器
        # return render(request, 'xyz/records.html', {'latest_records': latest_records})

    except Timeout:
        return HttpResponseServerError('Timeout: Unable to connect to the ADXL345 device.')
    except requests.exceptions.RequestException as e:
        return HttpResponseServerError(f'Error: {str(e)}')
