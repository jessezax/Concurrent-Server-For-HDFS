server: main.c child.c send_fd.c send_file.c send_n.c md5.c  lru.c hdfs_down.c checkin.c mysql_op.c
        g++  $^ -o $@  -I /usr/include/mysql/   -std=c++11 -lcrypto -static-libstdc++ -I /home/hadoop/app/hadoop-2.4.1/include/ -L /home/hadoop/app/hadoop-2.4.1/lib/native/ -lhdfs /home/hadoop/app/jdk1.7.0_65/jre/lib/i386/server/libjvm.so 
clean:
        rm server
