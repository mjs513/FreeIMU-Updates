function [OSx, OSy, OSz, SCx, SCy, SCz] = Ellipsoid_to_Sphere(data1)
  
  SpherePlot(data1(:,1), data1(:,2), data1(:,3), 'Uncalibrated data');

  %X=pinv([data(:,1) data(:,2) data(:,3) -data(:,2).^2 -data(:,3).^2 ones(size(data,1),1)])*(data(:,1).^2);
  %[data(:,2) -data(:,2).^2 -(data(:,2).^2)]
  H = [data1(:,1) data1(:,2) data1(:, 3) -data1(:,2).^2 -data1(:,3).^2 ones(size(data1,1),1)];
  w = data1(:,1).^2;
  X = H \ w;
  OSx=X(1)/2;
  OSy=X(2)/(2*X(4));
  OSz=X(3)/(2*X(5));
  A=X(6)+OSx^2+X(4)*OSy^2+X(5)*OSz^2;
  B=A/X(4);
  C=A/X(5);
  SCx=sqrt(A);
  SCy=sqrt(B);
  SCz=sqrt(C);

  xx=data1(:,1)-OSx;
  yy=data1(:,2)-OSy;
  zz=data1(:,3)-OSz;

  xxx=xx./SCx;
  yyy=yy./SCy;
  zzz=zz./SCz;

  SpherePlot(xxx, yyy, zzz, 'Calibrated data');
  
end

