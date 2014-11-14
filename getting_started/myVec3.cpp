//
//  myVec3.cpp
//  getting_started
//
//  Created by Atul Rungta on 11/12/14.
//
//

#include "myVec3.h"
#include <malloc/malloc.h>

myVec3::myVec3(float x, float y, float z) {
    vec[0] = x; vec[1] = y; vec[2] = z;
}

myVec3 myVec3::add(myVec3 otherVec) {
    myVec3 temp;
    
    temp.vec[0] = vec[0] + otherVec.vec[0];
    temp.vec[1] = vec[1] + otherVec.vec[1];
    temp.vec[2] = vec[2] + otherVec.vec[2];
    
    return temp;
}

void myVec3::normalize() {
    
    float mag = mod();
    vec[0]/=mag;
    vec[1]/=mag;
    vec[2]/=mag;

}

myVec3 myVec3::sub(myVec3 otherVec) {
    myVec3 temp;
    
    temp.vec[0] = vec[0] - otherVec.vec[0];
    temp.vec[1] = vec[1] - otherVec.vec[1];
    temp.vec[2] = vec[2] - otherVec.vec[2];
    
    return temp;
}

float myVec3::dot(myVec3 otherVec) {
    
    float dotProduct;
    
    dotProduct = vec[0] * otherVec.vec[0] + vec[1] * otherVec.vec[1] + vec[2] * otherVec.vec[2];
    
    return dotProduct;
}

myVec3 myVec3::scalarMultiply(float scalar) {
    myVec3 temp;
    
    temp.vec[0] = vec[0] * scalar;
    temp.vec[1] = vec[1] * scalar;
    temp.vec[2] = vec[2] * scalar;
    
    return temp;
    
}

float myVec3::mod() {
    float absVal;
    
    absVal = sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
    
    return absVal;
}
