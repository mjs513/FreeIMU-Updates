function SpherePlot(X, Y, Z, graph_title)
  figure;
  subplot(2,2,1); plot3(X,Y,Z);
  title(graph_title);
  subplot(2,2,2); plot(X, Y);
  title('Vector projection on XY');
  subplot(2,2,3); plot(Y,Z);
  title('Vector projection on YZ');
  subplot(2,2,4); plot(Z,X);
  title('Vector projection on ZX');
  %print(filename) % .pdf graphs generated
  
end
