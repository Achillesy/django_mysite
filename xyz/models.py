from django.db import models

# Create your models here.
class AccelerometerData(models.Model):
    x = models.FloatField()
    y = models.FloatField()
    z = models.FloatField()
    date = models.DateTimeField(auto_now_add=True)

    class Meta:
        app_label = 'xyz'
