2004.01                README.txt             Version 0.01

                   W2kPrintDrvSample

Feature
=======
* Support two page directions, portrait and landscape;
* Support just one page size, A4;
* Support two resolutions, 200 x 200 and 100 x 100 dpi;
* Support two color mode, color(24bpp) and monochrom;
* Support halftoning in monochromatic mode;
* Support color identifying(7 colors);
* Support type identifying(3 types);
...

Usage
=====
* 在monochromatic mode下，可以通过设置黑色的输出类型来控制输出;
* 在color mode下，可以通过设置各个颜色的输出类型来控制输出，7种
  颜色以外的颜色都作为黑色来识别;
* 在输出多页文件时，可以分别指定每页的输出文件名，也可以使用自动
  添加页号的功能;
...

Known Problems
==============
* 在color mode下，输出的色彩有一些问题;
...

Developer Notes
===============
* 你可以把这个例子作为一个非常简单的框架，根据自己的需要在合适的
  地方加入相应的代码;
...

Author
======
terrificskyfox <terrificskyfox@yahoo.com.cn>
