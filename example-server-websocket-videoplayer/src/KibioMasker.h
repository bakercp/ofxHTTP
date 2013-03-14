#pragma once

#include <vector>

#include "ofPolyline.h"

using std::vector;

class KibioBaseMask {
public:
    KibioBaseMask();
    virtual ~KibioBaseMask();
    
    
};


class KibioMasker {
public:
    enum MaskType {
        IMAGE,
        POLYGONS
    };
    
    
    KibioMasker();
    virtual ~KibioMasker();
    
    
protected:
    
    MaskType maskType;
    
    vector<ofPolyline> polylines;
    
};