from django.db import models

from django_extensions.db.fields import UUIDField, CreationDateTimeField, ModificationDateTimeField

class ToutatisModel(models.Model):
    id = UUIDField(primary_key=True)
    createdAt = CreationDateTimeField()
    modifiedAt = ModificationDateTimeField()
    deletedAt = models.DateTimeField(blank=True, null=True, default=None)
    
    class Meta:
        abstract = True

class Project(ToutatisModel):
    name = models.CharField(max_length=255)
    client = models.CharField(max_length=255)
    
class Task(ToutatisModel):
    project = models.ForeignKey(Project)
    name = models.CharField(max_length=255)
    status = models.CharField(max_length=31)

    active = models.BooleanField(default=False)
    lastStart = models.DateField(blank=True, null=True)
    duration = models.IntegerField()
    
    
class Event(ToutatisModel):
    task = models.ForeignKey(Task)
    type = models.CharField(max_length=255)
    message = models.TextField()
    location = models.CharField(max_length=255)
    
    start = models.DateTimeField()
    start = models.DateTimeField()
    duration = models.IntegerField()
    
class Note(ToutatisModel):
    task = models.ForeignKey(Task)
    title = models.CharField(max_length=255)
    content = models.TextField()
