//
//  script.cpp
//  HumanPoseDetector
//
//  Created by Lichao Chen on 12/27/12.
//  Copyright (c) 2012 Lichao Chen. All rights reserved.
//

#include "script.h"

int l2distance(string imga, string imgb){
    Mat a = imread(imga);
    Mat b = imread(imgb);
    cout<<"there distance is "<<Feature(a).l2(Feature(b))<<endl;
    return 0;
}

int main(int argc, const char * argv[])
{
    if (argc>3){
        if (string(argv[1])=="l2"){
            string imga = argv[2];
            string imgb = argv[3];
            return l2distance(imga, imgb);
        }
    } else {
        cerr<<"Must have >3 parameters"<<endl;
    }
    
}