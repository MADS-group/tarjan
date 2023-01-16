import os
import functools

path = os.path.join(os.path.dirname(os.path.realpath(__file__)),"measure/mpi")
os.chdir(path)
folders = [f for f in os.listdir('.')]
for folder in folders:
  os.chdir(os.path.join(path, folder))
  files = [f for f in os.listdir('.')]
  print("FILES:", files)
  for filename in files:
    with open(filename, 'r') as file:
      head, tail = file.read().split('\n', 1)
    #print(head)
    split = head.split(",")
    #print(split)
    split[-2], split[-3] = split[-3], split[-2]
    #print(split)
    head = functools.reduce(lambda x,y: x+","+y,split) + "\n"
    #print(head)
    with open(filename, 'w') as file:
      file.write(head)
      file.write(tail)
  