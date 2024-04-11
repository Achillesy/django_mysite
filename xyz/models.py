from django.db import models

# Create your models here.
class GY80Data(models.Model):
    acc_x = models.FloatField()
    acc_y = models.FloatField()
    acc_z = models.FloatField()
    gyro_x = models.FloatField()
    gyro_y = models.FloatField()
    gyro_z = models.FloatField()
    mag_x = models.FloatField()
    mag_y = models.FloatField()
    mag_z = models.FloatField()
    temperature = models.FloatField()
    pressure = models.FloatField()
    altitude = models.FloatField()
    date = models.DateTimeField(auto_now_add=True)
