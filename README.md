# 上位机软件

### 大致功能：

1. 通过多线程和tcp套接字同时监控上百个客户端设备。
2. 管理设备之间的关系（两两绑定任意设备，以及快捷解绑）。
3. 并行接收多个设备组的数据，并实时显示在表格中。
4. 当接收一定的数据后通过自定义公式进行计算，并将结果返回。
5. 支持表格导出为Excel文件。

主要的库：
QTcpSocker, QTcpServer, QThread, QListView, QTableWidget, QtXlsx
