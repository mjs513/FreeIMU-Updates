%  Accelerometer calibration by Fabio Varesano - varesano.net
%  Adapted from AN3192 by ST Microsistems
%
%  The code should run both in Octave and Matlab
%
%  Development of this code has been supported by the Department of Computer Science,
%  Universita' degli Studi di Torino, Italy within the Piemonte Project
%  http://www.piemonte.di.unito.it/
%
%
%  This program is free software: you can redistribute it and/or modify
%  it under the terms of the version 3 GNU General Public License as
%  published by the Free Software Foundation.
%  
%  This program is distributed in the hope that it will be useful,
%  but WITHOUT ANY WARRANTY; without even the implied warranty of
%  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
%  GNU General Public License for more details.
%  
%  You should have received a copy of the GNU General Public License
%  along with this program.  If not, see <http://www.gnu.org/licenses/>.


% The following table shows accelerometer expected values in 6 stationary points
% 
% -----------------------------------------------
% | Stationary Position | Acc_x | Acc_y | Acc_z |
% -----------------------------------------------
% | Z up                |   0   |   0   |   1g  |
% -----------------------------------------------
% | Z down              |   0   |   0   |  -1g  |
% |----------------------------------------------
% | Y up                |   0   |   1g  |   0   |
% -----------------------------------------------
% | Y down              |   0   |  -1g  |   0   |
% |----------------------------------------------
% | X up                |   1g  |   0   |   0   |
% |----------------------------------------------
% | X down              |  -1g  |   0   |   0   |
% -----------------------------------------------

% From the table above we obtain:
Y_1 = [0  0  1];
Y_2 = [0  0 -1];
Y_3 = [0  1  0];
Y_4 = [0 -1  0];
Y_5 = [1  0  0];
Y_6 = [-1 0  0];

%Scale_factor = 256;

Y = [Y_1; Y_2; Y_3; Y_4; Y_5; Y_6]; %.* Scale_factor;

% reading raw accelerometer readings collected by calibrate.py
data = dlmread('acc.txt', ' ') % expecting a 6 lines file with each line as "Ax Ay Az"

Acc_raw_x = data(:,1);
Acc_raw_y = data(:,2);
Acc_raw_z = data(:,3);

w = [Acc_raw_x Acc_raw_y Acc_raw_z ones(6,1)];

%w_T = w.'; % matrix traspose

%A = inv(w_T * w) * w_T;

X = w \ Y

Cal = w * X
