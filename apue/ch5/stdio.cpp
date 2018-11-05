/**
  * Copyright(C), 2017, Nsfocus
  * Name:
  * Author: Wilson Lan
  * Description:
  */

#include <cstdio>


void pr_stdio(const char *, FILE *);

int main() {
    FILE *fp;
    fputs("enter any character\n", stdout);
    if (getchar() == EOF) {
        fputs("getchar error\n", stderr);
        return -1;
    }
    fputs("one line to standard error\n", stderr);

    pr_stdio("stdin", stdin);
    pr_stdio("stdout", stdout);
    pr_stdio("stderr", stderr);

    if ((fp = fopen("/tmp/testfile", "r")) == NULL) {
        fputs("fopen error\n", stderr);
        return -1;
    }
    if (getc(fp) == EOF) {
        fputs("getc error\n", stderr);
        return -1;
    }
    pr_stdio("/tmp/testfile", fp);

    return 0;
}

void pr_stdio(const char *stream_name, FILE *fp) {
    printf("stream = %s, ", stream_name);

    if (fp->_IO_file_flags & _IO_UNBUFFERED) {
        printf("unbuffered");
    } else if (fp->_IO_file_flags & _IO_LINE_BUF) {
        printf("line buffered");
    } else {
        printf("fully buffered");
    }
    printf(", buffer size = %d\n", fp->_IO_buf_end - fp->_IO_buf_base);
}
