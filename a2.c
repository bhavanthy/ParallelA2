/* C Example */
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
 
void printVector(int * vector, int size){
  printf("[ ");
  for(int i = 0; i < size; i++){
    printf("%d  ",vector[i]);
  }
  printf("]\n");
}
void printMarix(int ** matrix, int size){
  for(int i = 0; i < size; i++){
    printf("[");
    for(int j = 0; j < size; j++){
     printf("%d  ", matrix[j][i]);
    }
    printf("]\n");
  }
}
int mutiMatrix(int * vector1, int * vector2, int size){
  int sum = 0;
  for(int i = 0; i < size; i++){
    sum += vector1[i]*vector2[i];
  }
  return sum;
}

int main (int argc, char* argv[]){
  int * vectorA, * vectorC, * result;
  int ** matrixB;
  int rank, size, cm_size, amount, pNum, resNum = 0;

  cm_size = (int) strtol(argv[1], NULL, 10);
  size = (int) strtol(argv[2],NULL, 10);
  amount = size/cm_size;

  MPI_Init(NULL,NULL);      /* starts MPI */
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);        /* get current process id */
  MPI_Comm_size(MPI_COMM_WORLD, &cm_size);        /* get number of processes */

  
  if (rank == 0) {
    vectorA = malloc(sizeof(int)*size);
    vectorC = malloc(sizeof(int)*amount);
    result = malloc(sizeof(int)*size);
    matrixB = malloc(sizeof(int*)*size);
    for(int i = 0; i < size; i++){
        matrixB[i]=malloc(sizeof(int)*size);
    }
    
    
    for(int i = 0; i < size; i++){
      for(int j = 0; j < size; j++){
        matrixB[i][j]=(random()%10)+1;
      }
      vectorA[i]=(random()%10)+1;
    }
    printf("Vector A: ");
    printVector(vectorA, size);
    printf("Matrix B: \n");
    printMarix(matrixB, size);
  
    for(pNum = 1; pNum < cm_size; pNum++){
      MPI_Send(vectorA, size, MPI_INT,pNum,0,MPI_COMM_WORLD);
      MPI_Send(matrixB[(pNum-1)*amount], size*(amount+1), MPI_INT,pNum,0,MPI_COMM_WORLD);
    }

    for(int i =(cm_size-1)* amount; i < size; i++){
      result[i] = mutiMatrix(matrixB[i],vectorA,size);
    }


    for(pNum = 1; pNum < cm_size; pNum++){
      MPI_Recv(vectorC, amount, MPI_INT, pNum,0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      
      for(int i = 0; i < amount; i++){
        result[resNum] = vectorC[i];
        resNum++;
      }
      
    }

    printf("Result: ");
    printVector(result,size);
    free(vectorA);
    for(int i = 0; i < amount; i++){
        free(matrixB[i]);
    }
    free(matrixB);
    free(vectorC);
    free(result);
    
    
  }
  else {
    vectorA = malloc(sizeof(int)*size);
    matrixB = malloc(sizeof(int*)*amount);
    vectorC = malloc(sizeof(int)*amount);
    for(int i = 0; i < amount; i++){
        matrixB[i]=malloc(sizeof(int)*size);
    }
    MPI_Recv(vectorA, size, MPI_INT, 0,0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(matrixB[0], size*(amount+1), MPI_INT, 0,0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    
    for(int i = 0; i < amount; i++){
      vectorC[i]=mutiMatrix(matrixB[i],vectorA,size);
    }
    MPI_Send(vectorC, amount, MPI_INT,0,0,MPI_COMM_WORLD);
    free(vectorA);
    for(int i = 0; i < amount; i++){
        free(matrixB[i]);
    }
    free(matrixB);
    free(vectorC);
  }
  

  MPI_Finalize();
  return 0;
}
