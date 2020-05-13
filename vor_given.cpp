/**
 * @file vor_given.cpp
 * Defintion of a vor(onoi) namespace.
 *
 */
#ifndef _VOR_GIVEN_H_
#define _VOR_GIVEN_H_

vector<center> filler::randSample(PNG& img, double density){                                          

    vector<center > centers;

    random_device rd;  
    mt19937 gen(rd()); 
    uniform_int_distribution<> dis1(0, img.width()-1);
    uniform_int_distribution<> dis2(0, img.height()-1);


    for (auto i = 0; i < density * img.width(); i++){
        for (auto j = 0; j < density * img.height(); j++) {
            //int x = dis1(gen); // uncomment this line for random effects
            //int y = dis2(gen); // uncomment this line for random effects
            int x = i / density; // comment this line for random effects
            int y = j / density; // comment this line for random effects
            HSLAPixel c = *(img.getPixel(x,y));
            centers.push_back(center(x,y,c));
        }
    }

    return centers;
}
#endif
