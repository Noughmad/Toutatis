from django.conf.urls import patterns
from django.views.generic import ListView
from models import Project

urlpatterns = patterns('',
    (r'projects/', ListView.as_view(model=Project)),
)