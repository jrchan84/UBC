#define CATCH_CONFIG_MAIN
#include "cs221util/catch.hpp"
#include <limits.h>
#include <vector>
#include <sys/stat.h>
#include <iostream>
#include "cs221util/PNG.h"
#include "cs221util/HSLAPixel.h"
#include "vor.h"
#include "dotColorPicker.h"
#include "solidColorPicker.h"
#include "fadeColorPicker.h"

using namespace std;
using namespace cs221util;

// use the following for color picker tests
#define FUNCTORTESTHEIGHT 60
#define FUNCTORTESTWIDTH 60
#define FUNCTORTESTGRIDSPACING 16
#define FUNCTORTESTDOTSIZE 4

// use the following for filler tests
//#define GRIDTESTIMAGE "originals/cloud.png"
#define GRIDTESTIMAGE "originals/leaf.png" // uncomment for given code
//#define GRIDTESTIMAGE "originals/park.png"
#define GRIDFRAMEFREQ 500 // uncomment for given code
//#define GRIDFRAMEFREQ 3000
#define GRIDGRIDSPACING 10
#define DOTSIZE 3

// use for general tests
#define FADERATE 0.95 //pct of L that remains
#define DENSITY 0.15 // uncomment for given code
//#define DENSITY 0.17


PNG testColorPicker(colorPicker& picker)
{
    PNG img;
    img.resize(FUNCTORTESTWIDTH, FUNCTORTESTHEIGHT);
    center ctr(FUNCTORTESTWIDTH/2,FUNCTORTESTHEIGHT/2,HSLAPixel(240,0.5, 0.5));
    HSLAPixel px;

    for (int x = 1; x < FUNCTORTESTWIDTH; x = x + x) {
        for (int y = 1; y < FUNCTORTESTHEIGHT; y = y + y) {
            point p(x,y,ctr,0);
            px = picker(p);
            // these create the output useful for debugging fills
            cout << "\toperator()(" << x << ", " << y << ") = {"
                << px.h << ", "
                << px.s << ", "
                << px.l << "}" << endl;
        }
    }

    for (int x = 0; x < FUNCTORTESTWIDTH; x++){
        for (int y = 0; y < FUNCTORTESTHEIGHT; y++){
            point newpoint(x,y,ctr,0);
            HSLAPixel * p = img.getPixel(x,y);
            * p = picker(newpoint);
        }
    }

    return img;
}

TEST_CASE("colorPicker::basic dot","[weight=1][part=colorPicker]"){
    dotColorPicker dotPicker(FUNCTORTESTGRIDSPACING, FUNCTORTESTDOTSIZE);

    PNG result = testColorPicker(dotPicker);
    result.writeToFile("images/dotColorPickerTest.png");
    PNG expected; expected.readFromFile("soln_images/dotColorPickerTest.png");
    REQUIRE(result == expected);
}

TEST_CASE("colorPicker::basic solid","[weight=1][part=colorPicker]"){
    solidColorPicker solidPicker;

    PNG result = testColorPicker(solidPicker);
    result.writeToFile("images/solidColorPickerTest.png");
    PNG expected; expected.readFromFile("soln_images/solidColorPickerTest.png");
    REQUIRE(result == expected);
}

TEST_CASE("colorPicker::basic fade","[weight=1][part=colorPicker]"){
    fadeColorPicker fadePicker(FADERATE);

    PNG result = testColorPicker(fadePicker);
    result.writeToFile("images/fadeColorPickerTest.png");
    PNG expected; expected.readFromFile("soln_images/fadeColorPickerTest.png");
    REQUIRE(result == expected);
}


TEST_CASE("fill::basic dot dfs","[weight=1][part=fill]"){
    PNG img;
    img.readFromFile(GRIDTESTIMAGE);

    animation anim;

    anim = filler::vorDotDFS(img, DENSITY, GRIDGRIDSPACING, DOTSIZE,
                                     GRIDFRAMEFREQ);

    PNG result = anim.write("images/dfsdotleaf.gif");
    result.writeToFile("images/dfsdotleaf.png");
    PNG expected; expected.readFromFile("soln_images/dfsdotleaf.png");
    REQUIRE(result==expected);
}

TEST_CASE("fill::basic solid dfs","[weight=1][part=fill]"){
    PNG img;
    img.readFromFile(GRIDTESTIMAGE);

    animation anim;

    anim = filler::vorSolidDFS(img, DENSITY, GRIDFRAMEFREQ);

    PNG result = anim.write("images/dfssolidleaf.gif");
    result.writeToFile("images/dfssolidleaf.png");
    PNG expected; expected.readFromFile("soln_images/dfssolidleaf.png");
    REQUIRE(result==expected);
}


TEST_CASE("fill::basic fade dfs","[weight=1][part=fill]"){
    PNG img;
    img.readFromFile(GRIDTESTIMAGE);

    animation anim;

    anim = filler::vorFadeDFS(img, DENSITY, FADERATE,
                                     GRIDFRAMEFREQ);

    PNG result = anim.write("images/dfsfadeleaf.gif");
    result.writeToFile("images/dfsfadeleaf.png");
    PNG expected; expected.readFromFile("soln_images/dfsfadeleaf.png");
    REQUIRE(result==expected);
}

TEST_CASE("fill::basic dot bfs","[weight=1][part=fill]"){
    PNG img;
    img.readFromFile(GRIDTESTIMAGE);

    animation anim;
    anim = filler::vorDotBFS(img, DENSITY, GRIDGRIDSPACING, DOTSIZE,
                                     GRIDFRAMEFREQ);
    PNG result = anim.write("images/bfsdotleaf.gif");
    result.writeToFile("images/bfsdotleaf.png");
    PNG expected; expected.readFromFile("soln_images/bfsdotleaf.png");
    REQUIRE(result==expected);
}

TEST_CASE("fill::basic solid bfs","[weight=1][part=fill]"){
    PNG img;
    img.readFromFile(GRIDTESTIMAGE);

    animation anim;
    anim = filler::vorSolidBFS(img,DENSITY, GRIDFRAMEFREQ);
    PNG result = anim.write("images/bfssolidleaf.gif");
    result.writeToFile("images/bfssolidleaf.png");
    PNG expected; expected.readFromFile("soln_images/bfssolidleaf.png");
    REQUIRE(result==expected);
}


TEST_CASE("fill::basic fade bfs","[weight=1][part=fill]"){
    PNG img;
    img.readFromFile(GRIDTESTIMAGE);

    animation anim;

    anim = filler::vorFadeBFS(img, DENSITY, FADERATE,
                                     GRIDFRAMEFREQ);

    PNG result = anim.write("images/bfsfadeleaf.gif");
    result.writeToFile("images/bfsfadeleaf.png");
    PNG expected; expected.readFromFile("soln_images/bfsfadeleaf.png");
    REQUIRE(result==expected);
}

