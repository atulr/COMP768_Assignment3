//
//  myVec3.h
//  getting_started
//
//  Created by Atul Rungta on 11/12/14.
//
//

#ifndef getting_started_myVec3_h
#define getting_started_myVec3_h

#include <math.h>

class myVec3 {
public:
    
    float vec[3];
    
    myVec3(float x, float y, float z);
    
    myVec3() {};
    
    myVec3 add(myVec3 vec);
    
    myVec3 sub(myVec3 vec);
    
    float dot(myVec3 vec);
    
    myVec3 scalarMultiply(float scalar);
    
    void normalize();
    
    float mod();
    
    void reset(float x, float y, float z);
    
};

#endif
