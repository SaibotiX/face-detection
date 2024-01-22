#.csv File Reader

Short instruction on how to use it:

1. Create a .c File.

2. Include the csv.h file with #include "csv.h" with the parameters "argv" and "argc" Should look like: csvfunction(argc, argv); (Between the csv[here]function should be a "_" it didnt allow me to write it there)

The output of the function is a "node **" That means you gotta define a variable with that data structure and store it in there. 

3. Now compile your .c File with gcc -o main [Your Filename]

A few options are available:

-s= [For scanning a .csv file]

-w= [For writing a .csv file]

-c= [For specifying the columns you want to write or scan (read a .csv file)]
combined with [-s] you can specifiy where to start and end scanning with the syntax: -c=[input from]:[input to]:[result].

-r= [For specifying the rows you want to write or scan]
combined with [-s] you can specifiy where to start and end scanning with the syntax: -r=[input from]:[input to]:[result]

Further implementations will be made if at least one demand is out there. Else I will just use it myself.
