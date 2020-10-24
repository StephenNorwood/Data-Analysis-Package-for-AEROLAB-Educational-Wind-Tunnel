# Wind-tunnel-analysis
This software was designed to help with data analysis from a AEROLAB Educational Wind Tunnel.
This program was written to help analysis data from multipule data files output from the AEROLAB Educational Wind Tunnel.
This software does the following. 
Get the input file name from the user, presumbably a file that has come straight from the wind tunnel software. 
Average data points that are next to each other in the file together into one point.
This is done by asking the user how many data points should be considered as one. 1 can be inputed to have no averaging take place.
Get values for the density of air and area of the object being analysized from the user. 
Calculate the coefficents of lift and drag for each instance. 
Apply manufacture provided correction factors to the angle of attack and coefficent of drag outputs. 
Output computed data to a user specified file.

The file TestData1 is a sample output file from the wind tunnel. 
It was taken on a model F-16 fighter jet at a 1:48 scale with a total wing area of 18.75 in^2, and a mean chord length of 2.83 in.
