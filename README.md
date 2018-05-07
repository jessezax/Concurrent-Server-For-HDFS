# Concurrent-Server-For-HDFS
为了解决当前面向大数据系统间数据传输没有充分发挥分布式系统的优点，无法高效并行快速读取数据的问题，CSFH提供面向Kafka、Storm、Hbase并行化的数据传输方式，提升面向大数据系统的整体吞吐能力。相比于传统的数据传输方式，CSFH可以达到节点服务器可扩展的向大数据系统并行传输数据。
	（1）多个节点服务器并行向HBase提供已格式化好的数据。数据内容具体：行键为文件名加上下载行数，而列族包含每行数据的下载时间，数据大小以及数据内容。
	（2）多个节点服务器并行向Kafka提供已格式化好的数据。将一份数据（数据内容和下载数据的时间戳）并行提供给Kafka中的topic，方便下一步的Comsummer进行消费信息。
	（3）多个节点服务器并行向Storm提供已格式化好的数据。将一份数据（数据内容和下载数据的时间戳）并行提供给Storm中的Spouts，方便其进行下一步的bolts运算。
	（4）为保证时序性，在协调器中，也为HDFS中的数据增加了时间戳的处理。
	（5）分支服务器与协调器间的并发数据传输功能：CSFH并发协调器接受高并发的分支服务器登录，并可以接受多个分支服务器（最多为系统限制进程数）的并行数据传输任务。在整个传输流程中，协调器和分支服务器使用MD5校验来对传输的数据进行高效的校验。分支服务器具备主动中断功能，能够及时中断数据传输，协调器同时也会中断传输。当分支服务器重连时，可以在断点处继续进行下载。每次传输数据时，分支服务器需将已有数据（并未下载完毕）的与协调器中数据进行一致性判定，避免将整体数据进行重传。同时，对于不断增量更新的数据，即不断在文件最后进行续写更新的数据也仍然能获取，
	（6）协调器的公平资源分配功能：当服务器负载到了限度（如开启到最大进程），还有新的分支服务器需要加入时，可能导致资源无法分配，或资源分配不公平的问题。CSFH引入Lru公平机制，即找到不再需要服务的分支服务器，将其踢出，使子进程服务新的分支服务器，以此达到资源的公平分配。