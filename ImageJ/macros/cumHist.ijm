// Cumulative Histogram 
getRawStatistics(area, mean, min, max, std, h);
for (i=1;i< h.length;i++) h[i] = h[i-1]+h[i]; 
for (i=0;i< h.length;i++) h[i] = h[i]/187500;
Plot.create("Cumulative Histogram of "+getTitle, "GrayScale Intensity", "CDF",h);
Plot.show(); 
// end 
