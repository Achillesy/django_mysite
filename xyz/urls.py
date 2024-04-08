from django.urls import path
from . import views

urlpatterns = [
    path('', views.query_xyz, name='query_xyz'),
]
