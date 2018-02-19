#ifndef IMAGES_H
#define IMAGES_H

static const uint8_t pixels_smiley[25] = {
   0, 1, 0, 1, 0,
   0, 1, 0, 1, 0,
   0, 0, 0, 0, 0,
   1, 0, 0, 0, 1,
   0, 1, 1, 1, 0
};

static const uint8_t pixels_sadly[25] = {
   0, 1, 0, 1, 0,
   0, 1, 0, 1, 0,
   0, 0, 0, 0, 0,
   0, 1, 1, 1, 0,
   1, 0, 0, 0, 1
};

static const uint8_t pixels_heart[25] = {
  0, 1, 0, 1, 0,
  1, 1, 1, 1, 1,
  1, 1, 1, 1, 1,
  0, 1, 1, 1, 0,
  0, 0, 1, 0, 0
};

static const uint8_t pixels_arrow_left[25] = {
  0, 0, 1, 0, 0,
  0, 1, 0, 0, 0,
  1, 1, 1, 1, 1,
  0, 1, 0, 0, 0,
  0, 0, 1, 0, 0
};
static const uint8_t pixels_arrow_right[25] = {
  0, 0, 1, 0, 0,
  0, 0, 0, 1, 0,
  1, 1, 1, 1, 1,
  0, 0, 0, 1, 0,
  0, 0, 1, 0, 0
};
static const uint8_t pixels_arrow_leftright[25] = {
  0, 0, 1, 0, 0,
  0, 1, 0, 1, 0,
  1, 0, 0, 0, 1,
  0, 1, 0, 1, 0,
  0, 0, 1, 0, 0
};


const MicroBitImage images[] = {
  MicroBitImage(5, 5, pixels_smiley),
  MicroBitImage(5, 5, pixels_sadly),
  MicroBitImage(5, 5, pixels_heart),
  MicroBitImage(5, 5, pixels_arrow_left),
  MicroBitImage(5, 5, pixels_arrow_right),
  MicroBitImage(5, 5, pixels_arrow_leftright),
};


/*
const uint8_t full[25] = {1, 1, 1, 1, 1,
                          1, 1, 1, 1, 1,
                          1, 1, 1, 1, 1,
                          1, 1, 1, 1, 1,
                          1, 1, 1, 1, 1
};
const uint8_t dot[25] = {0, 0, 0, 0, 0,
                         0, 0, 0, 0, 0,
                         0, 0, 1, 0, 0,
                         0, 0, 0, 0, 0,
                         0, 0, 0, 0, 0
};
const uint8_t small[25] = {0, 0, 0, 0, 0,
                           0, 1, 1, 1, 0,
                           0, 1, 0, 1, 0,
                           0, 1, 1, 1, 0,
                           0, 0, 0, 0, 0
};
const uint8_t large[25] = {1, 1, 1, 1, 1,
                           1, 0, 0, 0, 1,
                           1, 0, 0, 0, 1,
                           1, 0, 0, 0, 1,
                           1, 1, 1, 1, 1,
};
const uint8_t double_row[25] = {0, 1, 1, 0, 0,
                                0, 1, 1, 0, 0,
                                0, 1, 1, 0, 0,
                                0, 1, 1, 0, 0,
                                0, 1, 1, 0, 0
};
const uint8_t tick[25] = {0, 0, 0, 0, 0,
                          0, 0, 0, 0, 1,
                          0, 0, 0, 1, 0,
                          1, 0, 1, 0, 0,
                          0, 1, 0, 0, 0
};
const uint8_t rock[25] = {0, 0, 0, 0, 0,
                          0, 1, 1, 1, 0,
                          0, 1, 1, 1, 0,
                          0, 1, 1, 1, 0,
                          0, 0, 0, 0, 0
};
const uint8_t scissors[25] = {1, 0, 0, 0, 1,
                              0, 1, 0, 1, 0,
                              0, 0, 1, 0, 0,
                              0, 1, 0, 1, 0,
                              1, 0, 0, 0, 1
};
const uint8_t well[25] = {0, 1, 1, 1, 0,
                          1, 0, 0, 0, 1,
                          1, 0, 0, 0, 1,
                          1, 0, 0, 0, 1,
                          0, 1, 1, 1, 0
};
const uint8_t flash[25] = {0, 0, 1, 1, 0,
                           0, 1, 1, 0, 0,
                           1, 1, 1, 1, 1,
                           0, 0, 1, 1, 0,
                           0, 1, 1, 0, 0,

};
const uint8_t wave[7 * 5] = {0, 0, 0, 0, 0, 0, 0,
                             0, 1, 1, 0, 0, 0, 0,
                             1, 0, 0, 1, 0, 0, 1,
                             0, 0, 0, 0, 1, 1, 0,
                             0, 0, 0, 0, 0, 0, 0
};

const MicroBitImage Full(5, 5, full);
const MicroBitImage Dot(5, 5, dot);
const MicroBitImage SmallRect(5, 5, small);
const MicroBitImage LargeRect(5, 5, large);
const MicroBitImage ArrowLeft(5, 5, arrow_left);
const MicroBitImage ArrowRight(5, 5, arrow_right);
const MicroBitImage ArrowLeftRight(5, 5, arrow_leftright);
const MicroBitImage DoubleRow(5, 5, double_row);
const MicroBitImage Tick(5, 5, tick);
const MicroBitImage Heart(5, 5, heart);
const MicroBitImage Smiley(5, 5, smiley);
const MicroBitImage Sadly(5, 5, sadly);
const MicroBitImage Rock(5, 5, rock);
const MicroBitImage Scissors(5, 5, scissors);
const MicroBitImage Well(5, 5, well);
const MicroBitImage Flash(5, 5, flash);
const MicroBitImage Wave(7, 5, wave);
*/

#endif