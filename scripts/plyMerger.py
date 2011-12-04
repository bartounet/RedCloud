import shutil
import os
import sys
import linecache


def plyFusion(ply_dir, output_ply):
    #ply_dir = sys.argv[1]
    #output_ply = sys.argv[2]
    if (not os.path.isdir(ply_dir)) :
      print "Error : '%s' is not a directory" % ply_dir
      sys.exit(1)

    files = os.listdir(ply_dir)
    ply_files = []
    for f in files :
      if os.path.splitext(f)[1] == ".ply" :
        ply_files.append(os.path.join(ply_dir, f))

    nb_vertex = 0
    for f in ply_files :
      print f
      line = linecache.getline(f, 3)
      print line
      fields = line.split()
      nb_vertex = nb_vertex + int(fields[2])

    print nb_vertex

    output = open(output_ply, 'w')
    output.write("ply\n")
    output.write("format ascii 1.0\n")
    output.write("element vertex " + str(nb_vertex) + "\n")
    pf = ["x", "y", "z", "nx", "ny", "nz"]
    pu = ["red", "green", "blue"]

    for p in pf :
      output.write("property float " + p + "\n")
    for p in pu :
      output.write("property uchar diffuse_" + p + "\n")
    output.write("end_header\n")

    for f in ply_files :
      i = 0
      line = ""
      while line != "end_header\n" and line != "end_header\r\n" :
        i = i + 1
        line = linecache.getline(f, i)
      while True :
        i = i + 1
        line = linecache.getline(f, i)
        if (line == "" ) :
          break;
        output.write(line)

    output.close()
