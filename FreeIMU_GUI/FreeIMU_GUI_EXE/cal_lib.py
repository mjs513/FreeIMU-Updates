"""
cal_lib.py - Ellipsoid into Sphere calibration library based upon numpy and linalg
Copyright (C) 2012 Fabio Varesano <fabio at varesano dot net>

Development of this code has been supported by the Department of Computer Science,
Universita' degli Studi di Torino, Italy within the Piemonte Project
http://www.piemonte.di.unito.it/


This program is free software: you can redistribute it and/or modify
it under the terms of the version 3 GNU General Public License as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

"""

import numpy
from numpy import linalg



def calibrate(x, y, z):
  # H = numpy.array([x, y, z, -y**2, -z**2, numpy.ones([len(x), 1])])
  H = numpy.array([x, y, z, -y**2, -z**2, numpy.ones([len(x)])])
  H = numpy.transpose(H)
  w = x**2
  
  (X, residues, rank, shape) = linalg.lstsq(H, w)
  
  OSx = X[0] / 2
  OSy = X[1] / (2 * X[3])
  OSz = X[2] / (2 * X[4])
  
  A = X[5] + OSx**2 + X[3] * OSy**2 + X[4] * OSz**2
  B = A / X[3]
  C = A / X[4]
  
  SCx = numpy.sqrt(A)
  SCy = numpy.sqrt(B)
  SCz = numpy.sqrt(C)
  
  # type conversion from numpy.float64 to standard python floats
  offsets = [OSx, OSy, OSz]
  scale = [SCx, SCy, SCz]
  
  offsets = map(numpy.asscalar, offsets)
  scale = map(numpy.asscalar, scale)
  
  return (offsets, scale)


def calibrate_from_file(file_name):
  samples_f = open(file_name, 'r')
  samples_x = []
  samples_y = []
  samples_z = []
  for line in samples_f:
    reading = line.split()
    if len(reading) == 3:
      samples_x.append(int(reading[0]))
      samples_y.append(int(reading[1]))
      samples_z.append(int(reading[2]))

  return calibrate(numpy.array(samples_x), numpy.array(samples_y), numpy.array(samples_z))


def compute_calibrate_data(data, offsets, scale):
  output = [[], [], []]
  for i in range(len(data[0])):
    output[0].append((data[0][i] - offsets[0]) / scale[0])
    output[1].append((data[1][i] - offsets[1]) / scale[1])
    output[2].append((data[2][i] - offsets[2]) / scale[2])
  return output


if __name__ == "__main__":
  
  print "Calibrating from acc.txt"
  (offsets, scale) = calibrate_from_file("acc.txt")
  print "Offsets:"
  print offsets
  print "Scales:"
  print scale
  
  print "Calibrating from magn.txt"
  (offsets, scale) = calibrate_from_file("magn.txt")
  print "Offsets:"
  print offsets
  print "Scales:"
  print scale
