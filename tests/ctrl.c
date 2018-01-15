/*

  Copyright (c) 2018 Martin Sustrik

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"),
  to deal in the Software without restriction, including without limitation
  the rights to use, copy, modify, merge, publish, distribute, sublicense,
  and/or sell copies of the Software, and to permit persons to whom
  the Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included
  in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
  IN THE SOFTWARE.

*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "assert.h"
#include "../libdill.h"

coroutine void worker(void) {
    int val;
    int rc = chrecv(hctrl, &val, sizeof(val), -1);
    errno_assert(rc == 0);
    assert(val == 333);
    val = 444;
    rc = chsend(hctrl, &val, sizeof(val), -1);
    errno_assert(rc == 0);
}

int main(void) {
    /* Do a handshake with the child coroutine. */
    int h = go(worker());
    int val = 333;
    int rc = chsend(h, &val, sizeof(val), -1);
    errno_assert(rc == 0);
    rc = chrecv(h, &val, sizeof(val), -1);
    errno_assert(rc == 0);
    assert(val == 444);
    rc = hclose(h);
    errno_assert(rc == 0);

    return 0;
}
