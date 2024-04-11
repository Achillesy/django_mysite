# Generated by Django 5.0.4 on 2024-04-11 01:46

from django.db import migrations, models


class Migration(migrations.Migration):

    initial = True

    dependencies = [
    ]

    operations = [
        migrations.CreateModel(
            name='GY80Data',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('acc_x', models.FloatField()),
                ('acc_y', models.FloatField()),
                ('acc_z', models.FloatField()),
                ('gyro_x', models.FloatField()),
                ('gyro_y', models.FloatField()),
                ('gyro_z', models.FloatField()),
                ('mag_x', models.FloatField()),
                ('mag_y', models.FloatField()),
                ('mag_z', models.FloatField()),
                ('temperature', models.FloatField()),
                ('pressure', models.FloatField()),
                ('altitude', models.FloatField()),
                ('date', models.DateTimeField(auto_now_add=True)),
            ],
        ),
    ]
