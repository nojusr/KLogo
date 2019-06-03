#ifndef MATH_PRIMITIVES_H
#define MATH_PRIMITIVES_H

int add (int left, int right){
    return left+right;
}

int sub(int left, int right){
    return left-right;
}

int divide(int left, int right){
    return left/right;
}

int mul(int left, int right){
    return left*right;
}

int power(int left, int right){
    int buf = left;
    for (int i = 1; i <= right; i++){
        buf *= buf;
    }
    
    return buf;
}

int mod(int left, int right){
    return left % right;
}



#endif
