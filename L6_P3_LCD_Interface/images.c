/*Lab 6 - Interfacing a Graphic LCD to the MSP432 MCU
 *Part 3 - Updating characters at different places on the LCD
 *
 *Authors:
 *  Renzo Garza Motta
 *  Cianna Janicke
 *
 *Class: EGR 326
 *Section: 903
 *
 *Instructor:
 *  Dr. Nabeeh Kandalaft
 *
 *Description:
 *  This part of the laboratory activity produces the splash screen as in part II in
 *  addition to writing a function that counts from 0 to 9 with a 1sec delay in between
 *  counts which moves the cursor to different corners in the screen for each count.
 *  When the count reaches 9, the program will wait 3sec and repeat the sequence.
 */

#include "images.h"

#include "CR.h"

#include "Flux1.h"
#include "Flux2.h"
#include "Flux3.h"
#include "Flux4.h"

#include "SirWrenderGif/ezgif-frame-001.h"
#include "SirWrenderGif/ezgif-frame-002.h"
#include "SirWrenderGif/ezgif-frame-003.h"
#include "SirWrenderGif/ezgif-frame-004.h"
#include "SirWrenderGif/ezgif-frame-005.h"
#include "SirWrenderGif/ezgif-frame-006.h"
#include "SirWrenderGif/ezgif-frame-007.h"
#include "SirWrenderGif/ezgif-frame-008.h"
#include "SirWrenderGif/ezgif-frame-009.h"

#include "SirWrenderGif/ezgif-frame-010.h"
#include "SirWrenderGif/ezgif-frame-011.h"
#include "SirWrenderGif/ezgif-frame-012.h"
#include "SirWrenderGif/ezgif-frame-013.h"
#include "SirWrenderGif/ezgif-frame-014.h"
#include "SirWrenderGif/ezgif-frame-015.h"
#include "SirWrenderGif/ezgif-frame-016.h"
#include "SirWrenderGif/ezgif-frame-017.h"
#include "SirWrenderGif/ezgif-frame-018.h"
#include "SirWrenderGif/ezgif-frame-019.h"

#include "SirWrenderGif/ezgif-frame-020.h"
#include "SirWrenderGif/ezgif-frame-021.h"
#include "SirWrenderGif/ezgif-frame-022.h"
#include "SirWrenderGif/ezgif-frame-023.h"
#include "SirWrenderGif/ezgif-frame-024.h"
#include "SirWrenderGif/ezgif-frame-025.h"
#include "SirWrenderGif/ezgif-frame-026.h"
#include "SirWrenderGif/ezgif-frame-027.h"
#include "SirWrenderGif/ezgif-frame-028.h"
#include "SirWrenderGif/ezgif-frame-029.h"

#include "SirWrenderGif/ezgif-frame-030.h"
#include "SirWrenderGif/ezgif-frame-031.h"
#include "SirWrenderGif/ezgif-frame-032.h"
#include "SirWrenderGif/ezgif-frame-033.h"
#include "SirWrenderGif/ezgif-frame-034.h"
#include "SirWrenderGif/ezgif-frame-035.h"
#include "SirWrenderGif/ezgif-frame-036.h"
#include "SirWrenderGif/ezgif-frame-037.h"
#include "SirWrenderGif/ezgif-frame-038.h"
#include "SirWrenderGif/ezgif-frame-039.h"

#include "SirWrenderGif/ezgif-frame-040.h"
#include "SirWrenderGif/ezgif-frame-041.h"
#include "SirWrenderGif/ezgif-frame-042.h"
#include "SirWrenderGif/ezgif-frame-043.h"
#include "SirWrenderGif/ezgif-frame-044.h"
#include "SirWrenderGif/ezgif-frame-045.h"
#include "SirWrenderGif/ezgif-frame-046.h"
#include "SirWrenderGif/ezgif-frame-047.h"
#include "SirWrenderGif/ezgif-frame-048.h"
#include "SirWrenderGif/ezgif-frame-049.h"

#include "SirWrenderGif/ezgif-frame-050.h"
#include "SirWrenderGif/ezgif-frame-051.h"
#include "SirWrenderGif/ezgif-frame-052.h"
#include "SirWrenderGif/ezgif-frame-053.h"
#include "SirWrenderGif/ezgif-frame-054.h"
#include "SirWrenderGif/ezgif-frame-055.h"
#include "SirWrenderGif/ezgif-frame-056.h"
#include "SirWrenderGif/ezgif-frame-057.h"
#include "SirWrenderGif/ezgif-frame-058.h"
#include "SirWrenderGif/ezgif-frame-059.h"
#include "SirWrenderGif/ezgif-frame-060.h"

#include "SirWrenderGif/ezgif-frame-061.h"
#include "SirWrenderGif/ezgif-frame-062.h"
#include "SirWrenderGif/ezgif-frame-063.h"
#include "SirWrenderGif/ezgif-frame-064.h"
#include "SirWrenderGif/ezgif-frame-065.h"
#include "SirWrenderGif/ezgif-frame-066.h"
#include "SirWrenderGif/ezgif-frame-067.h"
#include "SirWrenderGif/ezgif-frame-068.h"
#include "SirWrenderGif/ezgif-frame-069.h"

#include "SirWrenderGif/ezgif-frame-070.h"
#include "SirWrenderGif/ezgif-frame-071.h"
#include "SirWrenderGif/ezgif-frame-072.h"
#include "SirWrenderGif/ezgif-frame-073.h"
#include "SirWrenderGif/ezgif-frame-074.h"
#include "SirWrenderGif/ezgif-frame-075.h"
#include "SirWrenderGif/ezgif-frame-076.h"
#include "SirWrenderGif/ezgif-frame-077.h"
#include "SirWrenderGif/ezgif-frame-078.h"
#include "SirWrenderGif/ezgif-frame-079.h"

#include "SirWrenderGif/ezgif-frame-080.h"
#include "SirWrenderGif/ezgif-frame-081.h"
#include "SirWrenderGif/ezgif-frame-082.h"
#include "SirWrenderGif/ezgif-frame-083.h"
#include "SirWrenderGif/ezgif-frame-084.h"
#include "SirWrenderGif/ezgif-frame-085.h"
#include "SirWrenderGif/ezgif-frame-086.h"
#include "SirWrenderGif/ezgif-frame-087.h"
#include "SirWrenderGif/ezgif-frame-088.h"
#include "SirWrenderGif/ezgif-frame-089.h"

#include "SirWrenderGif/ezgif-frame-090.h"
#include "SirWrenderGif/ezgif-frame-091.h"
#include "SirWrenderGif/ezgif-frame-092.h"
#include "SirWrenderGif/ezgif-frame-093.h"
#include "SirWrenderGif/ezgif-frame-094.h"
#include "SirWrenderGif/ezgif-frame-095.h"
#include "SirWrenderGif/ezgif-frame-096.h"
#include "SirWrenderGif/ezgif-frame-097.h"
#include "SirWrenderGif/ezgif-frame-098.h"
#include "SirWrenderGif/ezgif-frame-099.h"

#include "SirWrenderGif/ezgif-frame-100.h"
#include "SirWrenderGif/ezgif-frame-101.h"
#include "SirWrenderGif/ezgif-frame-102.h"
#include "SirWrenderGif/ezgif-frame-103.h"
#include "SirWrenderGif/ezgif-frame-104.h"
#include "SirWrenderGif/ezgif-frame-105.h"
#include "SirWrenderGif/ezgif-frame-106.h"
#include "SirWrenderGif/ezgif-frame-107.h"
#include "SirWrenderGif/ezgif-frame-108.h"
#include "SirWrenderGif/ezgif-frame-109.h"

#include "SirWrenderGif/ezgif-frame-110.h"
#include "SirWrenderGif/ezgif-frame-111.h"
#include "SirWrenderGif/ezgif-frame-112.h"
#include "SirWrenderGif/ezgif-frame-113.h"
#include "SirWrenderGif/ezgif-frame-114.h"
#include "SirWrenderGif/ezgif-frame-115.h"
#include "SirWrenderGif/ezgif-frame-116.h"
#include "SirWrenderGif/ezgif-frame-117.h"
#include "SirWrenderGif/ezgif-frame-118.h"
#include "SirWrenderGif/ezgif-frame-119.h"

#include "SirWrenderGif/ezgif-frame-120.h"
#include "SirWrenderGif/ezgif-frame-121.h"
#include "SirWrenderGif/ezgif-frame-122.h"
#include "SirWrenderGif/ezgif-frame-123.h"
#include "SirWrenderGif/ezgif-frame-124.h"
#include "SirWrenderGif/ezgif-frame-125.h"
#include "SirWrenderGif/ezgif-frame-126.h"
#include "SirWrenderGif/ezgif-frame-127.h"
#include "SirWrenderGif/ezgif-frame-128.h"
#include "SirWrenderGif/ezgif-frame-129.h"

#include "SirWrenderGif/ezgif-frame-130.h"
#include "SirWrenderGif/ezgif-frame-131.h"
#include "SirWrenderGif/ezgif-frame-132.h"
#include "SirWrenderGif/ezgif-frame-133.h"
#include "SirWrenderGif/ezgif-frame-134.h"
#include "SirWrenderGif/ezgif-frame-135.h"
