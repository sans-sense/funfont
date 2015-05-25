# Problem  
Use font recognition and classification to provide better search and classification service in fontli

# Uniqueness
## Pros  
1. Image Quality - image quality is not a problem

## Cons  
1. handlettering is common  
1. A lot of fonts and new ones are coming up

## Examples  
55131c3210daa5bd88000001_large is mozart written in a artistic way, whcih is very legible for people but not tesseract. 

# Iterations

## Iteration 1
1. customize tesseract, some cases promising, for example for 
[http://courseweb.stthomas.edu/mjodonnell/j105/type2.pdf](basic type information)

Key identifiers - Axis, terminals, Serifs, stroke
x-height
elephant foot serifs, designed to look "chiseled", mad to look drawn rather than written.

# Alternatives
Others who do similar things
1. https://www.myfonts.com/WhatTheFont/
1. http://www.whatfontis.com/

# State of Art
## Classficaition

### Vox-ATypI classification
Source:http://en.wikipedia.org/wiki/Vox-ATypI_classification
#### Traits  
downstroke, upstroke
forms of serifs
stroke axis
x-height
#### Cons  
Too roman focused

### Modern classficaition
adobe classfication based on ATypI 
http://www.adobe.com/type/browser/classifications.html

google classfication based on simplicity :)
Serif, Sans Serif, Display, Handwriting, Monospace



## Visual Characteristics
Source : http://www.fonts2u.com/dejavu-sans-extralight.font
Family type:Latin (text and display)
Serif style:Normal sans
Arm style:Straight arms / vertical
Letter form:Normal / contact
Weight:Very light
Proportion:Modern
Contrast:Very low
Stroke variation:Rapid / horizontal
Midline:Standard / trimmed
X-height (corpus size):Constant / large



## Build Instructions  
Many of the available versions are a bit obsolete, so a fresh build from source makes more sense, which does complicate the build a bit.

cd ocvrelease/
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local /data/work/ss-git/fun/pic2perfect/sandbox/opencv
make -j5
sudo make install

cd leptonica-1.72/
./make-for-local
make

g++ -ltesseract -o fontRecog fontRecog.cpp -llept -lz -lpng -ljpeg -ltiff && fontRecog /data/work/ss-git/fun/fontRecog/data/images/55131c3210daa5bd88000001_large.jpg

git clone https://code.google.com/p/tesseract-ocr/
cd tesseract-ocr/
./autogen.sh 
./configure --prefix=/data/work/ss-git/fun/pic2perfect/sandbox/lept-bin/

g++ -I /data/work/ss-git/fun/pic2perfect/sandbox/opencv/include $(pkg-config --libs opencv) -o textDetction textDetection.cpp

wget http://search.maven.org/remotecontent?filepath=org/piccolo2d/piccolo2d-core/3.0/piccolo2d-core-3.0.jar
wget http://search.maven.org/remotecontent?filepath=org/piccolo2d/piccolo2d-extras/3.0/piccolo2d-extras-3.0.jar
make ScrollView.jar

Serif
/data/work/ss-git/fun/fontRecog/data/images/51b3bcc010daa535da000005_large.jpg 
/data/work/ss-git/fun/fontRecog/data/images/515c87ca10daa50b0c000001_large.jpg 

Sans-Serif
/data/work/ss-git/fun/fontRecog/data/images/50a89cf310daa503e4000011_large.jpg


## Tesseract Internals
Ignore unless u want to change something in tesseract.

SetImage which handles bytes per pixel, Recognize (baseapi:832) which does the rest. R = FindLines + ApplyBoxes + CorrectClassifyWords + ExtractFontName + ApplyBoxTraining

FindLines = SegmentPage(pagesegmain:109) + PrepareForTessOCR

PageSegMode is configurable

Equationdetect tries to find indentation type
baseapi.cpp:FindLines (3.1) - Pageseg  - PrepareForTessOCR

blamer - for error detection, which stage it bombed
"Empty page" means AutoPageSeg found an empty page

### Terms
osd - orient and script detection.
PSM - Page Segment Mode
