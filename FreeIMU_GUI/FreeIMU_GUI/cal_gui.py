"""
cal_gui.py - Calibration GUI for FreeIMU boards
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

import sys, os
from PyQt4.QtGui import QApplication, QDialog, QMainWindow, QCursor, QFileDialog
from ui_freeimu_cal import Ui_FreeIMUCal
from PyQt4.QtCore import Qt,QObject, pyqtSlot, QThread, QSettings, SIGNAL
import numpy as np
import serial, time
from struct import unpack, pack
from binascii import unhexlify
from subprocess import call
import pyqtgraph.opengl as gl
import cal_lib, numpy

acc_file_name = "acc.txt"
magn_file_name = "magn.txt"
calibration_h_file_name = "calibration.h"

acc_range = 25000
magn_range = 1000

class FreeIMUCal(QMainWindow, Ui_FreeIMUCal):
  def __init__(self):
    QMainWindow.__init__(self)

    # Set up the user interface from Designer.
    self.setupUi(self)
    
    # load user settings
    self.settings = QSettings("FreeIMU Calibration Application", "Fabio Varesano")
    # restore previous serial port used
    self.serialPortEdit.setText(self.settings.value("calgui/serialPortEdit", "").toString())
    
    # when user hits enter, we generate the clicked signal to the button so that connection starts
    self.connect(self.serialPortEdit, SIGNAL("returnPressed()"), self.connectButton, SIGNAL("clicked()"))
    
    # Connect up the buttons to their functions
    self.connectButton.clicked.connect(self.serial_connect)
    self.samplingToggleButton.clicked.connect(self.sampling_start)
    self.set_status("Disconnected")
    
    # data storages
    self.acc_data = [[], [], []]
    self.magn_data = [[], [], []]
    
    # setup graphs
    self.accXY.setXRange(-acc_range, acc_range)
    self.accXY.setYRange(-acc_range, acc_range)
    self.accYZ.setXRange(-acc_range, acc_range)
    self.accYZ.setYRange(-acc_range, acc_range)
    self.accZX.setXRange(-acc_range, acc_range)
    self.accZX.setYRange(-acc_range, acc_range)
    
    self.accXY.setAspectLocked()
    self.accYZ.setAspectLocked()
    self.accZX.setAspectLocked()
    
    self.magnXY.setXRange(-magn_range, magn_range)
    self.magnXY.setYRange(-magn_range, magn_range)
    self.magnYZ.setXRange(-magn_range, magn_range)
    self.magnYZ.setYRange(-magn_range, magn_range)
    self.magnZX.setXRange(-magn_range, magn_range)
    self.magnZX.setYRange(-magn_range, magn_range)
    
    self.magnXY.setAspectLocked()
    self.magnYZ.setAspectLocked()
    self.magnZX.setAspectLocked()
    
    self.accXY_cal.setXRange(-1.5, 1.5)
    self.accXY_cal.setYRange(-1.5, 1.5)
    self.accYZ_cal.setXRange(-1.5, 1.5)
    self.accYZ_cal.setYRange(-1.5, 1.5)
    self.accZX_cal.setXRange(-1.5, 1.5)
    self.accZX_cal.setYRange(-1.5, 1.5)
    
    self.accXY_cal.setAspectLocked()
    self.accYZ_cal.setAspectLocked()
    self.accZX_cal.setAspectLocked()
    
    self.magnXY_cal.setXRange(-1.5, 1.5)
    self.magnXY_cal.setYRange(-1.5, 1.5)
    self.magnYZ_cal.setXRange(-1.5, 1.5)
    self.magnYZ_cal.setYRange(-1.5, 1.5)
    self.magnZX_cal.setXRange(-1.5, 1.5)
    self.magnZX_cal.setYRange(-1.5, 1.5)
    
    self.magnXY_cal.setAspectLocked()
    self.magnYZ_cal.setAspectLocked()
    self.magnZX_cal.setAspectLocked()
    
    self.acc3D.opts['distance'] = 30000
    self.acc3D.show()
    
    self.magn3D.opts['distance'] = 2000
    self.magn3D.show()
    
    ax = gl.GLAxisItem()
    ax.setSize(x=20000, y=20000, z=20000)
    self.acc3D.addItem(ax)
    
    mx = gl.GLAxisItem()
    mx.setSize(x=1000, y=1000, z=1000)
    self.magn3D.addItem(ax)
    
    self.acc3D_sp = gl.GLScatterPlotItem()
    self.acc3D.addItem(self.acc3D_sp)
    
    self.magn3D_sp = gl.GLScatterPlotItem()
    self.magn3D.addItem(self.magn3D_sp)
    
    # axis for the cal 3D graph
    g_a = gl.GLAxisItem()
    g_a.setSize(x=10000, y=10000, z=10000)
    self.acc3D_cal.addItem(g_a)
    g_m = gl.GLAxisItem()
    g_m.setSize(x=1000, y=1000, z=1000)
    self.magn3D_cal.addItem(g_m)
    

  def set_status(self, status):
    self.statusbar.showMessage(self.tr(status))

  def serial_connect(self):
    self.serial_port = str(self.serialPortEdit.text())
    # save serial value to user settings
    self.settings.setValue("calgui/serialPortEdit", self.serial_port)
    
    self.connectButton.setEnabled(False)
    # waiting mouse cursor
    QApplication.setOverrideCursor(QCursor(Qt.WaitCursor))
    self.set_status("Connecting to " + self.serial_port + " ...")
    
    # TODO: serial port field input validation!
    
    try:
      self.ser = serial.Serial(
        port= self.serial_port,
        baudrate=115200,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS
      )
      
      if self.ser.isOpen():
        print "Arduino serial port opened correctly"
        self.set_status("Connection Successfull. Waiting for Arduino reset...")

        # wait for arduino reset on serial open
        time.sleep(3)
        
        self.ser.write('v') # ask version
        self.set_status("Connected to: " + self.ser.readline()) # TODO: hangs if a wrong serial protocol has been loaded. To be fixed.
        
        self.connectButton.setText("Disconnect")
        self.connectButton.clicked.connect(self.serial_disconnect)
        self.serialPortEdit.setEnabled(False)
        self.serialProtocol.setEnabled(False)
        
        self.samplingToggleButton.setEnabled(True)
        
        self.clearCalibrationEEPROMButton.setEnabled(True)
        self.clearCalibrationEEPROMButton.clicked.connect(self.clear_calibration_eeprom)
        
    except serial.serialutil.SerialException, e:
      self.connectButton.setEnabled(True)
      self.set_status("Impossible to connect: " + str(e))
      
    # restore mouse cursor
    QApplication.restoreOverrideCursor()
    self.connectButton.setEnabled(True)

    
  def serial_disconnect(self):
    print "Disconnecting from " + self.serial_port
    self.ser.close()
    self.set_status("Disconnected")
    self.serialPortEdit.setEnabled(True)
    self.serialProtocol.setEnabled(True)
    
    self.connectButton.setText("Connect")
    self.connectButton.clicked.disconnect(self.serial_disconnect)
    self.connectButton.clicked.connect(self.serial_connect)
    
    self.samplingToggleButton.setEnabled(False)
    
    self.clearCalibrationEEPROMButton.setEnabled(False)
    self.clearCalibrationEEPROMButton.clicked.disconnect(self.clear_calibration_eeprom)
      
  def sampling_start(self):
    self.serWorker = SerialWorker(ser = self.ser)
    self.connect(self.serWorker, SIGNAL("new_data(PyQt_PyObject)"), self.newData)
    self.serWorker.start()
    print "Starting SerialWorker"
    self.samplingToggleButton.setText("Stop Sampling")
    
    self.samplingToggleButton.clicked.disconnect(self.sampling_start)
    self.samplingToggleButton.clicked.connect(self.sampling_end)
    
  def sampling_end(self):
    self.serWorker.exiting = True
    self.serWorker.quit()
    self.serWorker.wait()
    self.samplingToggleButton.setText("Start Sampling")
    self.samplingToggleButton.clicked.disconnect(self.sampling_end)
    self.samplingToggleButton.clicked.connect(self.sampling_start)
    
    self.calibrateButton.setEnabled(True)
    self.calAlgorithmComboBox.setEnabled(True)
    self.calibrateButton.clicked.connect(self.calibrate)
    
  
  def calibrate(self):
    # read file and run calibration algorithm
    (self.acc_offset, self.acc_scale) = cal_lib.calibrate_from_file(acc_file_name)
    (self.magn_offset, self.magn_scale) = cal_lib.calibrate_from_file(magn_file_name)
    
    # map floats into integers
    self.acc_offset = map(int, self.acc_offset)
    self.magn_offset = map(int, self.magn_offset)
    
    # show calibrated tab
    self.tabWidget.setCurrentIndex(1)
    
    #populate acc calibration output on gui
    self.calRes_acc_OSx.setText(str(self.acc_offset[0]))
    self.calRes_acc_OSy.setText(str(self.acc_offset[1]))
    self.calRes_acc_OSz.setText(str(self.acc_offset[2]))
    
    self.calRes_acc_SCx.setText(str(self.acc_scale[0]))
    self.calRes_acc_SCy.setText(str(self.acc_scale[1]))
    self.calRes_acc_SCz.setText(str(self.acc_scale[2]))
    
    #populate acc calibration output on gui
    self.calRes_magn_OSx.setText(str(self.magn_offset[0]))
    self.calRes_magn_OSy.setText(str(self.magn_offset[1]))
    self.calRes_magn_OSz.setText(str(self.magn_offset[2]))
    
    self.calRes_magn_SCx.setText(str(self.magn_scale[0]))
    self.calRes_magn_SCy.setText(str(self.magn_scale[1]))
    self.calRes_magn_SCz.setText(str(self.magn_scale[2]))
    
    # compute calibrated data
    self.acc_cal_data = cal_lib.compute_calibrate_data(self.acc_data, self.acc_offset, self.acc_scale)
    self.magn_cal_data = cal_lib.compute_calibrate_data(self.magn_data, self.magn_offset, self.magn_scale)
    
    # populate 2D graphs with calibrated data
    self.accXY_cal.plot(x = self.acc_cal_data[0], y = self.acc_cal_data[1], clear = True, pen='r')
    self.accYZ_cal.plot(x = self.acc_cal_data[1], y = self.acc_cal_data[2], clear = True, pen='g')
    self.accZX_cal.plot(x = self.acc_cal_data[2], y = self.acc_cal_data[0], clear = True, pen='b')
    
    self.magnXY_cal.plot(x = self.magn_cal_data[0], y = self.magn_cal_data[1], clear = True, pen='r')
    self.magnYZ_cal.plot(x = self.magn_cal_data[1], y = self.magn_cal_data[2], clear = True, pen='g')
    self.magnZX_cal.plot(x = self.magn_cal_data[2], y = self.magn_cal_data[0], clear = True, pen='b')
    
    # populate 3D graphs with calibrated data
    acc3D_cal_data = np.array(self.acc_cal_data).transpose()
    magn3D_cal_data = np.array(self.magn_cal_data).transpose()
    
    sp = gl.GLScatterPlotItem(pos=acc3D_cal_data, color = (1, 1, 1, 1), size=2)
    self.acc3D_cal.addItem(sp)
    
    sp = gl.GLScatterPlotItem(pos=magn3D_cal_data, color = (1, 1, 1, 1), size=2)
    self.magn3D_cal.addItem(sp)
    
    #enable calibration buttons to activate calibration storing functions
    self.saveCalibrationHeaderButton.setEnabled(True)
    self.saveCalibrationHeaderButton.clicked.connect(self.save_calibration_header)
    
    self.saveCalibrationEEPROMButton.setEnabled(True)
    self.saveCalibrationEEPROMButton.clicked.connect(self.save_calibration_eeprom)
    
    
  def save_calibration_header(self):
    text = """
/**
 * FreeIMU calibration header. Automatically generated by FreeIMU_GUI.
 * Do not edit manually unless you know what you are doing.
*/


#define CALIBRATION_H

const int acc_off_x = %d;
const int acc_off_y = %d;
const int acc_off_z = %d;
const float acc_scale_x = %f;
const float acc_scale_y = %f;
const float acc_scale_z = %f;

const int magn_off_x = %d;
const int magn_off_y = %d;
const int magn_off_z = %d;
const float magn_scale_x = %f;
const float magn_scale_y = %f;
const float magn_scale_z = %f;
"""
    calibration_h_text = text % (self.acc_offset[0], self.acc_offset[1], self.acc_offset[2], self.acc_scale[0], self.acc_scale[1], self.acc_scale[2], self.magn_offset[0], self.magn_offset[1], self.magn_offset[2], self.magn_scale[0], self.magn_scale[1], self.magn_scale[2])
    
    calibration_h_folder = QFileDialog.getExistingDirectory(self, "Select the Folder to which save the calibration.h file")
    calibration_h_file = open(os.path.join(str(calibration_h_folder), calibration_h_file_name), "w")
    calibration_h_file.write(calibration_h_text)
    calibration_h_file.close()
    
    self.set_status("Calibration saved to: " + str(calibration_h_folder) + calibration_h_file_name + " .\nRecompile and upload the program using the FreeIMU library to your microcontroller.")
  
  def save_calibration_eeprom(self):
    self.ser.write("c")
    # pack data into a string 
    offsets = pack('<hhhhhh', self.acc_offset[0], self.acc_offset[1], self.acc_offset[2], self.magn_offset[0], self.magn_offset[1], self.magn_offset[2])
    scales = pack('<ffffff', self.acc_scale[0], self.acc_scale[1], self.acc_scale[2], self.magn_scale[0], self.magn_scale[1], self.magn_scale[2])
    # transmit to microcontroller
    self.ser.write(offsets)
    self.ser.write(scales)
    self.set_status("Calibration saved to microcontroller EEPROM.")
    # debug written values to console
    print "Calibration values read back from EEPROM:"
    self.ser.write("C")
    for i in range(4):
      print self.ser.readline()
      
      
  def clear_calibration_eeprom(self):
    self.ser.write("x")
    # no feedback expected. we assume success.
    self.set_status("Calibration cleared from microcontroller EEPROM.")
    

  def newData(self, reading):
    
    # only display last reading in burst
    self.acc_data[0].append(reading[0])
    self.acc_data[1].append(reading[1])
    self.acc_data[2].append(reading[2])
    
    self.magn_data[0].append(reading[6])
    self.magn_data[1].append(reading[7])
    self.magn_data[2].append(reading[8])
    
    
    self.accXY.plot(x = self.acc_data[0], y = self.acc_data[1], clear = True, pen='r')
    self.accYZ.plot(x = self.acc_data[1], y = self.acc_data[2], clear = True, pen='g')
    self.accZX.plot(x = self.acc_data[2], y = self.acc_data[0], clear = True, pen='b')
    
    self.magnXY.plot(x = self.magn_data[0], y = self.magn_data[1], clear = True, pen='r')
    self.magnYZ.plot(x = self.magn_data[1], y = self.magn_data[2], clear = True, pen='g')
    self.magnZX.plot(x = self.magn_data[2], y = self.magn_data[0], clear = True, pen='b')
    
    acc_pos = numpy.array([self.acc_data[0],self.acc_data[1],self.acc_data[2]]).transpose()
    self.acc3D_sp.setData(pos=acc_pos, color = (1, 1, 1, 1), size=2)
    
    magn_pos = numpy.array([self.magn_data[0],self.magn_data[1],self.magn_data[2]]).transpose()
    self.magn3D_sp.setData(pos=magn_pos, color = (1, 1, 1, 1), size=2)


class SerialWorker(QThread):
  def __init__(self, parent = None, ser = None):
    QThread.__init__(self, parent)
    self.exiting = False
    self.ser = ser
    
    
    
  def run(self):
    print "sampling start.."
    self.acc_file = open(acc_file_name, 'w')
    self.magn_file = open(magn_file_name, 'w')
    count = 100
    in_values = 9
    reading = [0.0 for i in range(in_values)]
    while not self.exiting:
      self.ser.write('b')
      self.ser.write(chr(count))
      for j in range(count):
        for i in range(in_values):
          reading[i] = unpack('h', self.ser.read(2))[0]
        self.ser.read(2) # consumes remaining '\r\n'
        # prepare readings to store on file
        acc_readings_line = "%d %d %d\r\n" % (reading[0], reading[1], reading[2])
        self.acc_file.write(acc_readings_line)
        magn_readings_line = "%d %d %d\r\n" % (reading[6], reading[7], reading[8])
        self.magn_file.write(magn_readings_line)
      # every count times we pass some data to the GUI
      self.emit(SIGNAL("new_data(PyQt_PyObject)"), reading)
      print ".",
    # closing acc and magn files
    self.acc_file.close()
    self.magn_file.close()
    return 
  
  def __del__(self):
    self.exiting = True
    self.wait()
    print "SerialWorker exits.."


app = QApplication(sys.argv)
window = FreeIMUCal()

window.show()
sys.exit(app.exec_())