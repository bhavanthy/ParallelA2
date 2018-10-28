Bhavanthy Modchalingam (0896901)

Size    Processes
        1         2         3          4
100	    0.000032  0.000081  0.006956   0.007632
1000    0.003471  0.006093  0.052171   0.041846
10000   0.300145  0.337275  0.625242   2.264136
20000   1.249121  9.716028  27.112430  13.615010

tested using 
mpiexec -n {numberOfProcesses} ./a2  {numberOfProcesses}  {size}
mpiexec -n 1 ./a2 -g

